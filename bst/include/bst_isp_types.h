/*******************************************************************************
 *  This file contains proprietary information that is the sole intellectual
 *  property of Black Sesame Technologies, Inc. and its affiliates.
 *  No portions of this material may be reproduced in any form without the
 *  written permission of:
 *
 *  Black Sesame Technologies, Inc. and its affiliates
 *  Rm 1102-1108, Building 36, No.2777, East Jinxiu Rd,
 *  Pudong District, Shanghai, China
 *
 *  Copyright \@2020: all right RETerved.
 *  For distribution or production, further Software License Agreement is
 *  required.
 *
 ******************************************************************************/

#ifndef _BST_ISP_TYPES_H_
#define _BST_ISP_TYPES_H_

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include "bst_video_common.h"

namespace bvideo {

#define MAX_VIEW_NUM 3
#define MAX_ISP_VIDEO_NUM 12
#define MAX_CAMERA_PER_GROUP 8
#define MAX_CAMERA_NAME_LEN 32
#define MAX_ISP_CHANNEL 12
#define MAX_FRAME_CYCLE 120

#define YDNS_MAX 10
#define YDNS_MIN 0
#define UVDNS_MAX 10
#define UVDNS_MIN 0
#define SHARPNESS_MAX 10
#define SHARPNESS_MIN 0
#define BRIGHTNESS_MAX 255
#define BRIGHTNESS_MIN 0
#define CONTRAST_MAX 255
#define CONTRAST_MIN 0
#define SATURATION_MAX 255
#define SATURATION_MIN 0
#define HUE_MAX 255
#define HUE_MIN 0

enum {
    ISP_MODE_VIEWS = 1,
    ISP_MODE_PDNS = 2,
};

enum {
    VIDEO_NO_READABLE_BUF = 0,
    VIDEO_HAS_READABLE_BUF = 1,
};

enum {
    INTERNAL_TRIGGER_MODE = 0,
    EXTERNAL_TRIGGER_MODE = 1,
};

enum { GROUP_FLAG_UNIFIED_CAMERA = 0, GROUP_FLAG_DIFFERENT_CAMERA };
/*apply gamma 1.8~2.6*/
typedef enum {
    ISP_GAMMA_1_8 = 0,
    ISP_GAMMA_1_9 = 1,
    ISP_GAMMA_2_0 = 2,
    ISP_GAMMA_2_1 = 3,
    ISP_GAMMA_2_2 = 4,
    ISP_GAMMA_2_3 = 5,
    ISP_GAMMA_2_4 = 6,
    ISP_GAMMA_2_5 = 7,
    ISP_GAMMA_2_6 = 8,
} gamma_value_t;

typedef enum {
    ISP_VIEW_INVALID = 0,
    ISP_VIEW0 = 1,
    ISP_VIEW1 = 2,
    ISP_VIEW0_VIEW1 = 3,
    ISP_VIEWY = 4,
    ISP_VIEW0_VIEWY = 5,
    ISP_VIEW1_VIEWY = 6,
    ISP_VIEW0_VIEW1_VIEWY = 7,
} view_type_t;

typedef struct _isp_buf_t {
    video_buf_t views[MAX_VIEW_NUM];
    int buf_index;  // same to view buf's index
    int reserve[9];
} isp_buf_t;

typedef struct _view_format_t {
    int color_fmt;
    int width;
    int height;
} view_format_t;

typedef struct _camera_info_t {
    char camera_name[MAX_CAMERA_NAME_LEN];
    int camera_id;
    int is_streaming;
} camera_info_t;

struct trigger_info {
    int trigger_mode;
    int mipi_id;
    int internal_trigger_fps;
    int camera_trigger_gpio_port;
    int deser_trigger_gpio_port;
};

typedef struct isp_misc_device {
    camera_info_t cam_info[MAX_ISP_CHANNEL];
    unsigned int camera_num;
} isp_misc_device_t;

typedef struct isp_ctrl {
    uint32_t value;  // isp_set_iqinfo_t  iqVal  ,if ctrl_addr == 0  ; use value
    uint16_t aecManualExp[3];
    uint16_t aecManualGain[3];
    uint16_t manualAWBGain[3][3];  // set mannual white balance value
} isp_ctrl_t;

typedef struct mec_param {
    int longexp;
    int midexp;
    int shortexp;
    int longgain;
    int midgain;
    int shortgain;
} mec_param_t;

typedef enum abnormal_item {
    ABN_INVALID,
    ABN_QUEUE_IRQ,  // DDR NOC queue abnormal interrupt for low queue(offline
                    // mode) or high queue(online mode)
    ABN_RAW2DDR_WR_OVERFLOW,           // RAW to DDR write overflow
    ABN_ISP2DDR_PDNS2DDR_WR_OVERFLOW,  // ISP/PDNS to DDR write overflow
    ABN_HW_QUEUE_IRQ,  // DDR NOC queue abnormal interrupt for low queue(offline
                       // mode) or high queue(online mode)
    ABN_HW_RAW2DDR_WR_OVERFLOW,           // RAW to DDR write overflow
    ABN_HW_ISP2DDR_PDNS2DDR_WR_OVERFLOW,  // ISP/PDNS to DDR write overflow

    ABN_SW_RAWRDYLST_OVERFLOW = 0x80,
    ABN_SW_NO_SYNC_FRAME_DATA,
    ABN_SW_GRP_RAWRDYLST_OVERFLOW,
    ABN_SW_PDNS_P0RDYLST_OVERFLOW,
    ABN_SW_PDNS_P1RDYLST_OVERFLOW,
    ABN_SW_DVP_FRAMEBUF_OVERFLOW,
    ABN_SW_SRAM_ALLOC_OVERFLOW,
    ABN_SW_RAW_BADFRAME,
} abnormal_item_e;

typedef struct abnormal_info {
    uint8_t abnormal_id;
    uint8_t abnormal_type;
    uint32_t last_good_sequence;
    uint32_t total_bad_frames;
    uint32_t total_frames;
} __attribute__((packed)) abnormal_info_t;

typedef struct scale_size {
    uint16_t width;
    uint16_t height;
} scale_size_t;

typedef struct crop_size {
    uint16_t topCropBefore;
    uint16_t botCropBefore;
    uint16_t lefCropBefore;
    uint16_t rigCropBefore;
    uint16_t topCropAfter;
    uint16_t botCropAfter;
    uint16_t lefCropAfter;
    uint16_t rigCropAfter;
} crop_size_t;

typedef struct resolution_resize {
    crop_size_t crop_size;
    scale_size_t scale_size;
    uint8_t view_id;
} resolution_resize_t;

struct crop_rect {
    uint16_t left;
    uint16_t top;
    uint16_t width;
    uint16_t height;
};

typedef struct resize_info {
    struct crop_rect crop_before;
    struct crop_rect crop_after;
    struct scale_size scale;
    uint8_t view_id;
} resize_info_t;

}  // namespace bvideo

#endif  //_BST_VIDEO_TYPES_H_
