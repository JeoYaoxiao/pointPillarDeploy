#ifndef _PROCESS_LIDAR_PROCESS_DSP_H_
#define _PROCESS_LIDAR_PROCESS_DSP_H_

#include <mutex>
#include <queue>
#include <sys/time.h>
#include <vector>
#include <atomic>
#include <memory>
#include <functional>
#include <xtensa/sim.h>
#include <xtensa/tie/xt_ivp32.h>
#include <xtensa/tie/xt_ivpn.h>

#include "base.h"
#include "pre_process_dsp.h"
#include "post_process.h"

namespace lidar_perception {

class LidarInference;
class PostLidarProcess;

class LidarProcessDSP {
 public:
  LidarProcessDSP();
  virtual ~LidarProcessDSP();

  void Init(const std::string &model_path);
  void Update(void *ptr, int size);
  void Release();

  uint64_t time_memcpy = 0;
  uint64_t time_memcpy_OPT = 0;
  uint64_t time_memcpy_dsp = 0;
  uint64_t time_update = 0;
  uint64_t time_extract = 0;
  uint64_t time_RtCloudPreprocesss = 0;
  uint64_t time_points_to_voxels = 0;
  uint64_t time_total0 = 0;
  uint64_t time_total1 = 0;
  uint64_t time_total2 = 0;
  uint64_t time_total3 = 0;
  uint64_t time_total4 = 0;
  uint64_t time_total5 = 0;
 private:
  std::shared_ptr<LidarInference> lidar_inference_ = nullptr;
  std::shared_ptr<PostLidarProcess> post_lidar_process_ = nullptr;
  std::shared_ptr<signed char[]> int_buf_ = nullptr;
  std::shared_ptr<PointsInVoxelsDSP> piv = nullptr;

  int input_w_ = 352;
  int input_h_ = 400;
  int input_c_ = 16;
  int pre_output_c_ = 10;
  MatrixXf anchors;


  int cnt = 0;
};

}
#endif /* _PROCESS_LIDAR_PROCESS_DSP_H_ */
