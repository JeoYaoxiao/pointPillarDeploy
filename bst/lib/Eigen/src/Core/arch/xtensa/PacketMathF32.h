/* ------------------------------------------------------------------------ */
/* Copyright (c) 2022 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ('Cadence    */
/* Libraries') are the copyrighted works of Cadence Design Systems Inc.     */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2008-2009 Gael Guennebaud <gael.guennebaud@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EIGEN_PACKET_MATH_F32_XTENSA_H
  #define EIGEN_PACKET_MATH_F32_XTENSA_H
  #if EIGEN_HAVE_SP_VFPU && (XCHAL_HAVE_HIFI5_3 || XCHAL_HAVE_HIFI5 || XCHAL_HAVE_HIFI3Z || XCHAL_HAVE_HIFI1 || XCHAL_HAVE_BBENEP || XCHAL_HAVE_VISION || XCHAL_HAVE_PDX)
	  #if XCHAL_HAVE_HIFI5_3
	    #include "PacketMathF32_hifi5_3.h"
	    #include "PacketMathF32_Half_hifi5_3.h"
  	#elif XCHAL_HAVE_HIFI5
      #include "PacketMathF32_hifi5.h"
  	#elif XCHAL_HAVE_HIFI3Z || XCHAL_HAVE_HIFI1
   	  #include "xt_hifi_compatibility.h"
	    #include "PacketMathF32_hifi5.h"
    #elif XCHAL_HAVE_BBENEP || XCHAL_HAVE_VISION || XCHAL_HAVE_PDX
      #include "PacketMathF32_kx.h"
      #if XCHAL_HAVE_VISION
        #include "PacketMathF32_vx.h"
      #elif XCHAL_HAVE_PDX
        #include "PacketMathF32_gx.h"
      #endif
      #include "PacketMathF32_Half_kx.h"
      #if XCHAL_HAVE_VISION
        #include "PacketMathF32_Half_vx.h"
      #elif XCHAL_HAVE_PDX
        #include "PacketMathF32_Half_gx.h"
      #endif
    #endif
  #else
    #if defined(F32_FULL_PACKET_SUPPORT) && F32_FULL_PACKET_SUPPORT
      #pragma message("Incorrect value for F32_FULL_PACKET_SUPPORT. Resetting it back")
      #undef F32_FULL_PACKET_SUPPORT
      #define F32_FULL_PACKET_SUPPORT 0
    #else
      #define F32_FULL_PACKET_SUPPORT 0
    #endif
    #if defined(F32_HALF_PACKET_SUPPORT) && F32_HALF_PACKET_SUPPORT
      #pragma message("Incorrect value for F32_HALF_PACKET_SUPPORT. Resetting it back")
      #undef F32_HALF_PACKET_SUPPORT
      #define F32_HALF_PACKET_SUPPORT 0
    #else
      #define F32_HALF_PACKET_SUPPORT 0
    #endif
  #endif
#endif
