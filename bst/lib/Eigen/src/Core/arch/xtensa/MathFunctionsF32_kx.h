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

#ifndef EIGEN_MATH_FUNCTIONS_F32_KX_XTENSA_H
#define EIGEN_MATH_FUNCTIONS_F32_KX_XTENSA_H

#if F32_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

template <> EIGEN_STRONG_INLINE PacketN_2f psqrt<PacketN_2f>(const PacketN_2f &a) { FUNC_ENTRY; return BBE_SQRTN_2XF32(a); }

template <> EIGEN_STRONG_INLINE PacketN_2f prsqrt<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  PacketN_2f tvinf = PacketN_2f(INFINITY);
  PacketN_2f tvzero = PacketN_2f(0);
  PacketN_2f tvrsqrt = BBE_RSQRTN_2XF32(a);
  vboolN_2 tvbinf = (a == tvinf);
  vboolN_2 tvbzero = (a == tvzero);
  return PacketN_2f(BBE_MOVN_2XF32T(tvinf,BBE_MOVN_2XF32T(tvzero,tvrsqrt,tvbinf),tvbzero));
}

#if (XCLIB_SUPPORT)
template <> EIGEN_STRONG_INLINE PacketN_2f psin(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; vecsinf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f pcos(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; veccosf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f plog(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; veclogf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f pexp(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; vecexpf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f ptanh(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; vectanhf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f ptan(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; vectanf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f pacos(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; vecacosf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f pasin(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; vecasinf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f patan(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; vecatanf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f pcosh(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; veccoshf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f psinh(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; vecsinhf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f plog10(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; veclog10f((float*)&b,(float*)&a,SIMD_N_2); return b;}
#else
#define F32(_pkt,_idx) *(((float*)(&_pkt)) + _idx)
template <> EIGEN_STRONG_INLINE PacketN_2f psin(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; for(int i=0; i<SIMD_N_2;i++){F32(b,i) = sin(F32(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f pcos(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; for(int i=0; i<SIMD_N_2;i++){F32(b,i) = cos(F32(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f plog(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; for(int i=0; i<SIMD_N_2;i++){F32(b,i) = log(F32(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f pexp(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; for(int i=0; i<SIMD_N_2;i++){F32(b,i) = exp(F32(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f ptanh(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; for(int i=0; i<SIMD_N_2;i++){F32(b,i) = tanh(F32(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f ptan(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; for(int i=0; i<SIMD_N_2;i++){F32(b,i) = tan(F32(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f pacos(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; for(int i=0; i<SIMD_N_2;i++){F32(b,i) = acos(F32(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f pasin(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; for(int i=0; i<SIMD_N_2;i++){F32(b,i) = asin(F32(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f patan(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; for(int i=0; i<SIMD_N_2;i++){F32(b,i) = atan(F32(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f pcosh(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; for(int i=0; i<SIMD_N_2;i++){F32(b,i) = cosh(F32(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f psinh(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; for(int i=0; i<SIMD_N_2;i++){F32(b,i) = sinh(F32(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f plog10(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = 0; for(int i=0; i<SIMD_N_2;i++){F32(b,i) = log10(F32(a,i));}; return b;}
#endif

#if F32_HALF_PACKET_SUPPORT
template <> EIGEN_STRONG_INLINE PacketN_4f psqrt<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(psqrt<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f prsqrt<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(prsqrt<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f psin<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(psin<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f pcos<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(pcos<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f plog<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(plog<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f pexp<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(pexp<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f ptanh<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(ptanh<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f ptan<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(ptan<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f pacos<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(pacos<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f pasin<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(pasin<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f patan<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(patan<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f pcosh<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(pcosh<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f psinh<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(psinh<PacketN_2f>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_4f plog10<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; return PacketN_4f(plog10<PacketN_2f>(a.v)); }
#endif

} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_MATH_FUNCTIONS_F32_KX_XTENSA_H
