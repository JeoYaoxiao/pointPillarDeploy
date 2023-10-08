
#ifndef _BST_VIDEO_COMMON_H_
#define _BST_VIDEO_COMMON_H_

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

#include <stddef.h>
#include <stdint.h>

namespace bvideo {

#define MIN_VIDEO_BUF_NUM 3
#define MAX_VIDEO_BUF_NUM 64

enum bst_video_pads {
    VIDEO_INPUT,   // VIDEO_SINK
    VIDEO_OUTPUT,  // VIDEO_SRC
};

enum bst_mem_type {
    MEM_TYPE_INVALID = 0,
    MEM_TYPE_MMAP = 1,
    MEM_TYPE_DMAFD = 2,
    // MEM_TYPE_BOTH = 3,
};

enum video_format {
    BST_PIXEL_FORMAT_UNKNOWN = 0x0,
    // rgb format
    BST_PIXEL_FORMAT_RGBA_8888 = 0x1,
    BST_PIXEL_FORMAT_RGB_888 = 0x2,
    BST_PIXEL_FORMAT_BGR_888 = 0x3,
    BST_PIXEL_FORMAT_RGB_565 = 0x4,
    // yuv format
    BST_PIXEL_FORMAT_UYVY = 0x10,
    BST_PIXEL_FORMAT_YUYV = 0x11,
    BST_PIXEL_FORMAT_NV12 = 0x12,
    BST_PIXEL_FORMAT_NV21 = 0x13,
    // compressed format
    BST_PIXEL_FORMAT_H264 = 0x100,
    BST_PIXEL_FORMAT_MAX,
};

typedef struct _buf_req_t {
    enum bst_mem_type type;
    int count;
} buf_req_t;

typedef struct _video_buf_t {
    enum bst_mem_type type;
    int buf_index;
    int fd;
    uint32_t bytesused;  // number of bytes occupied by data (payload)
    uint32_t length;     // size of this buffer (NOT the payload) in bytes
    void *start;
    uint64_t ts_usec;
    uint32_t sequence;
    uint32_t reserve[7];
} video_buf_t;

typedef struct _image_rect_t {
    int xoffset;
    int yoffset;
    int width;
    int height;
    int wstride;
    int hstride;
    int format;
    int size;
} image_rect_t;

enum {
    RET_OK = 0,
    RET_ERR_COMMON = -1,
    RET_ERR_NOT_SUPPORTED = -2,
    RET_ERR_UNKNOWN = -3,
    RET_ERR_BAD_STATE = -4,
    RET_ERR_OUT_OF_MEMORY = -5,
    RET_ERR_OUT_OF_SPACE = -6,
    RET_ERR_NOT_EXIST = -7,
    RET_ERR_NOT_VALID = -8,  // deprecated, using RET_ERR_INVALID_ARG instead
    RET_ERR_OPEN_FAIL = -9,
    RET_ERR_CLOSE_FAIL = -10,
    RET_ERR_IOCTL_FAIL = -11,
    RET_ERR_MMAP_FAIL = -12,
    RET_TIMEOUT = -13,
    RET_TRYAGAIN = -14,
    RET_ERR_INVALID_ARG = -15,  // invalid argument
    RET_ERR_INTR = -16,         // interrupted
    RET_ERR_NONE_VALIED_CAMERA = -17,
};

typedef struct bst_timeval {
    long tv_sec;
    long tv_usec;
} bst_timeval_t;

typedef unsigned long handle_t;

#define INVALID_HANDLE (0)

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif  // NULL

#ifndef __cplusplus
typedef int bool;
#define false 0
#define true (!0)
#endif

}  // namespace bvideo

#endif
