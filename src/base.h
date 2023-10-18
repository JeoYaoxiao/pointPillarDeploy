#ifndef BASE_H
#define BASE_H

#include <Eigen/Core>
#include <Eigen/Eigen>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <numeric>

#include <xtensa/sim.h>
#include <xtensa/tie/xt_ivp32.h>
#include <xtensa/tie/xt_ivpn.h>

#define BST_CPU 0
#define BST_DSP 1

using namespace std;
using namespace Eigen;

typedef Array<float, 3, 1> Array3f;
typedef Array<float, 7, 1> Array7f;
typedef Array<int, 2, 1> Array2i;
typedef Array<int, 3, 1> Array3i;

typedef Matrix<float, Dynamic, 4> MatrixX4f;
typedef Matrix<float, Dynamic, 32> MatrixX32f;
typedef Matrix<int, Dynamic, 3> MatrixX3i;

// const Array3f pc_range_high(35.2, 40, 1);
const Array3f pc_range_high(70.4, 40, 1);
// const Array3f pc_range_low(-35.2, -40, -3);
const Array3f pc_range_low(0, -40, -3);
const Array3f voxel_size(0.2, 0.2, 4);

const int max_points_per_voxel = 50;
const int max_voxels = 12000;
const int num_in_features = 4;
// const int num_out_features = 32;
const int num_out_features = 10;
const int num_hidden_features = 32;

const string weight_root = "/userdata/modeltest/pre_process_out/";
const string linear1_weight_path = weight_root + "linear1_weight_32_50.bin";
const string norm1_weight_path = weight_root + "norm1_weight_4.bin";
const string norm1_bias_path = weight_root + "norm1_bias_4.bin";
const string norm1_running_mean_path = weight_root + "norm1_running_mean_4.bin";
const string norm1_running_var_path = weight_root + "norm1_running_var_4.bin";
const string linear2_weight_path = weight_root + "linear2_weight_26_4.bin";
const string norm2_weight_path = weight_root + "norm2_weight_32.bin";
const string norm2_bias_path = weight_root + "norm2_bias_32.bin";
const string norm2_running_mean_path = weight_root + "norm2_running_mean_32.bin";
const string norm2_running_var_path = weight_root + "norm2_running_var_32.bin";

const int box_coder_size = 7;

const string class_name_Car = "Car";
const Array3f size_Car(1.6, 3.9, 1.56);
const Array3f stride_Car(0.4, 0.4, 0.0);
// const Array3f offset_Car(-35.0, -39.8, -1.78);
const Array3f offset_Car(0.2, -39.8, -1.78);

const string class_name_Pedestrian = "Pedestrian";
const Array3f size_Pedestrian(0.6, 0.8, 1.73);
const Array3f stride_Pedestrian(0.4, 0.4, 0.0);
// const Array3f offset_Pedestrian(-35.0, -39.8, -1.465);
const Array3f offset_Pedestrian(0.2, -39.8, -1.465);

const string class_name_Cyclist = "Cyclist";
const Array3f size_Cyclist(0.6, 1.76, 1.73);
const Array3f stride_Cyclist(0.4, 0.4, 0.0);
// const Array3f offset_Cyclist(-35.0, -39.8, -1.465);
const Array3f offset_Cyclist(0.2, -39.8, -1.465);

const string class_name_Van = "Van";
const Array3f size_Van(1.87103749, 5.02808195, 2.20964255);
const Array3f stride_Van(0.4, 0.4, 0.0);
// const Array3f offset_Van(-35.0, -39.8, -1.78);
const Array3f offset_Van(0.2, -39.8, -1.78);

// const float nms_score_threshold = 0.45;
// 这是argsigmoid，为避免在后处理中计算sigmoid，将cls输出直接与这个值相比
const float nms_score_threshold = -0.20067069546215122;
const float nms_iou_threshold = 0.3;
const int num_pre_max_size = 100;

#endif
