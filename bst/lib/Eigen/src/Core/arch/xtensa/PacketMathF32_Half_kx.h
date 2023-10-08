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

#ifndef EIGEN_PACKET_MATH_F32_HALF_KX_XTENSA_H
#define EIGEN_PACKET_MATH_F32_HALF_KX_XTENSA_H

#if F32_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

#if F32_HALF_PACKET_SUPPORT
template <> struct unpacket_traits<PacketN_4f> {
  typedef float type;
  typedef PacketN_4f half;
  enum {size=SIMD_N_4, alignment=ALIGN_N_2, vectorizable=true, masked_load_available=false, masked_store_available=false};
};
#endif

#if F32_HALF_PACKET_SUPPORT
template <> EIGEN_STRONG_INLINE float predux_min<PacketN_4f>(const PacketN_4f &a) {
  FUNC_ENTRY;
  PacketN_2f tva = replicate_N_4f<PacketN_2f>(a.v);
  return predux_min<PacketN_2f>(tva);
}
template <> EIGEN_STRONG_INLINE float predux_max<PacketN_4f>(const PacketN_4f &a) {
  FUNC_ENTRY;
  PacketN_2f tva = replicate_N_4f<PacketN_2f>(a.v);
  return predux_max<PacketN_2f>(tva);
}
template <> EIGEN_STRONG_INLINE PacketN_4f pset1<PacketN_4f>(const float &from) {
  FUNC_ENTRY;
  return PacketN_4f(pset1<PacketN_2f>(from));
}
template <> EIGEN_STRONG_INLINE void pstoreu<float>(float *to, const PacketN_4f &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_STORE;
  valign         talign;
  xb_vecN_2xf32 *tpv0 = xt_reinterpret_cast<xb_vecN_2xf32,float>(to);
  talign           = BBE_ZALIGN();
  BBE_SAVN_2XF32_XP(from.v, talign, tpv0, sizeof(float)*SIMD_N_4);
  BBE_SAN_2XF32POS_FP(talign, tpv0);
}
template <> EIGEN_STRONG_INLINE void pstore<float>(float *to, const PacketN_4f &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_STORE;
  return pstoreu<float>(to, from);
}
template <> EIGEN_STRONG_INLINE PacketN_4f ploadu<PacketN_4f>(const float *from) {
  FUNC_ENTRY;
  PacketN_2f     tv0;
  valign         talign;
  xb_vecN_2xf32 *tpv0 = xt_reinterpret_const_cast<xb_vecN_2xf32,float>(from);
  talign              = BBE_LAN_2XF32_PP(tpv0);
  BBE_LAVN_2XF32_XP(tv0, talign, tpv0, sizeof(float)*SIMD_N_4);
  return PacketN_4f(tv0);
}
template <> EIGEN_STRONG_INLINE PacketN_4f pload<PacketN_4f>(const float *from) {
  FUNC_ENTRY;
  return ploadu<PacketN_4f>(from);
}
template <> EIGEN_STRONG_INLINE PacketN_4f pabs(const PacketN_4f &a) {
  FUNC_ENTRY;
  return PacketN_4f(pabs<PacketN_2f>(a.v));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pmin(const PacketN_4f &a, const PacketN_4f &b) {
  FUNC_ENTRY;
  return PacketN_4f(pmin<PacketN_2f>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pmax(const PacketN_4f &a, const PacketN_4f &b) {
  FUNC_ENTRY;
  return PacketN_4f(pmax<PacketN_2f>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE float predux<PacketN_4f>(const PacketN_4f &a) {
  FUNC_ENTRY;
  PacketN_2f tv0 = BBE_CONSTN_2XF32(0);
  PacketN_2f tva = extract_N_4f<PacketN_2f,PacketN_2f>(a.v, tv0);
  return predux<PacketN_2f>(tva);
}
template <> EIGEN_STRONG_INLINE PacketN_4f padd(const PacketN_4f &a, const PacketN_4f &b) {
  FUNC_ENTRY;
  return PacketN_4f(padd<PacketN_2f>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pmul<PacketN_4f>(const PacketN_4f &a, const PacketN_4f &b) {
  FUNC_ENTRY;
  return PacketN_4f(pmul<PacketN_2f>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pdiv<PacketN_4f>(const PacketN_4f &a, const PacketN_4f &b) {
  FUNC_ENTRY;
  PacketN_2f tv1 = BBE_CONSTN_2XF32(1);
  PacketN_2f tvb = extract_N_4f<PacketN_2f,PacketN_2f>(b.v, tv1); // Set denominator 1 in disabled lanes
  return PacketN_4f(pdiv<PacketN_2f>(a.v, tvb));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pnegate(const PacketN_4f &a) {
  FUNC_ENTRY;
  return PacketN_4f(pnegate<PacketN_2f>(a.v));
}
template <> EIGEN_STRONG_INLINE float predux_mul<PacketN_4f>(const PacketN_4f &a) {
  FUNC_ENTRY;
  PacketN_2f tv1 = BBE_CONSTN_2XF32(1);
  PacketN_2f tva = extract_N_4f<PacketN_2f,PacketN_2f>(a.v, tv1);
  return predux_mul<PacketN_2f>(tva);
}
template <> EIGEN_STRONG_INLINE PacketN_4f psub<PacketN_4f>(const PacketN_4f &a, const PacketN_4f &b) {
  FUNC_ENTRY;
  return PacketN_4f(psub<PacketN_2f>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_4f preverse(const PacketN_4f &a) {
  FUNC_ENTRY;
  PacketN_2f tva = replicate_N_4f<PacketN_2f>(a.v);
  return PacketN_4f(preverse<PacketN_2f>(tva));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pceil(const PacketN_4f &a) {
  FUNC_ENTRY;
  PacketN_2f tva = replicate_N_4f<PacketN_2f>(a.v);
  return PacketN_4f(pceil<PacketN_2f>(tva));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pfloor(const PacketN_4f &a) {
  FUNC_ENTRY;
  PacketN_2f tva = replicate_N_4f<PacketN_2f>(a.v);
  return PacketN_4f(pfloor<PacketN_2f>(tva));
}
template <> EIGEN_STRONG_INLINE PacketN_4f pround(const PacketN_4f &a) {
  FUNC_ENTRY;
  PacketN_2f tva = replicate_N_4f<PacketN_2f>(a.v);
  return PacketN_4f(pround<PacketN_2f>(tva));
}
template <> EIGEN_STRONG_INLINE PacketN_4f ploaddup<PacketN_4f>(const float *from) {
  FUNC_ENTRY;
  return PacketN_4f(ploaddup<PacketN_2f>(from));
}
template <> EIGEN_STRONG_INLINE float pfirst<PacketN_4f>(const PacketN_4f &a) {
  FUNC_ENTRY;
  PacketN_2f tva = replicate_N_4f<PacketN_2f>(a.v);
  return pfirst<PacketN_2f>(tva);
}
/** \internal \brief Returns a packet with coefficients (a,a+1,...,a+packet_size-1). */
template <> EIGEN_STRONG_INLINE PacketN_4f plset<PacketN_4f>(const float &a) {
  FUNC_ENTRY;
  return PacketN_4f(plset<PacketN_2f>(a));
}

template<> EIGEN_STRONG_INLINE PacketN_4f pcmp_le(const PacketN_4f& a, const PacketN_4f& b) {return rep_T(a,b,PacketN_2f,PacketN_4f,pcmp_le,replicate_N_4f);}
template<> EIGEN_STRONG_INLINE PacketN_4f pcmp_lt(const PacketN_4f& a, const PacketN_4f& b) {return rep_T(a,b,PacketN_2f,PacketN_4f,pcmp_lt,replicate_N_4f);}
template<> EIGEN_STRONG_INLINE PacketN_4f pcmp_lt_or_nan(const PacketN_4f& a, const PacketN_4f& b) {return rep_T(a,b,PacketN_2f,PacketN_4f,pcmp_lt_or_nan,replicate_N_4f);}
template<> EIGEN_STRONG_INLINE PacketN_4f pcmp_eq(const PacketN_4f& a, const PacketN_4f& b) {return rep_T(a,b,PacketN_2f,PacketN_4f,pcmp_eq,replicate_N_4f);}

#if SIMD_N_2_EQ_4()
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4f, CONST_2> &kernel) {
  FUNC_ENTRY;
  #if SIMD_N_2_EQ_4()
      #define DV(_num) xb_vecN_2xf32 tvs##_num = BBE_CONSTN_2XF32(0)
      #define LV(_num) xb_vecN_2xf32 tvf##_num = kernel.packet[_num].v
      
      DV(0); DV(1);
      LV(0); LV(1);

      BBE_DSELN_2XF32I(tvs1, tvs0, tvf1, tvf0, BBE_DSELI_32B_INTERLEAVE_1);
      BBE_DSELN_2XF32I(tvf1, tvf0, tvs1, tvs0, BBE_DSELI_32B_INTERLEAVE_2);

      #define SV(_num) kernel.packet[_num] = PacketN_4f(tvf##_num)
      SV(0); SV(1);

      #undef DV
      #undef LV
      #undef SV
  #endif
}
#endif

#if XCHAL_HAVE_BBENEP
#if SIMD_N_2_EQ_8() || SIMD_N_2_EQ_16() || SIMD_N_2_EQ_32()
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4f, CONST_4> &kernel) {
  FUNC_ENTRY;
#if SIMD_N_2_EQ_8()
    #define DV(_num) xb_vecN_2xf32 tvs##_num = BBE_CONSTN_2XF32(0)
    #define LV(_num) xb_vecN_2xf32 tvf##_num = kernel.packet[_num].v
    
    DV(0); DV(1); DV(2); DV(3);
    LV(0); LV(1); LV(2); LV(3);

    #define FIRST_STEP(_pat, _out, _inp)                             \
      BBE_DSELN_2XF32I(_out##2, _out##0, _inp##1, _inp##0, _pat);    \
      BBE_DSELN_2XF32I(_out##3, _out##1, _inp##3, _inp##2, _pat);    \

    #define SECOND_STEP(_pat, _out, _inp)                            \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);    \
      _out##2 = _inp##2; _out##3 = _inp##3;
    
    #define LAST_STEP(_pat, _out, _inp)                              \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
      BBE_DSELN_2XF32I(_out##3, _out##2, _inp##3, _inp##1, _pat);    \

    FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_1, tvs, tvf);
    SECOND_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvf, tvs);
    LAST_STEP(BBE_DSELI_32B_INTERLEAVE_4, tvs, tvf);
    
    #define SV(_num) kernel.packet[_num] = PacketN_4f(tvs##_num)
    SV(0); SV(1); SV(2); SV(3);

    #undef DV
    #undef LV
    #undef FIRST_STEP
    #undef SECOND_STEP
    #undef LAST_STEP
    #undef SV
#elif SIMD_N_2_EQ_16()
    #define DV(_num) xb_vecN_2xf32 tvs##_num = BBE_CONSTN_2XF32(0)
    #define LV(_num) xb_vecN_2xf32 tvf##_num = kernel.packet[_num].v
    
    DV(0); DV(1); DV(2); DV(3);
    LV(0); LV(1); LV(2); LV(3);

    #define FIRST_STEP(_pat, _out, _inp)                             \
      BBE_DSELN_2XF32I(_out##2, _out##0, _inp##1, _inp##0, _pat);    \
      BBE_DSELN_2XF32I(_out##3, _out##1, _inp##3, _inp##2, _pat);    \

    #define SECOND_STEP(_pat, _out, _inp)                            \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);    \
      _out##2 = _inp##2; _out##3 = _inp##3;
    
    #define LAST_STEP(_pat, _out, _inp)                              \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
      BBE_DSELN_2XF32I(_out##3, _out##2, _inp##3, _inp##1, _pat);    \

    FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_1, tvs, tvf);
    SECOND_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvf, tvs);
    LAST_STEP(BBE_DSELI_32B_INTERLEAVE_8, tvs, tvf);
    
    #define SV(_num) kernel.packet[_num] = PacketN_4f(tvs##_num)
    SV(0); SV(1); SV(2); SV(3);

    #undef DV
    #undef LV
    #undef FIRST_STEP
    #undef SECOND_STEP
    #undef LAST_STEP
    #undef SV
#elif SIMD_N_2_EQ_32()
    #define DV(_num) xb_vecN_2xf32 tvs##_num = BBE_CONSTN_2XF32(0)
    #define LV(_num) xb_vecN_2xf32 tvf##_num = kernel.packet[_num].v
    
    DV(0); DV(1); DV(2); DV(3);
    LV(0); LV(1); LV(2); LV(3);

    #define FIRST_STEP(_pat, _out, _inp)                             \
      BBE_DSELN_2XF32I(_out##2, _out##0, _inp##1, _inp##0, _pat);    \
      BBE_DSELN_2XF32I(_out##3, _out##1, _inp##3, _inp##2, _pat);    \

    #define SECOND_STEP(_pat, _out, _inp)                            \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);    \
      _out##2 = _inp##2; _out##3 = _inp##3;
    
    #define LAST_STEP(_pat, _out, _inp)                              \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
      BBE_DSELN_2XF32I(_out##3, _out##2, _inp##3, _inp##1, _pat);    \

    FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_1, tvs, tvf);
    SECOND_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvf, tvs);
    LAST_STEP(BBE_DSELI_32B_INTERLEAVE_16, tvs, tvf);
    
    #define SV(_num) kernel.packet[_num] = PacketN_4f(tvs##_num)
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
#if SIMD_N_2_EQ_16()
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4f, CONST_8> &kernel) {
  FUNC_ENTRY;
    #define DV(_num) xb_vecN_2xf32 tvs##_num = BBE_CONSTN_2XF32(0)
    #define LV(_num) xb_vecN_2xf32 tvf##_num = kernel.packet[_num].v

    DV(0); DV(1); DV(2); DV(3); DV(4); DV(5); DV(6); DV(7);
    LV(0); LV(1); LV(2); LV(3); LV(4); LV(5); LV(6); LV(7);

    #define FIRST_STEP(_pat, _out, _inp)                             \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);    \
      BBE_DSELN_2XF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);    \
      BBE_DSELN_2XF32I(_out##5, _out##4, _inp##5, _inp##4, _pat);    \
      BBE_DSELN_2XF32I(_out##7, _out##6, _inp##7, _inp##6, _pat);    \

    #define SECOND_STEP(_pat, _out, _inp)                            \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
      BBE_DSELN_2XF32I(_out##3, _out##2, _inp##6, _inp##4, _pat);    \
      _out##4 = _inp##1; _out##5 = _inp##3;                          \
      _out##6 = _inp##5; _out##7 = _inp##7;                          \

    #define LAST_STEP(_pat, _out, _inp)                              \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##4, _inp##0, _pat);    \
      BBE_DSELN_2XF32I(_out##3, _out##2, _inp##5, _inp##1, _pat);    \
      BBE_DSELN_2XF32I(_out##5, _out##4, _inp##6, _inp##2, _pat);    \
      BBE_DSELN_2XF32I(_out##7, _out##6, _inp##7, _inp##3, _pat);    \

    FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_1, tvs, tvf);
    SECOND_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvf, tvs);
    LAST_STEP(BBE_DSELI_32B_INTERLEAVE_4, tvs, tvf);
    LAST_STEP(BBE_DSELI_32B_INTERLEAVE_4, tvf, tvs);

    #define SV(_num) kernel.packet[_num] = PacketN_4f(tvf##_num)
    SV(0); SV(1); SV(2); SV(3); SV(4); SV(5); SV(6); SV(7);

    #undef DV
    #undef LV
    #undef FIRST_STEP
    #undef SECOND_STEP
    #undef LAST_STEP
    #undef SV
  
}
#endif
#if SIMD_N_2_EQ_32()
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4f, CONST_16> &kernel) {
  FUNC_ENTRY;
    #define DV(_num) xb_vecN_2xf32 tvs##_num = BBE_CONSTN_2XF32(0)
    #define LV(_num) xb_vecN_2xf32 tvf##_num = kernel.packet[_num].v

    DV(0); DV(1); DV(2); DV(3); DV(4); DV(5); DV(6); DV(7); DV(8); DV(9); DV(10); DV(11); DV(12); DV(13); DV(14); DV(15);
    LV(0); LV(1); LV(2); LV(3); LV(4); LV(5); LV(6); LV(7); LV(8); LV(9); LV(10); LV(11); LV(12); LV(13); LV(14); LV(15);

    #define FIRST_STEP(_pat, _out, _inp)                              \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
      BBE_DSELN_2XF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);     \
      BBE_DSELN_2XF32I(_out##5, _out##4, _inp##5, _inp##4, _pat);     \
      BBE_DSELN_2XF32I(_out##7, _out##6, _inp##7, _inp##6, _pat);     \
      BBE_DSELN_2XF32I(_out##9, _out##8, _inp##9, _inp##8, _pat);     \
      BBE_DSELN_2XF32I(_out##11, _out##10, _inp##11, _inp##10, _pat); \
      BBE_DSELN_2XF32I(_out##13, _out##12, _inp##13, _inp##12, _pat); \
      BBE_DSELN_2XF32I(_out##15, _out##14, _inp##15, _inp##14, _pat); \

    #define SECOND_STEP(_pat, _out, _inp)                             \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);     \
      BBE_DSELN_2XF32I(_out##3, _out##2, _inp##6, _inp##4, _pat);     \
      BBE_DSELN_2XF32I(_out##5, _out##4, _inp##10, _inp##8, _pat);    \
      BBE_DSELN_2XF32I(_out##7, _out##6, _inp##14, _inp##12, _pat);   \
      _out##8 = _inp##1;   _out##9 = _inp##3;                         \
      _out##10 = _inp##5;  _out##11 = _inp##7;                        \
      _out##12 = _inp##9;  _out##13 = _inp##11;                       \
      _out##14 = _inp##13; _out##15 = _inp##15;                       \

    #define THIRD_STEP(_pat, _out, _inp)                              \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);     \
      BBE_DSELN_2XF32I(_out##3, _out##2, _inp##3, _inp##1, _pat);     \
      BBE_DSELN_2XF32I(_out##5, _out##4, _inp##6, _inp##4, _pat);     \
      BBE_DSELN_2XF32I(_out##7, _out##6, _inp##7, _inp##5, _pat);     \
      _out##8 = _inp##8;   _out##9 = _inp##9;                         \
      _out##10 = _inp##10; _out##11 = _inp##11;                       \
      _out##12 = _inp##12; _out##13 = _inp##13;                       \
      _out##14 = _inp##14; _out##15 = _inp##15;                       \

    #define LAST_STEP(_pat, _out, _inp)                               \
      BBE_DSELN_2XF32I(_out##1, _out##0, _inp##8, _inp##0, _pat);     \
      BBE_DSELN_2XF32I(_out##3, _out##2, _inp##9, _inp##1, _pat);     \
      BBE_DSELN_2XF32I(_out##5, _out##4, _inp##10, _inp##2, _pat);    \
      BBE_DSELN_2XF32I(_out##7, _out##6, _inp##11, _inp##3, _pat);    \
      BBE_DSELN_2XF32I(_out##9, _out##8, _inp##12, _inp##4, _pat);    \
      BBE_DSELN_2XF32I(_out##11, _out##10, _inp##13, _inp##5, _pat);  \
      BBE_DSELN_2XF32I(_out##13, _out##12, _inp##14, _inp##6, _pat);  \
      BBE_DSELN_2XF32I(_out##15, _out##14, _inp##15, _inp##7, _pat);  \

    FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_1, tvs, tvf);
    SECOND_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvf, tvs);
    THIRD_STEP(BBE_DSELI_32B_INTERLEAVE_4, tvs, tvf);
    LAST_STEP(BBE_DSELI_32B_INTERLEAVE_8, tvf, tvs);
    LAST_STEP(BBE_DSELI_32B_INTERLEAVE_8, tvs, tvf);

    #define SV(_num) kernel.packet[_num] = PacketN_4f(tvs##_num)
    SV(0); SV(1); SV(2); SV(3); SV(4); SV(5); SV(6); SV(7); SV(8); SV(9); SV(10); SV(11); SV(12); SV(13); SV(14); SV(15);
    
    #undef DV
    #undef LV
    #undef FIRST_STEP
    #undef SECOND_STEP
    #undef THIRD_STEP
    #undef LAST_STEP
    #undef SV
}
#endif
#endif

template <> EIGEN_STRONG_INLINE PacketN_4f ploadquad<PacketN_4f>(const float *from) {
  FUNC_ENTRY;
  return PacketN_4f(ploadquad<PacketN_2f>(from));
}
#if XCHAL_HAVE_BBENEP
template <> EIGEN_DEVICE_FUNC inline PacketN_4f pgather<float, PacketN_4f>(const float *from, Index stride) {
  FUNC_ENTRY;
  float *tp = (float *)(from);
  xb_vecN_2xf32  ta;
  float *tpf = (float *)(&ta);
  int    i;
  int    stride_bytes = sizeof(float) * stride;
  xb_vecN_2xf32 tvf;

  for (i = 0; i < SIMD_N_4; i++) {
    BBE_LSN_2XF32_XP(tvf, tp, stride_bytes);
    BBE_SSN_2XF32_IP(tvf, tpf, sizeof(float));
  }
  return PacketN_4f(ta);
}
template <> EIGEN_DEVICE_FUNC inline void pscatter<float, PacketN_4f>(float *to, const PacketN_4f &from, Index stride) {
  FUNC_ENTRY;
  float *tpf = (float*)(&from.v);
  float *tpt = (float*)(to);
  int    stride_bytes = sizeof(float) * stride;
  xb_vecN_2xf32 tvf;
  int i;
  for (i = 0; i < SIMD_N_4; i++)
  {
    BBE_LSN_2XF32_IP(tvf, tpf, sizeof(float));
    BBE_SSN_2XF32_XP(tvf, tpt, stride_bytes);
  }
}
#endif
template<> EIGEN_STRONG_INLINE float predux_min<PropagateNaN,PacketN_4f>(const PacketN_4f &a) {
  FUNC_ENTRY;
  PacketN_2f tva = replicate_N_4f<PacketN_2f>(a.v);
  return predux_min<PropagateNaN,PacketN_2f>(tva);
}
template<> EIGEN_STRONG_INLINE float predux_max<PropagateNaN,PacketN_4f>(const PacketN_4f &a) {
  FUNC_ENTRY;
  PacketN_2f tva = replicate_N_4f<PacketN_2f>(a.v);
  return predux_max<PropagateNaN,PacketN_2f>(tva);
}
#endif

} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_F32_HALF_KX_XTENSA_H
