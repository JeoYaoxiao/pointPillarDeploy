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

#ifndef EIGEN_PACKET_MATH_CF32_HALF_KX_XTENSA_H
#define EIGEN_PACKET_MATH_CF32_HALF_KX_XTENSA_H

#if CF32_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

#if CF32_HALF_PACKET_SUPPORT
template <> EIGEN_STRONG_INLINE PacketN_4cf replicate_N_8cf(const PacketN_4cf& a){
  return extract_N_8cf<PacketN_4cf,PacketN_4cf>(a, a);
}
#else
typedef PacketN_4cf PacketN_8cf;
#endif

#if CF32_HALF_PACKET_SUPPORT
template <> EIGEN_STRONG_INLINE PacketN_8cf pset1<PacketN_8cf>(const complexfloat &from) {
  FUNC_ENTRY;
  return PacketN_8cf(pset1<PacketN_4cf>(from));
}
template <> EIGEN_STRONG_INLINE void pstoreu<complexfloat>(complexfloat *to, const PacketN_8cf &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_STORE;
  valign         talign;
  xb_vecN_4xcf32 *tpv0 = xt_reinterpret_cast<xb_vecN_4xcf32,complexfloat>(to);
  talign           = BBE_ZALIGN();
  BBE_SAVN_4XCF32_XP(from.v.v, talign, tpv0, sizeof(complexfloat)*SIMD_N_8);
  BBE_SAN_4XCF32POS_FP(talign, tpv0);
}
template <> EIGEN_STRONG_INLINE void pstore<complexfloat>(complexfloat *to, const PacketN_8cf &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_STORE;
  return pstoreu<complexfloat>(to, from);
}
template <> EIGEN_STRONG_INLINE PacketN_8cf ploadu<PacketN_8cf>(const complexfloat *from) {
  FUNC_ENTRY;
  xb_vecN_4xcf32     tv0;
  valign         talign;
  xb_vecN_4xcf32 *tpv0 = xt_reinterpret_const_cast<xb_vecN_4xcf32,complexfloat>(from);
  talign              = BBE_LAN_4XCF32_PP(tpv0);
  BBE_LAVN_4XCF32_XP(tv0, talign, tpv0, sizeof(complexfloat)*SIMD_N_8);
  return PacketN_8cf(tv0);
}
template <> EIGEN_STRONG_INLINE PacketN_8cf pload<PacketN_8cf>(const complexfloat *from) {
  FUNC_ENTRY;
  return ploadu<PacketN_8cf>(from);
}
template <> EIGEN_STRONG_INLINE complexfloat predux<PacketN_8cf>(const PacketN_8cf &a) {
  FUNC_ENTRY;
  PacketN_4cf tv0 = pset1<PacketN_4cf>(complexfloat(0,0));
  PacketN_4cf tva = extract_N_8cf<PacketN_4cf,PacketN_4cf>(a.v, tv0);
  return predux<PacketN_4cf>(tva);
}
template <> EIGEN_STRONG_INLINE PacketN_8cf padd(const PacketN_8cf &a, const PacketN_8cf &b) {
  FUNC_ENTRY;
  return PacketN_8cf(padd<PacketN_4cf>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_8cf pmul<PacketN_8cf>(const PacketN_8cf &a, const PacketN_8cf &b) {
  FUNC_ENTRY;
  return PacketN_8cf(pmul<PacketN_4cf>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_8cf pdiv<PacketN_8cf>(const PacketN_8cf &a, const PacketN_8cf &b) {
  FUNC_ENTRY;
  PacketN_4cf tv1 = pset1<PacketN_4cf>(complexfloat(1,0));
  PacketN_4cf tvb = extract_N_8cf<PacketN_4cf,PacketN_4cf>(b.v, tv1); // Set denominator 1 in disabled lanes
  return PacketN_8cf(pdiv<PacketN_4cf>(a.v, tvb));
}
template <> EIGEN_STRONG_INLINE PacketN_8cf pnegate(const PacketN_8cf &a) {
  FUNC_ENTRY;
  return PacketN_8cf(pnegate<PacketN_4cf>(a.v));
}
template <> EIGEN_STRONG_INLINE complexfloat predux_mul<PacketN_8cf>(const PacketN_8cf &a) {
  FUNC_ENTRY;
  PacketN_4cf tv1 = pset1<PacketN_4cf>(complexfloat(1,0));
  PacketN_4cf tva = extract_N_8cf<PacketN_4cf,PacketN_4cf>(a.v, tv1);
  return predux_mul<PacketN_4cf>(tva);
}
template <> EIGEN_STRONG_INLINE PacketN_8cf psub<PacketN_8cf>(const PacketN_8cf &a, const PacketN_8cf &b) {
  FUNC_ENTRY;
  return PacketN_8cf(psub<PacketN_4cf>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_8cf ploadquad<PacketN_8cf>(const complexfloat *from) {
  FUNC_ENTRY;
  return PacketN_8cf(ploadquad<PacketN_4cf>(from));
}
template <> EIGEN_STRONG_INLINE PacketN_8cf pconj(const PacketN_8cf &a) {
  FUNC_ENTRY;
  PacketN_4cf tva = replicate_N_8cf<PacketN_4cf>(a.v);
  return PacketN_8cf(pconj<PacketN_4cf>(tva));
}
template <> EIGEN_STRONG_INLINE PacketN_8cf preverse(const PacketN_8cf &a) {
  FUNC_ENTRY;
  PacketN_4cf tva = replicate_N_8cf<PacketN_4cf>(a.v);
  return PacketN_8cf(preverse<PacketN_4cf>(tva));
}
/** \internal \returns \a a with real and imaginary part flipped (for complex type only) */
template<> EIGEN_STRONG_INLINE PacketN_8cf pcplxflip<PacketN_8cf>(const PacketN_8cf& a) {
  FUNC_ENTRY;
  PacketN_4cf tva = replicate_N_8cf<PacketN_4cf>(a.v);
  return PacketN_8cf(pcplxflip<PacketN_4cf>(tva));
}
template <> EIGEN_STRONG_INLINE complexfloat pfirst<PacketN_8cf>(const PacketN_8cf &a) {
  FUNC_ENTRY;
  PacketN_4cf tva = replicate_N_8cf<PacketN_4cf>(a.v);
  return pfirst<PacketN_4cf>(tva);
}

#if XCHAL_HAVE_BBENEP
#if SIMD_N_EQ(8,2)
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8cf, CONST_2> &kernel) {
  FUNC_ENTRY;
  #if SIMD_N_EQ(8,2)
      #define DV(_num) xb_vecN_4xcf32 tvs##_num = BBE_CONSTN_4XCF32(0)
      #define LV(_num) xb_vecN_4xcf32 tvf##_num = kernel.packet[_num].v.v

      DV(0); DV(1);
      LV(0); LV(1);

      BBE_DSELN_4XCF32I(tvs1, tvs0, tvf1, tvf0, BBE_DSELI_64B_INTERLEAVE_1);
      BBE_DSELN_4XCF32I(tvf1, tvf0, tvs1, tvs0, BBE_DSELI_64B_INTERLEAVE_2);
      
      #define SV(_num) kernel.packet[_num] = PacketN_8cf(tvf##_num)
      SV(0); SV(1);

      #undef DV
      #undef LV
      #undef SV
  #endif
}
#endif
#if SIMD_N_EQ(8,4) || SIMD_N_EQ(8,8)
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8cf, CONST_4> &kernel) {
  FUNC_ENTRY;
  #if SIMD_N_EQ(8,4)
      #define DV(_num) xb_vecN_4xcf32 tvs##_num = BBE_CONSTN_4XCF32(0)
      #define LV(_num) xb_vecN_4xcf32 tvf##_num = kernel.packet[_num].v.v

      DV(0); DV(1); DV(2); DV(3);
      LV(0); LV(1); LV(2); LV(3);

      #define FIRST_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);    \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);    \

      #define SECOND_STEP(_pat, _out, _inp)                             \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        _out##2 = _inp##2; _out##3 = _inp##3;
      
      #define LAST_STEP(_pat, _out, _inp)                               \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##1, _pat);    \

      FIRST_STEP(BBE_DSELI_64B_INTERLEAVE_1, tvs, tvf);
      SECOND_STEP(BBE_DSELI_64B_INTERLEAVE_2, tvf, tvs);
      LAST_STEP(BBE_DSELI_64B_INTERLEAVE_4, tvs, tvf);
      
      #define SV(_num) kernel.packet[_num] = PacketN_8cf(tvs##_num)
      SV(0); SV(1); SV(2); SV(3);

      #undef DV
      #undef LV
      #undef FIRST_STEP
      #undef SECOND_STEP
      #undef LAST_STEP
      #undef SV
  #elif SIMD_N_EQ(8,8)
      #define DV(_num) xb_vecN_4xcf32 tvs##_num = BBE_CONSTN_4XCF32(0)
      #define LV(_num) xb_vecN_4xcf32 tvf##_num = kernel.packet[_num].v.v

      DV(0); DV(1); DV(2); DV(3);
      LV(0); LV(1); LV(2); LV(3);

      #define FIRST_STEP(_pat, _out, _inp)                             \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);   \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);   \

      #define SECOND_STEP(_pat, _out, _inp)                            \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);   \
        _out##2 = _inp##2; _out##3 = _inp##3;
      
      #define LAST_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);   \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##1, _pat);   \

      FIRST_STEP(BBE_DSELI_64B_INTERLEAVE_1, tvs, tvf);
      SECOND_STEP(BBE_DSELI_64B_INTERLEAVE_2, tvf, tvs);
      LAST_STEP(BBE_DSELI_64B_INTERLEAVE_8, tvs, tvf);
      
      #define SV(_num) kernel.packet[_num] = PacketN_8cf(tvs##_num)
      SV(0); SV(1); SV(2); SV(3);

      #undef DV
      #undef LV
      #undef FIRST_STEP
      #undef SECOND_STEP
      #undef LAST_STEP
      #undef SV
  #endif
}
#endif
#if SIMD_N_EQ(8,8)
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8cf, CONST_8> &kernel) {
  FUNC_ENTRY;
  #if SIMD_N_EQ(8,8)
      #define DV(_num) xb_vecN_4xcf32 tvs##_num = BBE_CONSTN_4XCF32(0)
      #define LV(_num) xb_vecN_4xcf32 tvf##_num = kernel.packet[_num].v.v

      DV(0); DV(1); DV(2); DV(3); DV(4); DV(5); DV(6); DV(7);
      LV(0); LV(1); LV(2); LV(3); LV(4); LV(5); LV(6); LV(7);

      #define FIRST_STEP(_pat, _out, _inp)                             \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);   \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);   \
        BBE_DSELN_4XCF32I(_out##5, _out##4, _inp##5, _inp##4, _pat);   \
        BBE_DSELN_4XCF32I(_out##7, _out##6, _inp##7, _inp##6, _pat);   \

      #define SECOND_STEP(_pat, _out, _inp)                            \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);   \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##6, _inp##4, _pat);   \
        _out##4 = _inp##1; _out##5 = _inp##3;                          \
        _out##6 = _inp##5; _out##7 = _inp##7;                          \

      #define THIRD_STEP(_pat, _out, _inp)                             \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);   \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##1, _pat);   \
        _out##4 = _inp##4; _out##5 = _inp##5;                          \
        _out##6 = _inp##6; _out##7 = _inp##7;                          \

      #define LAST_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##4, _inp##0, _pat);   \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##5, _inp##1, _pat);   \
        BBE_DSELN_4XCF32I(_out##5, _out##4, _inp##6, _inp##2, _pat);   \
        BBE_DSELN_4XCF32I(_out##7, _out##6, _inp##7, _inp##3, _pat);   \

      FIRST_STEP(BBE_DSELI_64B_INTERLEAVE_1, tvs, tvf);
      SECOND_STEP(BBE_DSELI_64B_INTERLEAVE_2, tvf, tvs);
      THIRD_STEP(BBE_DSELI_64B_INTERLEAVE_4, tvs, tvf);
      LAST_STEP(BBE_DSELI_64B_INTERLEAVE_8, tvf, tvs);

      #define SV(_num) kernel.packet[_num] = PacketN_8cf(tvf##_num)
      SV(0); SV(1); SV(2); SV(3); SV(4); SV(5); SV(6); SV(7);

      #undef DV
      #undef LV
      #undef FIRST_STEP
      #undef SECOND_STEP
      #undef LAST_STEP
      #undef SV
  #endif
}
#endif
#endif

template <> EIGEN_STRONG_INLINE PacketN_8cf ploaddup<PacketN_8cf>(const complexfloat *from) {
  FUNC_ENTRY;
  return PacketN_8cf(ploaddup<PacketN_4cf>(from));
}
template<> EIGEN_STRONG_INLINE PacketN_8cf pcmp_eq(const PacketN_8cf& a, const PacketN_8cf& b) {
  FUNC_ENTRY;
  PacketN_4cf tva = replicate_N_8cf<PacketN_4cf>(a.v);
  PacketN_4cf tvb = replicate_N_8cf<PacketN_4cf>(b.v);
  return PacketN_8cf(pcmp_eq<PacketN_4cf>(tva,tvb));
}
#if XCHAL_HAVE_BBENEP
template <> EIGEN_DEVICE_FUNC inline PacketN_8cf pgather<complexfloat, PacketN_8cf>(const complexfloat *from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtcomplexfloat *tp = xt_reinterpret_const_cast<xtcomplexfloat,complexfloat>(from);
  xb_vecN_4xcf32 ta;
  xtcomplexfloat *tpf = xt_reinterpret_cast<xtcomplexfloat,xb_vecN_4xcf32>(&ta);
  int    i;
  int    stride_bytes = sizeof(xtcomplexfloat) * stride;
  xb_vecN_4xcf32 tvf;

  for (i = 0; i < SIMD_N_8; i++) {
    BBE_LSN_4XCF32_XP(tvf, tp, stride_bytes);
    BBE_SSN_4XCF32_IP(tvf, tpf, sizeof(xtcomplexfloat));
  }
  return PacketN_8cf(ta);
}

template <> EIGEN_DEVICE_FUNC inline void pscatter<complexfloat, PacketN_8cf>(complexfloat *to, const PacketN_8cf &from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  int i;
  xtcomplexfloat *tpf = xt_reinterpret_const_cast<xtcomplexfloat,PacketN_8cf>(&from);
  xtcomplexfloat *tpt = xt_reinterpret_cast<xtcomplexfloat,complexfloat>(to);
  int    stride_bytes = sizeof(xtcomplexfloat) * stride;
  xb_vecN_4xcf32 tvf;
  for (i = 0; i < SIMD_N_8; i++)
  {
    BBE_LSN_4XCF32_IP(tvf, tpf, sizeof(xtcomplexfloat));
    BBE_SSN_4XCF32_XP(tvf, tpt, stride_bytes);
  }
}
#endif
#endif

} // end namespace internal

} // end namespace Eigen
#endif

#endif // EIGEN_PACKET_MATH_CF32_HALF_KX_XTENSA_H
