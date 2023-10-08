#ifndef __BST_ISP_SDK_API_H__
#define __BST_ISP_SDK_API_H__

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

#include "bst_isp_types.h"

#define EXPORT_API __attribute__((visibility("default")))

namespace bvideo {

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * @retval
 *
 **/
EXPORT_API const char *isp_get_sdk_version(void);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API handle_t isp_open_video(int video_id, int mode);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_close_video(handle_t handle);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_init_video(handle_t handle, view_type_t view_type,
                              int buf_count);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_start_capture(handle_t handle);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_stop_capture(handle_t handle);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_get_frame_buf(handle_t handle, isp_buf_t *buf_info,
                                 int timeout_ms);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_set_frame_drop_num(handle_t handle, int frame_cycle,
                                      int drop_num);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_get_expected_frame(handle_t handle, isp_buf_t *buf_info,
                                      int timeout_ms);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_put_frame_buf(handle_t handle, isp_buf_t *buf_info);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_put_frame_buf_by_index(handle_t handle, int buf_index);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_get_view_buf_fd(handle_t handle, int buf_index,
                                   int view_index);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_get_view_format(handle_t handle, int view_id,
                                   view_format_t *format);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_wait_for_data(handle_t *phandle, int *pbuf_flag,
                                        int camera_count,
                                        bst_timeval_t *ptimeout);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_get_tick(handle_t handle, uint64_t *ptick);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_set_brightness(handle_t handle, int value);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_get_brightness(handle_t handle, int *pvalue);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_set_contrast(handle_t handle, int value);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_get_contrast(handle_t handle, int *pvalue);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_set_hue(handle_t handle, int value);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_get_hue(handle_t handle, int *pvalue);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_set_saturation(handle_t handle, int value);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_get_saturation(handle_t handle, int *pvalue);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_set_sharpness(handle_t handle, int value);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_get_sharpness(handle_t handle, int *pvalue);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_set_denoise(handle_t handle, int y_value,
                                      int uv_value);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_get_denoise(handle_t handle, int *py_value,
                                      int *puv_value);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_set_aec(handle_t handle, bool is_enable);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_get_aec(handle_t handle, bool *pis_enable);

/**
 * @brief Set exposure manually
 * @param
 * @retval
 *
 **/
EXPORT_API int isp_camera_set_mec(handle_t handle, mec_param_t mec_param);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_set_awb(handle_t handle, bool is_enable);

/**
 * @brief  Set white balance manually
 * @param r_gain Red gain value
 * @param g_gain green gain value
 * @param b_gain blue gain value
 * @retval
 *
 **/
EXPORT_API int isp_camera_set_mwb(handle_t handle, int r_gain, int g_gain,
                                  int b_gain);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_get_awb(handle_t handle, bool *pis_enable);

/**
 * @brief
 * @param [in]
 * @retval
 *
 **/
EXPORT_API int isp_camera_set_gamma(handle_t handle, gamma_value_t gamma_value);

/**
 * @brief
 * @param [in]
 *
 * @retval
 *
 **/
EXPORT_API int isp_camera_get_gamma(handle_t handle,
                                    gamma_value_t *pgamma_value);

EXPORT_API int isp_camera_get_abnormal_info(handle_t handle,
                                            abnormal_info_t *abnormal_info);

EXPORT_API int isp_resize_resolution(handle_t handle,
                                     resize_info_t resize_info);

/**
 * @brief
 * @param [in]
 *
 * @retval
 *
 **/
EXPORT_API int isp_get_valid_camera_count(int *pcount);

/**
 * @brief
 * @param [in]
 *
 * @retval
 *
 **/
EXPORT_API int isp_get_valid_camera_info(camera_info_t *pinfo, int count,
                                         int *valied_camera_num);

EXPORT_API int isp_set_internal_trigger(handle_t handle, int deser_id,
                                        int camera_gpio, int fps);

EXPORT_API int isp_set_external_trigger(handle_t handle, int deser_id,
                                        int camera_gpio, int deser_gpio);

EXPORT_API int isp_create_empty_video_group(handle_t *group_handle, int flag,
                                            char *group_name, int camera_count);

EXPORT_API int isp_create_video_group(handle_t *group_handle, int flag,
                                      char *group_name, int *camera_id,
                                      int camera_count);

EXPORT_API int isp_get_single_camera_handle(handle_t group_handle,
                                            unsigned int camera_id,
                                            handle_t *video_handle);

EXPORT_API int isp_group_init_video(handle_t group_handle,
                                    view_type_t view_type, int buf_count);

EXPORT_API int isp_group_get_video_info(handle_t group_handle, int camera_id,
                                        camera_info_t *cam_info);

// EXPORT_API int isp_group_set_view_format(handle_t group_handle, int view_id,
// view_format_t format);

// EXPORT_API int isp_group_get_view_format(handle_t group_handle, int view_id,
// view_format_t* pformat);

EXPORT_API int isp_group_set_internal_trigger(handle_t group_handle,
                                              int deser_id, int camera_gpio,
                                              int fps);

EXPORT_API int isp_group_set_external_trigger(handle_t group_handle,
                                              int deser_id, int camera_gpio,
                                              int deser_gpio);

EXPORT_API int isp_group_get_video_handle(handle_t group_handle, int camera_id,
                                          handle_t *camera_handle);

EXPORT_API int isp_group_open_video(handle_t group_handle, int mode,
                                    int *video_status);

EXPORT_API int isp_group_close_video(handle_t group_handle);

EXPORT_API int isp_group_start_captrue(handle_t group_handle);

EXPORT_API int isp_group_stop_captrue(handle_t group_handle);

// EXPORT_API int isp_group_get_sync_frame(handle_t group_handle,
//                                         isp_buf_t *pbuf_info, int buf_num,
//                                         int timeout_us);
// /*check buf_num*/
// EXPORT_API int isp_group_put_frame(handle_t group_handle, isp_buf_t
// *pbuf_info,
//                                    int buf_num);

#ifdef __cplusplus
}
#endif

}  // namespace bvideo

#endif
