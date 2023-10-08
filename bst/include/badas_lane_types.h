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

#ifndef BADAS_LANE_TYPES_H_
#define BADAS_LANE_TYPES_H_

#include <map>
#include <vector>
#include "opencv2/opencv.hpp"

namespace Badas {

typedef std::map<int, cv::Mat> IntMatPair;
typedef std::map<int, int> IntIntPair;
typedef std::map<int, std::vector<cv::Mat>> IntVectorMatPair;
typedef std::map<int, std::vector<int>> IntVectorIntPair;
typedef std::map<int, std::vector<float>> IntVectorFloatPair;
typedef std::map<int, float> IntFloatPair;
typedef std::map<int, std::vector<float>> IntVectorFloatPair;

typedef cv::Vec<float, 2> Vec2f;  // cxcore.hpp (254)
typedef std::map<int, cv::Vec3b> IntVec3bPair;

typedef std::map<int, std::vector<std::vector<float>>> IntVectorVectorFloatPair;
typedef std::map<int, std::string> IntStringPair;
typedef std::map<int, bool> IntBoolPair;

typedef struct {
    IntVectorFloatPair dict_x;
    IntVectorFloatPair dict_y;
    IntVectorFloatPair dict_x_per;
    IntVectorFloatPair dict_y_per;
    IntVectorFloatPair dict_lane_parameters;
    IntFloatPair dict_lane_position;
    std::vector<int> merged_index;
} TracePointsResult;

typedef struct {
    std::vector<cv::Mat> lanes_per;
    std::vector<int> lane_ids;
    std::vector<cv::Mat> lanes_bev;
    std::vector<std::vector<float>> lane_params;
    std::vector<int> lane_shapes;
    std::vector<int> lane_colors;
    std::vector<float> lane_probs;
    std::vector<float> lane_positions;
} TrackLanesResult;

typedef struct {
    cv::Mat pts_per;
    std::vector<float> y_per;
    std::vector<float> x_per;
    cv::Mat curb_pts_per;
    std::vector<float> curb_y_per;
    std::vector<float> curb_x_per;
    cv::Mat pts_per_horizon;
    std::vector<float> y_per_horizon;
    std::vector<float> x_per_horizon;
    cv::Mat curb_pts_per_horizon;
    std::vector<float> curb_y_per_horizon;
    std::vector<float> curb_x_per_horizon;
} FindLocalMinimumResult;

typedef struct {
    int index;
    float distance;
} ComputeDistancePointsResult;

typedef struct {
    IntVectorFloatPair dict_lane_parameters;
    IntFloatPair dict_lane_position;
    std::vector<int> merged_index;
} MergeLaneResult;

typedef struct {
    float f_x;
    float f_y;
    float u_x;
    float u_y;
    float camera_height;
    float pitch;
    float yaw;
    float vanishing_y;  // y coordinate of vanishing point in net's input image
                        // (after preprocessing, 1280x640)
} CameraInfoHolder;

typedef struct {
    int input_width;
    int input_height;
    int out_width;
    int out_height;
    int left;
    int right;
    int top;
    int bottom;
} IpmInfoHolder;

typedef struct {
    int index;
    float distance;
} ComputeDistanceLanesResult;

typedef struct {
    int shape_argmax;
    int color_argmax;
    float lane_prob;
} LaneShapeColorProbResult;

}  // namespace Badas

#endif  // BADAS_LANE_TYPES_H_
