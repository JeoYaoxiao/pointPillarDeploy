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

#ifndef EIGEN_PACKET_MATH_F32_KX_XTENSA_H
#define EIGEN_PACKET_MATH_F32_KX_XTENSA_H

#if SIMD_N_2_EQ_4() || SIMD_N_2_EQ_8() || SIMD_N_2_EQ_16() || SIMD_N_2_EQ_32()
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

typedef xb_vecN_2xf32 PacketN_2f;

#  if SIMD_N_GE(4,2)
#    ifndef F32_HALF_PACKET_SUPPORT
#      define F32_HALF_PACKET_SUPPORT 1
#    endif
#  else
#    ifndef F32_HALF_PACKET_SUPPORT
#      define F32_HALF_PACKET_SUPPORT 0
#    else
#      if F32_HALF_PACKET_SUPPORT
#        warning "Incorrect value set for F32_HALF_PACKET_SUPPORT. Resetting it back"
#        undef F32_HALF_PACKET_SUPPORT
#        define F32_HALF_PACKET_SUPPORT 0
#      endif
#    endif
#  endif

#if F32_HALF_PACKET_SUPPORT
  struct PacketN_4f {
    EIGEN_STRONG_INLINE PacketN_4f() {}
    EIGEN_STRONG_INLINE explicit PacketN_4f(const PacketN_2f& a) : v(a) {}
    EIGEN_STRONG_INLINE operator PacketN_2f&() {return v;}
    PacketN_2f v;
  };

  template <typename Tto, typename Tfrom> EIGEN_STRONG_INLINE Tto extract_N_4f(const Tfrom& a, const Tfrom& /*b*/) {
    return a;
  }
  template <typename Tto, typename Tfrom> EIGEN_STRONG_INLINE Tto replicate_N_4f(const Tfrom& a) {
    return a;
  }

  template <> EIGEN_STRONG_INLINE PacketN_2f extract_N_4f(const PacketN_2f& a, const PacketN_2f& b){
    return BBE_SELN_2XF32I(b, a, BBE_SELI_32B_EXTRACT_LO_HALVES);
  }
  template <> EIGEN_STRONG_INLINE PacketN_2f replicate_N_4f(const PacketN_2f& a){
    return extract_N_4f<PacketN_2f,PacketN_2f>(a, a);
  }

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
    HasBlend     = 0,
    HasCmp       = 0,

    HasDiv   = 1,
    HasSqrt  = 0,
    HasRsqrt = 1,
    HasExp   = 0,
    HasExpm1 = 0,
    HasLog   = XCLIB_SUPPORT,
    HasLog1p = 0,
    HasLog10 = XCLIB_SUPPORT,
    HasPow   = 0,

    HasSin   = XCLIB_SUPPORT,
    HasCos   = XCLIB_SUPPORT,
    HasTan   = XCLIB_SUPPORT,
#if defined(HAVE_FUSIONG3FP) && HAVE_FUSIONG3FP
    // Disabling asin, acos, tanh for FUSIONG3FP
    HasASin  = 0,
    HasACos  = 0,
    HasTanh  = 0,
#else
    HasASin  = XCLIB_SUPPORT,
    HasACos  = XCLIB_SUPPORT,
    HasTanh  = XCLIB_SUPPORT,
#endif
    HasATan  = XCLIB_SUPPORT,
    HasSinh  = XCLIB_SUPPORT,
    HasCosh  = XCLIB_SUPPORT,
    HasLGamma = 0,
    HasDiGamma = 0,
    HasZeta = 0,
    HasPolygamma = 0,
    HasErf = 0,
    HasErfc = 0,
    HasNdtri = 0,
    HasBessel = 0,
    HasIGamma = 0,
    HasIGammaDerA = 0,
    HasGammaSampleDerAlpha = 0,
    HasIGammac = 0,
    HasBetaInc = 0,

    HasRound = 1,
    HasRint   = 0,
    HasFloor = 1,
    HasCeil  = 1,
    HasSign   = 0
  };
};


template <> struct unpacket_traits<PacketN_2f> {
  typedef float type;
  typedef PacketN_4f half;
  enum {size=SIMD_N_2, alignment=ALIGN_N_2, vectorizable=true, masked_load_available=false, masked_store_available=false};
};

template <> EIGEN_STRONG_INLINE PacketN_2f pset1<PacketN_2f>(const float &from) {
  FUNC_ENTRY;
  return from; 
}
template <> EIGEN_STRONG_INLINE PacketN_2f pload1<PacketN_2f>(const float *from) {
  FUNC_ENTRY;
  return *from; 
}

/** \internal \brief Returns a packet with coefficients (a,a+1,...,a+packet_size-1). */
template <> EIGEN_STRONG_INLINE PacketN_2f plset<PacketN_2f>(const float &a) {
  FUNC_ENTRY;
  return pset1<PacketN_2f>(a) + BBE_FLOATN_2X32(BBE_SEQN_2X32(), 0 /*No scale*/);
}

template <> EIGEN_STRONG_INLINE PacketN_2f padd<PacketN_2f>(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return a + b; 
}
template <> EIGEN_STRONG_INLINE PacketN_2f psub<PacketN_2f>(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return a - b; 
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
  return a * b; 
}
template <> EIGEN_STRONG_INLINE PacketN_2f pdiv<PacketN_2f>(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return BBE_DIVN_2XF32(a, b); 
}

/** \internal \returns a * b + c (coeff-wise) */
template <> EIGEN_STRONG_INLINE PacketN_2f pmadd(const PacketN_2f &a, const PacketN_2f &b, const PacketN_2f &c) {
  FUNC_ENTRY;
  PacketN_2f t0 = c;
  BBE_MULAN_2XF32(t0, a, b);
  return t0;
}

template <> EIGEN_STRONG_INLINE PacketN_2f pand(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return BBE_MOVN_2XF32_FROMN_2X32(BBE_ANDN_2X32(BBE_MOVN_2X32_FROMN_2XF32(a), BBE_MOVN_2X32_FROMN_2XF32(b)));
}
template <> EIGEN_STRONG_INLINE PacketN_2f por(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return BBE_MOVN_2XF32_FROMN_2X32(BBE_ORN_2X32(BBE_MOVN_2X32_FROMN_2XF32(a), BBE_MOVN_2X32_FROMN_2XF32(b)));
}
template <> EIGEN_STRONG_INLINE PacketN_2f pandnot(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return BBE_MOVN_2XF32_FROMN_2X32(BBE_ANDN_2X32(BBE_MOVN_2X32_FROMN_2XF32(a), BBE_NOTN_2X32(BBE_MOVN_2X32_FROMN_2XF32(b))));
}

template <> EIGEN_STRONG_INLINE PacketN_2f pmin(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return BBE_MINN_2XF32(a, b); 
}
template <> EIGEN_STRONG_INLINE PacketN_2f pmax(const PacketN_2f &a, const PacketN_2f &b) {
  FUNC_ENTRY;
  return BBE_MAXN_2XF32(a, b); 
}
template <> EIGEN_STRONG_INLINE PacketN_2f pround(const PacketN_2f &a) {
  FUNC_ENTRY;
  return BBE_FIROUNDN_2XF32(a); 
}
template <> EIGEN_STRONG_INLINE PacketN_2f pceil(const PacketN_2f &a) {
  FUNC_ENTRY;
  return BBE_FICEILN_2XF32(a); 
}
template <> EIGEN_STRONG_INLINE PacketN_2f pfloor(const PacketN_2f &a) {
  FUNC_ENTRY;
  return BBE_FIFLOORN_2XF32(a); 
}

template <> EIGEN_STRONG_INLINE PacketN_2f pload<PacketN_2f>(const float *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  return *(xt_reinterpret_const_cast<PacketN_2f,float>(from));
}

template <> EIGEN_STRONG_INLINE PacketN_2f ploadu<PacketN_2f>(const float *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_LOAD;

  PacketN_2f     tv0;
  xb_vecN_2xf32 *tpv0 = xt_reinterpret_const_cast<xb_vecN_2xf32,float>(from);
#if defined(IVP_L2UN_2XF32_XP)
  IVP_L2UN_2XF32_XP(tv0, tpv0, sizeof(float)*SIMD_N_2);
#else
  valign         talign;
  talign              = BBE_LAN_2XF32_PP(tpv0);
  BBE_LAN_2XF32_IP(tv0, talign, tpv0);
#endif
  return tv0;
}

/** \internal \returns a packet with elements of \a *from duplicated.
 * For instance, for a packet of 8 elements, 4 scalars will be read from \a *from and
 * duplicated to form: {from[0],from[0],from[1],from[1],from[2],from[2],from[3],from[3]}
 * Currently, this function is only used for scalar * complex products.
 */
template <> EIGEN_STRONG_INLINE PacketN_2f ploaddup<PacketN_2f>(const float *from) {
  FUNC_ENTRY;
  xb_vecN_2xf32 tv0 = ploadu<PacketN_2f>(from);
  return BBE_SELN_2XF32I(tv0, tv0, BBE_SELI_32B_INTERLEAVE_1_LO);
}

/** \internal \returns a packet with elements of \a *from quadrupled.
  * For instance, for a packet of 8 elements, 2 scalars will be read from \a *from and
  * replicated to form: {from[0],from[0],from[0],from[0],from[1],from[1],from[1],from[1]}
  * Currently, this function is only used in matrix products.
  * For packet-size smaller or equal to 4, this function is equivalent to pload1 
  */
template <> EIGEN_STRONG_INLINE PacketN_2f ploadquad<PacketN_2f>(const float *from) {
  FUNC_ENTRY;
  #if SIMD_N_2_EQ_4()
    return pload1<PacketN_2f>(from);
  #else
    xb_vecN_2xf32 tv0 = ploadu<PacketN_2f>(from);
    xb_vecN_2xf32 tv1 = BBE_SELN_2XF32I(tv0, tv0, BBE_SELI_32B_INTERLEAVE_1_LO);
    return BBE_SELN_2XF32I(tv1, tv1, BBE_SELI_32B_INTERLEAVE_1_LO);
  #endif
}

/** \internal equivalent to
  * \code
  * a0 = pload1(a+0);
  * a1 = pload1(a+1);
  * a2 = pload1(a+2);
  * a3 = pload1(a+3);
  * \endcode
  * \sa pset1, pload1, ploaddup, pbroadcast2
  */
template<> EIGEN_DEVICE_FUNC inline void pbroadcast4(const float *a, PacketN_2f& a0, PacketN_2f& a1, PacketN_2f& a2, PacketN_2f& a3) {
  FUNC_ENTRY;
  a0 = pload1<PacketN_2f>(a+0);
  a1 = pload1<PacketN_2f>(a+1);
  a2 = pload1<PacketN_2f>(a+2);
  a3 = pload1<PacketN_2f>(a+3);
}

/** \internal equivalent to
  * \code
  * a0 = pload1(a+0);
  * a1 = pload1(a+1);
  * \endcode
  * \sa pset1, pload1, ploaddup, pbroadcast4
  */
template<> EIGEN_DEVICE_FUNC inline void pbroadcast2(const float *a, PacketN_2f& a0, PacketN_2f& a1) {
  FUNC_ENTRY;
  a0 = pload1<PacketN_2f>(a+0);
  a1 = pload1<PacketN_2f>(a+1);
}

template <> EIGEN_STRONG_INLINE void pstore<float>(float *to, const PacketN_2f &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_STORE;
  *(xt_reinterpret_cast<PacketN_2f,float>(to)) = from;
}

template <> EIGEN_STRONG_INLINE void pstoreu<float>(float *to, const PacketN_2f &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_STORE;
  valign         talign;
  xb_vecN_2xf32 *tpv0 = xt_reinterpret_cast<xb_vecN_2xf32,float>(to);
  talign           = BBE_ZALIGN();
  BBE_SAN_2XF32_IP(from, talign, tpv0);
  BBE_SAN_2XF32POS_FP(talign, tpv0);
}

#if XCHAL_HAVE_BBENEP
  template <> EIGEN_DEVICE_FUNC inline PacketN_2f pgather<float, PacketN_2f>(const float *from, Index stride) {
    FUNC_ENTRY;
    float *tp = (float *)(from);
    xb_vecN_2xf32  ta;
    float *tpf = (float *)(&ta);
    int    i;
    int    stride_bytes = sizeof(float) * stride;
    xb_vecN_2xf32 tvf;

    for (i = 0; i < SIMD_N_2; i++) {
      BBE_LSN_2XF32_XP(tvf, tp, stride_bytes);
      BBE_SSN_2XF32_IP(tvf, tpf, sizeof(float));
    }
    return ta;
  }
#endif

#if XCHAL_HAVE_BBENEP
  template <> EIGEN_DEVICE_FUNC inline void pscatter<float, PacketN_2f>(float *to, const PacketN_2f &from, Index stride) {
    FUNC_ENTRY;
    float *tpf = (float*)(&from);
    float *tpt = (float*)(to);
    int    stride_bytes = sizeof(float) * stride;
    xb_vecN_2xf32 tvf;
    int i;
    for (i = 0; i < SIMD_N_2; i++)
    {
      BBE_LSN_2XF32_IP(tvf, tpf, sizeof(float));
      BBE_SSN_2XF32_XP(tvf, tpt, stride_bytes);
    }
  }
#endif

template <> EIGEN_STRONG_INLINE void pstore1<PacketN_2f>(float *to, const float &a) {
  FUNC_ENTRY;
  pstore(to, PacketN_2f((float)a)); 
}

template <> EIGEN_STRONG_INLINE float pfirst<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  return BBE_SELSN_2XF32(a, 0 /*Start element*/); 
}
template <> EIGEN_STRONG_INLINE PacketN_2f preverse(const PacketN_2f &a) {
  FUNC_ENTRY;
  return  BBE_SHFLN_2XF32I(a, BBE_SHFLI_32B_REVERSE_1); 
}
template <> EIGEN_STRONG_INLINE PacketN_2f pabs(const PacketN_2f &a) {
  FUNC_ENTRY;
  return BBE_ABSN_2XF32(a); 
}

template <> EIGEN_STRONG_INLINE float predux<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  return BBE_RADDN_2XF32(a); 
}

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

/** \internal \returns the sum of the elements of \a a by block of 4 elements.
  * For a packet {a0, a1, a2, a3, a4, a5, a6, a7}, it returns a half packet {a0+a4, a1+a5, a2+a6, a3+a7}
  * For packet-size smaller or equal to 4, this boils down to a noop.
  */
#if SIMD_N_2_EQ_8()
  template<> EIGEN_DEVICE_FUNC inline PacketN_4f predux_half_dowto4(const PacketN_2f& a) {
    FUNC_ENTRY;
    xb_vecN_2xf32 tv0, tv1, tv2;
    tv0 = a;
    tv1 = BBE_CONSTN_2XF32(0);
    tv2 = BBE_SELN_2XF32I(tv0, tv1, BBE_SELI_32B_ROTATE_RIGHT_4);
    tv2 = tv0 + tv2;
    tv0 = BBE_SELN_2XF32I(tv1, tv2, BBE_SELI_32B_ROTATE_LEFT_4);
    return PacketN_4f(tv0);
  }
#elif SIMD_N_2_EQ_16()
  template<> EIGEN_DEVICE_FUNC inline PacketN_4f predux_half_dowto4(const PacketN_2f& a) {
    FUNC_ENTRY;
    xb_vecN_2xf32 tv0, tv1, tv2;
    tv0 = a;
    tv1 = BBE_CONSTN_2XF32(0);
    tv2 = BBE_SELN_2XF32I(tv0, tv1, BBE_SELI_32B_ROTATE_RIGHT_8);
    tv2 = tv0 + tv2;
    tv0 = BBE_SELN_2XF32I(tv1, tv2, BBE_SELI_32B_ROTATE_LEFT_8);
    return PacketN_4f(tv0);
  }
#elif SIMD_N_2_EQ_32()
  template<> EIGEN_DEVICE_FUNC inline PacketN_4f predux_half_dowto4(const PacketN_2f& a) {
    FUNC_ENTRY;
    xb_vecN_2xf32 tv0, tv1, tv2;
    vboolN_2 tvb;
    tvb = BBE_LTRN_2I(SIMD_N_2/2);
    tv0 = a;
    tv1 = BBE_CONSTN_2XF32(0);
    tv2 = BBE_SELN_2XF32I(tv1, tv0, BBE_SELI_32B_ROTATE_RIGHT_16);
    tv2 = tv0 + tv2;
    tv0 = BBE_MOVN_2XF32T(tv2, tv1, tvb);
    return PacketN_4f(tv0);
  }
#endif

#if SIMD_N_2_EQ_4()
  #if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX
    /** \internal \returns the product of the elements of \a a*/
    template <> EIGEN_STRONG_INLINE float predux_mul<PacketN_2f>(const PacketN_2f &a) {
      FUNC_ENTRY;
      xb_vecN_2xf32 tv1 = BBE_MULMN_2XF32(a, a, 0x0, 0x9);
      xb_vecN_2xf32 tv2 = BBE_SELN_2XF32I(tv1, tv1, BBE_SELI_32B_EXTRACT_1_OF_2_OFF_0);
      xb_vecN_2xf32 tv3 = BBE_MULMN_2XF32(tv2, tv2, 0x0, 0x9);
      return BBE_SELSN_2XF32(tv3, 0);
    }
  #endif
#elif SIMD_N_2_EQ_8()
  #if XCHAL_HAVE_BBENEP
    /** \internal \returns the product of the elements of \a a*/
    template <> EIGEN_STRONG_INLINE float predux_mul<PacketN_2f>(const PacketN_2f &a) {
      FUNC_ENTRY;
      xb_vecN_2xf32 tv0, tv1;
      tv0 = a;
      tv1 = BBE_MULMN_2XF32(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_2XF32I(tv1, tv1, BBE_SELI_32B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_2XF32(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_2XF32I(tv1, tv1, BBE_SELI_32B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_2XF32(tv0, tv0, 0x0, 0x9);
      return BBE_SELSN_2XF32(tv1, 0);
    }
  #endif
#elif SIMD_N_2_EQ_16()
  #if XCHAL_HAVE_BBENEP
    /** \internal \returns the product of the elements of \a a*/
    template <> EIGEN_STRONG_INLINE float predux_mul<PacketN_2f>(const PacketN_2f &a) {
      FUNC_ENTRY;
      xb_vecN_2xf32 tv0, tv1;
      tv0 = a;
      tv1 = BBE_MULMN_2XF32(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_2XF32I(tv1, tv1, BBE_SELI_32B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_2XF32(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_2XF32I(tv1, tv1, BBE_SELI_32B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_2XF32(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_2XF32I(tv1, tv1, BBE_SELI_32B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_2XF32(tv0, tv0, 0x0, 0x9);
      return BBE_SELSN_2XF32(tv1, 0);
    }
  #endif
#elif SIMD_N_2_EQ_32()
  #if XCHAL_HAVE_BBENEP
    /** \internal \returns the product of the elements of \a a*/
    template <> EIGEN_STRONG_INLINE float predux_mul<PacketN_2f>(const PacketN_2f &a) {
      FUNC_ENTRY;
      xb_vecN_2xf32 tv0, tv1;
      tv0 = a;
      tv1 = BBE_MULMN_2XF32(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_2XF32I(tv1, tv1, BBE_SELI_32B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_2XF32(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_2XF32I(tv1, tv1, BBE_SELI_32B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_2XF32(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_2XF32I(tv1, tv1, BBE_SELI_32B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_2XF32(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_2XF32I(tv1, tv1, BBE_SELI_32B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_2XF32(tv0, tv0, 0x0, 0x9);
      return BBE_SELSN_2XF32(tv1, 0);
    }
  #endif
#endif

template <> EIGEN_STRONG_INLINE float predux_min<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  return BBE_RMINN_2XF32(a); 
}
template <> EIGEN_STRONG_INLINE float predux_max<PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  return BBE_RMAXN_2XF32(a); 
}

#if SIMD_N_2_EQ_4()
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_4> &kernel) {
    FUNC_ENTRY;
    PacketN_2f t00, t01, t02, t03;
    PacketN_2f t10, t11, t12, t13;
    t00 = kernel.packet[0];
    t01 = kernel.packet[1];
    t02 = kernel.packet[2];
    t03 = kernel.packet[3];
    BBE_DSELN_2XF32I(t11, t10, t01, t00, BBE_DSELI_32B_INTERLEAVE_1);
    BBE_DSELN_2XF32I(t13, t12, t03, t02, BBE_DSELI_32B_INTERLEAVE_1);
    BBE_DSELN_2XF32I(t01, t00, t12, t10, BBE_DSELI_32B_INTERLEAVE_2);
    BBE_DSELN_2XF32I(t03, t02, t13, t11, BBE_DSELI_32B_INTERLEAVE_2);
    kernel.packet[0] = t00;
    kernel.packet[1] = t01;
    kernel.packet[2] = t02;
    kernel.packet[3] = t03;
  }
#elif SIMD_N_2_EQ_8()
  #if XCHAL_HAVE_BBENEP
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_4> &kernel) {
      FUNC_ENTRY;
      xb_vecN_2xf32  tv0, tv1, tv2, tv3;
      xb_vecN_2xf32  tvT0, tvT1, tvT2, tvT3;
      xb_vecN_2xf32  tvS0, tvS1, tvS2, tvS3;
      tvT0 = kernel.packet[0];
      tvT1 = kernel.packet[1];
      tvT2 = kernel.packet[2];
      tvT3 = kernel.packet[3];

      BBE_DSELN_2XF32I(tv1, tv0, tvT1, tvT0, BBE_DSELI_32B_INTERLEAVE_1);
      BBE_DSELN_2XF32I(tvT1, tvT0, tv1, tv0, BBE_DSELI_32B_INTERLEAVE_4);
      
      BBE_DSELN_2XF32I(tv3, tv2, tvT3, tvT2, BBE_DSELI_32B_INTERLEAVE_1);
      BBE_DSELN_2XF32I(tvT3, tvT2, tv3, tv2, BBE_DSELI_32B_INTERLEAVE_4);

      BBE_DSELN_2XF32I(tv1, tv0, tvT2, tvT0, BBE_DSELI_32B_INTERLEAVE_2);
      BBE_DSELN_2XF32I(tvS1, tvS0, tv1, tv0, BBE_DSELI_32B_INTERLEAVE_4);
      
      BBE_DSELN_2XF32I(tv3, tv2, tvT3, tvT1, BBE_DSELI_32B_INTERLEAVE_2);
      BBE_DSELN_2XF32I(tvS3, tvS2, tv3, tv2, BBE_DSELI_32B_INTERLEAVE_4);

      BBE_DSELN_2XF32I(tv1, tv0, tvS1, tvS0, BBE_DSELI_32B_INTERLEAVE_4);

      BBE_DSELN_2XF32I(tv3, tv2, tvS3, tvS2, BBE_DSELI_32B_INTERLEAVE_4);

      kernel.packet[0] = tv0;
      kernel.packet[1] = tv2;
      kernel.packet[2] = tv1;
      kernel.packet[3] = tv3;
    }
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_8> &kernel) {
      FUNC_ENTRY;

      // Declare vec
      #define DR(_var,_num) xb_vecN_2xf32 _var##_num = BBE_CONSTN_2XF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = kernel.packet[_num]
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = _var##_src_num

      // Declare vectors
      #define DECL_VEC(_var) \
        DR(_var,0); DR(_var,1); DR(_var,2); DR(_var,3); DR(_var,4); DR(_var,5); DR(_var,6); DR(_var,7); 

      // Load vectors
      #define LOAD_VEC(_var) \
        LR(_var,0); LR(_var,1); LR(_var,2); LR(_var,3); LR(_var,4); LR(_var,5); LR(_var,6); LR(_var,7); 

      // Store vectors 
      #define STORE_VEC(_var) \
        SR(_var,0,0); SR(_var,1,1); SR(_var,2,4); SR(_var,3,5); SR(_var,4,2); SR(_var,5,3); SR(_var,6,6); SR(_var,7,7); 

      /* This is first step in transpose operation. In first step, input vectors and
       * output vectors have same sequence. This will change in 'NEXT_STEP', where
       * output vectors will have the same sequence but input vector sequence will be
       * different. */
      #define FIRST_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_2XF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
        BBE_DSELN_2XF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);     \
        BBE_DSELN_2XF32I(_out##5, _out##4, _inp##5, _inp##4, _pat);     \
        BBE_DSELN_2XF32I(_out##7, _out##6, _inp##7, _inp##6, _pat);     \

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_2XF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        BBE_DSELN_2XF32I(_out##3, _out##2, _inp##6, _inp##4, _pat);    \
        BBE_DSELN_2XF32I(_out##5, _out##4, _inp##3, _inp##1, _pat);    \
        BBE_DSELN_2XF32I(_out##7, _out##6, _inp##7, _inp##5, _pat);    \

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_1, tvs, tvf);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvf, tvs);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_4, tvs, tvf);
      STORE_VEC(tvs);

      #undef DR
      #undef LR
      #undef SR
      #undef DECL_VEC
      #undef LOAD_VEC
      #undef STORE_VEC
      #undef FIRST_STEP
      #undef NEXT_STEP
    }
  #endif
#elif SIMD_N_2_EQ_16()
  #if XCHAL_HAVE_BBENEP
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_4> &kernel) {
      FUNC_ENTRY;
      xb_vecN_2xf32  tv0, tv1, tv2, tv3;
      xb_vecN_2xf32  tvT0, tvT1, tvT2, tvT3;
      xb_vecN_2xf32  tvS0, tvS1, tvS2, tvS3;
      tvT0 = kernel.packet[0];
      tvT1 = kernel.packet[1];
      tvT2 = kernel.packet[2];
      tvT3 = kernel.packet[3];

      BBE_DSELN_2XF32I(tv1, tv0, tvT1, tvT0, BBE_DSELI_32B_INTERLEAVE_1);
      BBE_DSELN_2XF32I(tvT1, tvT0, tv1, tv0, BBE_DSELI_32B_DEINTERLEAVE_4);
      BBE_DSELN_2XF32I(tv3, tv2, tvT3, tvT2, BBE_DSELI_32B_INTERLEAVE_1);
      BBE_DSELN_2XF32I(tvT3, tvT2, tv3, tv2, BBE_DSELI_32B_DEINTERLEAVE_4);

      BBE_DSELN_2XF32I(tv1, tv0, tvT2, tvT0, BBE_DSELI_32B_INTERLEAVE_2);
      BBE_DSELN_2XF32I(tvS1, tvS0, tv1, tv0, BBE_DSELI_32B_DEINTERLEAVE_4);
      BBE_DSELN_2XF32I(tv3, tv2, tvT3, tvT1, BBE_DSELI_32B_INTERLEAVE_2);
      BBE_DSELN_2XF32I(tvS3, tvS2, tv3, tv2, BBE_DSELI_32B_DEINTERLEAVE_4);

      BBE_DSELN_2XF32I(tv1, tv0, tvS1, tvS0, BBE_DSELI_32B_INTERLEAVE_4);
      BBE_DSELN_2XF32I(tv3, tv2, tvS3, tvS2, BBE_DSELI_32B_INTERLEAVE_4);

      BBE_DSELN_2XF32I(tvS1, tvS0, tv2, tv0, BBE_DSELI_32B_INTERLEAVE_8);
      BBE_DSELN_2XF32I(tvS3, tvS2, tv3, tv1, BBE_DSELI_32B_INTERLEAVE_8);

      kernel.packet[0] = tvS0;
      kernel.packet[1] = tvS1;
      kernel.packet[2] = tvS2;
      kernel.packet[3] = tvS3;
    }
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_16> &kernel) {
      FUNC_ENTRY;

      // Declare vec
      #define DR(_var,_num) xb_vecN_2xf32 _var##_num = BBE_CONSTN_2XF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = kernel.packet[_num]
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = _var##_src_num

      // Declare vectors
      #define DECL_VEC(_var) \
        DR(_var,0); DR(_var,1); DR(_var,2); DR(_var,3); DR(_var,4); DR(_var,5); DR(_var,6); DR(_var,7); DR(_var,8); DR(_var,9); DR(_var,10); DR(_var,11); DR(_var,12); DR(_var,13); DR(_var,14); DR(_var,15);

      // Load vectors
      #define LOAD_VEC(_var) \
        LR(_var,0); LR(_var,1); LR(_var,2); LR(_var,3); LR(_var,4); LR(_var,5); LR(_var,6); LR(_var,7); LR(_var,8); LR(_var,9); LR(_var,10); LR(_var,11); LR(_var,12); LR(_var,13); LR(_var,14); LR(_var,15);

      // Store vectors
      #define STORE_VEC(_var) \
        SR(_var,0,0); SR(_var,1,1); SR(_var,2,8); SR(_var,3,9); SR(_var,4,4); SR(_var,5,5); SR(_var,6,12); SR(_var,7,13); SR(_var,8,2); SR(_var,9,3); SR(_var,10,10); SR(_var,11,11); SR(_var,12,6); SR(_var,13,7); SR(_var,14,14); SR(_var,15,15);

      /* This is first step in transpose operation. In first step, input vectors and
       * output vectors have same sequence. This will change in 'NEXT_STEP', where
       * output vectors will have the same sequence but input vector sequence will be
       * different. */
      #define FIRST_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_2XF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
        BBE_DSELN_2XF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);     \
        BBE_DSELN_2XF32I(_out##5, _out##4, _inp##5, _inp##4, _pat);     \
        BBE_DSELN_2XF32I(_out##7, _out##6, _inp##7, _inp##6, _pat);     \
        BBE_DSELN_2XF32I(_out##9, _out##8, _inp##9, _inp##8, _pat);     \
        BBE_DSELN_2XF32I(_out##11, _out##10, _inp##11, _inp##10, _pat); \
        BBE_DSELN_2XF32I(_out##13, _out##12, _inp##13, _inp##12, _pat); \
        BBE_DSELN_2XF32I(_out##15, _out##14, _inp##15, _inp##14, _pat);

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_2XF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        BBE_DSELN_2XF32I(_out##3, _out##2, _inp##6, _inp##4, _pat);    \
        BBE_DSELN_2XF32I(_out##5, _out##4, _inp##10, _inp##8, _pat);   \
        BBE_DSELN_2XF32I(_out##7, _out##6, _inp##14, _inp##12, _pat);  \
        BBE_DSELN_2XF32I(_out##9, _out##8, _inp##3, _inp##1, _pat);    \
        BBE_DSELN_2XF32I(_out##11, _out##10, _inp##7, _inp##5, _pat);  \
        BBE_DSELN_2XF32I(_out##13, _out##12, _inp##11, _inp##9, _pat); \
        BBE_DSELN_2XF32I(_out##15, _out##14, _inp##15, _inp##13, _pat);

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_1, tvs, tvf);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvf, tvs);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_4, tvs, tvf);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_8, tvf, tvs);
      STORE_VEC(tvf);

      #undef DR
      #undef LR
      #undef SR
      #undef DECL_VEC
      #undef LOAD_VEC
      #undef STORE_VEC
      #undef FIRST_STEP
      #undef NEXT_STEP
    }
  #endif
#elif SIMD_N_2_EQ_32()
  #if XCHAL_HAVE_BBENEP
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_4> &kernel) {
      FUNC_ENTRY;
      xb_vecN_2xf32  tv0, tv1, tv2, tv3;
      xb_vecN_2xf32  tvT0, tvT1, tvT2, tvT3;
      xb_vecN_2xf32  tvS0, tvS1, tvS2, tvS3;

      tvT0 = kernel.packet[0];
      tvT1 = kernel.packet[1];
      tvT2 = kernel.packet[2];
      tvT3 = kernel.packet[3];

      BBE_DSELN_2XF32I(tv1, tv0, tvT1, tvT0, BBE_DSELI_32B_INTERLEAVE_1);
      BBE_DSELN_2XF32I(tvT1, tvT0, tv1, tv0, BBE_DSELI_32B_DEINTERLEAVE_4);
      BBE_DSELN_2XF32I(tv3, tv2, tvT3, tvT2, BBE_DSELI_32B_INTERLEAVE_1);
      BBE_DSELN_2XF32I(tvT3, tvT2, tv3, tv2, BBE_DSELI_32B_DEINTERLEAVE_4);

      BBE_DSELN_2XF32I(tv1, tv0, tvT2, tvT0, BBE_DSELI_32B_INTERLEAVE_2);
      BBE_DSELN_2XF32I(tvS1, tvS0, tv1, tv0, BBE_DSELI_32B_DEINTERLEAVE_4);
      BBE_DSELN_2XF32I(tv3, tv2, tvT3, tvT1, BBE_DSELI_32B_INTERLEAVE_2);
      BBE_DSELN_2XF32I(tvS3, tvS2, tv3, tv2, BBE_DSELI_32B_DEINTERLEAVE_4);

      BBE_DSELN_2XF32I(tv1, tv0, tvS1, tvS0, BBE_DSELI_32B_INTERLEAVE_4);
      BBE_DSELN_2XF32I(tv3, tv2, tvS3, tvS2, BBE_DSELI_32B_INTERLEAVE_4);

      BBE_DSELN_2XF32I(tvS1, tvS0, tv2, tv0, BBE_DSELI_32B_INTERLEAVE_8);
      BBE_DSELN_2XF32I(tvS3, tvS2, tv3, tv1, BBE_DSELI_32B_INTERLEAVE_8);

      kernel.packet[0] = tvS0;
      kernel.packet[1] = tvS1;
      kernel.packet[2] = tvS2;
      kernel.packet[3] = tvS3;
    }
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_32> &kernel) {
      FUNC_ENTRY;

      // Declare vec
      #define DR(_var,_num) xb_vecN_2xf32 _var##_num = BBE_CONSTN_2XF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = kernel.packet[_num]
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = _var##_src_num

      // Declare vectors
      #define DECL_VEC(_var) \
        DR(_var,0); DR(_var,1); DR(_var,2); DR(_var,3); DR(_var,4); DR(_var,5); DR(_var,6); DR(_var,7); DR(_var,8); DR(_var,9); DR(_var,10); DR(_var,11); DR(_var,12); DR(_var,13); DR(_var,14); DR(_var,15); DR(_var,16); DR(_var,17); DR(_var,18); DR(_var,19); DR(_var,20); DR(_var,21); DR(_var,22); DR(_var,23); DR(_var,24); DR(_var,25); DR(_var,26); DR(_var,27); DR(_var,28); DR(_var,29); DR(_var,30); DR(_var,31);

      // Load vectors
      #define LOAD_VEC(_var) \
        LR(_var,0); LR(_var,1); LR(_var,2); LR(_var,3); LR(_var,4); LR(_var,5); LR(_var,6); LR(_var,7); LR(_var,8); LR(_var,9); LR(_var,10); LR(_var,11); LR(_var,12); LR(_var,13); LR(_var,14); LR(_var,15); LR(_var,16); LR(_var,17); LR(_var,18); LR(_var,19); LR(_var,20); LR(_var,21); LR(_var,22); LR(_var,23); LR(_var,24); LR(_var,25); LR(_var,26); LR(_var,27); LR(_var,28); LR(_var,29); LR(_var,30); LR(_var,31);

      // Store vectors
      #define STORE_VEC(_var) \
        SR(_var,0,0); SR(_var,1,1); SR(_var,2,16); SR(_var,3,17); SR(_var,4,8); SR(_var,5,9); SR(_var,6,24); SR(_var,7,25); SR(_var,8,4); SR(_var,9,5); SR(_var,10,20); SR(_var,11,21); SR(_var,12,12); SR(_var,13,13); SR(_var,14,28); SR(_var,15,29); SR(_var,16,2); SR(_var,17,3); SR(_var,18,18); SR(_var,19,19); SR(_var,20,10); SR(_var,21,11); SR(_var,22,26); SR(_var,23,27); SR(_var,24,6); SR(_var,25,7); SR(_var,26,22); SR(_var,27,23); SR(_var,28,14); SR(_var,29,15); SR(_var,30,30); SR(_var,31,31);

      /* This is first step in transpose operation. In first step, input vectors and
       * output vectors have same sequence. This will change in 'NEXT_STEP', where
       * output vectors will have the same sequence but input vector sequence will be
       * different. */
      #define FIRST_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_2XF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
        BBE_DSELN_2XF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);     \
        BBE_DSELN_2XF32I(_out##5, _out##4, _inp##5, _inp##4, _pat);     \
        BBE_DSELN_2XF32I(_out##7, _out##6, _inp##7, _inp##6, _pat);     \
        BBE_DSELN_2XF32I(_out##9, _out##8, _inp##9, _inp##8, _pat);     \
        BBE_DSELN_2XF32I(_out##11, _out##10, _inp##11, _inp##10, _pat); \
        BBE_DSELN_2XF32I(_out##13, _out##12, _inp##13, _inp##12, _pat); \
        BBE_DSELN_2XF32I(_out##15, _out##14, _inp##15, _inp##14, _pat); \
        BBE_DSELN_2XF32I(_out##17, _out##16, _inp##17, _inp##16, _pat); \
        BBE_DSELN_2XF32I(_out##19, _out##18, _inp##19, _inp##18, _pat); \
        BBE_DSELN_2XF32I(_out##21, _out##20, _inp##21, _inp##20, _pat); \
        BBE_DSELN_2XF32I(_out##23, _out##22, _inp##23, _inp##22, _pat); \
        BBE_DSELN_2XF32I(_out##25, _out##24, _inp##25, _inp##24, _pat); \
        BBE_DSELN_2XF32I(_out##27, _out##26, _inp##27, _inp##26, _pat); \
        BBE_DSELN_2XF32I(_out##29, _out##28, _inp##29, _inp##28, _pat); \
        BBE_DSELN_2XF32I(_out##31, _out##30, _inp##31, _inp##30, _pat);

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                               \
        BBE_DSELN_2XF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);     \
        BBE_DSELN_2XF32I(_out##3, _out##2, _inp##6, _inp##4, _pat);     \
        BBE_DSELN_2XF32I(_out##5, _out##4, _inp##10, _inp##8, _pat);    \
        BBE_DSELN_2XF32I(_out##7, _out##6, _inp##14, _inp##12, _pat);   \
        BBE_DSELN_2XF32I(_out##9, _out##8, _inp##18, _inp##16, _pat);   \
        BBE_DSELN_2XF32I(_out##11, _out##10, _inp##22, _inp##20, _pat); \
        BBE_DSELN_2XF32I(_out##13, _out##12, _inp##26, _inp##24, _pat); \
        BBE_DSELN_2XF32I(_out##15, _out##14, _inp##30, _inp##28, _pat); \
        BBE_DSELN_2XF32I(_out##17, _out##16, _inp##3, _inp##1, _pat);   \
        BBE_DSELN_2XF32I(_out##19, _out##18, _inp##7, _inp##5, _pat);   \
        BBE_DSELN_2XF32I(_out##21, _out##20, _inp##11, _inp##9, _pat);  \
        BBE_DSELN_2XF32I(_out##23, _out##22, _inp##15, _inp##13, _pat); \
        BBE_DSELN_2XF32I(_out##25, _out##24, _inp##19, _inp##17, _pat); \
        BBE_DSELN_2XF32I(_out##27, _out##26, _inp##23, _inp##21, _pat); \
        BBE_DSELN_2XF32I(_out##29, _out##28, _inp##27, _inp##25, _pat); \
        BBE_DSELN_2XF32I(_out##31, _out##30, _inp##31, _inp##29, _pat);

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_1, tvs, tvf);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvf, tvs);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_4, tvs, tvf);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_8, tvf, tvs);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_16, tvs, tvf);
      STORE_VEC(tvs);

      #undef DR
      #undef LR
      #undef SR
      #undef DECL_VEC
      #undef LOAD_VEC
      #undef STORE_VEC
      #undef FIRST_STEP
      #undef NEXT_STEP
    }
  #endif
#endif

// Scalar path for pmadd with FMA to ensure consistency with vectorized path.
template <> EIGEN_STRONG_INLINE float pmadd(const float &a, const float &b, const float &c) {
  FUNC_ENTRY;
  xtfloat t0 = c;
  XT_MADD_S(t0, a, b);
  return t0;
}

template<> EIGEN_STRONG_INLINE PacketN_2f pcmp_eq(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  xb_vecN_2x32Uv tv0 = 0x00000000u;
  xb_vecN_2x32Uv tv1 = 0xffffffffu;
  return BBE_MOVN_2XF32_FROMN_2X32(BBE_MOVN_2X32UT(tv1, tv0, BBE_OEQN_2XF32(a,b)));
}
template<> EIGEN_STRONG_INLINE float pcmp_le(const float& a, const float& b) {
  FUNC_ENTRY;
  ui32_f32 t = {0xffffffffu};
  float ret = 0, t0 = 0, t1 = t.f32;
  BBE_ADDF32T(ret, t0, t1, XT_OLE_S(a,b));
  return ret;
}
template<> EIGEN_STRONG_INLINE float pcmp_lt(const float& a, const float& b) {
  FUNC_ENTRY;
  ui32_f32 t = {0xffffffffu};
  float ret = 0, t0 = 0, t1 = t.f32;
  BBE_ADDF32T(ret, t0, t1, XT_OLT_S(a,b));
  return ret;
}
template<> EIGEN_STRONG_INLINE float pcmp_lt_or_nan(const float& a, const float& b) {
  FUNC_ENTRY;
  ui32_f32 t = {0xffffffffu};
  float ret = 0, t0 = 0, t1 = t.f32;
  BBE_ADDF32T(ret, t0, t1, XT_ULT_S(a,b));
  return ret;
}
template<> EIGEN_STRONG_INLINE PacketN_2f pcmp_le(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  xb_vecN_2x32Uv tv0 = 0x00000000u;
  xb_vecN_2x32Uv tv1 = 0xffffffffu;
  return BBE_MOVN_2XF32_FROMN_2X32(BBE_MOVN_2X32UT(tv1, tv0, BBE_OLEN_2XF32(a,b)));
}
template<> EIGEN_STRONG_INLINE PacketN_2f pcmp_lt(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  xb_vecN_2x32Uv tv0 = 0x00000000u;
  xb_vecN_2x32Uv tv1 = 0xffffffffu;
  return BBE_MOVN_2XF32_FROMN_2X32(BBE_MOVN_2X32UT(tv1, tv0, BBE_OLTN_2XF32(a,b)));
}
template<> EIGEN_STRONG_INLINE PacketN_2f pcmp_lt_or_nan(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  xb_vecN_2x32Uv tv0 = 0x00000000u;
  xb_vecN_2x32Uv tv1 = 0xffffffffu;
  return BBE_MOVN_2XF32_FROMN_2X32(BBE_MOVN_2X32UT(tv1, tv0, BBE_ULTN_2XF32(a,b)));
}

template<> EIGEN_DEVICE_FUNC inline PacketN_2f pselect(const PacketN_2f& mask, const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  return BBE_MOVN_2XF32T(b,a,(mask==PacketN_2f(0)));
}
template<> EIGEN_STRONG_INLINE PacketN_2f ptrue<PacketN_2f>(const PacketN_2f& /*a*/) {
  FUNC_ENTRY;
  return VEC_XF32_FROM_VEC_X32U(0xffffffffu);
}

template <typename Packet, typename Op>
EIGEN_STRONG_INLINE Packet pminmax_propagate_numbers(const Packet& a, const Packet& b, Op op) {
  FUNC_ENTRY;
  Packet ta = BBE_MOVN_2XF32T(a,b,BBE_OEQN_2XF32(a,a));
  Packet tb = BBE_MOVN_2XF32T(b,a,BBE_OEQN_2XF32(b,b));
  return op(ta,tb);
}

template <typename Packet, typename Op>
EIGEN_STRONG_INLINE Packet pminmax_propagate_nan(const Packet& a, const Packet& b, Op op) {
  FUNC_ENTRY;
  Packet ta = BBE_MOVN_2XF32T(b,a,BBE_OEQN_2XF32(a,a));
  Packet tb = BBE_MOVN_2XF32T(a,b,BBE_OEQN_2XF32(b,b));
  return op(ta,tb);
}

// Add specializations for min/max with prescribed NaN progation.
template<> EIGEN_STRONG_INLINE PacketN_2f pmin<PropagateNumbers, PacketN_2f>(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  return pminmax_propagate_numbers(a, b, pmin<PacketN_2f>);
}
template<> EIGEN_STRONG_INLINE PacketN_2f pmax<PropagateNumbers, PacketN_2f>(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  return pminmax_propagate_numbers(a, b, pmax<PacketN_2f>);
}
template<> EIGEN_STRONG_INLINE PacketN_2f pmin<PropagateNaN, PacketN_2f>(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  return pminmax_propagate_nan(a, b, pmin<PacketN_2f>);
}
template<> EIGEN_STRONG_INLINE PacketN_2f pmax<PropagateNaN, PacketN_2f>(const PacketN_2f& a, const PacketN_2f& b) {
  FUNC_ENTRY;
  return pminmax_propagate_nan(a, b, pmax<PacketN_2f>);
}
template<> EIGEN_STRONG_INLINE float predux_min<PropagateNaN,PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  PacketN_2f tva, tvb, tvc;
  vboolN_2 tba = BBE_OEQN_2XF32(a,a);
  tva = BBE_MOVN_2XF32T(BBE_CONSTN_2XF32(0),a,tba);
  tvb = BBE_RADDN_2XF32(tva);
  tvc = a;
  BBE_ADDN_2XF32T(tvc,a,tvb,tba);
  return BBE_RMINNUMN_2XF32(tvc);
}
template<> EIGEN_STRONG_INLINE float predux_max<PropagateNaN,PacketN_2f>(const PacketN_2f &a) {
  FUNC_ENTRY;
  PacketN_2f tva, tvb, tvc;
  vboolN_2 tba = BBE_OEQN_2XF32(a,a);
  tva = BBE_MOVN_2XF32T(BBE_CONSTN_2XF32(0),a,tba);
  tvb = BBE_RADDN_2XF32(tva);
  tvc = a;
  BBE_ADDN_2XF32T(tvc,a,tvb,tba);
  return BBE_RMAXNUMN_2XF32(tvc);
}
} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_F32_KX_XTENSA_H
