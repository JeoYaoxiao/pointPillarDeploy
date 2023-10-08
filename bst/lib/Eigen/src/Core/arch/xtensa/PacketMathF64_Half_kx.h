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

#ifndef EIGEN_PACKET_MATH_F64_HALF_KX_XTENSA_H
#define EIGEN_PACKET_MATH_F64_HALF_KX_XTENSA_H

#if F64_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

#if F64_HALF_PACKET_SUPPORT
template <> struct unpacket_traits<PacketN_8d> {
  typedef double type;
  typedef PacketN_8d half;
  enum {size=SIMD_N_8, alignment=ALIGN_N_2, vectorizable=true, masked_load_available=false, masked_store_available=false};
};
#endif

#if F64_HALF_PACKET_SUPPORT
template <> EIGEN_STRONG_INLINE double predux_min<PacketN_8d>(const PacketN_8d &a) {
  FUNC_ENTRY;
  PacketN_4d tva = replicate_N_8d<PacketN_4d>(a.v);
  return predux_min<PacketN_4d>(tva);
}
template <> EIGEN_STRONG_INLINE double predux_max<PacketN_8d>(const PacketN_8d &a) {
  FUNC_ENTRY;
  PacketN_4d tva = replicate_N_8d<PacketN_4d>(a.v);
  return predux_max<PacketN_4d>(tva);
}
template <> EIGEN_STRONG_INLINE PacketN_8d pset1<PacketN_8d>(const double &from) {
  FUNC_ENTRY;
  return PacketN_8d(pset1<PacketN_4d>(from));
}
template <> EIGEN_STRONG_INLINE void pstoreu<double>(double *to, const PacketN_8d &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_STORE;
  valign         talign;
  xb_vecN_4xf64 *tpv0 = xt_reinterpret_cast<xb_vecN_4xf64,double>(to);
  talign           = BBE_ZALIGN();
  BBE_SAVN_4XF64_XP(from.v, talign, tpv0, sizeof(double)*SIMD_N_8);
  BBE_SAN_4XF64POS_FP(talign, tpv0);
}
template <> EIGEN_STRONG_INLINE void pstore<double>(double *to, const PacketN_8d &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_STORE;
  return pstoreu<double>(to, from);
}
template <> EIGEN_STRONG_INLINE PacketN_8d ploadu<PacketN_8d>(const double *from) {
  FUNC_ENTRY;
  PacketN_4d     tv0;
  valign         talign;
  xb_vecN_4xf64 *tpv0 = xt_reinterpret_const_cast<xb_vecN_4xf64,double>(from);
  talign              = BBE_LAN_4XF64_PP(tpv0);
  BBE_LAVN_4XF64_XP(tv0, talign, tpv0, sizeof(double)*SIMD_N_8);
  return PacketN_8d(tv0);
}
template <> EIGEN_STRONG_INLINE PacketN_8d pload<PacketN_8d>(const double *from) {
  FUNC_ENTRY;
  return ploadu<PacketN_8d>(from);
}
template <> EIGEN_STRONG_INLINE PacketN_8d pabs(const PacketN_8d &a) {
  FUNC_ENTRY;
  return PacketN_8d(pabs<PacketN_4d>(a.v));
}
template <> EIGEN_STRONG_INLINE PacketN_8d pmin(const PacketN_8d &a, const PacketN_8d &b) {
  FUNC_ENTRY;
  return PacketN_8d(pmin<PacketN_4d>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_8d pmax(const PacketN_8d &a, const PacketN_8d &b) {
  FUNC_ENTRY;
  return PacketN_8d(pmax<PacketN_4d>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE double predux<PacketN_8d>(const PacketN_8d &a) {
  FUNC_ENTRY;
  PacketN_4d tv0 = BBE_CONSTN_4XF64(0);
  PacketN_4d tva = extract_N_8d<PacketN_4d,PacketN_4d>(a.v, tv0);
  return predux<PacketN_4d>(tva);
}
template <> EIGEN_STRONG_INLINE PacketN_8d padd(const PacketN_8d &a, const PacketN_8d &b) {
  FUNC_ENTRY;
  return PacketN_8d(padd<PacketN_4d>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_8d pmul<PacketN_8d>(const PacketN_8d &a, const PacketN_8d &b) {
  FUNC_ENTRY;
  return PacketN_8d(pmul<PacketN_4d>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_8d pdiv<PacketN_8d>(const PacketN_8d &a, const PacketN_8d &b) {
  FUNC_ENTRY;
  PacketN_4d tv1 = BBE_CONSTN_4XF64(1);
  PacketN_4d tvb = extract_N_8d<PacketN_4d,PacketN_4d>(b.v, tv1); // Set denominator 1 in disabled lanes
  return PacketN_8d(pdiv<PacketN_4d>(a.v, tvb));
}
template <> EIGEN_STRONG_INLINE PacketN_8d pnegate(const PacketN_8d &a) {
  FUNC_ENTRY;
  return PacketN_8d(pnegate<PacketN_4d>(a.v));
}
template <> EIGEN_STRONG_INLINE double predux_mul<PacketN_8d>(const PacketN_8d &a) {
  FUNC_ENTRY;
  PacketN_4d tv1 = BBE_CONSTN_4XF64(1);
  PacketN_4d tva = extract_N_8d<PacketN_4d,PacketN_4d>(a.v, tv1);
  return predux_mul<PacketN_4d>(tva);
}
template <> EIGEN_STRONG_INLINE PacketN_8d psub<PacketN_8d>(const PacketN_8d &a, const PacketN_8d &b) {
  FUNC_ENTRY;
  return PacketN_8d(psub<PacketN_4d>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_8d preverse(const PacketN_8d &a) {
  FUNC_ENTRY;
  PacketN_4d tva = replicate_N_8d<PacketN_4d>(a.v);
  return PacketN_8d(preverse<PacketN_4d>(tva));
}
template <> EIGEN_STRONG_INLINE PacketN_8d pceil(const PacketN_8d &a) {
  FUNC_ENTRY;
  PacketN_4d tva = replicate_N_8d<PacketN_4d>(a.v);
  return PacketN_8d(pceil<PacketN_4d>(tva));
}
template <> EIGEN_STRONG_INLINE PacketN_8d pfloor(const PacketN_8d &a) {
  FUNC_ENTRY;
  PacketN_4d tva = replicate_N_8d<PacketN_4d>(a.v);
  return PacketN_8d(pfloor<PacketN_4d>(tva));
}
template <> EIGEN_STRONG_INLINE PacketN_8d pround(const PacketN_8d &a) {
  FUNC_ENTRY;
  PacketN_4d tva = replicate_N_8d<PacketN_4d>(a.v);
  return PacketN_8d(pround<PacketN_4d>(tva));
}
template <> EIGEN_STRONG_INLINE PacketN_8d ploaddup<PacketN_8d>(const double *from) {
  FUNC_ENTRY;
  return PacketN_8d(ploaddup<PacketN_4d>(from));
}
template <> EIGEN_STRONG_INLINE double pfirst<PacketN_8d>(const PacketN_8d &a) {
  FUNC_ENTRY;
  PacketN_4d tva = replicate_N_8d<PacketN_4d>(a.v);
  return pfirst<PacketN_4d>(tva);
}
/** \internal \brief Returns a packet with coefficients (a,a+1,...,a+packet_size-1). */
template <> EIGEN_STRONG_INLINE PacketN_8d plset<PacketN_8d>(const double &a) {
  FUNC_ENTRY;
  return PacketN_8d(plset<PacketN_4d>(a));
}

template<> EIGEN_STRONG_INLINE PacketN_8d pcmp_le(const PacketN_8d& a, const PacketN_8d& b) {return rep_T(a,b,PacketN_4d,PacketN_8d,pcmp_le,replicate_N_8d);}
template<> EIGEN_STRONG_INLINE PacketN_8d pcmp_lt(const PacketN_8d& a, const PacketN_8d& b) {return rep_T(a,b,PacketN_4d,PacketN_8d,pcmp_lt,replicate_N_8d);}
template<> EIGEN_STRONG_INLINE PacketN_8d pcmp_lt_or_nan(const PacketN_8d& a, const PacketN_8d& b) {return rep_T(a,b,PacketN_4d,PacketN_8d,pcmp_lt_or_nan,replicate_N_8d);}
template<> EIGEN_STRONG_INLINE PacketN_8d pcmp_eq(const PacketN_8d& a, const PacketN_8d& b) {return rep_T(a,b,PacketN_4d,PacketN_8d,pcmp_eq,replicate_N_8d);}

#if SIMD_N_2_EQ_4()
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8d, CONST_2> &kernel) {
  FUNC_ENTRY;
  #if SIMD_N_2_EQ_4()
      #define DV(_num) xb_vecN_4xf64 tvs##_num = BBE_CONSTN_4XF64(0)
      #define LV(_num) xb_vecN_4xf64 tvf##_num = kernel.packet[_num].v
      
      DV(0); DV(1);
      LV(0); LV(1);

      BBE_DSELN_4XF64I(tvs1, tvs0, tvf1, tvf0, BBE_DSELI_32B_INTERLEAVE_1);
      BBE_DSELN_4XF64I(tvf1, tvf0, tvs1, tvs0, BBE_DSELI_32B_INTERLEAVE_2);

      #define SV(_num) kernel.packet[_num] = PacketN_8d(tvf##_num)
      SV(0); SV(1);

      #undef DV
      #undef LV
      #undef SV
  #endif
}
#endif

#if XCHAL_HAVE_BBENEP
#if SIMD_N_EQ(8,2)
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8d, CONST_2> &kernel) {
  FUNC_ENTRY;
  #if SIMD_N_EQ(8,2)
      #define DV(_num) xb_vecN_4xcf32 tvs##_num = BBE_CONSTN_4XCF32(0)
      #define LV(_num) xb_vecN_4xcf32 tvf##_num = BBE_MOVN_4XCF32_FROMN_4XF64(kernel.packet[_num].v)

      DV(0); DV(1);
      LV(0); LV(1);

      BBE_DSELN_4XCF32I(tvs1, tvs0, tvf1, tvf0, BBE_DSELI_64B_INTERLEAVE_1);
      BBE_DSELN_4XCF32I(tvf1, tvf0, tvs1, tvs0, BBE_DSELI_64B_INTERLEAVE_2);
      
      #define SV(_num) kernel.packet[_num] = PacketN_8d(BBE_MOVN_4XF64_FROMN_4XCF32(tvf##_num))
      SV(0); SV(1);

      #undef DV
      #undef LV
      #undef SV
  #endif
}
#endif
#if SIMD_N_EQ(8,4) || SIMD_N_EQ(8,8)
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8d, CONST_4> &kernel) {
  FUNC_ENTRY;
  #if SIMD_N_EQ(8,4)
      #define DV(_num) xb_vecN_4xcf32 tvs##_num = BBE_CONSTN_4XCF32(0)
      #define LV(_num) xb_vecN_4xcf32 tvf##_num = BBE_MOVN_4XCF32_FROMN_4XF64(kernel.packet[_num].v)

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
      
      #define SV(_num) kernel.packet[_num] = PacketN_8d(BBE_MOVN_4XF64_FROMN_4XCF32(tvs##_num))
      SV(0); SV(1); SV(2); SV(3);

      #undef DV
      #undef LV
      #undef FIRST_STEP
      #undef SECOND_STEP
      #undef LAST_STEP
      #undef SV
  #elif SIMD_N_EQ(8,8)
      #define DV(_num) xb_vecN_4xcf32 tvs##_num = BBE_CONSTN_4XCF32(0)
      #define LV(_num) xb_vecN_4xcf32 tvf##_num = BBE_MOVN_4XCF32_FROMN_4XF64(kernel.packet[_num].v)

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
      
      #define SV(_num) kernel.packet[_num] = PacketN_8d(BBE_MOVN_4XF64_FROMN_4XCF32(tvs##_num))
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
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8d, CONST_8> &kernel) {
  FUNC_ENTRY;
  #if SIMD_N_EQ(8,8)
      #define DV(_num) xb_vecN_4xcf32 tvs##_num = BBE_CONSTN_4XCF32(0)
      #define LV(_num) xb_vecN_4xcf32 tvf##_num = BBE_MOVN_4XCF32_FROMN_4XF64(kernel.packet[_num].v)

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

      #define SV(_num) kernel.packet[_num] = PacketN_8d(BBE_MOVN_4XF64_FROMN_4XCF32(tvf##_num))
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

template <> EIGEN_STRONG_INLINE PacketN_8d ploadquad<PacketN_8d>(const double *from) {
  FUNC_ENTRY;
  return PacketN_8d(ploadquad<PacketN_4d>(from));
}
#if XCHAL_HAVE_BBENEP
template <> EIGEN_DEVICE_FUNC inline PacketN_8d pgather<double, PacketN_8d>(const double *from, Index stride) {
  FUNC_ENTRY;
  double *tp = (double *)(from);
  xb_vecN_4xf64  ta;
  double *tpf = (double *)(&ta);
  int    i;
  int    stride_bytes = sizeof(double) * stride;
  xb_vecN_4xf64 tvf;

  for (i = 0; i < SIMD_N_8; i++) {
    BBE_LSN_4XF64_XP(tvf, tp, stride_bytes);
    BBE_SSN_4XF64_IP(tvf, tpf, sizeof(double));
  }
  return PacketN_8d(ta);
}
template <> EIGEN_DEVICE_FUNC inline void pscatter<double, PacketN_8d>(double *to, const PacketN_8d &from, Index stride) {
  FUNC_ENTRY;
  double *tpf = (double*)(&from.v);
  double *tpt = (double*)(to);
  int    stride_bytes = sizeof(double) * stride;
  xb_vecN_4xf64 tvf;
  int i;
  for (i = 0; i < SIMD_N_8; i++)
  {
    BBE_LSN_4XF64_IP(tvf, tpf, sizeof(double));
    BBE_SSN_4XF64_XP(tvf, tpt, stride_bytes);
  }
}
#endif
template<> EIGEN_STRONG_INLINE double predux_min<PropagateNaN,PacketN_8d>(const PacketN_8d &a) {
  FUNC_ENTRY;
  PacketN_4d tva = replicate_N_8d<PacketN_4d>(a.v);
  return predux_min<PropagateNaN,PacketN_4d>(tva);
}
template<> EIGEN_STRONG_INLINE double predux_max<PropagateNaN,PacketN_8d>(const PacketN_8d &a) {
  FUNC_ENTRY;
  PacketN_4d tva = replicate_N_8d<PacketN_4d>(a.v);
  return predux_max<PropagateNaN,PacketN_4d>(tva);
}
#endif

} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_F64_HALF_KX_XTENSA_H
