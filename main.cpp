#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <sched.h>
#include <iostream>

#include "lidar_process.h"
#include "lidar_data_common.h"

using namespace std;

lidar_perception::LidarProcess *processor_ = nullptr;

int main(int argc, char **argv) {
  char *image_path = NULL;
  char *video_path = NULL;

  int run_mode = 1;
  int run_type = -1;

  processor_ = new lidar_perception::LidarProcess();
  processor_->Init("../model/lidar_front");
  {
    std::ifstream file("000019.bin", std::ios::in | std::ios::binary);
    if (!file) {
        printf("no such file\n");
    }

    int begin = file.tellg();
    file.seekg(0, std::ios::end);
    int end = file.tellg();
    size_t len = end - begin;
    printf("len:%d\n", len);
    size_t data_len = len / sizeof(float);

    size_t cols = 4;
    size_t rows = data_len / cols;
    file.close();

    std::vector<lidar_perception::PointXYZI> cloud_data;
    std::ifstream in("000019.bin", std::ios::in | std::ios::binary);
    float tmp_data;
    for (int i=0; i<rows; i++) {
      lidar_perception::PointXYZI current;
      in.read((char *)&tmp_data, sizeof(float));
      current.x = tmp_data;
      in.read((char *)&tmp_data, sizeof(float));
      current.y = tmp_data;
      in.read((char *)&tmp_data, sizeof(float));
      current.z = tmp_data;
      in.read((char *)&tmp_data, sizeof(float));

      current.intensity = tmp_data * 255;
      cloud_data.push_back(current);
    }

    uint64_t time_total0 = 0;
    auto t00 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1; i++) {
        processor_->Update(&cloud_data[0], cloud_data.size());
    }

    auto t10 = std::chrono::high_resolution_clock::now();
    auto cost0 = std::chrono::duration_cast<std::chrono::microseconds>(t10 - t00).count();
    cout << "average cost:" << (cost0 / 1000) <<endl;

    in.close();
  }
  processor_->Release();
  delete processor_;
  processor_ = nullptr;

  return 0;
}

