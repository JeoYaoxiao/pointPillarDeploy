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

#ifndef EIGEN_PACKET_MATH_CF64_HALF_KX_XTENSA_H
#define EIGEN_PACKET_MATH_CF64_HALF_KX_XTENSA_H

#if CF64_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

#if CF64_HALF_PACKET_SUPPORT
template <> EIGEN_STRONG_INLINE PacketN_8cd replicate_N_16cd(const PacketN_8cd& a){
  return extract_N_16cd<PacketN_8cd,PacketN_8cd>(a, a);
}
#else
typedef PacketN_8cd PacketN_16cd;
#endif

#if CF64_HALF_PACKET_SUPPORT
template <> EIGEN_STRONG_INLINE PacketN_16cd pset1<PacketN_16cd>(const complexdouble &from) {
  FUNC_ENTRY;
  return PacketN_16cd(pset1<PacketN_8cd>(from));
}
template <> EIGEN_STRONG_INLINE void pstoreu<complexdouble>(complexdouble *to, const PacketN_16cd &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_STORE;
  valign         talign;
  xb_vecN_8xcf64 *tpv0 = xt_reinterpret_cast<xb_vecN_8xcf64,complexdouble>(to);
  talign           = BBE_ZALIGN();
  BBE_SAVN_8XCF64_XP(from.v.v, talign, tpv0, sizeof(complexdouble)*SIMD_N_16);
  BBE_SAN_8XCF64POS_FP(talign, tpv0);
}
template <> EIGEN_STRONG_INLINE void pstore<complexdouble>(complexdouble *to, const PacketN_16cd &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_STORE;
  return pstoreu<complexdouble>(to, from);
}
template <> EIGEN_STRONG_INLINE PacketN_16cd ploadu<PacketN_16cd>(const complexdouble *from) {
  FUNC_ENTRY;
  xb_vecN_8xcf64     tv0;
  valign         talign;
  xb_vecN_8xcf64 *tpv0 = xt_reinterpret_const_cast<xb_vecN_8xcf64,complexdouble>(from);
  talign              = BBE_LAN_8XCF64_PP(tpv0);
  BBE_LAVN_8XCF64_XP(tv0, talign, tpv0, sizeof(complexdouble)*SIMD_N_16);
  return PacketN_16cd(tv0);
}
template <> EIGEN_STRONG_INLINE PacketN_16cd pload<PacketN_16cd>(const complexdouble *from) {
  FUNC_ENTRY;
  return ploadu<PacketN_16cd>(from);
}
template <> EIGEN_STRONG_INLINE complexdouble predux<PacketN_16cd>(const PacketN_16cd &a) {
  FUNC_ENTRY;
  PacketN_8cd tv0 = pset1<PacketN_8cd>(complexdouble(0,0));
  PacketN_8cd tva = extract_N_16cd<PacketN_8cd,PacketN_8cd>(a.v, tv0);
  return predux<PacketN_8cd>(tva);
}
template <> EIGEN_STRONG_INLINE PacketN_16cd padd(const PacketN_16cd &a, const PacketN_16cd &b) {
  FUNC_ENTRY;
  return PacketN_16cd(padd<PacketN_8cd>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_16cd pmul<PacketN_16cd>(const PacketN_16cd &a, const PacketN_16cd &b) {
  FUNC_ENTRY;
  return PacketN_16cd(pmul<PacketN_8cd>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_16cd pdiv<PacketN_16cd>(const PacketN_16cd &a, const PacketN_16cd &b) {
  FUNC_ENTRY;
  PacketN_8cd tv1 = pset1<PacketN_8cd>(complexdouble(1,0));
  PacketN_8cd tvb = extract_N_16cd<PacketN_8cd,PacketN_8cd>(b.v, tv1); // Set denominator 1 in disabled lanes
  return PacketN_16cd(pdiv<PacketN_8cd>(a.v, tvb));
}
template <> EIGEN_STRONG_INLINE PacketN_16cd pnegate(const PacketN_16cd &a) {
  FUNC_ENTRY;
  return PacketN_16cd(pnegate<PacketN_8cd>(a.v));
}
template <> EIGEN_STRONG_INLINE complexdouble predux_mul<PacketN_16cd>(const PacketN_16cd &a) {
  FUNC_ENTRY;
  PacketN_8cd tv1 = pset1<PacketN_8cd>(complexdouble(1,0));
  PacketN_8cd tva = extract_N_16cd<PacketN_8cd,PacketN_8cd>(a.v, tv1);
  return predux_mul<PacketN_8cd>(tva);
}
template <> EIGEN_STRONG_INLINE PacketN_16cd psub<PacketN_16cd>(const PacketN_16cd &a, const PacketN_16cd &b) {
  FUNC_ENTRY;
  return PacketN_16cd(psub<PacketN_8cd>(a.v, b.v));
}
template <> EIGEN_STRONG_INLINE PacketN_16cd ploadquad<PacketN_16cd>(const complexdouble *from) {
  FUNC_ENTRY;
  return PacketN_16cd(ploadquad<PacketN_8cd>(from));
}
template <> EIGEN_STRONG_INLINE PacketN_16cd pconj(const PacketN_16cd &a) {
  FUNC_ENTRY;
  PacketN_8cd tva = replicate_N_16cd<PacketN_8cd>(a.v);
  return PacketN_16cd(pconj<PacketN_8cd>(tva));
}
template <> EIGEN_STRONG_INLINE PacketN_16cd preverse(const PacketN_16cd &a) {
  FUNC_ENTRY;
  PacketN_8cd tva = replicate_N_16cd<PacketN_8cd>(a.v);
  return PacketN_16cd(preverse<PacketN_8cd>(tva));
}
/** \internal \returns \a a with real and imaginary part flipped (for complex type only) */
template<> EIGEN_STRONG_INLINE PacketN_16cd pcplxflip<PacketN_16cd>(const PacketN_16cd& a) {
  FUNC_ENTRY;
  PacketN_8cd tva = replicate_N_16cd<PacketN_8cd>(a.v);
  return PacketN_16cd(pcplxflip<PacketN_8cd>(tva));
}
template <> EIGEN_STRONG_INLINE complexdouble pfirst<PacketN_16cd>(const PacketN_16cd &a) {
  FUNC_ENTRY;
  PacketN_8cd tva = replicate_N_16cd<PacketN_8cd>(a.v);
  return pfirst<PacketN_8cd>(tva);
}

#if XCHAL_HAVE_BBENEP
#if SIMD_N_EQ(16,2) || SIMD_N_EQ(16,4)
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_16cd, CONST_2> &kernel) {
  FUNC_ENTRY;
  #if SIMD_N_EQ(16,2)
      #define DV(_num) xb_vecN_8xcf64 tvs##_num = BBE_CONSTN_8XCF64(0)
      #define LV(_num) xb_vecN_8xcf64 tvf##_num = kernel.packet[_num].v.v

      DV(0); DV(1);
      LV(0); LV(1);

      #define FIRST_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##1, _inp##0, _pat);    \


      FIRST_STEP(BBE_DSELI_64B_INTERLEAVE_2, tvs, tvf);
      FIRST_STEP(BBE_DSELI_64B_INTERLEAVE_4, tvf, tvs);
      
      #define SV(_num) kernel.packet[_num] = PacketN_16cd(tvf##_num)
      SV(0); SV(1);

      #undef DV
      #undef LV
      #undef FIRST_STEP
      #undef SV
  #elif SIMD_N_EQ(16,4)
      #define DV(_num) xb_vecN_8xcf64 tvs##_num = BBE_CONSTN_8XCF64(0)
      #define LV(_num) xb_vecN_8xcf64 tvf##_num = kernel.packet[_num].v.v

      DV(0); DV(1);
      LV(0); LV(1);

      #define FIRST_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##1, _inp##0, _pat);    \


      FIRST_STEP(BBE_DSELI_64B_INTERLEAVE_2, tvs, tvf);
      FIRST_STEP(BBE_DSELI_64B_INTERLEAVE_8, tvf, tvs);
      
      #define SV(_num) kernel.packet[_num] = PacketN_16cd(tvf##_num)
      SV(0); SV(1);

      #undef DV
      #undef LV
      #undef FIRST_STEP
      #undef SV
  #endif
}
#endif
#if SIMD_N_EQ(16,4)
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_16cd, CONST_4> &kernel) {
  FUNC_ENTRY;

  #define DV(_num) xb_vecN_8xcf64 tvs##_num = BBE_CONSTN_8XCF64(0)
  #define LV(_num) xb_vecN_8xcf64 tvf##_num = kernel.packet[_num].v.v

  DV(0); DV(1); DV(2); DV(3);
  LV(0); LV(1); LV(2); LV(3);

  #define FIRST_STEP(_pat, _out, _inp)                             \
    BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##1, _inp##0, _pat);   \
    BBE_DSELN_8XCF64I(_out##3, _out##2, _inp##3, _inp##2, _pat);   \

  #define SECOND_STEP(_pat, _out, _inp)                            \
    BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##2, _inp##0, _pat);   \
    _out##2 = _inp##2; _out##3 = _inp##3;
  
  #define LAST_STEP(_pat, _out, _inp)                              \
    BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##2, _inp##0, _pat);   \
    BBE_DSELN_8XCF64I(_out##3, _out##2, _inp##3, _inp##1, _pat);   \

  FIRST_STEP(BBE_DSELI_64B_INTERLEAVE_2, tvs, tvf);
  SECOND_STEP(BBE_DSELI_64B_INTERLEAVE_4, tvf, tvs);
  LAST_STEP(BBE_DSELI_64B_INTERLEAVE_8, tvs, tvf);
  
  #define SV(_num) kernel.packet[_num] = PacketN_16cd(tvs##_num)
  SV(0); SV(1); SV(2); SV(3);

  #undef DV
  #undef LV
  #undef FIRST_STEP
  #undef SECOND_STEP
  #undef LAST_STEP
  #undef SV
}
#endif
#endif

template <> EIGEN_STRONG_INLINE PacketN_16cd ploaddup<PacketN_16cd>(const complexdouble *from) {
  FUNC_ENTRY;
  return PacketN_16cd(ploaddup<PacketN_8cd>(from));
}
template<> EIGEN_STRONG_INLINE PacketN_16cd pcmp_eq(const PacketN_16cd& a, const PacketN_16cd& b) {
  FUNC_ENTRY;
  PacketN_8cd tva = replicate_N_16cd<PacketN_8cd>(a.v);
  PacketN_8cd tvb = replicate_N_16cd<PacketN_8cd>(b.v);
  return PacketN_16cd(pcmp_eq<PacketN_8cd>(tva,tvb));
}
#if XCHAL_HAVE_BBENEP
template <> EIGEN_DEVICE_FUNC inline PacketN_16cd pgather<complexdouble, PacketN_16cd>(const complexdouble *from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtcomplexdouble *tp = xt_reinterpret_const_cast<xtcomplexdouble,complexdouble>(from);
  xb_vecN_8xcf64 ta;
  xtcomplexdouble *tpf = xt_reinterpret_cast<xtcomplexdouble,xb_vecN_8xcf64>(&ta);
  int    i;
  int    stride_bytes = sizeof(xtcomplexdouble) * stride;
  xtcomplexdouble tvf;

  for (i = 0; i < SIMD_N_16; i++) {
    BBE_LSN_8XCF64_XP(tvf, tp, stride_bytes);
    BBE_SSN_8XCF64_IP(BBE_MOVN_8XCF64_FROMCF64(tvf), tpf, sizeof(xtcomplexdouble));
  }
  return PacketN_16cd(ta);
}

template <> EIGEN_DEVICE_FUNC inline void pscatter<complexdouble, PacketN_16cd>(complexdouble *to, const PacketN_16cd &from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  int i;
  xtcomplexdouble *tpf = xt_reinterpret_const_cast<xtcomplexdouble,PacketN_16cd>(&from);
  xtcomplexdouble *tpt = xt_reinterpret_cast<xtcomplexdouble,complexdouble>(to);
  int    stride_bytes = sizeof(xtcomplexdouble) * stride;
  xtcomplexdouble tvf;
  for (i = 0; i < SIMD_N_16; i++)
  {
    BBE_LSN_8XCF64_IP(tvf, tpf, sizeof(xtcomplexdouble));
    BBE_SSN_8XCF64_XP(BBE_MOVN_8XCF64_FROMCF64(tvf), tpt, stride_bytes);
  }
}
#endif
#endif

} // end namespace internal

} // end namespace Eigen
#endif

#endif // EIGEN_PACKET_MATH_CF64_HALF_KX_XTENSA_H
