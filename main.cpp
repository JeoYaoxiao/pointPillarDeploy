#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <sched.h>
#include <iostream>
#include "lidar_process.h"
#include "lidar_process_dsp.h"
#include "lidar_data_common.h"

using namespace std;

lidar_perception::LidarProcess *processor_ = nullptr;

lidar_perception::LidarProcessDSP *processor_dsp = nullptr;

int main(int argc, char **argv) {
  char *image_path = NULL;
  char *video_path = NULL;

  int run_mode = 1;
  int run_type = -1;
  MAYBE_UNUSED(uint64_t start = 0LL, stop = 0LL);
  MAYBE_UNUSED(uint64_t updatestart = 0LL, updatestop = 0LL);
#if BST_CPU

  printf("\nCPU==============================================================\n");

  TIME_STAMP(start);
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

    TIME_STAMP(stop);
    printf("> processor_->Init cycles = %llu \n", (stop - start));

    uint64_t time_total0 = 0;
    auto t00 = std::chrono::high_resolution_clock::now();

    TIME_STAMP(updatestart);
    for (int i = 0; i < 1; i++) {
      processor_->Update(&cloud_data[0], cloud_data.size());
    }
    TIME_STAMP(updatestop);
    processor_->time_update = updatestop - updatestart;
    printf("> processor_->Update cycles = %llu \n",processor_->time_update);

    auto t10 = std::chrono::high_resolution_clock::now();
    auto cost0 = std::chrono::duration_cast<std::chrono::microseconds>(t10 - t00).count();
    cout << "average cost:" << (cost0 / 1000) <<endl;
    cout << "> memcpy cycles = percent\n" << (processor_->time_memcpy / processor_->time_update);
//    printf("> memcpy cycles = percent %llu \n", (processor_->time_memcpy / processor_->time_update));
//    printf("> points_to_voxels cycles = percent %llu \n", (processor_->time_points_to_voxels / processor_->time_update) );
//    printf("> RtCloudPreprocess cycles = percent %llu \n", (processor_->time_RtCloudPreprocesss / processor_->time_update) );

    in.close();
  }
  processor_->Release();
  delete processor_;
  processor_ = nullptr;
#endif //BST_CPU


  //=============================================================
  //== DSP Compare
  //=============================================================
#if BST_DSP

  printf("\nDSP============================================================\n");
  printf("XCHAL_IVPN_SIMD_WIDTH-------------------------------------------%d\n",
		  XCHAL_IVPN_SIMD_WIDTH);
  TIME_STAMP(start);
  processor_dsp = new lidar_perception::LidarProcessDSP();
  processor_dsp->Init("../model/lidar_front");
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

      TIME_STAMP(stop);
      printf("> processor_dsp->Init cycles = %llu \n", (stop - start));

      uint64_t time_total0 = 0;
      auto t00 = std::chrono::high_resolution_clock::now();

      TIME_STAMP(updatestart);
      for (int i = 0; i < 1; i++) {
        processor_dsp->Update(&cloud_data[0], cloud_data.size());
      }
      TIME_STAMP(updatestop);

      printf("> processor_dsp->Update cycles = %llu \n",(updatestop - updatestart));

      auto t10 = std::chrono::high_resolution_clock::now();
      auto cost0 = std::chrono::duration_cast<std::chrono::microseconds>(t10 - t00).count();
      cout << " processor_dsp average cost:" << (cost0 / 1000) <<endl;


//    printf("> memcpy cycles = percent %llu \n", (processor_dsp->time_memcpy / processor_dsp->time_update));
//    printf("> points_to_voxels cycles = percent %llu \n", (processor_dsp->time_points_to_voxels / processor_dsp->time_update) );
//    printf("> RtCloudPreprocess cycles = percent %llu \n", (processor_dsp->time_RtCloudPreprocesss / processor_dsp->time_update) );
//
      in.close();
    }

    processor_dsp->Release();
    delete processor_dsp;
    processor_dsp = nullptr;
#endif // BST_DSP

  return 0;
}

