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

#ifndef EIGEN_PACKET_MATH_F32_HALF_HIFI5_3_XTENSA_H
#define EIGEN_PACKET_MATH_F32_HALF_HIFI5_3_XTENSA_H

#if F32_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

#if F32_HALF_PACKET_SUPPORT
template <> struct unpacket_traits<PacketN_4f> {
  typedef float type;
  typedef PacketN_4f half;
  enum {size=SIMD_N_4, alignment=ALIGN_N_2, vectorizable=true, masked_load_available=false, masked_store_available=false};
};

inline std::ostream & operator <<(std::ostream & s, const PacketN_4f & a) {
  float *ptr = (float *)(&a.v);
  int itr = 0;
  for (itr = 0; itr < (SIMD_N_4-1); itr++)
  {
    s << ptr[itr] << ", ";
  }
  s << ptr[itr];
  return s;
}
#endif

#if F32_HALF_PACKET_SUPPORT
template <> EIGEN_STRONG_INLINE float predux_min<PacketN_4f>(const PacketN_4f &a) {
  FUNC_ENTRY;
  return RMIN_S(a.v);
}
template <> EIGEN_STRONG_INLINE float predux_max<PacketN_4f>(const PacketN_4f &a) {
  FUNC_ENTRY;
  return RMAX_S(a.v);
}
template <> EIGEN_STRONG_INLINE PacketN_4f pset1<PacketN_4f>(const float &from) {
  FUNC_ENTRY;
  return PacketN_4f(pset1<PacketN_2f>(from));
}
template <> EIGEN_STRONG_INLINE void pstoreu<float>(float *to, const PacketN_4f &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_STORE;
  ae_valign tal;
  xtfloatx2* tpv;
  tpv=xt_reinterpret_cast<xtfloatx2,float>(to);
  tal = AE_ZALIGN64();
  AE_SASX2IP(from.v,tal,tpv);
  AE_SA64POS_FP(tal, tpv);
}
template <> EIGEN_STRONG_INLINE void pstore<float>(float *to, const PacketN_4f &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_STORE;
  xtfloatx2 *pto = xt_reinterpret_cast<xtfloatx2,float>(to);
  *pto = from.v;
}
template <> EIGEN_STRONG_INLINE PacketN_4f ploadu<PacketN_4f>(const float *from) {
  FUNC_ENTRY;
  xtfloatx2 tva;
  ae_valign tal;
  const xtfloatx2* tpv;
  tpv=xt_reinterpret_const_cast<xtfloatx2,float>(from);
  tal=AE_LA64_PP(tpv);
  AE_LASX2IP(tva,tal,tpv);
  return PacketN_4f(tva);
}
template <> EIGEN_STRONG_INLINE PacketN_4f pload<PacketN_4f>(const float *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  return PacketN_4f(*(xt_reinterpret_const_cast<xtfloatx2,float>(from)));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pabs(const PacketN_4f &a) {
  FUNC_ENTRY;
  return PacketN_4f(ABS_SX2(a.v));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pmin(const PacketN_4f &a, const PacketN_4f &b) {
  FUNC_ENTRY;
  return PacketN_4f(MINNUM_SX2(a.v,b.v)); 
}
template <> EIGEN_STRONG_INLINE PacketN_4f pmax(const PacketN_4f &a, const PacketN_4f &b) {
  FUNC_ENTRY;
  return PacketN_4f(MAXNUM_SX2(a.v,b.v));
}
template <> EIGEN_STRONG_INLINE float predux<PacketN_4f>(const PacketN_4f &a) {
  FUNC_ENTRY;
  return RADD_SX2(a.v);
}
template <> EIGEN_STRONG_INLINE PacketN_4f padd(const PacketN_4f &a, const PacketN_4f &b) {
  FUNC_ENTRY;
  return PacketN_4f(a.v+b.v);
}
template <> EIGEN_STRONG_INLINE PacketN_4f pmul<PacketN_4f>(const PacketN_4f &a, const PacketN_4f &b) {
  FUNC_ENTRY;
  return PacketN_4f(a.v*b.v);
}
template <> EIGEN_STRONG_INLINE PacketN_4f pdiv<PacketN_4f>(const PacketN_4f &a, const PacketN_4f &b) {
  FUNC_ENTRY;
  return PacketN_4f(DIV_SX2(a.v,b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pnegate(const PacketN_4f &a) {
  FUNC_ENTRY;
  return PacketN_4f(-a.v);
}
template <> EIGEN_STRONG_INLINE float predux_mul<PacketN_4f>(const PacketN_4f &a) {
  FUNC_ENTRY;
  return MUL_S(HIGH_S(a.v),LOW_S(a.v));
}
template <> EIGEN_STRONG_INLINE PacketN_4f psub<PacketN_4f>(const PacketN_4f &a, const PacketN_4f &b) {
  FUNC_ENTRY;
  return PacketN_4f(a.v-b.v);
}
template <> EIGEN_STRONG_INLINE PacketN_4f preverse(const PacketN_4f &a) {
  FUNC_ENTRY;
  return PacketN_4f(AE_SEL32_LH_SX2(a.v, a.v));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pceil(const PacketN_4f &a) {
  FUNC_ENTRY;
  return PacketN_4f(FICEIL_SX2(a.v));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pfloor(const PacketN_4f &a) {
  FUNC_ENTRY;
  return PacketN_4f(FIFLOOR_SX2(a.v));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pround(const PacketN_4f &a) {
  FUNC_ENTRY;
  return PacketN_4f(FIROUND_SX2(a.v));
}
template <> EIGEN_STRONG_INLINE PacketN_4f ploaddup<PacketN_4f>(const float *from) {
  FUNC_ENTRY;
  return PacketN_4f(ploaddup<PacketN_2f>(from));
}
template <> EIGEN_STRONG_INLINE float pfirst<PacketN_4f>(const PacketN_4f &a) {
  FUNC_ENTRY;
  return HIGH_S(a.v);
}
/** \internal \brief Returns a packet with coefficients (a,a+1,...,a+packet_size-1). */
template <> EIGEN_STRONG_INLINE PacketN_4f plset<PacketN_4f>(const float &a) {
  FUNC_ENTRY;
  xtfloatx2 vt,va;
  float t[2] __attribute__((aligned(16))) = {0,1};
  vt = *(xt_reinterpret_cast<xtfloatx2,float>(t));
  va = a;
  return PacketN_4f(va+vt);
}

template<> EIGEN_STRONG_INLINE PacketN_4f pcmp_eq(const PacketN_4f& a, const PacketN_4f& b) {
  FUNC_ENTRY;
  ae_int32x2 tv0 = 0x00000000u;
  ae_int32x2 tv1 = 0xffffffffu;
  ae_int32x2 tvret = tv0;
  AE_MOVT32X2(tvret,tv1,OEQ_SX2(a.v,b.v));
  return PacketN_4f(AE_MOVXTFLOATX2_FROMINT32X2(tvret));
}
template<> EIGEN_STRONG_INLINE PacketN_4f pcmp_le(const PacketN_4f& a, const PacketN_4f& b) {
  FUNC_ENTRY;
  ae_int32x2 tv0 = 0x00000000u;
  ae_int32x2 tv1 = 0xffffffffu;
  ae_int32x2 tvret = tv0;
  AE_MOVT32X2(tvret,tv1,OLE_SX2(a.v,b.v));
  return PacketN_4f(AE_MOVXTFLOATX2_FROMINT32X2(tvret));
}
template<> EIGEN_STRONG_INLINE PacketN_4f pcmp_lt(const PacketN_4f& a, const PacketN_4f& b) {
  FUNC_ENTRY;
  ae_int32x2 tv0 = 0x00000000u;
  ae_int32x2 tv1 = 0xffffffffu;
  ae_int32x2 tvret = tv0;
  AE_MOVT32X2(tvret,tv1,OLT_SX2(a.v,b.v));
  return PacketN_4f(AE_MOVXTFLOATX2_FROMINT32X2(tvret));
}
template<> EIGEN_STRONG_INLINE PacketN_4f pcmp_lt_or_nan(const PacketN_4f& a, const PacketN_4f& b) {
  FUNC_ENTRY;
  ae_int32x2 tv0 = 0x00000000u;
  ae_int32x2 tv1 = 0xffffffffu;
  ae_int32x2 tvret = tv0;
  AE_MOVT32X2(tvret,tv1,ULT_SX2(a.v,b.v));
  return PacketN_4f(AE_MOVXTFLOATX2_FROMINT32X2(tvret));
}
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4f, CONST_2> &kernel) {
  FUNC_ENTRY;
  #define TRANSPOSE_2x2(_x0_out,_x1_out,_x0_inp,_x1_inp)\
  {\
    xtfloatx2 _c0;\
    _c0 = AE_SEL32_HH_SX2(_x0_inp, _x1_inp);\
    _x1_out = AE_SEL32_LL_SX2(_x0_inp, _x1_inp);\
    _x0_out = _c0;\
  }
  TRANSPOSE_2x2(kernel.packet[0].v,kernel.packet[1].v,kernel.packet[0].v,kernel.packet[1].v);
  #undef TRANSPOSE_2x2
}
template <> EIGEN_STRONG_INLINE PacketN_4f pload1<PacketN_4f>(const float *from) {
  FUNC_ENTRY;
  return pset1<PacketN_4f>(*from);
}
template <> EIGEN_DEVICE_FUNC inline PacketN_4f pgather<float, PacketN_4f>(const float *from, Index stride) {
  FUNC_ENTRY;
  float *tp = (float *)(from);
  xtfloatx2 ta;
  float *tpf = (float *)(&ta);
  int    i;
  int    stride_bytes = sizeof(float) * stride;
  float  tvf;
  for (i = 0; i < SIMD_N_4; i++) {
    AE_LSXP(tvf, tp, stride_bytes);
    AE_SSIP(tvf, tpf, sizeof(float));
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
    AE_LSIP(tvf, tpf, sizeof(float));
    AE_SSXP(tvf, tpt, stride_bytes);
  }
}

#endif

} // end namespace internal

} // end namespace Eigen

#endif

#endif  // EIGEN_PACKET_MATH_F32_HALF_HIFI5_3_XTENSA_H
