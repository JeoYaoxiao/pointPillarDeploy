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

#ifndef EIGEN_MATH_FUNCTIONS_CF32_KX_XTENSA_H
#define EIGEN_MATH_FUNCTIONS_CF32_KX_XTENSA_H

#if CF32_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

// Placeholder intrinsics
template <> EIGEN_STRONG_INLINE PacketN_4cf psqrt<PacketN_4cf>(const PacketN_4cf &a) { FUNC_ENTRY; return a; }
#if CF32_HALF_PACKET_SUPPORT
template <> EIGEN_STRONG_INLINE PacketN_8cf psqrt<PacketN_8cf>(const PacketN_8cf &a) {
  FUNC_ENTRY;
  PacketN_4cf tva = replicate_N_8cf<PacketN_4cf>(a.v);
  return PacketN_8cf(psqrt<PacketN_4cf>(tva));
}
#endif

} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_MATH_FUNCTIONS_CF32_KX_XTENSA_H
