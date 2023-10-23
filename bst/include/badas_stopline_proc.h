/*******************************************************************************
 * Copyright(c) Black Sesame Technologies Inc., All right reserved.
 *
 * No part of this file may be distributed, duplicated or transmitted in any
 *form or by any means, without prior consent of Black Sesame Technologies Inc.
 *
 * This file is  property. It contains BST's trade secret, proprietary
 * and confidential information.
 *
 *********************************************************************************/

#ifndef BADAS_SL_PROC_H_
#define BADAS_SL_PROC_H_

/*
  Postprocessing of stopline.

  Output a list of stopline points given an image.

  @param frame input RGB image, technically the resolution can be random since
  the function will resize the input image to (1280,640). Therefore, to retain a
  decent performance, the input image's resolution ratio is recommended to be 2.
  If the input image's resolution ratio is not 2, it is recommended to crop top
  to keep a resolution ration of 2.

  @return res list of stopline points. The order of the points is from
  left to right.
*/

#include "badas_base.h"

namespace Badas {
typedef struct {
    cv::Mat dist1_w;
    cv::Mat dist1_b;

    cv::Mat dist2_w;
    cv::Mat dist2_b;

    cv::Mat score1_w;
    cv::Mat score1_b;

    cv::Mat score2_w;
    cv::Mat score2_b;

    cv::Mat poly1_w;
    cv::Mat poly1_b;

    cv::Mat poly2_w;
    cv::Mat poly2_b;
} SL_Net_NN_t;

class Badas_SL_Proc : public Badas_Base {
   public:
    Badas_SL_Proc();
    ~Badas_SL_Proc();

   public:
    int InitProcNetPara(std::string nn_path);

    int PostProcessNet(int8_t *p, const float &quant_scale,
                       float &stopline_score);

    int PostProcessNet(float *p, float &stopline_score);

    int PostProcessNetV10(float *score, float *dist, float *coef,
                          float &stopline_score);

    int ProcFrame(float *lane_prediction, float *lane_attribute,
                  float *stopline_score, StoplineInfo &out_sl_info,
                  std::vector<std::vector<float>> &shared_sl_pts);

    int ProcFrame(int8_t *lane_prediction, const float qsc_lane_prediction,
                  int8_t *lane_attribute, const float qsc_lane_attribute,
                  float *stopline_score, StoplineInfo &out_sl_info,
                  std::vector<std::vector<float>> &shared_sl_pts);

    int SetNetVersion(int net_ver);

   protected:
    cv::Mat ReduceMean(int8_t *p, const float &quant_scale);
    cv::Mat ReduceMean(float *p);

    static std::vector<std::vector<float>> sort_two_vectors_sl(
        const std::vector<float> &x, const std::vector<float> &y);
    static void clamp(std::vector<float> &y, const float &upper,
                      const float &lower);
    static std::vector<float> fit_and_clamp(const std::vector<float> &coef,
                                            const std::vector<float> &x,
                                            const bool &if_normalized,
                                            const int &ww, const int &hh,
                                            const float &offset,
                                            const bool if_clamp);

    void Relu(cv::Mat &input);

    cv::Mat TwoFullyConnected(const cv::Mat &reduced, const cv::Mat &weight1,
                              const cv::Mat &bias1, const cv::Mat &weight2,
                              const cv::Mat &bias2);

    std::vector<std::vector<float>> FindLocalMinimum(float *&p);
    std::vector<std::vector<float>> FindLocalMinimum(int8_t *&p,
                                                     const float quant_scale);

    std::vector<std::vector<float>> GetStoplinePoints(
        std::vector<std::vector<float>> &hor_lane_pts, float *&p,
        float *&stopline_score);

    std::vector<std::vector<float>> GetStoplinePoints(
        std::vector<std::vector<float>> &hor_lane_pts, int8_t *&p,
        const float quant_scale, float *&stopline_score);

    bool IfSlExistFromPts(const std::vector<std::vector<float>> &pts);

    int RefineStoplinePoints(std::vector<std::vector<float>> &pts,
                             const float &xmin_pred, const float &xmax_pred,
                             const float &ymean_pred,
                             std::vector<float> &poly_coef,
                             StoplineInfo &out_sl_info);
    int RemoveNoisyStopline(std::vector<std::vector<float>> &pts,
                            StoplineInfo &out_sl_info);

    void StackVector2Points(std::vector<std::vector<float>> &pts,
                            std::vector<std::pair<float, float>> &out_fs);

   private:
    // net
    int stride_;  // depth_to_space stride, depends on the format of the net's
    // output
    int ww_net_;  // used to reduce computation, it depends on ww and stride, no
    // need to change
    int hh_net_;  // used to reduce computation, it depends on hh and stride, no
    // need to change

    //
    float sl_laneness_th_;
    float sl_laneness_th_final_;  // used to reduce computation, depends on
                                  // sl_laneness_th_, no need to change

    float sl_lanedistance_th_;
    float sl_lanedistance_th_final_;  // used to reduce computation, depends on
                                      // sl_lanedistance_th_, no need to change

    int min_number_pts_;

    float stopline_exist_th_;
    float stopline_exist_th_final_;  // used to reduce computation, depends on
                                     // stopline_exist_th_, no need to change

    float stopline_pts_cls_th_;
    float
        stopline_pts_cls_th_final_;  // used to reduce computation, depends on
                                     // stopline_pts_cls_th_, no need to change

    int num_channels_color_shape_;
    int color_shape_stride_;
    int ts_channels_color_shape_;

    float left_image_ratio_;
    float right_image_ratio_;

    float min_ratio_unique_x_;
    int min_num_unique_x_;

    int many_num_pts_th_;
    int segment_threshold_;
    float segment_slope_;
    int longest_th_;
    float fit_adjust_error_th_;
    float threshold_height1_;
    float threshold_height2_;
    float fit_pts_error_th_;
    float max_slope_th_;
    float dy_th_;

    // for access the net output tensor
    int dis_channel_;
    int laneness_channel_;
    int sl_channel_;
    int ts_channels_;
    int num_channels_;  // after depth_to_space, the number of channel,
                        // [1,ww,hh,
    // num_channels]

    SL_Net_NN_t sl_net_nn_;

    std::vector<float> stopline_dist_;
    std::vector<float> stopline_poly_;

    int reduce_wt_;
    int reduce_ht_;
    int reduce_ct_;
};

}  // namespace Badas
#endif  // BADAS_STOPLINE_PROC_H_
