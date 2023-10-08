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

#ifndef EIGEN_PACKET_MATH_F64_KX_XTENSA_H
#define EIGEN_PACKET_MATH_F64_KX_XTENSA_H

#if (XCHAL_HAVE_BBENEP && (SIMD_N_2_EQ_4() || SIMD_N_2_EQ_8() || SIMD_N_2_EQ_16() || SIMD_N_2_EQ_32())) || (XCHAL_HAVE_VISION && (SIMD_N_2_EQ_16() || SIMD_N_2_EQ_32()))
  #ifndef F64_FULL_PACKET_SUPPORT
    #define F64_FULL_PACKET_SUPPORT 1
  #endif
#else
  #if defined(F64_FULL_PACKET_SUPPORT) && F64_FULL_PACKET_SUPPORT
    #pragma message("Incorrect value for F64_FULL_PACKET_SUPPORT. Resetting it back")
    #undef F64_FULL_PACKET_SUPPORT
  #endif
  #define F64_FULL_PACKET_SUPPORT 0
#endif

#if !F64_FULL_PACKET_SUPPORT
  #if defined(F64_HALF_PACKET_SUPPORT) && F64_HALF_PACKET_SUPPORT
    #pragma message("Incorrect value for F64_HALF_PACKET_SUPPORT. Resetting it back")
    #undef F64_HALF_PACKET_SUPPORT
    #define F64_HALF_PACKET_SUPPORT 0
  #else
    #define F64_HALF_PACKET_SUPPORT 0
  #endif
#endif

#if F64_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

typedef xb_vecN_4xf64 PacketN_4d;

#  if SIMD_N_GE(8,2)
#    ifndef F64_HALF_PACKET_SUPPORT
#      define F64_HALF_PACKET_SUPPORT 1
#    endif
#  else
#    ifndef F64_HALF_PACKET_SUPPORT
#      define F64_HALF_PACKET_SUPPORT 0
#    else
#      if F64_HALF_PACKET_SUPPORT
#        warning "Incorrect value set for F64_HALF_PACKET_SUPPORT. Resetting it back"
#        undef F64_HALF_PACKET_SUPPORT
#        define F64_HALF_PACKET_SUPPORT 0
#      endif
#    endif
#  endif

#if F64_HALF_PACKET_SUPPORT
  typedef struct PacketN_8d {
    EIGEN_STRONG_INLINE PacketN_8d() {}
    EIGEN_STRONG_INLINE explicit PacketN_8d(const PacketN_4d& a) : v(a) {}
    EIGEN_STRONG_INLINE explicit PacketN_8d(PacketN_4d& a) : v(a) {}
    EIGEN_STRONG_INLINE operator PacketN_4d&() {return v;}
    PacketN_4d v;
  } PacketN_8d;

  template <typename Tto, typename Tfrom> EIGEN_STRONG_INLINE Tto extract_N_8d(const Tfrom& a, const Tfrom& /*b*/) {
    return a;
  }
  template <typename Tto, typename Tfrom> EIGEN_STRONG_INLINE Tto replicate_N_8d(const Tfrom& a) {
    return a;
  }

#if XCHAL_HAVE_BBENEP
  template <> EIGEN_STRONG_INLINE PacketN_4d extract_N_8d(const PacketN_4d& a, const PacketN_4d& b){
    return BBE_SELN_4XF64I(b, a, BBE_SELI_64B_EXTRACT_LO_HALVES);
  }
  template <> EIGEN_STRONG_INLINE PacketN_4d replicate_N_8d(const PacketN_4d& a){
    return extract_N_8d<PacketN_4d,PacketN_4d>(a, a);
  }
#endif

#else
  typedef PacketN_4d PacketN_8d;
#endif

inline std::ostream & operator <<(std::ostream & s, const PacketN_4d & v) {
  double *ptr = (double *)(&v);
  int itr = 0;
  for (itr = 0; itr < (SIMD_N_4-1); itr++)
  {
    s << ptr[itr] << ", ";
  }
  s << ptr[itr];
  return s;
}

#if F64_HALF_PACKET_SUPPORT
inline std::ostream & operator <<(std::ostream & s, const PacketN_8d & v) {
  double *ptr = (double *)(&v.v);
  int itr = 0;
  for (itr = 0; itr < (SIMD_N_8-1); itr++)
  {
    s << ptr[itr] << ", ";
  }
  s << ptr[itr];
  return s;
}
#endif

template <> struct is_arithmetic<PacketN_4d> {
  enum { value = true };
};

template <> struct packet_traits<double> : default_packet_traits {
  typedef PacketN_4d type;
  typedef PacketN_8d half;
  enum {
    Vectorizable    = 1,
    AlignedOnScalar = 1,
    size            = SIMD_N_4,
    #if F64_HALF_PACKET_SUPPORT
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
    HasASin  = XCLIB_SUPPORT,
    HasACos  = XCLIB_SUPPORT,
    HasATan  = XCLIB_SUPPORT,
    HasSinh  = XCLIB_SUPPORT,
    HasCosh  = XCLIB_SUPPORT,
    HasTanh  = XCLIB_SUPPORT,
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


template <> struct unpacket_traits<PacketN_4d> {
  typedef double type;
  typedef PacketN_8d half;
  enum {size=SIMD_N_4, alignment=ALIGN_N_2, vectorizable=true, masked_load_available=false, masked_store_available=false};
};

template <> EIGEN_STRONG_INLINE PacketN_4d pset1<PacketN_4d>(const double &from) {
  FUNC_ENTRY;
  return from; 
}
template <> EIGEN_STRONG_INLINE PacketN_4d pload1<PacketN_4d>(const double *from) {
  FUNC_ENTRY;
  return *from; 
}

/** \internal \brief Returns a packet with coefficients (a,a+1,...,a+packet_size-1). */
template <> EIGEN_STRONG_INLINE PacketN_4d plset<PacketN_4d>(const double &a) {
  FUNC_ENTRY;
  return pset1<PacketN_4d>(a) + BBE_FLOAT64N_4X64(BBE_SEQN_4X64(), 0 /*No scale*/);
}

template <> EIGEN_STRONG_INLINE PacketN_4d padd<PacketN_4d>(const PacketN_4d &a, const PacketN_4d &b) {
  FUNC_ENTRY;
  return a + b; 
}
template <> EIGEN_STRONG_INLINE PacketN_4d psub<PacketN_4d>(const PacketN_4d &a, const PacketN_4d &b) {
  FUNC_ENTRY;
  return a - b; 
}
template <> EIGEN_STRONG_INLINE PacketN_4d pnegate(const PacketN_4d &a) {
  FUNC_ENTRY;
  return -a; 
}
template <> EIGEN_STRONG_INLINE PacketN_4d pconj(const PacketN_4d &a) {
  FUNC_ENTRY;
  return a; 
}
template <> EIGEN_STRONG_INLINE PacketN_4d pmul<PacketN_4d>(const PacketN_4d &a, const PacketN_4d &b) {
  FUNC_ENTRY;
  return a * b; 
}
template <> EIGEN_STRONG_INLINE PacketN_4d pdiv<PacketN_4d>(const PacketN_4d &a, const PacketN_4d &b) {
  FUNC_ENTRY;
  return BBE_DIVN_4XF64(a, b); 
}

/** \internal \returns a * b + c (coeff-wise) */
template <> EIGEN_STRONG_INLINE PacketN_4d pmadd(const PacketN_4d &a, const PacketN_4d &b, const PacketN_4d &c) {
  FUNC_ENTRY;
  PacketN_4d t0 = c;
  BBE_MULAN_4XF64(t0, a, b);
  return t0;
}

template <> EIGEN_STRONG_INLINE PacketN_4d pand(const PacketN_4d &a, const PacketN_4d &b) {
  FUNC_ENTRY;
  return BBE_MOVN_4XF64_FROMN_4X64(BBE_ANDN_4X64(BBE_MOVN_4X64_FROMN_4XF64(a), BBE_MOVN_4X64_FROMN_4XF64(b)));
}
template <> EIGEN_STRONG_INLINE PacketN_4d por(const PacketN_4d &a, const PacketN_4d &b) {
  FUNC_ENTRY;
  return BBE_MOVN_4XF64_FROMN_4X64(BBE_ORN_4X64(BBE_MOVN_4X64_FROMN_4XF64(a), BBE_MOVN_4X64_FROMN_4XF64(b)));
}
template <> EIGEN_STRONG_INLINE PacketN_4d pandnot(const PacketN_4d &a, const PacketN_4d &b) {
  FUNC_ENTRY;
  return BBE_MOVN_4XF64_FROMN_4X64(BBE_ANDN_4X64(BBE_MOVN_4X64_FROMN_4XF64(a), BBE_NOTN_4X64(BBE_MOVN_4X64_FROMN_4XF64(b))));
}

template <> EIGEN_STRONG_INLINE PacketN_4d pmin(const PacketN_4d &a, const PacketN_4d &b) {
  FUNC_ENTRY;
  return BBE_MINN_4XF64(a, b); 
}
template <> EIGEN_STRONG_INLINE PacketN_4d pmax(const PacketN_4d &a, const PacketN_4d &b) {
  FUNC_ENTRY;
  return BBE_MAXN_4XF64(a, b); 
}
template <> EIGEN_STRONG_INLINE PacketN_4d pround(const PacketN_4d &a) {
  FUNC_ENTRY;
  return BBE_FIROUNDN_4XF64(a); 
}
template <> EIGEN_STRONG_INLINE PacketN_4d pceil(const PacketN_4d &a) {
  FUNC_ENTRY;
  return BBE_FICEILN_4XF64(a); 
}
template <> EIGEN_STRONG_INLINE PacketN_4d pfloor(const PacketN_4d &a) {
  FUNC_ENTRY;
  return BBE_FIFLOORN_4XF64(a); 
}

template <> EIGEN_STRONG_INLINE PacketN_4d pload<PacketN_4d>(const double *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  return *(xt_reinterpret_const_cast<PacketN_4d,double>(from));
}

template <> EIGEN_STRONG_INLINE PacketN_4d ploadu<PacketN_4d>(const double *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_LOAD;
  PacketN_4d     tv0;
  valign         talign;
  xb_vecN_4xf64 *tpv0 = xt_reinterpret_const_cast<xb_vecN_4xf64,double>(from);
  talign              = BBE_LAN_4XF64_PP(tpv0);
  BBE_LAN_4XF64_IP(tv0, talign, tpv0);
  return tv0;
}

/** \internal \returns a packet with elements of \a *from duplicated.
 * For instance, for a packet of 8 elements, 4 scalars will be read from \a *from and
 * duplicated to form: {from[0],from[0],from[1],from[1],from[2],from[2],from[3],from[3]}
 * Currently, this function is only used for scalar * complex products.
 */
template <> EIGEN_STRONG_INLINE PacketN_4d ploaddup<PacketN_4d>(const double *from) {
  FUNC_ENTRY;
  xb_vecN_4xf64 tv0 = ploadu<PacketN_4d>(from);
  return BBE_SELN_4XF64I(tv0, tv0, BBE_SELI_64B_INTERLEAVE_1_LO);
}

/** \internal \returns a packet with elements of \a *from quadrupled.
  * For instance, for a packet of 8 elements, 2 scalars will be read from \a *from and
  * replicated to form: {from[0],from[0],from[0],from[0],from[1],from[1],from[1],from[1]}
  * Currently, this function is only used in matrix products.
  * For packet-size smaller or equal to 4, this function is equivalent to pload1 
  */
template <> EIGEN_STRONG_INLINE PacketN_4d ploadquad<PacketN_4d>(const double *from) {
  FUNC_ENTRY;
  #if SIMD_N_2_EQ_4()
    return pload1<PacketN_4d>(from);
  #else
    xb_vecN_4xf64 tv0 = ploadu<PacketN_4d>(from);
    xb_vecN_4xf64 tv1 = BBE_SELN_4XF64I(tv0, tv0, BBE_SELI_64B_INTERLEAVE_1_LO);
    return BBE_SELN_4XF64I(tv1, tv1, BBE_SELI_64B_INTERLEAVE_1_LO);
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
template<> EIGEN_DEVICE_FUNC inline void pbroadcast4(const double *a, PacketN_4d& a0, PacketN_4d& a1, PacketN_4d& a2, PacketN_4d& a3) {
  FUNC_ENTRY;
  a0 = pload1<PacketN_4d>(a+0);
  a1 = pload1<PacketN_4d>(a+1);
  a2 = pload1<PacketN_4d>(a+2);
  a3 = pload1<PacketN_4d>(a+3);
}

/** \internal equivalent to
  * \code
  * a0 = pload1(a+0);
  * a1 = pload1(a+1);
  * \endcode
  * \sa pset1, pload1, ploaddup, pbroadcast4
  */
template<> EIGEN_DEVICE_FUNC inline void pbroadcast2(const double *a, PacketN_4d& a0, PacketN_4d& a1) {
  FUNC_ENTRY;
  a0 = pload1<PacketN_4d>(a+0);
  a1 = pload1<PacketN_4d>(a+1);
}

template <> EIGEN_STRONG_INLINE void pstore<double>(double *to, const PacketN_4d &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_STORE;
  *(xt_reinterpret_cast<PacketN_4d,double>(to)) = from;
}

template <> EIGEN_STRONG_INLINE void pstoreu<double>(double *to, const PacketN_4d &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_STORE;
  valign         talign;
  xb_vecN_4xf64 *tpv0 = xt_reinterpret_cast<xb_vecN_4xf64,double>(to);
  talign           = BBE_ZALIGN();
  BBE_SAN_4XF64_IP(from, talign, tpv0);
  BBE_SAN_4XF64POS_FP(talign, tpv0);
}

#if XCHAL_HAVE_BBENEP
  template <> EIGEN_DEVICE_FUNC inline PacketN_4d pgather<double, PacketN_4d>(const double *from, Index stride) {
    FUNC_ENTRY;
    double *tp = (double *)(from);
    xb_vecN_4xf64  ta;
    double *tpf = (double *)(&ta);
    int    i;
    int    stride_bytes = sizeof(double) * stride;
    xb_vecN_4xf64 tvf;

    for (i = 0; i < SIMD_N_4; i++) {
      BBE_LSN_4XF64_XP(tvf, tp, stride_bytes);
      BBE_SSN_4XF64_IP(tvf, tpf, sizeof(double));
    }
    return ta;
  }
#endif

#if XCHAL_HAVE_BBENEP
  template <> EIGEN_DEVICE_FUNC inline void pscatter<double, PacketN_4d>(double *to, const PacketN_4d &from, Index stride) {
    FUNC_ENTRY;
    double *tpf = (double*)(&from);
    double *tpt = (double*)(to);
    int    stride_bytes = sizeof(double) * stride;
    xb_vecN_4xf64 tvf;
    int i;
    for (i = 0; i < SIMD_N_4; i++)
    {
      BBE_LSN_4XF64_IP(tvf, tpf, sizeof(double));
      BBE_SSN_4XF64_XP(tvf, tpt, stride_bytes);
    }
  }
#endif

template <> EIGEN_STRONG_INLINE void pstore1<PacketN_4d>(double *to, const double &a) {
  FUNC_ENTRY;
  pstore(to, PacketN_4d((double)a)); 
}

template <> EIGEN_STRONG_INLINE double pfirst<PacketN_4d>(const PacketN_4d &a) {
  FUNC_ENTRY;
  return BBE_SELSN_4XF64(a, 0 /*Start element*/); 
}
template <> EIGEN_STRONG_INLINE PacketN_4d preverse(const PacketN_4d &a) {
  FUNC_ENTRY;
  return  BBE_SHFLN_4XF64I(a, BBE_SHFLI_64B_REVERSE_1); 
}
template <> EIGEN_STRONG_INLINE PacketN_4d pabs(const PacketN_4d &a) {
  FUNC_ENTRY;
  return BBE_ABSN_4XF64(a); 
}

template <> EIGEN_STRONG_INLINE double predux<PacketN_4d>(const PacketN_4d &a) {
  FUNC_ENTRY;
  return BBE_RADDN_4XF64(a); 
}

/** \internal \returns the sum of the elements of \a a by block of 4 elements.
  * For a packet {a0, a1, a2, a3, a4, a5, a6, a7}, it returns a half packet {a0+a4, a1+a5, a2+a6, a3+a7}
  * For packet-size smaller or equal to 4, this boils down to a noop.
  */
#if SIMD_N_2_EQ_16()
  template<> EIGEN_DEVICE_FUNC inline PacketN_8d predux_half_dowto4(const PacketN_4d& a) {
    FUNC_ENTRY;
    xb_vecN_4xf64 tv0, tv1, tv2;
    tv0 = a;
    tv1 = BBE_CONSTN_4XF64(0);
    tv2 = BBE_SELN_4XF64I(tv0, tv1, BBE_SELI_64B_ROTATE_RIGHT_4);
    tv2 = tv0 + tv2;
    tv0 = BBE_SELN_4XF64I(tv1, tv2, BBE_SELI_64B_ROTATE_LEFT_4);
    return PacketN_8d(tv0);
  }
#elif SIMD_N_2_EQ_32()
  template<> EIGEN_DEVICE_FUNC inline PacketN_8d predux_half_dowto4(const PacketN_4d& a) {
    FUNC_ENTRY;
    xb_vecN_4xf64 tv0, tv1, tv2, tv3;
    tv0 = a;
    tv1 = BBE_CONSTN_4XF64(0);
    tv2 = BBE_SELN_4XF64I(tv0, tv1, BBE_SELI_64B_ROTATE_RIGHT_8);
    tv2 = tv0 + tv2;
    tv3 = BBE_SELN_4XF64I(tv1, tv2, BBE_SELI_64B_ROTATE_RIGHT_8);
    return PacketN_8d(tv3);
  }
#endif

#if SIMD_N_2_EQ_8()
  #if XCHAL_HAVE_BBENEP
    /** \internal \returns the product of the elements of \a a*/
    template <> EIGEN_STRONG_INLINE double predux_mul<PacketN_4d>(const PacketN_4d &a) {
      FUNC_ENTRY;
      xb_vecN_4xf64 tv0, tv1;
      tv0 = a;
      tv1 = BBE_MULMN_4XF64(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_4XF64I(tv1, tv1, BBE_SELI_64B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_4XF64(tv0, tv0, 0x0, 0x9);
      return BBE_SELSN_4XF64(tv1, 0);
    }
  #endif
#elif SIMD_N_2_EQ_16()
  #if XCHAL_HAVE_BBENEP
    /** \internal \returns the product of the elements of \a a*/
    template <> EIGEN_STRONG_INLINE double predux_mul<PacketN_4d>(const PacketN_4d &a) {
      FUNC_ENTRY;
      xb_vecN_4xf64 tv0, tv1;
      tv0 = a;
      tv1 = BBE_MULMN_4XF64(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_4XF64I(tv1, tv1, BBE_SELI_64B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_4XF64(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_4XF64I(tv1, tv1, BBE_SELI_64B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_4XF64(tv0, tv0, 0x0, 0x9);
      return BBE_SELSN_4XF64(tv1, 0);
    }
  #endif
#elif SIMD_N_2_EQ_32()
  #if XCHAL_HAVE_BBENEP
    /** \internal \returns the product of the elements of \a a*/
    template <> EIGEN_STRONG_INLINE double predux_mul<PacketN_4d>(const PacketN_4d &a) {
      FUNC_ENTRY;
      xb_vecN_4xf64 tv0, tv1;
      tv0 = a;
      tv1 = BBE_MULMN_4XF64(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_4XF64I(tv1, tv1, BBE_SELI_64B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_4XF64(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_4XF64I(tv1, tv1, BBE_SELI_64B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_4XF64(tv0, tv0, 0x0, 0x9); tv0 = BBE_SELN_4XF64I(tv1, tv1, BBE_SELI_64B_EXTRACT_1_OF_2_OFF_0);
      tv1 = BBE_MULMN_4XF64(tv0, tv0, 0x0, 0x9);
      return BBE_SELSN_4XF64(tv1, 0);
    }
  #endif
#endif

template <> EIGEN_STRONG_INLINE double predux_min<PacketN_4d>(const PacketN_4d &a) {
  FUNC_ENTRY;
  return BBE_RMINN_4XF64(a); 
}
template <> EIGEN_STRONG_INLINE double predux_max<PacketN_4d>(const PacketN_4d &a) {
  FUNC_ENTRY;
  return BBE_RMAXN_4XF64(a); 
}

#if SIMD_N_2_EQ_4()
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4d, CONST_2> &kernel) {
    FUNC_ENTRY;
    PacketN_4d t00, t01;
    PacketN_4d t10, t11;

    t00 = kernel.packet[0];
    t01 = kernel.packet[1];

    BBE_DSELN_4XF64I(t11, t10, t01, t00, BBE_DSELI_64B_INTERLEAVE_1);

    kernel.packet[0] = t10;
    kernel.packet[1] = t11;
  }
#elif SIMD_N_2_EQ_8()
  #if XCHAL_HAVE_BBENEP
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4d, CONST_4> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_4xcf32 _var##_num = BBE_CONSTN_4XCF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = BBE_MOVN_4XCF32_FROMN_4XF64(kernel.packet[_num])
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = BBE_MOVN_4XF64_FROMN_4XCF32(_var##_src_num)

      // Declare vectors
      #define DECL_VEC(_var) \
        DR(_var,0); DR(_var,1); DR(_var,2); DR(_var,3);

      // Load vectors
      #define LOAD_VEC(_var) \
        LR(_var,0); LR(_var,1); LR(_var,2); LR(_var,3);

      // Store vectors
      #define STORE_VEC(_var) \
        SR(_var,0,0); SR(_var,1,1); SR(_var,2,2); SR(_var,3,3);

      /* This is first step in transpose operation. In first step, input vectors and
       * output vectors have same sequence. This will change in 'NEXT_STEP', where
       * output vectors will have the same sequence but input vector sequence will be
       * different. */
      #define FIRST_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);     \

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                               \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##1, _pat);    \

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvs, tvf);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_4, tvf, tvs);
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
#elif SIMD_N_2_EQ_16()
  #if XCHAL_HAVE_BBENEP
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4d, CONST_4> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_4xcf32 _var##_num = BBE_CONSTN_4XCF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = BBE_MOVN_4XCF32_FROMN_4XF64(kernel.packet[_num])
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = BBE_MOVN_4XF64_FROMN_4XCF32(_var##_src_num)

      // Declare vectors
      #define DECL_VEC(_var) \
        DR(_var,0); DR(_var,1); DR(_var,2); DR(_var,3);

      // Load vectors
      #define LOAD_VEC(_var) \
        LR(_var,0); LR(_var,1); LR(_var,2); LR(_var,3);

      // Store vectors
      #define STORE_VEC(_var) \
        SR(_var,0,0); SR(_var,1,1); SR(_var,2,2); SR(_var,3,3);

      /* This is first step in transpose operation. In first step, input vectors and
       * output vectors have same sequence. This will change in 'NEXT_STEP', where
       * output vectors will have the same sequence but input vector sequence will be
       * different. */
      #define FIRST_STEP(_pat, _out, _inp)                           \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##1, _inp##0, _pat); \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##2, _pat); \

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                            \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##2, _inp##0, _pat); \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##1, _pat); \

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvs, tvf);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_4, tvf, tvs);
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
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4d, CONST_8> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_4xcf32 _var##_num = BBE_CONSTN_4XCF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = BBE_MOVN_4XCF32_FROMN_4XF64(kernel.packet[_num])
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = BBE_MOVN_4XF64_FROMN_4XCF32(_var##_src_num)

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
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);     \
        BBE_DSELN_4XCF32I(_out##5, _out##4, _inp##5, _inp##4, _pat);     \
        BBE_DSELN_4XCF32I(_out##7, _out##6, _inp##7, _inp##6, _pat);     \

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                               \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##6, _inp##4, _pat);    \
        BBE_DSELN_4XCF32I(_out##5, _out##4, _inp##3, _inp##1, _pat);   \
        BBE_DSELN_4XCF32I(_out##7, _out##6, _inp##7, _inp##5, _pat);  \

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvs, tvf);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_4, tvf, tvs);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_8, tvs, tvf);
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
#elif SIMD_N_2_EQ_32()
  #if XCHAL_HAVE_BBENEP
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4d, CONST_4> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_4xcf32 _var##_num = BBE_CONSTN_4XCF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = BBE_MOVN_4XCF32_FROMN_4XF64(kernel.packet[_num])
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = PacketN_4d(BBE_MOVN_4XF64_FROMN_4XCF32(_var##_src_num))

      // Declare vectors
      #define DECL_VEC(_var) \
        DR(_var,0); DR(_var,1); DR(_var,2); DR(_var,3);

      // Load vectors
      #define LOAD_VEC(_var) \
        LR(_var,0); LR(_var,1); LR(_var,2); LR(_var,3);

      // Store vectors
      #define STORE_VEC(_var) \
        SR(_var,0,0); SR(_var,1,1); SR(_var,2,2); SR(_var,3,3);

      /* This is first step in transpose operation. In first step, input vectors and
       * output vectors have same sequence. This will change in 'NEXT_STEP', where
       * output vectors will have the same sequence but input vector sequence will be
       * different. */
      #define FIRST_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);     \

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                               \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##1, _pat);    \

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvs, tvf);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_4, tvf, tvs);
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
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4d, CONST_16> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_4xcf32 _var##_num = BBE_CONSTN_4XCF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = BBE_MOVN_4XCF32_FROMN_4XF64(kernel.packet[_num])
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = PacketN_4d(BBE_MOVN_4XF64_FROMN_4XCF32(_var##_src_num))

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
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);     \
        BBE_DSELN_4XCF32I(_out##5, _out##4, _inp##5, _inp##4, _pat);     \
        BBE_DSELN_4XCF32I(_out##7, _out##6, _inp##7, _inp##6, _pat);     \
        BBE_DSELN_4XCF32I(_out##9, _out##8, _inp##9, _inp##8, _pat);     \
        BBE_DSELN_4XCF32I(_out##11, _out##10, _inp##11, _inp##10, _pat); \
        BBE_DSELN_4XCF32I(_out##13, _out##12, _inp##13, _inp##12, _pat); \
        BBE_DSELN_4XCF32I(_out##15, _out##14, _inp##15, _inp##14, _pat);

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        BBE_DSELN_4XCF32I(_out##3, _out##2, _inp##6, _inp##4, _pat);    \
        BBE_DSELN_4XCF32I(_out##5, _out##4, _inp##10, _inp##8, _pat);   \
        BBE_DSELN_4XCF32I(_out##7, _out##6, _inp##14, _inp##12, _pat);  \
        BBE_DSELN_4XCF32I(_out##9, _out##8, _inp##3, _inp##1, _pat);    \
        BBE_DSELN_4XCF32I(_out##11, _out##10, _inp##7, _inp##5, _pat);  \
        BBE_DSELN_4XCF32I(_out##13, _out##12, _inp##11, _inp##9, _pat); \
        BBE_DSELN_4XCF32I(_out##15, _out##14, _inp##15, _inp##13, _pat);

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvs, tvf);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_4, tvf, tvs);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_8, tvs, tvf);
      NEXT_STEP(BBE_DSELI_32B_INTERLEAVE_16, tvf, tvs);
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
#endif

// Scalar path for pmadd with FMA to ensure consistency with vectorized path.
template <> EIGEN_STRONG_INLINE double pmadd(const double &a, const double &b, const double &c) {
  FUNC_ENTRY;
  xtdouble t0 = c;
  XT_MADD_D(t0, a, b);
  return t0;
}

template<> EIGEN_STRONG_INLINE PacketN_4d pcmp_eq(const PacketN_4d& a, const PacketN_4d& b) {
  FUNC_ENTRY;
  xb_vecN_4x64U tv0 = 0x0000000000000000u;
  xb_vecN_4x64U tv1 = 0xffffffffffffffffu;
  return BBE_MOVN_4XF64_FROMN_4X64(BBE_MOVN_4X64UT(tv1, tv0, BBE_OEQN_4XF64(a,b)));
}
template<> EIGEN_STRONG_INLINE double pcmp_le(const double& a, const double& b) {
  FUNC_ENTRY;
  ui64_f64 t = {0xffffffffffffffffu};
  double ret = 0, t0 = 0, t1 = t.f64;
  BBE_ADDF64T(ret, t0, t1, XT_OLE_D(a,b));
  return ret;
}
template<> EIGEN_STRONG_INLINE double pcmp_lt(const double& a, const double& b) {
  FUNC_ENTRY;
  ui64_f64 t = {0xffffffffffffffffu};
  double ret = 0, t0 = 0, t1 = t.f64;
  BBE_ADDF64T(ret, t0, t1, XT_OLT_D(a,b));
  return ret;
}
template<> EIGEN_STRONG_INLINE double pcmp_lt_or_nan(const double& a, const double& b) {
  FUNC_ENTRY;
  ui64_f64 t = {0xffffffffffffffffu};
  double ret = 0, t0 = 0, t1 = t.f64;
  BBE_ADDF64T(ret, t0, t1, XT_ULT_D(a,b));
  return ret;
}
template<> EIGEN_STRONG_INLINE PacketN_4d pcmp_le(const PacketN_4d& a, const PacketN_4d& b) {
  FUNC_ENTRY;
  xb_vecN_4x64U tv0 = 0x0000000000000000u;
  xb_vecN_4x64U tv1 = 0xffffffffffffffffu;
  return BBE_MOVN_4XF64_FROMN_4X64(BBE_MOVN_4X64UT(tv1, tv0, BBE_OLEN_4XF64(a,b)));
}
template<> EIGEN_STRONG_INLINE PacketN_4d pcmp_lt(const PacketN_4d& a, const PacketN_4d& b) {
  FUNC_ENTRY;
  xb_vecN_4x64U tv0 = 0x0000000000000000u;
  xb_vecN_4x64U tv1 = 0xffffffffffffffffu;
  return BBE_MOVN_4XF64_FROMN_4X64(BBE_MOVN_4X64UT(tv1, tv0, BBE_OLTN_4XF64(a,b)));
}
template<> EIGEN_STRONG_INLINE PacketN_4d pcmp_lt_or_nan(const PacketN_4d& a, const PacketN_4d& b) {
  FUNC_ENTRY;
  xb_vecN_4x64U tv0 = 0x0000000000000000u;
  xb_vecN_4x64U tv1 = 0xffffffffffffffffu;
  return BBE_MOVN_4XF64_FROMN_4X64(BBE_MOVN_4X64UT(tv1, tv0, BBE_ULTN_4XF64(a,b)));
}

template<> EIGEN_DEVICE_FUNC inline PacketN_4d pselect(const PacketN_4d& mask, const PacketN_4d& a, const PacketN_4d& b) {
  FUNC_ENTRY;
  return BBE_MOVN_4XF64T(b,a,(mask==PacketN_4d(0)));
}
template<> EIGEN_STRONG_INLINE PacketN_4d ptrue<PacketN_4d>(const PacketN_4d& /*a*/) {
  FUNC_ENTRY;
  return VEC_XF64_FROM_VEC_X64U(0xffffffffffffffffu);
}

template <typename Packet, typename Op>
EIGEN_STRONG_INLINE Packet pminmax_propagate_numbers_F64(const Packet& a, const Packet& b, Op op) {
  FUNC_ENTRY;
  Packet ta = BBE_MOVN_4XF64T(a,b,BBE_OEQN_4XF64(a,a));
  Packet tb = BBE_MOVN_4XF64T(b,a,BBE_OEQN_4XF64(b,b));
  return op(ta,tb);
}

template <typename Packet, typename Op>
EIGEN_STRONG_INLINE Packet pminmax_propagate_nan_F64(const Packet& a, const Packet& b, Op op) {
  FUNC_ENTRY;
  Packet ta = BBE_MOVN_4XF64T(b,a,BBE_OEQN_4XF64(a,a));
  Packet tb = BBE_MOVN_4XF64T(a,b,BBE_OEQN_4XF64(b,b));
  return op(ta,tb);
}

// Add specializations for min/max with prescribed NaN progation.
template<> EIGEN_STRONG_INLINE PacketN_4d pmin<PropagateNumbers, PacketN_4d>(const PacketN_4d& a, const PacketN_4d& b) {
  FUNC_ENTRY;
  return pminmax_propagate_numbers_F64(a, b, pmin<PacketN_4d>);
}
template<> EIGEN_STRONG_INLINE PacketN_4d pmax<PropagateNumbers, PacketN_4d>(const PacketN_4d& a, const PacketN_4d& b) {
  FUNC_ENTRY;
  return pminmax_propagate_numbers_F64(a, b, pmax<PacketN_4d>);
}
template<> EIGEN_STRONG_INLINE PacketN_4d pmin<PropagateNaN, PacketN_4d>(const PacketN_4d& a, const PacketN_4d& b) {
  FUNC_ENTRY;
  return pminmax_propagate_nan_F64(a, b, pmin<PacketN_4d>);
}
template<> EIGEN_STRONG_INLINE PacketN_4d pmax<PropagateNaN, PacketN_4d>(const PacketN_4d& a, const PacketN_4d& b) {
  FUNC_ENTRY;
  return pminmax_propagate_nan_F64(a, b, pmax<PacketN_4d>);
}
template<> EIGEN_STRONG_INLINE double predux_min<PropagateNaN,PacketN_4d>(const PacketN_4d &a) {
  FUNC_ENTRY;
  PacketN_4d tva, tvb, tvc;
  vboolN_4 tba = BBE_OEQN_4XF64(a,a);
  tva = BBE_MOVN_4XF64T(BBE_CONSTN_4XF64(0),a,tba);
  tvb = BBE_RADDN_4XF64(tva);
  tvc = a;
  BBE_ADDN_4XF64T(tvc,a,tvb,tba);
  return BBE_RMINNUMN_4XF64(tvc);
}
template<> EIGEN_STRONG_INLINE double predux_max<PropagateNaN,PacketN_4d>(const PacketN_4d &a) {
  FUNC_ENTRY;
  PacketN_4d tva, tvb, tvc;
  vboolN_4 tba = BBE_OEQN_4XF64(a,a);
  tva = BBE_MOVN_4XF64T(BBE_CONSTN_4XF64(0),a,tba);
  tvb = BBE_RADDN_4XF64(tva);
  tvc = a;
  BBE_ADDN_4XF64T(tvc,a,tvb,tba);
  return BBE_RMAXNUMN_4XF64(tvc);
}
} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_F64_KX_XTENSA_H
