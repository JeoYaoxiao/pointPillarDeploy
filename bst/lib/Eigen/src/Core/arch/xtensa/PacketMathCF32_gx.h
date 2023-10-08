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

#ifndef EIGEN_PACKET_MATH_CF32_GX_XTENSA_H
#define EIGEN_PACKET_MATH_CF32_GX_XTENSA_H

#if CF32_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

template <> EIGEN_STRONG_INLINE PacketN_4cf pandnot(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  return PacketN_4cf(VEC_XCF32_FROM_VEC_X32(PDX_AND_MX32(VEC_X32_FROM_VEC_XCF32(a.v), PDX_NOT_MX32(VEC_X32_FROM_VEC_XCF32(b.v)))));
}

template <> EIGEN_DEVICE_FUNC inline PacketN_4cf pgather<complexfloat, PacketN_4cf>(const complexfloat *from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtcomplexfloat *tp = xt_reinterpret_const_cast<xtcomplexfloat,complexfloat>(from);
  xb_vecM2xcf32 ta;
  xtcomplexfloat *tpf = xt_reinterpret_cast<xtcomplexfloat,xb_vecM2xcf32>(&ta);
  int    stride_bytes = sizeof(xtcomplexfloat) * stride;
  xb_vecM2xcf32 tvf;
  PDX_LSR_CF32_XP(tvf, tp, stride_bytes);
  PDX_SS_CF32_IP(tvf, tpf, sizeof(xtcomplexfloat));
  PDX_LSR_CF32_XP(tvf, tp, stride_bytes);
  PDX_SS_CF32_IP(tvf, tpf, sizeof(xtcomplexfloat));
  return PacketN_4cf(ta);
}

} // end namespace internal

} // end namespace Eigen


#endif
#endif // EIGEN_PACKET_MATH_CF32_GX_XTENSA_H
