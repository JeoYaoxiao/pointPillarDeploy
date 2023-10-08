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

#ifndef BADAS_FS_PROC_H_
#define BADAS_FS_PROC_H_

/*
  Postprocessing of freespace.

  Output a list of freespace contour points given an image.

  @param frame input RGB image, technically the resolution can be random since
  the function will resize the input image to (1280,640). Therefore, to retain a
  decent performance, the input image's resolution ratio is recommended to be 2.
  If the input image's resolution ratio is not 2, it is recommended to crop top
  to keep a resolution ration of 2.

  @return res list of freespace contour points. The order of the points is from
  left to right. Left boudary column has at least one point(0,hh-1); right
  boudary has least one points (ww-1, hh-1), the rest of columns has at most one
  point, some column has no points.
*/

#include <memory>
#include "badas_base.h"

namespace Badas {

class Badas_FS_Proc : public Badas_Base {
   public:
    Badas_FS_Proc();

    ~Badas_FS_Proc() {}

    int ProcFrame(float *fs_prediction,
                  std::vector<std::pair<float, float>> &out_free_space,
                  bool if_reduce_pts,
                  std::vector<std::vector<float>> &shared_fs_pts);

    int ProcFrame(int8_t *fs_prediction, float qnt_scale,
                  std::vector<std::pair<float, float>> &out_free_space,
                  bool if_reduce_pts,
                  std::vector<std::vector<float>> &shared_fs_pts);

    int SetNetVersion(int net_ver);

   private:
    class impl;
    std::shared_ptr<impl> pimpl;
};

}  // namespace Badas
#endif  // BADAS_FREESPACE_PROC_H_
