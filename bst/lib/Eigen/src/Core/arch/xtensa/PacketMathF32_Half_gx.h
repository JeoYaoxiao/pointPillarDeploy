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

#ifndef EIGEN_PACKET_MATH_F32_HALF_GX_XTENSA_H
#define EIGEN_PACKET_MATH_F32_HALF_GX_XTENSA_H

#if F32_FULL_PACKET_SUPPORT

#if F32_HALF_PACKET_SUPPORT
namespace Eigen {

namespace internal {
template <> EIGEN_DEVICE_FUNC inline PacketN_4f pgather<float, PacketN_4f>(const float *from, Index stride) {
  FUNC_ENTRY;
  float *tp = (float *)(from);
  xb_vecMxf32  ta;
  float *tpf = (float *)(&ta);
  int    i;
  int    stride_bytes = sizeof(float) * stride;
  float tvf;
  for (i = 0; i < SIMD_N_4; i++) {
    PDX_LS_F32_XP(tvf, tp, stride_bytes);
    PDX_SS_F32_IP(tvf, tpf, sizeof(float));
  }
  return PacketN_4f(ta);
}

template <> EIGEN_DEVICE_FUNC inline void pscatter<float, PacketN_4f>(float *to, const PacketN_4f &from, Index stride) {
  FUNC_ENTRY;
  float *tpf = (float*)(&from.v);
  float *tpt = (float*)(to);
  int    stride_bytes = sizeof(float) * stride;
  float tvf;
  int i;
  for (i = 0; i < SIMD_N_4; i++)
  {
    PDX_LS_F32_IP(tvf, tpf, sizeof(float));
    PDX_SS_F32_XP(tvf, tpt, stride_bytes);
  } 
}

} // end namespace internal

} // end namespace Eigen

#endif

#endif

#endif // EIGEN_PACKET_MATH_F32_HALF_GX_XTENSA_H
