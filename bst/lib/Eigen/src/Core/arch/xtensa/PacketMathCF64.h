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

#ifndef EIGEN_PACKET_MATH_CF64_XTENSA_H
  #define EIGEN_PACKET_MATH_CF64_XTENSA_H
  #if EIGEN_HAVE_DP_VFPU && (XCHAL_HAVE_BBENEP || (XCHAL_HAVE_VISION && SIMD_N_2_EQ_32()))
    #include "PacketMathCF64_kx.h"
    #if XCHAL_HAVE_VISION
      #include "PacketMathCF64_vx.h"
    #elif XCHAL_HAVE_PDX
      #pragma message("Support not yet added for XCHAL_HAVE_PDX")
    #endif
    #include "PacketMathCF64_Half_kx.h"
    #if XCHAL_HAVE_VISION
      #include "PacketMathCF64_Half_vx.h"
    #endif
  #else
    #if defined(CF64_FULL_PACKET_SUPPORT) && CF64_FULL_PACKET_SUPPORT
      #pragma message("Incorrect value for CF64_FULL_PACKET_SUPPORT. Resetting it back")
      #undef CF64_FULL_PACKET_SUPPORT
      #define CF64_FULL_PACKET_SUPPORT 0
    #else
      #define CF64_FULL_PACKET_SUPPORT 0
    #endif
    #if defined(CF64_HALF_PACKET_SUPPORT) && CF64_HALF_PACKET_SUPPORT
      #pragma message("Incorrect value for CF64_HALF_PACKET_SUPPORT. Resetting it back")
      #undef CF64_HALF_PACKET_SUPPORT
      #define CF64_HALF_PACKET_SUPPORT 0
    #else
      #define CF64_HALF_PACKET_SUPPORT 0
    #endif
  #endif
#endif
