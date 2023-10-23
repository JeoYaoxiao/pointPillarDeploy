#include "lidar_process_dsp.h"

#include <chrono>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <time.h>
#include <thread>
#include <memory>
#include <algorithm>

#include "lidar_data_common.h"
//#include "lidar_inference.h"
#include "post_lidar_process.h"

#include "base.h"
#include "anchor.h"

#ifndef MIN
#  define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#  define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

namespace lidar_perception
{
#if 0

  void image_to_file(const vector<float> &image, const string &path)
  {
    ofstream out_file(path, ios::out | ios::trunc | ios::binary);
    for (int i = 0; i < image.size(); ++i)
    {
      out_file.write((char *)&image[i], sizeof(image[i]));
    }

    out_file.close();
  }
  vector<float> test_preprocess(const string &points_path, const string &out_path)
  {
    vector<float> batch_image = std::move(pre_process_dsp(points_path));

    return batch_image;
  }

  void flatten(const vector<int> &shape, vector<int> &ids, int &id, bool is_flatten = true)
  {
    int nelements = 1;
    for (auto s : shape)
    {
      nelements *= s;
    }
    assert(shape.size() == ids.size());
    int cur_id = id;
    if (is_flatten)
    {
      id = 0;
    }
    for (int i = 0; i < shape.size(); ++i)
    {
      nelements /= shape[i];
      if (is_flatten)
      {
        id += ids[i] * nelements;
      }
      else
      {
        ids[i] = cur_id / nelements;
        cur_id %= nelements;
      }
    }
  }

  vector<int> permute(const vector<int> &permute, const vector<int> &src)
  {
    assert(permute.size() == src.size());
    vector<int> dst(permute.size());
    for (int i = 0; i < permute.size(); ++i)
    {
      dst[i] = src[permute[i]];
    }
    return dst;
  }

  // pred: (1, c, h, w)被拉平之后的1维数组
  // shape: pred被拉平之前的维度(1, c, h, w)
  // permute_param：torch.permute的参数 比如(0, 2, 3, 1)
  // reshape_param: torch.view的参数 比如(bs, h*w, c) (目前不支持-1)
  // transform: pred -> re_flatten -> permute -> view
  MatrixXf transform(const vector<float> &pred, const vector<int> &shape, const vector<int> &permute_param, const vector<int> &reshape_param)
  {
    assert(4 == shape.size() && 1 == shape[0] && 1 == reshape_param[0]);
    int dim = shape.size();
    int reshape_dim = reshape_param.size();
    int c = shape[1];
    int h = shape[2];
    int w = shape[3];
    int reshape_c = reshape_param[reshape_dim - 1];
    assert(pred.size() == c * h * w);
    int num_rows = h * w * c / reshape_c;
    int num_cols = reshape_c;
    MatrixXf out(num_rows, num_cols);

    for (int i = 0; i < pred.size(); ++i)
    {
      // reflatten
      vector<int> ids(dim);
      flatten(shape, ids, i, false);
      // permute
      vector<int> ids_permuted = permute(permute_param, ids);
      vector<int> shape_permuted = permute(permute_param, shape);
      // flatten
      int id_permuted_flatten = 0;
      flatten(shape_permuted, ids_permuted, id_permuted_flatten, true);
      // reshape & reflatten
      vector<int> reshape_ids(reshape_dim);
      flatten(reshape_param, reshape_ids, id_permuted_flatten, false);

      out(reshape_ids[1], reshape_ids[2]) = pred[i];
    }

    return out;
  }

  MatrixXf stack(const vector<MatrixXf> &input)
  {
    int num_input = input.size();
    int input_rows = input[0].rows();
    int input_cols = input[0].cols();
    MatrixXf out(input_rows * num_input, input_cols);
    for (int i = 0; i < num_input; ++i)
    {
      out.block(i * input_rows, 0, input_rows, input_cols) = input[i];
    }
    return out;
  }

  PostRet test_postprocess(
      const vector<vector<float>> &preds,
      const MatrixXf anchors)
  {

    vector<int> cls_pred_shape{1, 8, 200, 176};
    vector<int> cls_pred_reshape{1, 70400, 4};
    vector<int> box_pred_shape{1, 14, 200, 176};
    vector<int> box_pred_reshape{1, 70400, 7};
    vector<int> dir_pred_shape{1, 4, 200, 176};
    vector<int> dir_pred_reshape{1, 70400, 2};
    vector<int> permute_param{0, 2, 3, 1};

    int num_preds = preds.size();
    vector<MatrixXf> cls_pred(4);
    vector<MatrixXf> box_pred(4);
    vector<MatrixXf> dir_pred(4);
    for (int i = 0; i < 4; ++i)
    {
      cls_pred[i] = transform(preds[i * 3 + 0], cls_pred_shape, permute_param, cls_pred_reshape);
      box_pred[i] = transform(preds[i * 3 + 1], box_pred_shape, permute_param, box_pred_reshape);
      dir_pred[i] = transform(preds[i * 3 + 2], dir_pred_shape, permute_param, dir_pred_reshape);
    }

    MatrixXf cls_preds = stack(cls_pred);
    MatrixXf box_preds = stack(box_pred);
    MatrixXf dir_preds = stack(dir_pred);

    int num_pred = anchors.rows();

    vector<MatrixXf> batch_boxes{box_preds};
    vector<MatrixXf> batch_cls{cls_preds};
    vector<MatrixXf> batch_dir_cls{dir_preds};

    return post_process(batch_boxes, batch_cls, batch_dir_cls, anchors);
  }

  MatrixXf test_anchors()
  {
    return create_anchor();
  }
#endif
  MatrixXf RtCloudPreprocessDSP(MatrixXf &res, bool is_trans)
  {
    if (is_trans)
    {
      return res.transpose();
    }
    else
    {
      return res;
    }
  }

  LidarProcessDSP::LidarProcessDSP() {}

  LidarProcessDSP::~LidarProcessDSP() {}

  void LidarProcessDSP::Init(const std::string &model_path)
  {
#if 0
   lidar_inference_ = std::make_shared<LidarInference>();
   lidar_inference_->Init(model_path, "front_lidar", INFER_USER_ADDR);
   post_lidar_process_ = std::make_shared<PostLidarProcess>();
   post_lidar_process_->Init();
    int_buf_. = std::make_shared<>(new char[10*400*352]);
   int_buf_.reset(new signed char[16 * 400 * 352]);
   anchors = std::move(test_anchors());
#endif
    piv = std::make_shared<PointsInVoxelsDSP>();
    piv->Init();
  }

  void LidarProcessDSP::Update(void *ptr, int size)
  {
	 //timer
    MAYBE_UNUSED(uint64_t memcpystart = 0LL, memcpystop = 0LL);
    MAYBE_UNUSED(uint64_t RtCloudPreprocessstart = 0LL, RtCloudPreprocessstop = 0LL);
    MAYBE_UNUSED(uint64_t points_to_voxelsstart = 0LL, points_to_voxelsstop = 0LL);
    MAYBE_UNUSED(uint64_t extractstart = 0LL, extractstop = 0LL);

    auto ptr_tmp = (PointXYZI *)ptr;
    MatrixXf res(4, size);
    auto stride = sizeof(PrePointXYZI);

TIME_STAMP(memcpystart);

    /*memcpy*/
    for (int i = 0; i < size; i++)
    {
      auto &data = ptr_tmp[i];
      PrePointXYZI pre_point;
      pre_point.x = data.x;
      pre_point.y = data.y;
      pre_point.z = data.z;
      pre_point.intensity = data.intensity / 255.0;

      memcpy((char *)res.data() + i * stride, &pre_point.x, 4);
      memcpy((char *)res.data() + i * stride + 4, &pre_point.y, 4);
      memcpy((char *)res.data() + i * stride + 8, &pre_point.z, 4);
      memcpy((char *)res.data() + i * stride + 12, &pre_point.intensity, 4);
    }

TIME_STAMP(memcpystop);
this->time_memcpy = memcpystop - memcpystart;

printf("> processor_dsp->Update >> memcpy cycles = %llu \n",this->time_memcpy);
    /*RtCloudPreprocess*/
    TIME_STAMP(RtCloudPreprocessstart);
    auto points = std::move(RtCloudPreprocessDSP(res, true));
    VoxelDSP voxel_dsp = VoxelDSP();

    TIME_STAMP(points_to_voxelsstart);
    voxel_dsp.points_to_voxels(points, piv);
    TIME_STAMP(points_to_voxelsstop);

    this->time_points_to_voxels = points_to_voxelsstop - points_to_voxelsstart;

    PointPillarsNet ppn;

    TIME_STAMP(extractstart);
    vector<float> batch_image = ppn.extract(piv);
    TIME_STAMP(extractstop);
    this->time_extract  = extractstop - extractstart;
    
    piv->Reset();
    TIME_STAMP(RtCloudPreprocessstop);

    this->time_RtCloudPreprocesss = RtCloudPreprocessstop - RtCloudPreprocessstart;
    printf("> processor_dsp->Update >> points_to_voxels cycles = %llu \n",(this->time_points_to_voxels));
    printf("> processor_dsp->Update >> voxel.time_extract cycles = %llu \n",(this->time_extract));
    printf("> processor_dsp->Update >> RtCloudPreprocess cycles = %llu \n",(this->time_RtCloudPreprocesss));
float *p_npy_data_temp = &batch_image[0];
#if 0
   signed char *p_npy_data = int_buf_.get();
   // NCHW-->NHWC, CH:10-->16 padding
   {
     int s1 = input_h_*input_w_;
     int s2 = input_w_ * input_c_;
     float* offset_l1_w = 0;
     signed char* offset_l1_c = 0;
     float* offset_l2_w = 0;
     signed char* offset_l2_c = 0;

     #pragma omp parallel for
     for (int i = 0; i < input_h_; i++)
     {
       offset_l1_w = i * input_w_ + p_npy_data_temp;
       offset_l1_c = p_npy_data + i * s2;
       for (int m = 0; m < input_w_; m++)
       {
         offset_l2_w = offset_l1_w + m;
         offset_l2_c = m * input_c_ + offset_l1_c;
         for (int c = 0; c < pre_output_c_; c++)
         {
           float tmp = *(c * s1 + offset_l2_w) * 2.0;
           tmp = std::floor(tmp);
           tmp = MIN(MAX(tmp,-128),127);
           *(offset_l2_c + c) = (signed char)tmp;
         }
       }
     }
   }

   DataBuffer infer_img_tmp = {0};
   infer_img_tmp.start = int_buf_.get();
   infer_img_tmp.length = 16 * 400 * 352;

   std::vector<int> keep_ids;
   keep_ids.reserve(16);
   std::vector<float> scores;
   scores.reserve(16);
   std::vector<int> cls_argmax;
   cls_argmax.reserve(16);
   std::vector<int> dir_cls_argmax;
   dir_cls_argmax.reserve(16);
   std::vector<std::vector<float>> boxes;
   boxes.reserve(16);
   lidar_inference_->Update(infer_img_tmp, nms_score_threshold, keep_ids, scores, cls_argmax, dir_cls_argmax, boxes);

   if (!boxes.size())
   {
     return;
   }

   // 后处理
   PostRet post_ret = std::move(post_process_1(boxes, scores, cls_argmax, dir_cls_argmax, keep_ids, anchors));

   cout << "output Rst:" << endl;
   std::cout << "post_ret.boxes.rows():" << post_ret.boxes.rows() << std::endl;
   std::cout << "post_ret.boxes.cols():" << post_ret.boxes.cols() << std::endl;
   std::cout << "post_ret.scores.rows():" << post_ret.scores.rows() << std::endl;
   std::cout << "post_ret.scores.cols():" << post_ret.scores.cols() << std::endl;
   std::cout << "post_ret.labels.rows():" << post_ret.labels.rows() << std::endl;
   std::cout << "post_ret.labels.cols():" << post_ret.labels.cols() << std::endl;

   cout << post_ret.boxes << endl
        << endl;
   cout << post_ret.scores << endl
        << endl;
   cout << post_ret.labels << endl
        << endl;


   std::vector<LidarPostObstaclePre> lidar_v_src;
   std::vector<LidarPostObstaclePre> lidar_p_src;
   std::vector<LidarPostObstacleOut> lidar_v_out;
   std::vector<LidarPostObstacleOut> lidar_p_out;

   for (int i = 0; i < post_ret.boxes.rows(); i++)
   {
     LidarPostObstaclePre post_pre;
     post_pre.x = post_ret.boxes(i, 0);
     post_pre.y = post_ret.boxes(i, 1);
     post_pre.z = post_ret.boxes(i, 2);
     post_pre.l = post_ret.boxes(i, 3);
     post_pre.w = post_ret.boxes(i, 4);
     post_pre.h = post_ret.boxes(i, 5);
     post_pre.direction = post_ret.boxes(i, 6);
     post_pre.cls = post_ret.labels(i, 0);
     post_pre.flag = 1;

     if (0 == post_pre.cls)
     {
       post_pre.cls = 1;
       lidar_v_src.push_back(post_pre);
     }
     else if (3 == post_pre.cls)
     {
       post_pre.cls = 2;
       lidar_v_src.push_back(post_pre);
     }
     else if (1 == post_pre.cls)
     {
       post_pre.cls = 3;
       lidar_p_src.push_back(post_pre);
     }
     else if (2 == post_pre.cls)
     {
       post_pre.cls = 4;
       lidar_p_src.push_back(post_pre);
     }
   }

   post_lidar_process_->UpdateV(lidar_v_src, lidar_v_out);
   post_lidar_process_->UpdateP(lidar_p_src, lidar_p_out);
#endif
  }

  void LidarProcessDSP::Release()
  {
#if 0
   if (post_lidar_process_)
   {
     post_lidar_process_->Release();
   }
   if (lidar_inference_)
   {
     lidar_inference_->Release();
   }
#endif
    printf("Exit LidarProcess.\n");
  }
}
