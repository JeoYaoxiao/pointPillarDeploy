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

#ifndef EIGEN_PACKET_MATH_F32_HIFI5_3_XTENSA_H
#define EIGEN_PACKET_MATH_F32_HIFI5_3_XTENSA_H

#if SIMD_N_2_EQ_4()
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

typedef xtfloatx4 PacketN_2f;

#  if SIMD_N_EQ(4,2)
#    ifndef F32_HALF_PACKET_SUPPORT
#      define F32_HALF_PACKET_SUPPORT 1
#    endif
#  else
#    ifndef F32_HALF_PACKET_SUPPORT
#      define F32_HALF_PACKET_SUPPORT 0
#    else
#      if F32_HALF_PACKET_SUPPORT
#        pragma message("Incorrect value set for F32_HALF_PACKET_SUPPORT. Resetting it back")
#        undef F32_HALF_PACKET_SUPPORT
#        define F32_HALF_PACKET_SUPPORT 0
#      endif
#    endif
#  endif

#if F32_HALF_PACKET_SUPPORT
  struct PacketN_4f {
    EIGEN_STRONG_INLINE PacketN_4f() {}
    EIGEN_STRONG_INLINE explicit PacketN_4f(const PacketN_2f& a) {v = AE_EXTRACTSX2_FROMSX4_H(a);}
	EIGEN_STRONG_INLINE explicit PacketN_4f(const xtfloatx2& a) : v(a) {}
	EIGEN_STRONG_INLINE explicit PacketN_4f(xtfloatx2& a) : v(a) {}

	EIGEN_STRONG_INLINE operator PacketN_2f&() {xtfloatx2 zero = 0.0f; vx4 = AE_JOINSX4_FROMSX2(v, zero); return vx4;} 
    xtfloatx4 vx4;
    xtfloatx2 v;
  };
#else
  typedef PacketN_2f PacketN_4f; 
#endif

template <> struct is_arithmetic<PacketN_2f> {
  enum { value = true };
};

template <> struct packet_traits<float> : default_packet_traits {
  typedef PacketN_2f type;
  typedef PacketN_4f half;
  enum {
    Vectorizable    = 1,
    AlignedOnScalar = 1,
    size            = SIMD_N_2,
    #if F32_HALF_PACKET_SUPPORT
      HasHalfPacket   = 1,
    #else
      HasHalfPacket   = 0,
    #endif

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
  typedef PacketN_4f half;
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
  float t[4] __attribute__((aligned(16))) = {0,1,2,3};
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
  return DIV_SX4(a,b);
}

/** \internal \returns a * b + c (coeff-wise) */
template <> EIGEN_STRONG_INLINE PacketN_2f pmadd(const PacketN_2f &a, const PacketN_2f &b, const PacketN_2f &c) {
  FUNC_ENTRY;
  return (a*b)+c;
}

#define EXTRACTSX2_FROMSX4(_out_x2_1, _out_x2_0, _inp_x4)\
  _out_x2_0 = AE_EXTRACTSX2_FROMSX4_L(_inp_x4); _out_x2_1 = AE_EXTRACTSX2_FROMSX4_H(_inp_x4);

template <> EIGEN_STRONG_INLINE PacketN_2f pand(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  xtfloatx4 c = AE_MOVXTFLOATX4_FROMINT32X4(AE_INT32X4_AND(AE_MOVINT32X4_FROMXTFLOATX4(a), AE_MOVINT32X4_FROMXTFLOATX4(b)));
  return PacketN_2f(c);
}
template <> EIGEN_STRONG_INLINE PacketN_2f por(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  xtfloatx4 c = AE_MOVXTFLOATX4_FROMINT32X4(AE_INT32X4_OR(AE_MOVINT32X4_FROMXTFLOATX4(a), AE_MOVINT32X4_FROMXTFLOATX4(b)));
  return PacketN_2f(c);
}
template <> EIGEN_STRONG_INLINE PacketN_2f pandnot(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  xtfloatx4 c = AE_MOVXTFLOATX4_FROMINT32X4(AE_INT32X4_AND(AE_MOVINT32X4_FROMXTFLOATX4(a), AE_INT32X4_BNOT(AE_MOVINT32X4_FROMXTFLOATX4(b))));
  return PacketN_2f(c);
}

template <> EIGEN_STRONG_INLINE PacketN_2f pmin(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return MINNUM_SX4(a,b);
}
template <> EIGEN_STRONG_INLINE PacketN_2f pmax(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return MAXNUM_SX4(a,b);
}
template <> EIGEN_STRONG_INLINE PacketN_2f pround(const PacketN_2f &a) {
  FUNC_ENTRY;
  return FIROUND_SX4(a);
}
template <> EIGEN_STRONG_INLINE PacketN_2f pceil(const PacketN_2f &a) {
  FUNC_ENTRY;
  return FICEIL_SX4(a);
}
template <> EIGEN_STRONG_INLINE PacketN_2f pfloor(const PacketN_2f &a) {
  FUNC_ENTRY;
  return FIFLOOR_SX4(a);
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
  ae_valignx2 tal;
  const PacketN_2f* tpv;
  tpv=xt_reinterpret_const_cast<PacketN_2f,float>(from);
  tal=AE_LASX4PP(tpv);
  AE_LASX4IP(tva,tal,tpv);
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
  ae_valignx2 tal;
  PacketN_2f* tpv;
  tpv=xt_reinterpret_cast<PacketN_2f,float>(to);
  tal = AE_ZALIGN128();
  AE_SASX4IP(from,tal,tpv);
  AE_SA128POS_FP(tal, tpv);
}

template <> EIGEN_STRONG_INLINE PacketN_2f ploaddup<PacketN_2f>(const float *from) {
  FUNC_ENTRY;
  float a[SIMD_N_2] = {0};
  a[0] = from[0];
  a[1] = from[0];
  a[2] = from[1];
  a[3] = from[1];
  return ploadu<PacketN_2f>(a);
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
  return HIGH_S(AE_EXTRACTSX2_FROMSX4_H(a)); 
}

template <> EIGEN_STRONG_INLINE PacketN_2f preverse(const PacketN_2f &a) {
  FUNC_ENTRY;
  xtfloatx2 a_L,a_H;
  EXTRACTSX2_FROMSX4(a_H, a_L, a);
  xtfloatx2 va_L = AE_SEL32_LH_SX2(a_L, a_L);
  xtfloatx2 va_H = AE_SEL32_LH_SX2(a_H, a_H);
  return PacketN_2f(AE_JOINSX4_FROMSX2(va_L, va_H)); 
}

template <> EIGEN_STRONG_INLINE PacketN_2f pabs(const PacketN_2f &a) {
  FUNC_ENTRY;
  return ABS_SX4(a); 
}

template <> EIGEN_STRONG_INLINE float predux<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  return RADD_SX4(a);
}

/** \internal \returns the product of the elements of \a a*/
template <> EIGEN_STRONG_INLINE float predux_mul<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  xtfloatx2 a0, a1;
  xtfloatx2 a0x1;
  EXTRACTSX2_FROMSX4(a1, a0, a);
  a0x1 = MUL_SX2(a0,a1);
  return MUL_S(HIGH_S(a0x1),LOW_S(a0x1));
}

template <> EIGEN_STRONG_INLINE float predux_min<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  return RMIN_SX4(a); 
}
template <> EIGEN_STRONG_INLINE float predux_max<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  return RMAX_SX4(a); 
}

EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_4> &kernel) {
  FUNC_ENTRY;
  #define INPLACE_TRANSPOSE_2x2(_x0,_x1)\
  {\
    xtfloatx2 _c0 = AE_SEL32_HH_SX2(_x0, _x1);\
    _x1 = AE_SEL32_LL_SX2(_x0, _x1);\
	_x0 = _c0;\
  }
  xtfloatx4 r0, r1, r2, r3;

  r0 = kernel.packet[0];
  r1 = kernel.packet[1];
  r2 = kernel.packet[2];
  r3 = kernel.packet[3];

  xtfloatx2 r0_c01, r0_c23;
  xtfloatx2 r1_c01, r1_c23;
  xtfloatx2 r2_c01, r2_c23;
  xtfloatx2 r3_c01, r3_c23;

  EXTRACTSX2_FROMSX4(r0_c23, r0_c01, r0);
  EXTRACTSX2_FROMSX4(r1_c23, r1_c01, r1);
  EXTRACTSX2_FROMSX4(r2_c23, r2_c01, r2);
  EXTRACTSX2_FROMSX4(r3_c23, r3_c01, r3);

  INPLACE_TRANSPOSE_2x2(r0_c01, r1_c01); INPLACE_TRANSPOSE_2x2(r0_c23, r1_c23);
  INPLACE_TRANSPOSE_2x2(r2_c01, r3_c01); INPLACE_TRANSPOSE_2x2(r2_c23, r3_c23);

  r0 = AE_JOINSX4_FROMSX2(r0_c23, r2_c23);
  r1 = AE_JOINSX4_FROMSX2(r1_c23, r3_c23);
  r2 = AE_JOINSX4_FROMSX2(r0_c01, r2_c01);
  r3 = AE_JOINSX4_FROMSX2(r1_c01, r3_c01);

  kernel.packet[0] = r0;
  kernel.packet[1] = r1;
  kernel.packet[2] = r2;
  kernel.packet[3] = r3;
}

template<> EIGEN_STRONG_INLINE PacketN_2f pcmp_eq(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  ae_int32x4 tv0 = 0x00000000u;	xtfloatx4 cv0 = AE_MOVXTFLOATX4_FROMINT32X4(tv0); 
  ae_int32x4 tv1 = 0xffffffffu; xtfloatx4 cv1 = AE_MOVXTFLOATX4_FROMINT32X4(tv1); 
  xtbool4 tvb = OEQ_SX4(a, b);
  MOVT_SX4(cv0,cv1,tvb); 
  return PacketN_2f(cv0);
}
template<> EIGEN_STRONG_INLINE PacketN_2f pcmp_le(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  ae_int32x4 tv0 = 0x00000000u;	xtfloatx4 cv0 = AE_MOVXTFLOATX4_FROMINT32X4(tv0); 
  ae_int32x4 tv1 = 0xffffffffu; xtfloatx4 cv1 = AE_MOVXTFLOATX4_FROMINT32X4(tv1); 
  xtbool4 tvb = OLE_SX4(a, b);
  MOVT_SX4(cv0,cv1,tvb); 
  return PacketN_2f(cv0);
}
template<> EIGEN_STRONG_INLINE PacketN_2f pcmp_lt(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  ae_int32x4 tv0 = 0x00000000u;	xtfloatx4 cv0 = AE_MOVXTFLOATX4_FROMINT32X4(tv0); 
  ae_int32x4 tv1 = 0xffffffffu; xtfloatx4 cv1 = AE_MOVXTFLOATX4_FROMINT32X4(tv1); 
  xtbool4 tvb = OLT_SX4(a, b);
  MOVT_SX4(cv0,cv1,tvb); 
  return PacketN_2f(cv0);
}
template<> EIGEN_STRONG_INLINE PacketN_2f pcmp_lt_or_nan(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  ae_int32x2 tv0 = 0x00000000u;
  ae_int32x2 tv1 = 0xffffffffu;
  ae_int32x2 tv2;
  xtfloatx2 a0,a1;
  xtfloatx2 b0,b1;
  xtfloatx2 c0,c1;
  EXTRACTSX2_FROMSX4(a1, a0, a);
  EXTRACTSX2_FROMSX4(b1, b0, b);
  tv2 = tv0; AE_MOVT32X2(tv2,tv1,ULT_SX2(a0,b0)); c0 = AE_MOVXTFLOATX2_FROMINT32X2(tv2);
  tv2 = tv0; AE_MOVT32X2(tv2,tv1,ULT_SX2(a1,b1)); c1 = AE_MOVXTFLOATX2_FROMINT32X2(tv2);
  return PacketN_2f(AE_JOINSX4_FROMSX2(c1, c0));
}
template<> EIGEN_DEVICE_FUNC inline PacketN_2f pselect(const PacketN_2f& mask, const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  xtfloatx4 tvret = a;
  MOVT_SX4(tvret,b,OEQ_SX4(mask,CONST_SX4(0)));
  return PacketN_2f(tvret);
}

} // end namespace internal

} // end namespace Eigen

#endif

#endif
