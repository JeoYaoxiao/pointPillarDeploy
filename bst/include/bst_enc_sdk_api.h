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

#ifndef __BST_ENCODER_SDK_API_H__
#define __BST_ENCODER_SDK_API_H__

#include <stdint.h>
#include "bst_video_common.h"

#define EXPORT_API __attribute__((visibility("default")))

namespace bvideo {

#ifdef __cplusplus
extern "C" {
#endif

#define ENCODER_MAX_CHANNEL_NUM 8
#define ENCODER_SPS_PPS_LEN 32

typedef struct _enc_param {
    int bit_rate;
    int gop;
    int qp;
    int framerate;
    int ref_frame_num;  // reference frame number, currently only 1 and 2 are
                        // supported
    int reserve[11];
} enc_param_t;

typedef struct _enc_buf {
    int buf_index;
    video_buf_t
        video_buf[2];  // video_buf[0]:nv12 and video_buf[1] is 1/16 y for
                       // VIDEO_INPUT, video_buf[0] is h264 for VIDEO_OUTPUT
    int reserve[6];
} enc_buf_t;

EXPORT_API const char *enc_get_sdk_version(void);

EXPORT_API handle_t enc_open_video(int channel_index, int mode);

EXPORT_API int enc_init_buf(handle_t handle, buf_req_t *buf_req,
                            image_rect_t *rect, enum bst_video_pads direction,
                            enc_buf_t *pbuf);

EXPORT_API int enc_set_param(handle_t handle, enc_param_t param);

EXPORT_API int enc_get_sps_pps(handle_t handle, void *sps_pps_addr, int len);

EXPORT_API int enc_do_encode(handle_t handle, enc_buf_t *in_buf,
                             enc_buf_t *out_buf, int *encode_length);

EXPORT_API int enc_put_video_buf(handle_t handle, enc_buf_t *buf,
                                 enum bst_video_pads pad);

EXPORT_API int enc_close_video(handle_t handle);

#ifdef __cplusplus
}
#endif

}  // namespace bvideo

#endif
