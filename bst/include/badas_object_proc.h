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

#ifndef BADAS_OBJECT_PROC_H_
#define BADAS_OBJECT_PROC_H_

#include <memory>
#include "badas_base.h"

namespace Badas {

class Badas_OBJ_Proc : public Badas_Base {
   public:
    Badas_OBJ_Proc();

    ~Badas_OBJ_Proc(){};

    int ProcFrame(float** obj_prediction, std::vector<BBoxInfo>& out_boxes,
                  bool split_head, bool track_en);

    int ProcFrame(int8_t** obj_prediction, std::vector<float>& qnt_scales,
                  std::vector<BBoxInfo>& out_boxes, bool split_head,
                  bool track_en);

    int GetNumScale();

    int GetNumClass();

    int SetNetVersion(int net_ver);

    int SetNmsThreshold(std::vector<float> nms_thres);

   private:
    class impl;
    std::shared_ptr<impl> pimpl;
};

}  // namespace Badas

#endif  // BADAS_OBJECT_PROC_H_
