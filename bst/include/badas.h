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

#ifndef BADAS_H_
#define BADAS_H_

#include <unistd.h>

#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <opencv2/core/mat.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "badas_config.h"

namespace Badas {

//
typedef struct {
    int32_t major;
    int32_t minor;
    int32_t patch;
    int32_t year;
    int32_t mon;
    int32_t day;
    int32_t hour;
    int32_t minute;
} BADAS_Version;

//
#define MAX_NUM_TENSORS 32
#define MAX_NUM_NDIMS 5
#define MAX_TENSOR_NAME 255

// debug level
#define DBG_SILENT 0
#define DBG_MINIMAL 1
#define DBG_NORMAL 2
#define DBG_VERBOSE 3
#define DBG_DEBUG 4

// return code
#define RET_OK 0
#define RET_FILE_NONEXIST -1
#define RET_FILE_INVALID -2
#define RET_LOAD_GRAPH_FAIL -3
#define RET_TF_CONFIG_FAIL -4
#define RET_TF_SESSION_FAIL -5
#define RET_TF_TENSOR_FAIL -6
#define RET_TF_RUN_FAIL -7
#define RET_ALLOC_MEM_FAIL -8
#define RET_BSNN_IMPORT_FAIL -9
#define RET_BSNN_LOAD_FAIL -10
#define RET_BSNN_MEM_FAIL -11
#define RET_BSNN_UNLOAD_FAIL -12
#define RET_BSNN_INPUT_FAIL -13
#define RET_BSNN_RUN_FAIL -14
#define RET_NET_VER_INVALID -15
#define RET_NET_VER_MISMATCH -16
#define RET_PARAM_INVALID -17
#define RET_BSNN_OUTPUT_FAIL -18
#define RET_BSNN_REL_OUT_FAIL -19
#define RET_NN_LOAD_FAIL -20
#define RET_INPUT_INVALID -21
#define RET_LWNN_LOAD_FAIL -22
#define RET_LWNN_MEM_FAIL -23
#define RET_LWNN_UNLOAD_FAIL -24
#define RET_LWNN_INPUT_FAIL -25
#define RET_LWNN_RUN_FAIL -26
#define RET_LWNN_OUTPUT_FAIL -27
#define RET_LWNN_REL_OUT_FAIL -28
#define RET_LWNN_PROC_OUT_FAIL -29
#define RET_BSNN_ERR -30

// class_ind
#define OBJ_CLS_BK 0
#define OBJ_CLS_CAR 1
#define OBJ_CLS_TRUCK 2
#define OBJ_CLS_BUS 3
#define OBJ_CLS_BICYCLE 4
#define OBJ_CLS_MOTOCYCLE 5
#define OBJ_CLS_PEDESTRAIN 6
#define OBJ_CLS_CYCLIST 7
#define OBJ_CLS_TRAFFIC_LIGHT 8
#define OBJ_CLS_INFO_SIGN 9
#define OBJ_CLS_SPECIAL_VEHICLE 10
#define OBJ_CLS_TRICYCLE 11
#define OBJ_CLS_SUV 12
#define OBJ_CLS_VAN 13
#define OBJ_CLS_CONE 14
#define OBJ_CLS_PLATE 15

// traffic light state (one-stage detected)
#define TFL_NONE_ARROW_NULL 0
#define TFL_NONE_ARROW_RED 1
#define TFL_NONE_ARROW_YELLOW 2
#define TFL_NONE_ARROW_GREEN 3
#define TFL_OTHER_LIGHTS 4
#define TFL_OFF 5

// traffic light state (second-stage detected)
#define TLR_UNKNOWN 0
#define TLR_ROUND_GREEN 1
#define TLR_ROUND_YELLOW 2
#define TLR_ROUND_RED 3
#define TLR_LEFT_GREEN 4
#define TLR_LEFT_YELLOW 5
#define TLR_LEFT_RED 6
#define TLR_FWD_GREEN 7
#define TLR_FWD_YELLOW 8
#define TLR_FWD_RED 9
#define TLR_RIGHT_GREEN 10
#define TLR_RIGHT_YELLOW 11
#define TLR_RIGHT_RED 12

// traffic light state (second-stage detected)
#define TSR_PL5 0
#define TSR_PL20 1
#define TSR_PL30 2
#define TSR_PL40 3
#define TSR_PL50 4
#define TSR_PL60 5
#define TSR_PL70 6
#define TSR_PL80 7
#define TSR_PL100 8
#define TSR_PL120 9
#define TSR_PH2_4 10
#define TSR_PH3 11
#define TSR_PH3_8 12
#define TSR_PH4 13
#define TSR_PH4_2 14
#define TSR_PH4_3 15
#define TSR_PH4_5 16
#define TSR_PH5 17
#define TSR_PM10 18
#define TSR_PM20 19
#define TSR_PM30 20
#define TSR_PM40 21
#define TSR_PM55 22
#define TSR_PR30 23
#define TSR_PR40 24
#define TSR_IL50 25
#define TSR_IL60 26
#define TSR_IL70 27
#define TSR_IL80 28
#define TSR_IL90 29
#define TSR_IL100 30
#define TSR_I2 31
#define TSR_I4 32
#define TSR_P10 33
#define TSR_PB 34
#define TSR_PNE 35
#define TSR_UNKNOWN 36

// arrow class ind
#define ARR_FWD 0
#define ARR_LEFT 1
#define ARR_RIGHT 2
#define ARR_UTURN 3

/**
 * @brief bounding box
 *
 */
typedef struct {
    float xmin, ymin, xmax, ymax;
} BBox;

/**
 * @brief 3D bounding box
 *
 */
typedef struct {
    std::vector<std::pair<float, float>>
        vertices;  // the positions of the vertices composing the bounding box
    std::vector<bool>
        vertices_validity;  // the validity of each vertex for display
    float xmin, ymin, xmax,
        ymax;  // these values are kept for interface consistancy
    int yaw;   // yaw angle in degree
} BBox3d;

/**
 * @brief Bounding Box info
 *
 */
typedef struct {
    BBox bbox;
    BBox3d bbox3d;

    float score;       /*!< The class score for current bounding box*/
    uint8_t class_ind; /*!< The class index for bounding box*/

    //
    uint8_t ort_ind; /*!< Orientation class index for bounding box*/
    float ort_score; /*!< Orientation regression score and rotation angle*/
    float ort_reg;

    //
    float obj_dist;

    // only
    uint8_t tf_ind; /*!< when class_ind == traffic_light */
    float tf_score;

    uint8_t class_tlr_ind;
    uint8_t class_tsr_type;
    uint8_t class_tsr_value;

    // cipv
    bool is_cipv;
    float cipv_score;

} BBoxInfo;

/**
 * @brief New lane info
 *
 */
typedef struct {
    std::vector<int> lane_ids;
    std::vector<int> lane_shapes;
    std::vector<int> lane_colors;
    std::vector<float> lane_probs;
    std::vector<int> curbside_ids;
    // perspective
    std::vector<cv::Mat> lanes_per;
    std::vector<cv::Mat> curbside_per;
    std::vector<cv::Mat> lanes_width;
} LaneDetectionResult;

/**
 * @brief Stop Line info
 *
 */
typedef struct {
    bool if_sl_exist;    // if stopline exist,
    int x_begin;         // xmin on perspective, non-nomalized
    int x_end;           // xmin on perspective, non-nomalized
    float poly_coef[3];  // polynomial coefficient: [0th order, 1st order, 2nd
                         // order], non-nomalized
} StoplineInfo;

/**
 * @brief Cross Walk info
 *
 */
typedef struct {
    bool if_cw_exist;  // if crosswalk exist
    std::vector<std::vector<cv::Point2f>>
        list_of_polygons;  // crosswalk polygons, from topleft to bottom left
                           // (clockwise)
} CrosswalkInfo;

/**
 * @brief Arrow info
 *
 */
typedef struct {
    uint8_t class_ind;  // arrow class id
    float score;        // arrow score
    cv::Point awp;      // arrow tip point
} ArrowInfo;

/**
 * @brief ADAS info
 *
 */
typedef struct {
    std::vector<BBoxInfo> bbox_info;
    std::vector<std::pair<float, float>> fs_info;

    //
    std::vector<std::pair<float, float>>
        lane_info; /*!< Each element indicates the position of a pixel (h, w)
                     that belongs to lane*/

    std::vector<std::pair<float, float>>
        curbside_info; /*!< Each element indicates the position of a pixel (h,
                     w) that belongs to curbside*/

    LaneDetectionResult lane_info_ex; /*!< Extended lane info */
    StoplineInfo sl_info;
    CrosswalkInfo cw_info;
    std::vector<ArrowInfo> aw_info;
    std::vector<std::vector<cv::Point>> pole_info;
} AdasInfo;

}  // namespace Badas
#endif  // BADAS_H_
