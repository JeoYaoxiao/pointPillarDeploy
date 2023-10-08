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

#ifndef EIGEN_PACKET_MATH_F32_HIFI5_XTENSA_H
#define EIGEN_PACKET_MATH_F32_HIFI5_XTENSA_H

#if SIMD_N_2_EQ_2()
  #ifndef F32_FULL_PACKET_SUPPORT
    #define F32_FULL_PACKET_SUPPORT 1
  #endif
#else
  #if defined(F32_FULL_PACKET_SUPPORT) && F32_FULL_PACKET_SUPPORT
    #pragma message("Incorrect value for F32_FULL_PACKET_SUPPORT. Resetting it back")
    #undef F32_FULL_PACKET_SUPPORT
  #endif
  #define F32_FULL_PACKET_SUPPORT 0
#endif

#if F32_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

typedef xtfloatx2 PacketN_2f;

#ifndef F32_HALF_PACKET_SUPPORT
#  define F32_HALF_PACKET_SUPPORT 0
#endif
#if F32_HALF_PACKET_SUPPORT
#  warning "Incorrect value set for F32_HALF_PACKET_SUPPORT. Resetting it back"
#  undef F32_HALF_PACKET_SUPPORT
#  define F32_HALF_PACKET_SUPPORT 0
#endif

template <> struct is_arithmetic<PacketN_2f> {
  enum { value = true };
};

template <> struct packet_traits<float> : default_packet_traits {
  typedef PacketN_2f type;
  typedef PacketN_2f half;
  enum {
    Vectorizable    = 1,
    AlignedOnScalar = 1,
    size            = SIMD_N_2,
    HasHalfPacket   = 0,

    HasAdd       = 1,
    HasSub       = 1,
    HasShift     = 1,
    HasMul       = 1,
    HasNegate    = 1,
    HasAbs       = 1,
    HasArg       = 0,
    HasAbs2      = 0,
    HasAbsDiff   = 0,
    HasMin       = 1,
    HasMax       = 1,
    HasConj      = 1,
    HasSetLinear = 0,

    HasDiv   = 1,
    HasSin   = XCLIB_SUPPORT,
    HasCos   = XCLIB_SUPPORT,
    HasLog   = XCLIB_SUPPORT,
    HasExp   = 0,
    HasSqrt  = 1,
    HasRsqrt = 1,
    HasTanh  = 0,
    HasBlend = 0,
    HasRound = 1,
    HasFloor = 1,
    HasCeil  = 1,

    HasTan   = XCLIB_SUPPORT,
    HasACos  = 0,
    HasASin  = 0,
    HasATan  = XCLIB_SUPPORT,
    HasCosh  = 0,
    HasSinh  = 0,
    HasLog10 = XCLIB_SUPPORT,
    HasLog1p = 0,

    HasSign = 0,
    HasErf  = 0
  };
};


template <> struct unpacket_traits<PacketN_2f> {
  typedef float type;
  typedef PacketN_2f half;
  enum {size=SIMD_N_2, alignment=ALIGN_N_2, vectorizable=true, masked_load_available=false, masked_store_available=false};
};

inline std::ostream & operator <<(std::ostream & s, const PacketN_2f & v) {
  float *ptr = (float *)(&v);
  int itr = 0;
  for (itr = 0; itr < (SIMD_N_2-1); itr++)
  {
    s << ptr[itr] << ", ";
  }
  s << ptr[itr];
  return s;
}

template <> EIGEN_STRONG_INLINE PacketN_2f pset1<PacketN_2f>(const float &from) {
  FUNC_ENTRY;
  return from;

}
template <> EIGEN_STRONG_INLINE PacketN_2f pload1<PacketN_2f>(const float *from) {
  FUNC_ENTRY;
  return pset1<PacketN_2f>(*from);
}

/** \internal \brief Returns a packet with coefficients (a,a+1,...,a+packet_size-1). */
template <> EIGEN_STRONG_INLINE PacketN_2f plset<PacketN_2f>(const float &a) {
  FUNC_ENTRY;
  PacketN_2f vt,va;
  float t[2] __attribute__((aligned(16))) = {0,1};
  vt = *(xt_reinterpret_cast<PacketN_2f,float>(t));
  va = a;
  return va+vt;
}

template <> EIGEN_STRONG_INLINE PacketN_2f padd<PacketN_2f>(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return a+b;
}
template <> EIGEN_STRONG_INLINE PacketN_2f psub<PacketN_2f>(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return a-b;
}
template <> EIGEN_STRONG_INLINE PacketN_2f pnegate(const PacketN_2f &a) {
  FUNC_ENTRY;
  return -a;
}
template <> EIGEN_STRONG_INLINE PacketN_2f pconj(const PacketN_2f &a) {
  FUNC_ENTRY;
  return a; 
}
template <> EIGEN_STRONG_INLINE PacketN_2f pmul<PacketN_2f>(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return a*b;
}
template <> EIGEN_STRONG_INLINE PacketN_2f pdiv<PacketN_2f>(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return DIV_SX2(a,b);
}

/** \internal \returns a * b + c (coeff-wise) */
template <> EIGEN_STRONG_INLINE PacketN_2f pmadd(const PacketN_2f &a, const PacketN_2f &b, const PacketN_2f &c) {
  FUNC_ENTRY;
  return (a*b)+c;
}

template <> EIGEN_STRONG_INLINE PacketN_2f pand(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_INT32X2_AND_INT32X2(XT_AE_MOVINT32X2_FROMXTFLOATX2(a), XT_AE_MOVINT32X2_FROMXTFLOATX2(b)));
}
template <> EIGEN_STRONG_INLINE PacketN_2f por(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_INT32X2_OR_INT32X2(XT_AE_MOVINT32X2_FROMXTFLOATX2(a), XT_AE_MOVINT32X2_FROMXTFLOATX2(b)));
}
template <> EIGEN_STRONG_INLINE PacketN_2f pandnot(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_INT32X2_AND_INT32X2(XT_AE_MOVINT32X2_FROMXTFLOATX2(a), AE_INT32X2_NOT(XT_AE_MOVINT32X2_FROMXTFLOATX2(b))));
}

template <> EIGEN_STRONG_INLINE PacketN_2f pmin(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return MINNUM_SX2(a,b);
}
template <> EIGEN_STRONG_INLINE PacketN_2f pmax(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return MAXNUM_SX2(a,b);
}
template <> EIGEN_STRONG_INLINE PacketN_2f pround(const PacketN_2f &a) {
  FUNC_ENTRY;
  return FIROUND_SX2(a);
}
template <> EIGEN_STRONG_INLINE PacketN_2f pceil(const PacketN_2f &a) {
  FUNC_ENTRY;
  return FICEIL_SX2(a);
}
template <> EIGEN_STRONG_INLINE PacketN_2f pfloor(const PacketN_2f &a) {
  FUNC_ENTRY;
  return FIFLOOR_SX2(a);
}

template <> EIGEN_STRONG_INLINE PacketN_2f pload<PacketN_2f>(const float *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  return *(xt_reinterpret_const_cast<PacketN_2f,float>(from));
}

template <> EIGEN_STRONG_INLINE PacketN_2f ploadu<PacketN_2f>(const float *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_LOAD;
  PacketN_2f tva;
  ae_valign tal;
  const PacketN_2f* tpv;
  tpv=xt_reinterpret_const_cast<PacketN_2f,float>(from);
  tal=AE_LA64_PP(tpv);
  AE_LASX2IP(tva,tal,tpv);
  return tva;
}

template <> EIGEN_STRONG_INLINE void pstore<float>(float *to, const PacketN_2f &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_STORE;
  PacketN_2f *pto = xt_reinterpret_cast<PacketN_2f,float>(to);
  *pto = from;
}

template <> EIGEN_STRONG_INLINE void pstoreu<float>(float *to, const PacketN_2f &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_STORE;
  ae_valign tal;
  PacketN_2f* tpv;
  tpv=xt_reinterpret_cast<PacketN_2f,float>(to);
  tal = AE_ZALIGN64();
  AE_SASX2IP(from,tal,tpv);
  AE_SA64POS_FP(tal, tpv);
}

template <> EIGEN_DEVICE_FUNC inline PacketN_2f pgather<float, PacketN_2f>(const float *from, Index stride) {
  FUNC_ENTRY;
  float *tp = (float *)(from);
  PacketN_2f  tva;
  float *tpf = (float *)(&tva);
  int    stride_bytes = sizeof(float) * stride;
  int    i;
  int len = sizeof(PacketN_2f)/sizeof(float);
  float tf;

  for (i = 0; i < len; i++) {
    AE_LSXP(tf, tp, stride_bytes);
    AE_SSIP(tf, tpf, sizeof(float));
  }
  return tva;
}

template <> EIGEN_DEVICE_FUNC inline void pscatter<float, PacketN_2f>(float *to, const PacketN_2f &from, Index stride) {
  FUNC_ENTRY;
  float *tpf = (float*)(&from);
  float *tpt = (float*)(to);
  int    stride_bytes = sizeof(float) * stride;
  float tf;
  int i;
  int len = sizeof(PacketN_2f)/sizeof(float);
  for (i = 0; i < len; i++)
  {
    AE_LSIP(tf, tpf, sizeof(float));
    AE_SSXP(tf, tpt, stride_bytes);
  }
}

template <> EIGEN_STRONG_INLINE void pstore1<PacketN_2f>(float *to, const float &a) {
  FUNC_ENTRY;
  pstore(to, PacketN_2f((float)a)); 
}

template <> EIGEN_STRONG_INLINE float pfirst<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  return HIGH_S(a); 
}

template <> EIGEN_STRONG_INLINE PacketN_2f preverse(const PacketN_2f &a) {
  FUNC_ENTRY;
  return AE_SEL32_LH_SX2(a, a);
}
template <> EIGEN_STRONG_INLINE PacketN_2f pabs(const PacketN_2f &a) {
  FUNC_ENTRY;
  return ABS_SX2(a); 
}

template <> EIGEN_STRONG_INLINE float predux<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  return RADD_SX2(a);
}

/** \internal \returns the product of the elements of \a a*/
template <> EIGEN_STRONG_INLINE float predux_mul<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  return MUL_S(HIGH_S(a),LOW_S(a));
}

template <> EIGEN_STRONG_INLINE float predux_min<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  return RMIN_S(a); 
}
template <> EIGEN_STRONG_INLINE float predux_max<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  return RMAX_S(a); 
}

EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_2> &kernel) {
  FUNC_ENTRY;
  #define TRANSPOSE_2x2(_x0_out,_x1_out,_x0_inp,_x1_inp)\
  {\
    xtfloatx2 _c0;\
    _c0 = AE_SEL32_HH_SX2(_x0_inp, _x1_inp);\
    _x1_out = AE_SEL32_LL_SX2(_x0_inp, _x1_inp);\
    _x0_out = _c0;\
  }

  TRANSPOSE_2x2(kernel.packet[0],kernel.packet[1],kernel.packet[0],kernel.packet[1]);
}

template<> EIGEN_STRONG_INLINE PacketN_2f pcmp_eq(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  ae_int32x2 tv0 = 0x00000000u;
  ae_int32x2 tv1 = 0xffffffffu;
  ae_int32x2 tvret = tv0;
  AE_MOVT32X2(tvret,tv1,OEQ_SX2(a,b));
  return PacketN_2f(AE_MOVXTFLOATX2_FROMINT32X2(tvret));
}
template<> EIGEN_STRONG_INLINE PacketN_2f pcmp_le(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  ae_int32x2 tv0 = 0x00000000u;
  ae_int32x2 tv1 = 0xffffffffu;
  ae_int32x2 tvret = tv0;
  AE_MOVT32X2(tvret,tv1,OLE_SX2(a,b));
  return PacketN_2f(AE_MOVXTFLOATX2_FROMINT32X2(tvret));
}
template<> EIGEN_STRONG_INLINE PacketN_2f pcmp_lt(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  ae_int32x2 tv0 = 0x00000000u;
  ae_int32x2 tv1 = 0xffffffffu;
  ae_int32x2 tvret = tv0;
  AE_MOVT32X2(tvret,tv1,OLT_SX2(a,b));
  return PacketN_2f(AE_MOVXTFLOATX2_FROMINT32X2(tvret));
}
template<> EIGEN_STRONG_INLINE PacketN_2f pcmp_lt_or_nan(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  ae_int32x2 tv0 = 0x00000000u;
  ae_int32x2 tv1 = 0xffffffffu;
  ae_int32x2 tvret = tv0;
  AE_MOVT32X2(tvret,tv1,ULT_SX2(a,b));
  return PacketN_2f(AE_MOVXTFLOATX2_FROMINT32X2(tvret));
}
template<> EIGEN_DEVICE_FUNC inline PacketN_2f pselect(const PacketN_2f& mask, const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  xtfloatx2 tvret = a;
  MOVT_SX2(tvret,b,OEQ_SX2(mask,PacketN_2f(CONST_S(0))));
  return tvret;
}

} // end namespace internal

} // end namespace Eigen

#endif

#endif
