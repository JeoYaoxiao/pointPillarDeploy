#ifndef _LIDAR_DATA_COMMON_H_
#define _LIDAR_DATA_COMMON_H_

namespace lidar_perception {

typedef struct PointXYZI  ///< user defined point type
{
  float x;
  float y;
  float z;
  unsigned char intensity;
} PointXYZI;
typedef struct PointXYZIRT ///< user defined point type
{
  float x;
  float y;
  float z;
  unsigned char intensity;
  double timestamp;
  unsigned short ring;
} PointXYZIRT;

typedef struct PrePointXYZI  ///< user defined point type
{
  float x;
  float y;
  float z;
  float intensity;
} PrePointXYZI;

}
#endif /* _LIDAR_DATA_COMMON_H_ */