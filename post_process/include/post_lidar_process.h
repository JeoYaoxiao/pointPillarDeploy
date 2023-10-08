/*
 * @Author: likai
 * @Date: 2021-10-27 10:57:13
 * @Description: interface hpc_lidar and C-code of Matlab.
 */

#ifndef _POST_LIDAR_PROCESS_H_
#define _POST_LIDAR_PROCESS_H_

#include <vector>

namespace lidar_perception {

typedef struct LidarPostObstaclePre {
  double x;
  double y;
  double z;
  double l;
  double w;
  double h;
  double direction;
  unsigned char flag;
  unsigned char cls;
} LidarPostObstaclePre;

typedef struct LidarPostObstacleOut {
  double x;
  double y;
  double z;
  double l;
  double w;
  double h;
  double direction;
  double rel_vx;
  double rel_vy;
  unsigned char ID;
  unsigned char cls;
  unsigned char state;
} LidarPostObstacleOut;

class PostLidarProcess {
public:
  PostLidarProcess();
  virtual ~PostLidarProcess();

  void Init();
  void UpdateV(const std::vector<LidarPostObstaclePre> lidar_src,
              std::vector<LidarPostObstacleOut> &lidar_out,
              int flag = 0); // flag:0-front,1-left,2-right
  void UpdateP(const std::vector<LidarPostObstaclePre> lidar_src,
              std::vector<LidarPostObstacleOut> &lidar_out,
              int flag = 0); // flag:0-front,1-left,2-right
  void Release();
};

} // namespace lidar_perception
#endif /* _POST_LIDAR_PROCESS_H_ */