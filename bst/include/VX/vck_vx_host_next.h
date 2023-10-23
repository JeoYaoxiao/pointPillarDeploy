/*
 * Copyright (c) 2017 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.
 *
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of Cadence Design
 * Systems Inc. They may be adapted and modified by bona fide purchasers for
 * internal use, but neither the original nor any adapted or modified version
 * may be disclosed or distributed to third parties in any manner, medium, or
 * form, in whole or in part, without the prior written consent of Cadence
 * Design Systems Inc or in accordance with the terms of your Design Technology
 * License Agreement with Cadence Design Systems. This software and its
 * derivatives are to be executed solely on products incorporating a Cadence
 * Design Systems processor.
 */

#ifndef VCK_VX_HOST_NEXT_H
#define VCK_VX_HOST_NEXT_H

#include <VX/vx.h>

/* Additional kernel attributes */
enum tenvx_kernel_attribute_next_e {
    TENVX_KERNEL_HINT_USE_EXTRA_LOCAL_DATA = VX_ATTRIBUTE_BASE(VX_ID_CADENCE, VX_TYPE_KERNEL) + 0x10,
    /* For kernel parameters, with tenvxSetKernelParamAttribute */
    TENVX_KERNEL_PARAM_TILE_REQ_BORDER = VX_ATTRIBUTE_BASE(VX_ID_CADENCE, VX_TYPE_KERNEL) + 0x11,
};


#endif // VCK_VX_HOST_NEXT_H
