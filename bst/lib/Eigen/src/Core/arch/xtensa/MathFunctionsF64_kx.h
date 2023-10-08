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

#ifndef EIGEN_MATH_FUNCTIONS_F64_KX_XTENSA_H
#define EIGEN_MATH_FUNCTIONS_F64_KX_XTENSA_H

#if F64_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

template <> EIGEN_STRONG_INLINE PacketN_4d psqrt<PacketN_4d>(const PacketN_4d &a) { FUNC_ENTRY; return BBE_SQRTN_4XF64(a); }

template <> EIGEN_STRONG_INLINE PacketN_4d prsqrt<PacketN_4d>(const PacketN_4d &a) {
  FUNC_ENTRY;
  PacketN_4d tvinf = PacketN_4d(INFINITY);
  PacketN_4d tvzero = PacketN_4d(0);
  PacketN_4d tvrsqrt = BBE_RSQRTN_4XF64(a);
  vboolN_4 tvbinf = (a == tvinf);
  vboolN_4 tvbzero = (a == tvzero);
  return PacketN_4d(BBE_MOVN_4XF64T(tvinf,BBE_MOVN_4XF64T(tvzero,tvrsqrt,tvbinf),tvbzero));
}

#if (XCLIB_SUPPORT)
#define SCD(_ptr) xt_reinterpret_const_cast<double,PacketN_4d>(&_ptr)
template <> EIGEN_STRONG_INLINE PacketN_4d psin(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; vecsin(SCD(b),SCD(a),SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d pcos(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; veccos(SCD(b),SCD(a),SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d plog(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; veclog(SCD(b),SCD(a),SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d pexp(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; vecexp(SCD(b),SCD(a),SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d ptanh(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; vectanh(SCD(b),SCD(a),SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d ptan(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; vectan(SCD(b),SCD(a),SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d pacos(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; vecacos(SCD(b),SCD(a),SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d pasin(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; vecasin(SCD(b),SCD(a),SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d patan(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; vecatan(SCD(b),SCD(a),SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d pcosh(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; veccosh(SCD(b),SCD(a),SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d psinh(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; vecsinh(SCD(b),SCD(a),SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d plog10(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; veclog10(SCD(b),SCD(a),SIMD_N_4); return b;}
#undef SCD
#else
#define F64(_pkt,_idx) *(((double*)(&_pkt)) + _idx)
template <> EIGEN_STRONG_INLINE PacketN_4d psin(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; for(int i=0; i<SIMD_N_4;i++){F64(b,i) = sin(F64(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d pcos(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; for(int i=0; i<SIMD_N_4;i++){F64(b,i) = cos(F64(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d plog(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; for(int i=0; i<SIMD_N_4;i++){F64(b,i) = log(F64(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d pexp(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; for(int i=0; i<SIMD_N_4;i++){F64(b,i) = exp(F64(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d ptanh(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; for(int i=0; i<SIMD_N_4;i++){F64(b,i) = tanh(F64(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d ptan(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; for(int i=0; i<SIMD_N_4;i++){F64(b,i) = tan(F64(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d pacos(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; for(int i=0; i<SIMD_N_4;i++){F64(b,i) = acos(F64(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d pasin(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; for(int i=0; i<SIMD_N_4;i++){F64(b,i) = asin(F64(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d patan(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; for(int i=0; i<SIMD_N_4;i++){F64(b,i) = atan(F64(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d pcosh(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; for(int i=0; i<SIMD_N_4;i++){F64(b,i) = cosh(F64(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d psinh(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; for(int i=0; i<SIMD_N_4;i++){F64(b,i) = sinh(F64(a,i));}; return b;}
template <> EIGEN_STRONG_INLINE PacketN_4d plog10(const PacketN_4d& a) { FUNC_ENTRY; PacketN_4d b = 0; for(int i=0; i<SIMD_N_4;i++){F64(b,i) = log10(F64(a,i));}; return b;}
#endif

#if F64_HALF_PACKET_SUPPORT
template <> EIGEN_STRONG_INLINE PacketN_8d psqrt<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(psqrt<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d prsqrt<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(prsqrt<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d psin<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(psin<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d pcos<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(pcos<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d plog<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(plog<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d pexp<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(pexp<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d ptanh<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(ptanh<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d ptan<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(ptan<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d pacos<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(pacos<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d pasin<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(pasin<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d patan<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(patan<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d pcosh<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(pcosh<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d psinh<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(psinh<PacketN_4d>(a.v)); }
template <> EIGEN_STRONG_INLINE PacketN_8d plog10<PacketN_8d>(const PacketN_8d &a) { FUNC_ENTRY; return PacketN_8d(plog10<PacketN_4d>(a.v)); }
#endif

} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_MATH_FUNCTIONS_F64_KX_XTENSA_H
