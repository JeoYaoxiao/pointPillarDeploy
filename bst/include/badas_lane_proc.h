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

#ifndef BADAS_LN_PROC_H_
#define BADAS_LN_PROC_H_

#include <memory>
#include "badas_base.h"
#include "badas_lane_types.h"

namespace Badas {

class Badas_LN_Proc : public Badas_Base {
   public:
    Badas_LN_Proc();

    ~Badas_LN_Proc(){};

    // for V6 and before
    int ProcFrame(float *ln_prediction,
                  std::vector<std::pair<float, float>> &out_lane);

    // for V6 and before
    int ProcFrame(int8_t *ln_prediction, float qnt_scale,
                  std::vector<std::pair<float, float>> &out_lane);

    // for V7
    int ProcFrame(float *lane, float *ln_shape_color,
                  LaneDetectionResult &out_lane_ex,
                  std::vector<std::vector<float>> &fs_points,
                  std::vector<std::vector<float>> &sl_points, const bool &if_sl,
                  const float &sl_score, const bool track_enable = true,
                  const int &track_num = 1,
                  const int &output_num_pts_per_lane = 100,
                  const bool if_inner_lane = false);

    // for V7
    int ProcFrame(int8_t *lane, float lane_qnt_scale, int8_t *ln_shape_color,
                  float ln_shape_color_qnt_scale,
                  LaneDetectionResult &out_lane_ex,
                  std::vector<std::vector<float>> &fs_points,
                  std::vector<std::vector<float>> &sl_points, const bool &if_sl,
                  const float &sl_score, const bool track_enable = true,
                  const int &track_num = 1,
                  const int &output_num_pts_per_lane = 100,
                  const bool if_inner_lane = false);

    // for V7 shared
    int proc_frame(float *lane_curb_pred, int8_t *lane_curb_pred_int,
                   const float &lane_qnt_scale, const bool &if_quantized,
                   float *ln_shp_color, int8_t *ln_shp_color_int,
                   const float &ln_shape_color_qnt_scale,
                   LaneDetectionResult &out_lane_ex,
                   std::vector<std::vector<float>> &fs_points,
                   std::vector<std::vector<float>> &sl_points,
                   const bool &if_sl, const float &sl_score,
                   const bool track_enable = true, const int &track_num = 1,
                   const int &output_num_pts_per_lane = 100,
                   const bool if_inner_lane = false);

    // for V7 fast version
    int ProcFrame(float *lane, std::vector<std::pair<float, float>> &out_lane,
                  std::vector<std::pair<float, float>> &out_curbside,
                  std::vector<std::vector<float>> &fs_points,
                  std::vector<std::vector<float>> &sl_points, const bool &if_sl,
                  const float &sl_score);

    // for V7 fast version
    int ProcFrame(int8_t *lane, float lane_qnt_scale,
                  std::vector<std::pair<float, float>> &out_lane,
                  std::vector<std::pair<float, float>> &out_curbside,
                  std::vector<std::vector<float>> &fs_points,
                  std::vector<std::vector<float>> &sl_points, const bool &if_sl,
                  const float &sl_score);

    void SetDebugLevel(int debug_level);

    int SetNetVersion(int net_ver);

    int SetCameraInfo(const CameraInfoHolder camera_info, int img_top_offset);

    int SetLanenessThreshold(float lane_thres);

    int SetCurbnessThreshold(float curb_thres);

   private:
    class impl;
    std::shared_ptr<impl> pimpl;
};

}  // namespace Badas

#endif  // BADAS_LN_PROC_H_
