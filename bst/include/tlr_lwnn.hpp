
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

#pragma once

#include <memory>
#include <string>

#include "badas.h"
#include "badas_base.h"
#include "lwnn/lwnn_user_api.h"

namespace Badas {

class TLR_LWNN : public Badas_Base {
   public:
    TLR_LWNN();
    ~TLR_LWNN(){};

    int InitTLRModel(std::string model_path);

    void SetDebugLevel(int debug_level);
    void SetDump(bool if_dump);

    int ProcTLR(std::vector<BBoxInfo>& bbox_info, const cv::Mat& cur_frame,
                const cv::Mat& net_frm, const int disp_x_off,
                const int disp_y_off, const float disp_x_ratio,
                const float disp_y_ratio);
    int ProcTLRSingle(BBoxInfo& b_box, const cv::Mat& cur_frame,
                      const cv::Mat& net_frm, const int disp_x_off,
                      const int disp_y_off, const float disp_x_ratio,
                      const float disp_y_ratio);

   private:
    class Impl;
    std::shared_ptr<Impl> pimpl_;
};

}  // namespace Badas