/*****************************************************
 * author: likai
 * data and time: 20220316 0840
 * name: inference.h
 * function: inference base class
 * ***************************************************/

#ifndef _INFERENCE_LIDAR_INFERENCE_H_
#define _INFERENCE_LIDAR_INFERENCE_H_

#include <string>
#include <vector>
#include <cmath>

#include "bsnn/bsnn_user_api.h"

namespace lidar_perception {

struct TensorSize {
  int n;
  int c;
  int h;
  int w;
};

typedef struct DataBuffer{
  void *start;
  int length;
} DataBuffer;

typedef enum InferType {
  INFER_USER_ADDR = 0,
  INFER_BUS_ADDR = 1,
} InferType;

class LidarInference {
public:
  LidarInference();
  virtual ~LidarInference();

  virtual void Init(const std::string &model_path, const std::string &model_name, 
                    const InferType &type, const int engine = 0);
  virtual void Update(const DataBuffer &image, std::vector<std::vector<float>> &out_buffers);

  virtual void Update(const DataBuffer &image, float nms_score_threshold, std::vector<int> &keep_ids, std::vector<float> &scores, std::vector<int> &cls_argmax, std::vector<int> &dir_cls_argmax, std::vector<std::vector<float>> &boxes);

  virtual void Release();

  const std::vector<std::vector<float>> &GetInferResults() { return infer_results_; } 

private:
  int set_nn_input(const DataBuffer &image, bsnn_user_instance_t &instance);
  void parse_pointpillars_out(float nms_score_threshold, int group_id, int cls_in_shape_0, int boxes_in_shape_0, int dir_cls_in_shape_0, int in_shape_12, int down_ratio, 
                              uint64_t cls_cur_ts_addr, float cls_ts_deqnt_scale_per_channel,
                              uint64_t boxes_cur_ts_addr, float boxes_ts_deqnt_scale_per_channel,
                              uint64_t dir_cls_cur_ts_addr, float dir_cls_ts_deqnt_scale_per_channel,
                              std::vector<int> &keep_ids, std::vector<float> &scores, std::vector<int> &cls_argmax, std::vector<int> &dir_cls_argmax, std::vector<std::vector<float>> &boxes);

private:
  bsnn_user_model_t bsnn_model_;
  bsnn_user_instance_t bsnn_instance_;
  InferType infer_image_type_;

  bsnn_user_output_t u_output_;
  std::vector<std::vector<float>> infer_results_;
  std::string model_name_;

  uint64_t time_total0 = 0;
  uint64_t time_total1 = 0;
  uint64_t time_total2 = 0;
  uint64_t time_total3 = 0;
};

}
#endif /* _INFERENCE_LIDAR_INFERENCE_H_ */