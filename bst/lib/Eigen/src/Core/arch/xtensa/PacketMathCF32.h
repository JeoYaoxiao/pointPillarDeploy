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

#ifndef EIGEN_PACKET_MATH_CF32_XTENSA_H
  #define EIGEN_PACKET_MATH_CF32_XTENSA_H
  #if EIGEN_HAVE_SP_VFPU && (XCHAL_HAVE_BBENEP || XCHAL_HAVE_VISION || XCHAL_HAVE_PDX)
    #include "PacketMathCF32_kx.h"
    #if XCHAL_HAVE_VISION
      #include "PacketMathCF32_vx.h"
    #elif XCHAL_HAVE_PDX
      #include "PacketMathCF32_gx.h"
    #endif
    #include "PacketMathCF32_Half_kx.h"
    #if XCHAL_HAVE_VISION
      #include "PacketMathCF32_Half_vx.h"
    #endif
  #elif EIGEN_HAVE_SP_VFPU && (XCHAL_HAVE_HIFI5_3 || XCHAL_HAVE_HIFI1_1)
	  #if XCHAL_HAVE_HIFI5_3
      #include "PacketMathCF32_hifi5_3.h"
  	#else
      #include "PacketMathCF32_hifi1_1.h"
	  #endif
  #else
    #if defined(CF32_FULL_PACKET_SUPPORT) && CF32_FULL_PACKET_SUPPORT
      #pragma message("Incorrect value for CF32_FULL_PACKET_SUPPORT. Resetting it back")
      #undef CF32_FULL_PACKET_SUPPORT
      #define CF32_FULL_PACKET_SUPPORT 0
    #else
      #define CF32_FULL_PACKET_SUPPORT 0
    #endif
    #if defined(CF32_HALF_PACKET_SUPPORT) && CF32_HALF_PACKET_SUPPORT
      #pragma message("Incorrect value for CF32_HALF_PACKET_SUPPORT. Resetting it back")
      #undef CF32_HALF_PACKET_SUPPORT
      #define CF32_HALF_PACKET_SUPPORT 0
    #else
      #define CF32_HALF_PACKET_SUPPORT 0
    #endif
  #endif
#endif
