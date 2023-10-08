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

#ifndef EIGEN_MATH_FUNCTIONS_F32_HIFI5_XTENSA_H
#define EIGEN_MATH_FUNCTIONS_F32_HIFI5_XTENSA_H

#if F32_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

#if SIMD_N_2_EQ_2()
template <> EIGEN_STRONG_INLINE PacketN_2f psqrt<PacketN_2f>(const PacketN_2f &a) { FUNC_ENTRY; return SQRT_SX2(a); }
template <> EIGEN_STRONG_INLINE PacketN_2f prsqrt<PacketN_2f>(const PacketN_2f &a) {
    FUNC_ENTRY;
    PacketN_2f tvinf = PacketN_2f(INFINITY);
    PacketN_2f tvzero = PacketN_2f(0.0f);
    PacketN_2f tvrsqrt = RSQRT_SX2(a);
    xtbool2 tvbinf = OEQ_SX2(a,tvinf);
    xtbool2 tvbzero = OEQ_SX2(a,tvzero);
    MOVT_SX2(tvrsqrt,tvzero,tvbinf);
    MOVT_SX2(tvrsqrt,tvinf,tvbzero);
    return tvrsqrt;
}
#elif SIMD_N_2_EQ_4()
template <> EIGEN_STRONG_INLINE PacketN_2f psqrt<PacketN_2f>(const PacketN_2f &a) { FUNC_ENTRY; return SQRT_SX4(a); } 
template <> EIGEN_STRONG_INLINE PacketN_2f prsqrt<PacketN_2f>(const PacketN_2f &a) {
    FUNC_ENTRY;
    PacketN_2f tvinf = PacketN_2f(INFINITY);
    PacketN_2f tvzero = PacketN_2f(0.0f);
    PacketN_2f tvrsqrt = RSQRT_SX4(a);
	xtbool4 tvbinf = OEQ_SX4(a,tvinf);
    xtbool4 tvbzero = OEQ_SX4(a,tvzero);
    MOVT_SX4(tvrsqrt,tvzero,tvbinf);
    MOVT_SX4(tvrsqrt,tvinf,tvbzero);
    return tvrsqrt;
}
#endif

#if (XCLIB_SUPPORT)
template <> EIGEN_STRONG_INLINE PacketN_2f psin(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b; vecsinf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f pcos(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b; veccosf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f plog(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b; veclogf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f ptan(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b; vectanf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f patan(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b; vecatanf((float*)&b,(float*)&a,SIMD_N_2); return b;}
template <> EIGEN_STRONG_INLINE PacketN_2f plog10(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b; veclog10f((float*)&b,(float*)&a,SIMD_N_2); return b;}
#else
#define F32(_pkt,_idx) *(((float*)(&_pkt)) + _idx)
template <> EIGEN_STRONG_INLINE PacketN_2f pexp(const PacketN_2f& a) { FUNC_ENTRY; PacketN_2f b = CONST_S(0); for(int i=0; i<SIMD_N_2;i++){F32(b,i) = exp(F32(a,i));}; return b;}
#endif

#if F32_HALF_PACKET_SUPPORT
template <> EIGEN_STRONG_INLINE PacketN_4f psqrt<PacketN_4f>(const PacketN_4f &a) { FUNC_ENTRY; PacketN_4f tv; tv.v = SQRT_SX2(a.v); return tv; }
template <> EIGEN_STRONG_INLINE PacketN_4f prsqrt<PacketN_4f>(const PacketN_4f &a) { 
    FUNC_ENTRY;
    xtfloatx2 tvinf = INFINITY;
    xtfloatx2 tvzero = 0.0f;
    PacketN_4f tvrsqrt;
	tvrsqrt.v = RSQRT_SX2(a.v);
	xtbool2 tvbinf = OEQ_SX2(a.v,tvinf);
    xtbool2 tvbzero = OEQ_SX2(a.v,tvzero);
    MOVT_SX2(tvrsqrt.v,tvzero,tvbinf);
    MOVT_SX2(tvrsqrt.v,tvinf,tvbzero);
    return tvrsqrt;
}

#if (XCLIB_SUPPORT)
template <> EIGEN_STRONG_INLINE PacketN_4f psin(const PacketN_4f& a) { FUNC_ENTRY; PacketN_4f b; vecsinf((float*)&b.v,(float*)&a.v,SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4f pcos(const PacketN_4f& a) { FUNC_ENTRY; PacketN_4f b; veccosf((float*)&b.v,(float*)&a.v,SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4f plog(const PacketN_4f& a) { FUNC_ENTRY; PacketN_4f b; veclogf((float*)&b.v,(float*)&a.v,SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4f ptan(const PacketN_4f& a) { FUNC_ENTRY; PacketN_4f b; vectanf((float*)&b.v,(float*)&a.v,SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4f patan(const PacketN_4f& a) { FUNC_ENTRY; PacketN_4f b; vecatanf((float*)&b.v,(float*)&a.v,SIMD_N_4); return b;}
template <> EIGEN_STRONG_INLINE PacketN_4f plog10(const PacketN_4f& a) { FUNC_ENTRY; PacketN_4f b; veclog10f((float*)&b.v,(float*)&a.v,SIMD_N_4); return b;}
#else
#define F32(_pkt,_idx) *(((float*)(&_pkt)) + _idx)
template <> EIGEN_STRONG_INLINE PacketN_4f pexp(const PacketN_4f& a) { FUNC_ENTRY; PacketN_4f b; b.v = CONST_S(0); for(int i=0; i<SIMD_N_4;i++){F32(b.v,i) = exp(F32(a.v,i));}; return b;}
#endif

#endif

} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_MATH_FUNCTIONS_F32_HIFI5_XTENSA_H
