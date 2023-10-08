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

#ifndef __BST_GWARP_SDK_API_H__
#define __BST_GWARP_SDK_API_H__

#include <stdint.h>
#include "bst_video_common.h"

#define EXPORT_API __attribute__((visibility("default")))

namespace bvideo {

#ifdef __cplusplus
extern "C" {
#endif

#define GWARP_MAX_CHANNEL_NUM 12

#define GWARP_INPUT_BUF_PITCH 0x1
#define GWARP_OUTPUT_BUF_PITCH 0x2
#define GWARP_INPUT_BUF_YUV_SEP 0x3
#define GWARP_OUTPUT_BUF_YUV_SEP 0x4

EXPORT_API const char *gwarp_get_sdk_version(void);

EXPORT_API handle_t gwarp_open_video(int channel_index, int mode);

EXPORT_API int gwarp_init_buf(handle_t handle, buf_req_t *buf_req,
                              image_rect_t *rect, enum bst_video_pads direction,
                              video_buf_t *pbuf);

EXPORT_API int gwarp_set_yuv_separate(handle_t handle, uint32_t yuv_sep_flag);

EXPORT_API int gwarp_set_buf_pitch(handle_t handle, uint32_t pitch_flag);

EXPORT_API int gwarp_config_gdc(handle_t handle, void *init_param,
                                int init_size, void *gdc_param, int gdc_size);

EXPORT_API int gwarp_gdc(handle_t handle, video_buf_t *in_buf,
                         video_buf_t *out_buf);

EXPORT_API int gwarp_resize(handle_t handle, video_buf_t *in_buf,
                            video_buf_t *out_buf);

EXPORT_API int gwarp_put_buf(handle_t handle, video_buf_t *buf,
                             enum bst_video_pads pad);

EXPORT_API int gwarp_close_video(handle_t handle);

#ifdef __cplusplus
}
#endif

}  // namespace bvideo

#endif
