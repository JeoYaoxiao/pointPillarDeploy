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

#ifndef EIGEN_PACKET_MATH_CF32_KX_XTENSA_H
#define EIGEN_PACKET_MATH_CF32_KX_XTENSA_H

#if (XCHAL_HAVE_BBENEP && (SIMD_N_4_EQ_2() || SIMD_N_4_EQ_4() || SIMD_N_4_EQ_8() || SIMD_N_4_EQ_16())) || (XCHAL_HAVE_VISION && SIMD_N_4_EQ_16()) || (XCHAL_HAVE_PDX && SIMD_N_4_EQ_2())
  #ifndef CF32_FULL_PACKET_SUPPORT
    #define CF32_FULL_PACKET_SUPPORT 1
  #endif
#else
  #if defined(CF32_FULL_PACKET_SUPPORT) && CF32_FULL_PACKET_SUPPORT
    #pragma message("Incorrect value for CF32_FULL_PACKET_SUPPORT. Resetting it back")
    #undef CF32_FULL_PACKET_SUPPORT
  #endif
  #define CF32_FULL_PACKET_SUPPORT 0
#endif

#if !CF32_FULL_PACKET_SUPPORT
  #if defined(CF32_HALF_PACKET_SUPPORT) && CF32_HALF_PACKET_SUPPORT
    #pragma message("Incorrect value for CF32_HALF_PACKET_SUPPORT. Resetting it back")
    #undef CF32_HALF_PACKET_SUPPORT
    #define CF32_HALF_PACKET_SUPPORT 0
  #else
    #define CF32_HALF_PACKET_SUPPORT 0
  #endif
#endif

#if CF32_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

typedef std::complex<float> complexfloat;

template <> struct is_arithmetic<xb_vecN_4xcf32> {
  enum { value = true };
};

#  if SIMD_N_GE(8,2) && (F32_HALF_PACKET_SUPPORT==1)
#    ifndef CF32_HALF_PACKET_SUPPORT
#      define CF32_HALF_PACKET_SUPPORT 1
#    endif
#  else
#    ifndef CF32_HALF_PACKET_SUPPORT
#      define CF32_HALF_PACKET_SUPPORT 0
#    else
#      if CF32_HALF_PACKET_SUPPORT
#        warning "Incorrect value set for CF32_HALF_PACKET_SUPPORT. Resetting it back"
#        undef CF32_HALF_PACKET_SUPPORT
#        define CF32_HALF_PACKET_SUPPORT 0
#      endif
#    endif
#  endif

struct PacketN_4cf {
  EIGEN_STRONG_INLINE PacketN_4cf() {}
  EIGEN_STRONG_INLINE explicit PacketN_4cf(const PacketN_2f& a) {v = VEC_XCF32_FROM_VEC_XF32(a);}
  #if (F32_HALF_PACKET_SUPPORT)
  EIGEN_STRONG_INLINE explicit PacketN_4cf(const PacketN_4f& a) {v = VEC_XCF32_FROM_VEC_XF32(a.v);}
  #endif // (F32_HALF_PACKET_SUPPORT)
  EIGEN_STRONG_INLINE explicit PacketN_4cf(xb_vecN_2xf32& a) {v = VEC_XCF32_FROM_VEC_XF32(a);}
  EIGEN_STRONG_INLINE explicit PacketN_4cf(const xb_vecN_4xcf32& a) : v(a) {}
  EIGEN_STRONG_INLINE explicit PacketN_4cf(xb_vecN_4xcf32& a) : v(a) {}
  EIGEN_STRONG_INLINE operator PacketN_2f() {return VEC_XF32_FROM_VEC_XCF32(v);}
  #if (F32_HALF_PACKET_SUPPORT)
  EIGEN_STRONG_INLINE operator PacketN_4f() {return PacketN_4f(VEC_XF32_FROM_VEC_XCF32(v));}
  #endif // (F32_HALF_PACKET_SUPPORT)
  #if defined(BBE_OEQN_4XCF32)
  EIGEN_STRONG_INLINE friend vboolN_4 operator==(const PacketN_4cf& a, const PacketN_4cf& b) {return BBE_OEQN_4XCF32(a.v,b.v);}
  #else
  EIGEN_STRONG_INLINE friend vboolN_4 operator==(const PacketN_4cf& a, const PacketN_4cf& b) {
    #define TT(_v) BBE_SHFLN_2XF32I(_v,BBE_SHFLI_32B_SWAP_1)
    xb_vecN_2xf32 va,vb;
    va = VEC_XF32_FROM_VEC_XCF32(a.v);
    vb = VEC_XF32_FROM_VEC_XCF32(b.v);
    vboolN_2 vb0 = BBE_OEQN_2XF32(va,vb);
    vboolN_2 vb1 = BBE_OEQN_2XF32(TT(va),TT(vb));
    #undef TT
    return BBE_MOVN_4_FROM2N(BBE_MOV2N_FROMN_2(BBE_ANDBN_2(vb0,vb1)));
  }
  #endif // defined(BBE_OEQN_4XCF32)
  xb_vecN_4xcf32 v;
};

#if CF32_HALF_PACKET_SUPPORT
  struct PacketN_8cf {
    EIGEN_STRONG_INLINE PacketN_8cf() {}
    EIGEN_STRONG_INLINE explicit PacketN_8cf(const PacketN_4cf& a) {v.v = a.v;}
    #if (F32_HALF_PACKET_SUPPORT)
    EIGEN_STRONG_INLINE explicit PacketN_8cf(const PacketN_4f& a) {v.v = VEC_XCF32_FROM_VEC_XF32(a.v);}
    #endif // (F32_HALF_PACKET_SUPPORT)
    EIGEN_STRONG_INLINE explicit PacketN_8cf(const xb_vecN_4xcf32& a) {v.v = a;}
    EIGEN_STRONG_INLINE explicit PacketN_8cf(xb_vecN_4xcf32& a) {v.v = a;}
    #if (F32_HALF_PACKET_SUPPORT)
    EIGEN_STRONG_INLINE operator PacketN_4f() {return PacketN_4f(VEC_XF32_FROM_VEC_XCF32(v.v));}
    #endif // (F32_HALF_PACKET_SUPPORT)
    EIGEN_STRONG_INLINE operator PacketN_4cf&() {return v;}
    PacketN_4cf v;
  };

  template <typename Tto, typename Tfrom> EIGEN_STRONG_INLINE Tto extract_N_8cf(const Tfrom& a, const Tfrom& /*b*/) {
    return a;
  }
  template <typename Tto, typename Tfrom> EIGEN_STRONG_INLINE Tto replicate_N_8cf(const Tfrom& a) {
    return a;
  }

  #if XCHAL_HAVE_BBENEP
    template <> EIGEN_STRONG_INLINE PacketN_4cf extract_N_8cf(const PacketN_4cf& a, const PacketN_4cf& b){
      return PacketN_4cf(BBE_SELN_4XCF32I(b.v, a.v, BBE_SELI_64B_EXTRACT_LO_HALVES));
    }
  #endif

#else
  typedef PacketN_4cf PacketN_8cf;
#endif

template <> struct packet_traits<complexfloat > : default_packet_traits {
  typedef PacketN_4cf type;
  typedef PacketN_8cf half;
  enum {
    Vectorizable    = 1,
    AlignedOnScalar = 1,
    size            = SIMD_N_4,
    #if CF32_HALF_PACKET_SUPPORT
      HasHalfPacket   = 1,
    #else
      HasHalfPacket   = 0,
    #endif

    HasAdd       = 1,
    HasSub       = 1,
    HasShift     = 0,
    HasMul       = 1,
    HasNegate    = 1,
    HasAbs       = 0,
    HasArg       = 0,
    HasAbs2      = 0,
    HasAbsDiff   = 0,
    HasMin       = 0,
    HasMax       = 0,
    HasConj      = 1,
    HasSetLinear = 0,
    HasBlend     = 0,
    HasCmp       = 0,

    HasDiv   = 1,
    HasSqrt  = 0,
    HasRsqrt = 0,
    HasExp   = 0,
    HasExpm1 = 0,
    HasLog   = 0,
    HasLog1p = 0,
    HasLog10 = 0,
    HasPow    = 0,

    HasSin   = 0,
    HasCos   = 0,
    HasTan   = 0,
    HasASin  = 0,
    HasACos  = 0,
    HasATan  = 0,
    HasSinh  = 0,
    HasCosh  = 0,
    HasTanh  = 0,
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

    HasRound  = 0,
    HasRint   = 0,
    HasFloor  = 0,
    HasCeil   = 0,
    HasSign   = 0
  };
};

#if CF32_HALF_PACKET_SUPPORT
  template <> struct unpacket_traits<PacketN_8cf> {
    typedef complexfloat type;
    typedef PacketN_8cf half;
    enum {size=SIMD_N_8, alignment=ALIGN_N_2, vectorizable=true, masked_load_available=false, masked_store_available=false};
  };
#endif

template <> struct unpacket_traits<PacketN_4cf> {
  typedef complexfloat type;
  typedef PacketN_8cf half;
  enum {size=SIMD_N_4, alignment=ALIGN_N_2, vectorizable=true, masked_load_available=false, masked_store_available=false};
};

template <> EIGEN_STRONG_INLINE PacketN_4cf pset1<PacketN_4cf>(const complexfloat &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtfloat *tp0 = (xt_reinterpret_const_cast<xtfloat,complexfloat >(&from)); 
  xtfloat treal, timag;
  treal = BBE_LSRN_2XF32_I(tp0, 0*sizeof(xtfloat) /* real */);
  timag = BBE_LSRN_2XF32_I(tp0, 1*sizeof(xtfloat) /* imag */);
  xtcomplexfloat tcplx = BBE_CMPLXF32(timag, treal);
  xb_vecN_4xcf32 tvcplx = BBE_LSRN_4XCF32_I(&tcplx, 0);
  return PacketN_4cf(tvcplx);
}

template <> EIGEN_STRONG_INLINE PacketN_4cf padd<PacketN_4cf>(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  return PacketN_4cf(a.v + b.v); 
}
template <> EIGEN_STRONG_INLINE PacketN_4cf psub<PacketN_4cf>(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  return PacketN_4cf(a.v - b.v); 
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pnegate(const PacketN_4cf &a) {
  FUNC_ENTRY;
  return PacketN_4cf(BBE_NEGN_4XCF32(a.v)); 
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pconj(const PacketN_4cf &a) {
  FUNC_ENTRY;
  return PacketN_4cf(BBE_CONJN_4XCF32(a.v)); 
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pmul<PacketN_4cf>(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  return PacketN_4cf(a.v * b.v); 
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pdiv<PacketN_4cf>(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  return PacketN_4cf(BBE_DIVN_4XCF32(a.v, b.v)); 
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pand(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  return PacketN_4cf(VEC_XCF32_FROM_VEC_X32(BBE_ANDN_2X32(VEC_X32_FROM_VEC_XCF32(a.v), VEC_X32_FROM_VEC_XCF32(b.v))));
}
template <> EIGEN_STRONG_INLINE PacketN_4cf por(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  return PacketN_4cf(VEC_XCF32_FROM_VEC_X32(BBE_ORN_2X32(VEC_X32_FROM_VEC_XCF32(a.v), VEC_X32_FROM_VEC_XCF32(b.v))));
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pxor(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  return PacketN_4cf(VEC_XCF32_FROM_VEC_X32(BBE_XORN_2X32(VEC_X32_FROM_VEC_XCF32(a.v), VEC_X32_FROM_VEC_XCF32(b.v))));
}
#if XCHAL_HAVE_BBENEP
  template <> EIGEN_STRONG_INLINE PacketN_4cf pandnot(const PacketN_4cf &a, const PacketN_4cf &b) {
    FUNC_ENTRY;
    return PacketN_4cf(VEC_XCF32_FROM_VEC_X32(BBE_ANDNOTN_2X32(VEC_X32_FROM_VEC_XCF32(a.v), VEC_X32_FROM_VEC_XCF32(b.v))));
  }
#endif

template <> EIGEN_STRONG_INLINE PacketN_4cf pload<PacketN_4cf>(const complexfloat *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  return *(xt_reinterpret_const_cast<PacketN_4cf,complexfloat >(from));
}

template <> EIGEN_STRONG_INLINE PacketN_4cf ploadu<PacketN_4cf>(const complexfloat *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_LOAD;
  xb_vecN_4xcf32     tv0;
  xb_vecN_4xcf32 *tpv0 = xt_reinterpret_const_cast<xb_vecN_4xcf32,complexfloat >(from);
#if defined(IVP_L2UN_4XCF32_XP)
  IVP_L2UN_4XCF32_XP(tv0, tpv0, sizeof(complexfloat)*SIMD_N_4);
#else
  valign         talign;
  talign              = BBE_LAN_4XCF32_PP(tpv0);
  BBE_LAN_4XCF32_IP(tv0, talign, tpv0);
#endif
  return PacketN_4cf(tv0);
}

#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX
  /** \internal \returns a packet with elements of \a *from duplicated.
   * For instance, for a packet of 8 elements, 4 scalars will be read from \a *from and
   * duplicated to form: {from[0],from[0],from[1],from[1],from[2],from[2],from[3],from[3]}
   * Currently, this function is only used for scalar * complex products.
   */
  template <> EIGEN_STRONG_INLINE PacketN_4cf ploaddup<PacketN_4cf>(const complexfloat *from) {
    FUNC_ENTRY;
    EIGEN_DEBUG_ALIGNED_LOAD;
    PacketN_4cf tv0 = ploadu<PacketN_4cf>(from);
    return PacketN_4cf(BBE_SELN_4XCF32I(tv0.v, tv0.v, BBE_SELI_32B_INTERLEAVE_2_LO));
  }
#endif

#if SIMD_N_4_EQ_4() || SIMD_N_4_EQ_8() || SIMD_N_4_EQ_16()
  #if XCHAL_HAVE_BBENEP
    /** \internal \returns a packet with elements of \a *from quadrupled.
      * For instance, for a packet of 8 elements, 2 scalars will be read from \a *from and
      * replicated to form: {from[0],from[0],from[0],from[0],from[1],from[1],from[1],from[1]}
      * Currently, this function is only used in matrix products.
      * For packet-size smaller or equal to 4, this function is equivalent to pload1 
      */
    template <> EIGEN_STRONG_INLINE PacketN_4cf ploadquad<PacketN_4cf>(const complexfloat *from) {
      FUNC_ENTRY;
      EIGEN_DEBUG_ALIGNED_LOAD;
      PacketN_4cf tv0 = ploadu<PacketN_4cf>(from);
      xb_vecN_4xcf32 tv1 = BBE_SELN_4XCF32I(tv0.v, tv0.v, BBE_SELI_64B_INTERLEAVE_1_LO);
      return PacketN_4cf(BBE_SELN_4XCF32I(tv1, tv1, BBE_SELI_64B_INTERLEAVE_1_LO));
    }
  #endif
#endif

template <> EIGEN_STRONG_INLINE void pstore<complexfloat >(complexfloat *to, const PacketN_4cf &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_STORE;
  *(xt_reinterpret_cast<PacketN_4cf,complexfloat >(to)) = from;
}

template <> EIGEN_STRONG_INLINE void pstoreu<complexfloat >(complexfloat *to, const PacketN_4cf &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_STORE;
  valign         talign;
  xb_vecN_4xcf32 *tpv0 = xt_reinterpret_cast<xb_vecN_4xcf32,complexfloat>(to);
  talign           = BBE_ZALIGN();
  BBE_SAN_4XCF32_IP(from.v, talign, tpv0);
  BBE_SAN_4XCF32POS_FP(talign, tpv0);
}

#if XCHAL_HAVE_BBENEP
  template <> EIGEN_DEVICE_FUNC inline PacketN_4cf pgather<complexfloat, PacketN_4cf>(const complexfloat *from, Index stride) {
    FUNC_ENTRY;
    EIGEN_DEBUG_ALIGNED_LOAD;
    #if SIMD_N_4_EQ_2()
      xtcomplexfloat *tp = xt_reinterpret_const_cast<xtcomplexfloat,complexfloat>(from);
      xb_vecN_4xcf32 tv0, tv1, tv2;
      tv0 = BBE_LSN_4XCF32_X(tp, 0 * sizeof(xtcomplexfloat) * stride);
      tv1 = BBE_LSN_4XCF32_X(tp, 1 * sizeof(xtcomplexfloat) * stride);
      tv2 = BBE_SELN_4XCF32I(tv1, tv0, BBE_SELI_64B_INTERLEAVE_1_EVEN);
      return PacketN_4cf(tv2);
    #else
      xtcomplexfloat *tp = xt_reinterpret_const_cast<xtcomplexfloat,complexfloat>(from);
      xb_vecN_4xcf32 ta;
      xtcomplexfloat *tpf = xt_reinterpret_cast<xtcomplexfloat,xb_vecN_4xcf32>(&ta);
      int    i;
      int    stride_bytes = sizeof(xtcomplexfloat) * stride;
      xb_vecN_4xcf32 tvf;

      for (i = 0; i < SIMD_N_4; i++) {
        BBE_LSN_4XCF32_XP(tvf, tp, stride_bytes);
        BBE_SSN_4XCF32_IP(tvf, tpf, sizeof(xtcomplexfloat));
      }
      return PacketN_4cf(ta);
    #endif
  }
#endif

#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX
  template <> EIGEN_DEVICE_FUNC inline void pscatter<complexfloat, PacketN_4cf>(complexfloat *to, const PacketN_4cf &from, Index stride) {
    FUNC_ENTRY;
    EIGEN_DEBUG_ALIGNED_LOAD;
    #if SIMD_N_4_EQ_2()
      xtcomplexfloat *tpt = xt_reinterpret_cast<xtcomplexfloat,complexfloat>(to);
      tpt[stride * 0] = BBE_SELSN_4XCF32(from.v, 0);
      tpt[stride * 1] = BBE_SELSN_4XCF32(from.v, 1);
    #else
      int i;
      xtcomplexfloat *tpf = xt_reinterpret_const_cast<xtcomplexfloat,PacketN_4cf>(&from);
      xtcomplexfloat *tpt = xt_reinterpret_cast<xtcomplexfloat,complexfloat>(to);
      int    stride_bytes = sizeof(xtcomplexfloat) * stride;
      xb_vecN_4xcf32 tvf;
      for (i = 0; i < SIMD_N_4; i++)
      {
        BBE_LSN_4XCF32_IP(tvf, tpf, sizeof(xtcomplexfloat));
        BBE_SSN_4XCF32_XP(tvf, tpt, stride_bytes);
      }
    #endif
  }
#endif

inline std::ostream & operator <<(std::ostream & s, const PacketN_4cf & v) {
  complexfloat *ptr = xt_reinterpret_const_cast<complexfloat,xb_vecN_4xcf32>(&v.v);
  int itr = 0;
  for (itr = 0; itr < (SIMD_N_4-1); itr++)
  {
    s << ptr[itr] << ", ";
  }
  s << ptr[itr];
  return s;
}

template <> EIGEN_STRONG_INLINE complexfloat pfirst<PacketN_4cf>(const PacketN_4cf &a) {
  FUNC_ENTRY;
  xtcomplexfloat t0 = BBE_SELSN_4XCF32(a.v, 0 /*Start element*/); 
  return *(xt_reinterpret_cast<complexfloat,xtcomplexfloat>(&t0));
}
#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX
  template <> EIGEN_STRONG_INLINE PacketN_4cf preverse(const PacketN_4cf &a) {
    FUNC_ENTRY;
    return PacketN_4cf(BBE_SHFLN_4XCF32I(a.v, BBE_SHFLI_64B_REVERSE_1)); 
  }
#endif

template <> EIGEN_STRONG_INLINE complexfloat predux<PacketN_4cf>(const PacketN_4cf &a) {
  FUNC_ENTRY;
  xtcomplexfloat t0 = BBE_RADDN_4XCF32(a.v);
  return *(xt_reinterpret_cast<complexfloat,xtcomplexfloat>(&t0));
}

#if XCHAL_HAVE_BBENEP
  #if SIMD_N_4_EQ_8() || SIMD_N_4_EQ_16()
    /** \internal \returns the sum of the elements of \a a by block of 4 elements.
      * For a packet {a0, a1, a2, a3, a4, a5, a6, a7}, it returns a half packet {a0+a4, a1+a5, a2+a6, a3+a7}
      * For packet-size smaller or equal to 4, this boils down to a noop.
      */
    template<> EIGEN_DEVICE_FUNC inline PacketN_8cf predux_half_dowto4(const PacketN_4cf& a) {
      FUNC_ENTRY;
      #if SIMD_N_4_EQ_8()
        xb_vecN_4xcf32 tv0, tv1, tv2;
        tv0 = a.v;
        tv1 = BBE_CONSTN_4XCF32(0);
        tv2 = BBE_SELN_4XCF32I(tv0, tv1, BBE_SELI_64B_ROTATE_RIGHT_4);
        tv2 = tv0 + tv2;
        tv0 = BBE_SELN_4XCF32I(tv1, tv2, BBE_SELI_64B_ROTATE_LEFT_4);
        return PacketN_8cf(tv0);
      #elif SIMD_N_4_EQ_16()
        xb_vecN_4xcf32 tv0, tv1, tv2;
        vboolN_4 tvb;
        tvb = BBE_LTRN_4I(SIMD_N_4/2);
        tv0 = a.v;
        tv1 = BBE_CONSTN_4XCF32(0);
        tv2 = BBE_SELN_4XCF32I(tv1, tv0, BBE_SELI_64B_ROTATE_RIGHT_8);
        tv2 = tv0 + tv2;
        tv0 = BBE_MOVN_4XCF32T(tv2, tv1, tvb);
        return PacketN_8cf(tv0);
      #endif
    }
  #endif
#endif

#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX
  /** \internal \returns the product of the elements of \a a*/
  template <> EIGEN_STRONG_INLINE complexfloat predux_mul<PacketN_4cf>(const PacketN_4cf &a) {
    FUNC_ENTRY;
    #if SIMD_N_4_EQ_2()
      xb_vecN_4xcf32 tv1 = a.v;
      xb_vecN_4xcf32 tv2 = BBE_SELN_4XCF32I(tv1, tv1, BBE_SELI_64B_ROTATE_LEFT_1);
      xb_vecN_4xcf32 tv3 = BBE_MULN_4XCF32(tv1, tv2);
      xtcomplexfloat t0 = BBE_SELSN_4XCF32(tv3, 0);
      return *(xt_reinterpret_cast<complexfloat,xtcomplexfloat>(&t0));
    #elif SIMD_N_4_EQ_4()
      xb_vecN_4xcf32 tv0, tv1;
      tv0 = a.v;
      tv1 = BBE_MULN_4XCF32(tv0, BBE_SELN_4XCF32I(tv0, tv0, BBE_SELI_64B_ROTATE_LEFT_2));
      tv0 = BBE_MULN_4XCF32(tv1, BBE_SELN_4XCF32I(tv1, tv1, BBE_SELI_64B_ROTATE_LEFT_1));
      xtcomplexfloat t0 = BBE_SELSN_4XCF32(tv0, 0);
      return *(xt_reinterpret_cast<complexfloat,xtcomplexfloat>(&t0));
    #elif SIMD_N_4_EQ_8()
      xb_vecN_4xcf32 tv0, tv1;
      tv0 = a.v;
      tv1 = BBE_MULN_4XCF32(tv0, BBE_SELN_4XCF32I(tv0, tv0, BBE_SELI_64B_ROTATE_LEFT_4));
      tv0 = BBE_MULN_4XCF32(tv1, BBE_SELN_4XCF32I(tv1, tv1, BBE_SELI_64B_ROTATE_LEFT_2));
      tv1 = BBE_MULN_4XCF32(tv0, BBE_SELN_4XCF32I(tv0, tv0, BBE_SELI_64B_ROTATE_LEFT_1));
      xtcomplexfloat t0 = BBE_SELSN_4XCF32(tv1, 0);
      return *(xt_reinterpret_cast<complexfloat,xtcomplexfloat>(&t0));
    #elif SIMD_N_4_EQ_16()
      xb_vecN_4xcf32 tv0, tv1;
      tv0 = a.v;
      tv1 = BBE_MULN_4XCF32(tv0, BBE_SELN_4XCF32I(tv0, tv0, BBE_SELI_64B_ROTATE_RIGHT_8));
      tv0 = BBE_MULN_4XCF32(tv1, BBE_SELN_4XCF32I(tv1, tv1, BBE_SELI_64B_ROTATE_LEFT_4));
      tv1 = BBE_MULN_4XCF32(tv0, BBE_SELN_4XCF32I(tv0, tv0, BBE_SELI_64B_ROTATE_LEFT_2));
      tv0 = BBE_MULN_4XCF32(tv1, BBE_SELN_4XCF32I(tv1, tv1, BBE_SELI_64B_ROTATE_LEFT_1));
      xtcomplexfloat t0 = BBE_SELSN_4XCF32(tv0, 0);
      return *(xt_reinterpret_cast<complexfloat,xtcomplexfloat>(&t0));
    #endif
  }
#endif

#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX
  #if SIMD_N_4_EQ_2()
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4cf, CONST_2> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_4xcf32 _var##_num = BBE_CONSTN_4XCF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = kernel.packet[_num].v
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num].v = (_var##_src_num)

      // Declare vectors
      #define DECL_VEC(_var) \
        DR(_var,0); DR(_var,1);

      // Load vectors
      #define LOAD_VEC(_var) \
        LR(_var,0); LR(_var,1);

      // Store vectors
      #define STORE_VEC(_var) \
        SR(_var,0,0); SR(_var,1,1);

      /* This is first step in transpose operation. In first step, input vectors and
       * output vectors have same sequence. This will change in 'NEXT_STEP', where
       * output vectors will have the same sequence but input vector sequence will be
       * different. */
      #define FIRST_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                               \
        BBE_DSELN_4XCF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);     \

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_32B_INTERLEAVE_2, tvs, tvf);
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
  #elif SIMD_N_4_EQ_4()
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4cf, CONST_4> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_4xcf32 _var##_num = BBE_CONSTN_4XCF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = kernel.packet[_num].v
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num].v = (_var##_src_num)

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
  #elif SIMD_N_4_EQ_8()
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4cf, CONST_4> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_4xcf32 _var##_num = BBE_CONSTN_4XCF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = kernel.packet[_num].v
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num].v = (_var##_src_num)

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
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4cf, CONST_8> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_4xcf32 _var##_num = BBE_CONSTN_4XCF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = kernel.packet[_num].v
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num].v = (_var##_src_num)

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
  #elif SIMD_N_4_EQ_16()
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4cf, CONST_4> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_4xcf32 _var##_num = BBE_CONSTN_4XCF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = kernel.packet[_num].v
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num].v = (_var##_src_num)

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
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4cf, CONST_16> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_4xcf32 _var##_num = BBE_CONSTN_4XCF32(0)
      // Load vec
      #define LR(_var,_num) _var##_num = kernel.packet[_num].v
      // Store vec
      #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = PacketN_4cf(_var##_src_num)

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

#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX
/** \internal \returns \a a with real and imaginary part flipped (for complex type only) */
template<> EIGEN_STRONG_INLINE PacketN_4cf pcplxflip<PacketN_4cf>(const PacketN_4cf& x) {
  FUNC_ENTRY;
  return PacketN_4cf(BBE_SHFLN_4XCF32I(x.v, BBE_SHFLI_32B_SWAP_1));
}
#endif

template<> struct conj_helper<PacketN_4cf, PacketN_4cf, false,true> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf& x, const PacketN_4cf& y, const PacketN_4cf& c) const {
    return padd(pmul(x,y),c);
  }

  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& a, const PacketN_4cf& b) const {
    return internal::pmul(a, pconj(b));
  }
};

template<> struct conj_helper<PacketN_4cf, PacketN_4cf, true,false> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf& x, const PacketN_4cf& y, const PacketN_4cf& c) const {
    return padd(pmul(x,y),c);
  }

  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& a, const PacketN_4cf& b) const {
    return internal::pmul(pconj(a), b);
  }
};

template<> struct conj_helper<PacketN_4cf, PacketN_4cf, true,true> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf& x, const PacketN_4cf& y, const PacketN_4cf& c) const {
    return padd(pmul(x,y),c);
  }

  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& a, const PacketN_4cf& b) const {
    return pconj(internal::pmul(a, b));
  }
};

// EIGEN_MAKE_CONJ_HELPER_CPLX_REAL(PacketN_4cf,PacketN_2f)
template <> struct conj_helper<PacketN_2f, PacketN_4cf, false, false> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_2f &x, const PacketN_4cf &y, const PacketN_4cf &c) const {
    return padd(c, pmul(x,y));
  }                                                                                
  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_2f& x, const PacketN_4cf& y) const                        
  { PacketN_2f yy = VEC_XF32_FROM_VEC_XCF32(y.v); return PacketN_4cf(Eigen::internal::pmul<PacketN_2f>(x, yy)); }                                           
};                                                                                                              

template <> struct conj_helper<PacketN_4cf, PacketN_2f, false, false> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf &x, const PacketN_2f &y, const PacketN_4cf &c) const {
    return padd(c, pmul(x,y));
  }                                                                                
  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& x, const PacketN_2f& y) const                        
  { PacketN_2f xx = VEC_XF32_FROM_VEC_XCF32(x.v); return PacketN_4cf(Eigen::internal::pmul<PacketN_2f>(xx, y)); }                                           
};

template<> EIGEN_STRONG_INLINE PacketN_4cf pcmp_eq(const PacketN_4cf& a, const PacketN_4cf& b) {
  FUNC_ENTRY;
  xb_vecN_2x32Uv tv0 = 0x00000000u;
  xb_vecN_2x32Uv tv1 = 0xffffffffu;
  vboolN_2 tb0, tb1, tb2;
  xb_vecN_4xcf32 tva, tvb;
  xb_vecN_2xcf32 tvaa, tvbb;
  xb_vecN_2xf32 tvaa_r, tvaa_i;
  xb_vecN_2xf32 tvbb_r, tvbb_i;
  tva = a.v;
  tvb = b.v;
  tb0 = BBE_OEQN_2XF32(VEC_XF32_FROM_VEC_XCF32(tva),VEC_XF32_FROM_VEC_XCF32(tvb));

  tvaa = BBE_JOINN_2XCF32_FROMN_4XCF32(tva,tva);
  tvbb = BBE_JOINN_2XCF32_FROMN_4XCF32(tvb,tvb);

  tvaa_r = BBE_CREALN_2XCF32(tvaa); tvaa_i = BBE_CIMAGN_2XCF32(tvaa);
  tvbb_r = BBE_CREALN_2XCF32(tvbb); tvbb_i = BBE_CIMAGN_2XCF32(tvbb);

  tvaa = BBE_CMPLXN_2XF32(tvaa_r,tvaa_i);
  tvbb = BBE_CMPLXN_2XF32(tvbb_r,tvbb_i);

  tva = BBE_EXTRACTN_4XCF32_FROMN_2XCF32_V0(tvaa);
  tvb = BBE_EXTRACTN_4XCF32_FROMN_2XCF32_V0(tvbb);

  tb1 = BBE_OEQN_2XF32(VEC_XF32_FROM_VEC_XCF32(tva),VEC_XF32_FROM_VEC_XCF32(tvb));
  tb2 = BBE_ANDBN_2(tb0,tb1);

  return PacketN_4cf(BBE_MOVN_2XF32_FROMN_2X32(BBE_MOVN_2X32UT(tv1, tv0, tb2)));
}

#if (F32_HALF_PACKET_SUPPORT)
// EIGEN_MAKE_CONJ_HELPER_CPLX_REAL(PacketN_4cf,PacketN_4f)
template <> struct conj_helper<PacketN_4f, PacketN_4cf, false, false> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4f &x, const PacketN_4cf &y, const PacketN_4cf &c) const {
    return padd(c, pmul(x,y));
  }                                                                                
  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4f& x, const PacketN_4cf& y) const                        
  { PacketN_4f yy = PacketN_4f(VEC_XF32_FROM_VEC_XCF32(y.v)); return PacketN_4cf(Eigen::internal::pmul<PacketN_4f>(x, yy)); }                                           
};                                                                                                              

template <> struct conj_helper<PacketN_4cf, PacketN_4f, false, false> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf &x, const PacketN_4f &y, const PacketN_4cf &c) const {
    return padd(c, pmul(x,y));
  }                                                                                
  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& x, const PacketN_4f& y) const                        
  { PacketN_4f xx = PacketN_4f(VEC_XF32_FROM_VEC_XCF32(x.v)); return PacketN_4cf(Eigen::internal::pmul<PacketN_4f>(xx, y)); }                                           
};
#endif
} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_CF32_KX_XTENSA_H
