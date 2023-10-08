#include "lidar_inference.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

namespace lidar_perception
{

  typedef unsigned long long timestamp_t;
  static timestamp_t
  get_timestamp()
  {
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
  }
  LidarInference::LidarInference() {}

  LidarInference::~LidarInference() {}

  void LidarInference::Init(const std::string &model_path, const std::string &model_name,
                            const InferType &type, const int engine)
  {
    model_name_ = model_name;
    printf("model path:%s\n", model_path.c_str());
    infer_image_type_ = type;
    if (BSNN_SUCC != bsnn_model_load(&bsnn_model_, model_path.c_str()))
    {
      std::cerr << "main(), failed to load model" << std::endl;
      bsnn_model_unload(&bsnn_model_);
    }

    printf("input tensor num %d\n", bsnn_model_.input_ts_num);
    printf("output tensor num %d\n", bsnn_model_.output_ts_num);

    // create instance
    bsnn_user_instance_cfg_t u_ins_cfg;
    u_ins_cfg.queue_type = GET_IN_OUT_BLOCKING;
    if (INFER_USER_ADDR == infer_image_type_)
    {
      u_ins_cfg.in_type = USER_ADDR;
      printf("user addr \n");
    }
    else if (INFER_BUS_ADDR == infer_image_type_)
    {
      u_ins_cfg.in_type = BUS_ADDR;
    }
    u_ins_cfg.input_bufferq_size = 1;
    u_ins_cfg.output_bufferq_size = 1;
    u_ins_cfg.in_out_timeout_period = 5000;
    u_ins_cfg.mac_arr_num = 4;
    u_ins_cfg.target_engine = BSNN_ENGINE_NET_4_MACA;
    u_ins_cfg.batch_size = 1;

    if (BSNN_SUCC !=
        bsnn_instance_create(&bsnn_model_, &bsnn_instance_, &u_ins_cfg))
    {
      std::cerr << "main(), failed to create instance" << std::endl;

      // bsnn_all_destroy();
      bsnn_model_unload(&bsnn_model_);
      bsnn_instance_destroy(&bsnn_instance_);
      // exit(EXIT_FAILURE);
    }
  }

  void LidarInference::parse_pointpillars_out(float nms_score_threshold, int group_id, int cls_in_shape_0, int boxes_in_shape_0, int dir_cls_in_shape_0, int in_shape_12, int down_ratio,
                                              uint64_t cls_cur_ts_addr, float cls_ts_deqnt_scale_per_channel,
                                              uint64_t boxes_cur_ts_addr, float boxes_ts_deqnt_scale_per_channel,
                                              uint64_t dir_cls_cur_ts_addr, float dir_cls_ts_deqnt_scale_per_channel,
                                              std::vector<int> &keep_ids, std::vector<float> &scores, std::vector<int> &cls_argmax, std::vector<int> &dir_cls_argmax, std::vector<std::vector<float>> &boxes)
  {
    int cls_out_shape_1 = cls_in_shape_0 / down_ratio;
    int boxes_out_shape_1 = boxes_in_shape_0 / down_ratio;
    int base_id = group_id * in_shape_12 * down_ratio;

    #pragma omp parallel for
    for (int i = 0; i < in_shape_12; ++i)
    {
      for (int j = 0; j < down_ratio; j++)
      {
        int arg_max_cls = -1;
        float max_cls = 0;
        float nms_score_threshold_cur = nms_score_threshold;
        int s1 = j * cls_out_shape_1;
        int s2 = s1 * in_shape_12;
        for (int k = 0; k < cls_out_shape_1; ++k)
        {
          int id = i + (j * cls_out_shape_1 + k) * in_shape_12;
          float cur_cls = (*((signed char *)(cls_cur_ts_addr) + id)) * cls_ts_deqnt_scale_per_channel;

          if (cur_cls >= nms_score_threshold_cur)
          {
            max_cls = cur_cls;
            arg_max_cls = k;
            nms_score_threshold_cur = cur_cls;
          }
        }
        if (arg_max_cls >= 0)
        {
          #pragma omp critical
          {
            keep_ids.push_back(base_id + i * down_ratio + j);
            scores.push_back(max_cls);
            cls_argmax.push_back(arg_max_cls);

            std::vector<float> cur_box(boxes_out_shape_1);
            int s3 = i + j * boxes_out_shape_1 * in_shape_12;
            for (int k = 0; k < boxes_out_shape_1; ++k)
            {
              int id = s3 + k * in_shape_12;
              float cur_boxes = (*((signed char *)(boxes_cur_ts_addr) + id)) * boxes_ts_deqnt_scale_per_channel;
              cur_box[k] = cur_boxes;
            }
            boxes.push_back(cur_box);

            int id = i + (j << 1 + 0) * in_shape_12;
            float dir_cls_0 = (*((signed char *)(dir_cls_cur_ts_addr) + id)) * dir_cls_ts_deqnt_scale_per_channel;
            int id1 = i + (j << 1 + 1) * in_shape_12;
            float dir_cls_1 = (*((signed char *)(dir_cls_cur_ts_addr) + id1)) * dir_cls_ts_deqnt_scale_per_channel;
            int cur_dir_cls_argmax = (dir_cls_0 > dir_cls_1) ? 0 : 1;
            dir_cls_argmax.push_back(cur_dir_cls_argmax);
          }
        }
      }
    }
  }

  void LidarInference::Update(const DataBuffer &image, float nms_score_threshold, std::vector<int> &keep_ids, std::vector<float> &scores, std::vector<int> &cls_argmax, std::vector<int> &dir_cls_argmax, std::vector<std::vector<float>> &boxes)
  {
    if (-1 == set_nn_input(image, bsnn_instance_))
    {
      std::cout << "main() failed to prepare input" << std::endl;
      bsnn_instance_destroy(&bsnn_instance_);
      return;
    }

    if (bsnn_instance_run(&bsnn_instance_) != BSNN_SUCC)
    {
      std::cerr << " ERROR! Unable to run instance" << std::endl;
      bsnn_instance_destroy(&bsnn_instance_);
      return;
    }

    int post_idx = BSNN_ERR_FAIL;
    if (BSNN_SUCC != bsnn_output_get(&bsnn_instance_, &u_output_, &post_idx))
    {
      std::cerr << " ERROR! Unable to get output" << std::endl;
      bsnn_instance_destroy(&bsnn_instance_);
      return;
    }
 
    for (int i = 0; i < (u_output_.output_ts_num / 3); i++)
    {
      bsnn_tensor_attr_t cls_out_ts_attr;
      cls_out_ts_attr.index = i * 3 + 0;
      bsnn_model_query(&bsnn_model_, BSNN_QUERY_OUTPUT_ATTR, &cls_out_ts_attr, 0);
      bsnn_tensor_attr_t boxes_out_ts_attr;
      boxes_out_ts_attr.index = i * 3 + 1;
      bsnn_model_query(&bsnn_model_, BSNN_QUERY_OUTPUT_ATTR, &boxes_out_ts_attr, 0);
      bsnn_tensor_attr_t dir_cls_out_ts_attr;
      dir_cls_out_ts_attr.index = i * 3 + 2;
      bsnn_model_query(&bsnn_model_, BSNN_QUERY_OUTPUT_ATTR, &dir_cls_out_ts_attr, 0);

      parse_pointpillars_out(nms_score_threshold, i, cls_out_ts_attr.size[2], boxes_out_ts_attr.size[2], dir_cls_out_ts_attr.size[2], cls_out_ts_attr.size[0] * cls_out_ts_attr.size[1], 2,
                             u_output_.ts_addr[i * 3 + 0], u_output_.ts_deqnt_scale_per_channel[i * 3 + 0][0],
                             u_output_.ts_addr[i * 3 + 1], u_output_.ts_deqnt_scale_per_channel[i * 3 + 1][0],
                             u_output_.ts_addr[i * 3 + 2], u_output_.ts_deqnt_scale_per_channel[i * 3 + 2][0],
                             keep_ids, scores, cls_argmax, dir_cls_argmax, boxes);
    }
    bsnn_output_release(&bsnn_instance_, post_idx);
  }

  void LidarInference::Update(const DataBuffer &image, std::vector<std::vector<float>> &out_buffers)
  {
    if (-1 == set_nn_input(image, bsnn_instance_))
    {
      std::cout << "main() failed to prepare input" << std::endl;
      bsnn_instance_destroy(&bsnn_instance_);
      return;
    }
    if (bsnn_instance_run(&bsnn_instance_) != BSNN_SUCC)
    {
      std::cerr << " ERROR! Unable to run instance" << std::endl;
      bsnn_instance_destroy(&bsnn_instance_);
      return;
    }

    int post_idx = BSNN_ERR_FAIL;
    if (BSNN_SUCC != bsnn_output_get(&bsnn_instance_, &u_output_, &post_idx))
    {
      std::cerr << " ERROR! Unable to get output" << std::endl;
      bsnn_instance_destroy(&bsnn_instance_);
      return;
    }
    infer_results_.clear();
    for (int i = 0; i < u_output_.output_ts_num; i++)
    {
      bsnn_tensor_attr_t out_ts_attr;
      out_ts_attr.index = i;
      bsnn_model_query(&bsnn_model_, BSNN_QUERY_OUTPUT_ATTR, &out_ts_attr, 0);

      std::vector<float> tmp_buffer(out_ts_attr.size[0] * out_ts_attr.size[1] * out_ts_attr.size[2] * out_ts_attr.size[3]);
      for (int m = 0; m < (out_ts_attr.size[0] * out_ts_attr.size[1] * out_ts_attr.size[2] * out_ts_attr.size[3]); m++)
      {
        tmp_buffer[m] = *((signed char *)(u_output_.ts_addr[i]) + m);
        tmp_buffer[m] = tmp_buffer[m] * u_output_.ts_deqnt_scale_per_channel[i][0];
      }

      infer_results_.push_back(tmp_buffer);
    }
    bsnn_output_release(&bsnn_instance_, post_idx);
    out_buffers = infer_results_;
  }

  void LidarInference::Release()
  {
    bsnn_instance_destroy(&bsnn_instance_);
    bsnn_model_unload(&bsnn_model_);
  }

  int LidarInference::set_nn_input(const DataBuffer &image, bsnn_user_instance_t &instance)
  {
    bsnn_user_input_t input;

    if (BSNN_SUCC != bsnn_input_get(&instance, &input))
    {
      std::cerr << "failed to run bsnn_input_get()" << std::endl;
      return -1;
    }

    if (infer_image_type_ == INFER_USER_ADDR)
    {
      #pragma omp parallel for  
      for (int i = 0; i < image.length; i++)
      {
        *((signed char *)input.frame_ts_usr_addr[0] + i) = *((signed char *)image.start + i);
      }
    }
    else if (infer_image_type_ == INFER_BUS_ADDR)
    {
      input.frame_ts_bus_addr[0] = (uint64_t)image.start;
    }
    if (BSNN_SUCC != bsnn_input_set(&instance, &input))
    {
      std::cerr << "failed to run bsnn_input_set()" << std::endl;
      return -1;
    }

    return 0;
  }
}
