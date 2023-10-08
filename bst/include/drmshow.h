
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

#ifndef __BST_DRM_SHOW__
#define __BST_DRM_SHOW__

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

// init primary and osd plane's buffer internally
extern int display_init_by_internal(int width, int height);

// init osd plane's buffer internally, primary buffer passed by external buffer
// fd
extern int display_init_by_fd(int width, int height, int buf_fd_array[],
                              uint32_t buf_num);

extern void display_deinit(void);

// get internal primary plane buffer, supports NV12 format
extern int get_primary_plane_next_buf(uint8_t **buf_ptr, int *buf_size);

// get internal osd plane buffer, supports ARGB format
extern int get_osd_plane_next_buf(uint8_t **buf_ptr, int *buf_size);

// commit primary and osd layer, primary by buffer pointer
extern int display_commit_by_buf(uint8_t *primary_buf, uint8_t *osd_buf);

// commit primary and osd layer, primary by buffer fd
extern int display_commit_by_fd(int fd_index, uint8_t *osd_buf);

#if defined(__cplusplus)
}
#endif

#endif
