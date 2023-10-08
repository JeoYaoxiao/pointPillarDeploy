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

#ifndef EIGEN_PACKET_MATH_CF64_KX_XTENSA_H
#define EIGEN_PACKET_MATH_CF64_KX_XTENSA_H

#if (XCHAL_HAVE_BBENEP && (SIMD_N_2_EQ_8() || SIMD_N_2_EQ_16() || SIMD_N_2_EQ_32())) || (XCHAL_HAVE_VISION && SIMD_N_2_EQ_32())
  #ifndef CF64_FULL_PACKET_SUPPORT
    #define CF64_FULL_PACKET_SUPPORT 1
  #endif
#else
  #if defined(CF64_FULL_PACKET_SUPPORT) && CF64_FULL_PACKET_SUPPORT
    #pragma message("Incorrect value for CF64_FULL_PACKET_SUPPORT. Resetting it back")
    #undef CF64_FULL_PACKET_SUPPORT
  #endif
  #define CF64_FULL_PACKET_SUPPORT 0
#endif

#if !CF64_FULL_PACKET_SUPPORT
  #if defined(CF64_HALF_PACKET_SUPPORT) && CF64_HALF_PACKET_SUPPORT
    #pragma message("Incorrect value for CF64_HALF_PACKET_SUPPORT. Resetting it back")
    #undef CF64_HALF_PACKET_SUPPORT
    #define CF64_HALF_PACKET_SUPPORT 0
  #else
    #define CF64_HALF_PACKET_SUPPORT 0
  #endif
#endif

#if CF64_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

typedef std::complex<double> complexdouble;

template <> struct is_arithmetic<xb_vecN_8xcf64> {
  enum { value = true };
};

#  if SIMD_N_GE(16,2) && (F64_HALF_PACKET_SUPPORT==1)
#    ifndef CF64_HALF_PACKET_SUPPORT
#      define CF64_HALF_PACKET_SUPPORT 1
#    endif
#  else
#    ifndef CF64_HALF_PACKET_SUPPORT
#      define CF64_HALF_PACKET_SUPPORT 0
#    else
#      if CF64_HALF_PACKET_SUPPORT
#        warning "Incorrect value set for CF64_HALF_PACKET_SUPPORT. Resetting it back"
#        undef CF64_HALF_PACKET_SUPPORT
#        define CF64_HALF_PACKET_SUPPORT 0
#      endif
#    endif
#  endif

struct PacketN_8cd {
  EIGEN_STRONG_INLINE PacketN_8cd() {}
  EIGEN_STRONG_INLINE explicit PacketN_8cd(const PacketN_4d& a) {v = VEC_XCF64_FROM_VEC_XF64(a);}
  EIGEN_STRONG_INLINE explicit PacketN_8cd(PacketN_4d& a) {v = VEC_XCF64_FROM_VEC_XF64(a);}
  #if (F64_HALF_PACKET_SUPPORT)
  EIGEN_STRONG_INLINE explicit PacketN_8cd(const PacketN_8d& a) {v = VEC_XCF64_FROM_VEC_XF64(a.v);}
  #endif // (F64_HALF_PACKET_SUPPORT)
  EIGEN_STRONG_INLINE explicit PacketN_8cd(const xb_vecN_8xcf64& a) : v(a) {}
  EIGEN_STRONG_INLINE explicit PacketN_8cd(xb_vecN_8xcf64& a) : v(a) {}
  EIGEN_STRONG_INLINE operator PacketN_4d() {return VEC_XF64_FROM_VEC_XCF64(v);}
  #if (F64_HALF_PACKET_SUPPORT)
  EIGEN_STRONG_INLINE operator PacketN_8d() {return PacketN_8d(VEC_XF64_FROM_VEC_XCF64(v));}
  #endif // (F64_HALF_PACKET_SUPPORT)
  
  #if defined(BBE_OEQN_8XCF64)
  EIGEN_STRONG_INLINE friend vboolN_8 operator==(const PacketN_8cd& a, const PacketN_8cd& b) {return BBE_OEQN_8XCF64(a.v,b.v);}
  #else
  EIGEN_STRONG_INLINE friend vboolN_8 operator==(const PacketN_8cd& a, const PacketN_8cd& b) {
    #define TT(_v) BBE_SHFLN_4XF64I(_v,BBE_SHFLI_64B_SWAP_1)
    xb_vecN_4xf64 va,vb;
    va = VEC_XF64_FROM_VEC_XCF64(a.v);
    vb = VEC_XF64_FROM_VEC_XCF64(b.v);
    vboolN_4 vb0 = BBE_OEQN_4XF64(va,vb);
    vboolN_4 vb1 = BBE_OEQN_4XF64(TT(va),TT(vb));
    #undef TT
    return BBE_MOVN_8_FROM2N(BBE_MOV2N_FROMN_4(BBE_ANDBN_4(vb0,vb1)));
  }
  #endif // !defined(BBE_OEQN_8XCF64)

  xb_vecN_8xcf64 v;
};

inline std::ostream & operator <<(std::ostream & s, const PacketN_8cd & v) {
  complexdouble *ptr = xt_reinterpret_const_cast<complexdouble,xb_vecN_8xcf64>(&v.v);
  int itr = 0;
  for (itr = 0; itr < (SIMD_N_8-1); itr++)
  {
    s << ptr[itr] << ", ";
  }
  s << ptr[itr];
  return s;
}

#if CF64_HALF_PACKET_SUPPORT
  struct PacketN_16cd {
    EIGEN_STRONG_INLINE PacketN_16cd() {}
    EIGEN_STRONG_INLINE explicit PacketN_16cd(const PacketN_8cd& a) {v.v = a.v;}
    #if (F64_HALF_PACKET_SUPPORT)
    EIGEN_STRONG_INLINE explicit PacketN_16cd(const PacketN_8d& a) {v.v = VEC_XCF64_FROM_VEC_XF64(a.v);}
    #endif // (F64_HALF_PACKET_SUPPORT)
    EIGEN_STRONG_INLINE explicit PacketN_16cd(const xb_vecN_8xcf64& a) {v.v = a;}
    EIGEN_STRONG_INLINE explicit PacketN_16cd(xb_vecN_8xcf64& a) {v.v = a;}
    #if (F64_HALF_PACKET_SUPPORT)
    EIGEN_STRONG_INLINE operator PacketN_8d() {return PacketN_8d(VEC_XF64_FROM_VEC_XCF64(v.v));}
    #endif // (F64_HALF_PACKET_SUPPORT)
    EIGEN_STRONG_INLINE operator PacketN_8cd&() {return v;}
    PacketN_8cd v;
  };

  template <typename Tto, typename Tfrom> EIGEN_STRONG_INLINE Tto extract_N_16cd(const Tfrom& a, const Tfrom& /*b*/) {
    return a;
  }
  template <typename Tto, typename Tfrom> EIGEN_STRONG_INLINE Tto replicate_N_16cd(const Tfrom& a) {
    return a;
  }

  #if XCHAL_HAVE_BBENEP || XCHAL_HAVE_VISION
    template <> EIGEN_STRONG_INLINE PacketN_8cd extract_N_16cd(const PacketN_8cd& a, const PacketN_8cd& b){
      return PacketN_8cd(BBE_SELN_8XCF64I(b.v, a.v, BBE_SELI_64B_EXTRACT_LO_HALVES));
    }
  #endif

inline std::ostream & operator <<(std::ostream & s, const PacketN_16cd & v) {
  complexdouble *ptr = xt_reinterpret_const_cast<complexdouble,xb_vecN_8xcf64>(&v.v.v);
  int itr = 0;
  for (itr = 0; itr < (SIMD_N_16-1); itr++)
  {
    s << ptr[itr] << ", ";
  }
  s << ptr[itr];
  return s;
}

#else
  typedef PacketN_8cd PacketN_16cd;
#endif

template <> struct packet_traits<complexdouble > : default_packet_traits {
  typedef PacketN_8cd type;
  typedef PacketN_16cd half;
  enum {
    Vectorizable    = 1,
    AlignedOnScalar = 1,
    size            = SIMD_N_8,
    #if CF64_HALF_PACKET_SUPPORT
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

#if CF64_HALF_PACKET_SUPPORT
  template <> struct unpacket_traits<PacketN_16cd> {
    typedef complexdouble type;
    typedef PacketN_16cd half;
    enum {size=SIMD_N_16, alignment=ALIGN_N_2, vectorizable=true, masked_load_available=false, masked_store_available=false};
  };
#endif

template <> struct unpacket_traits<PacketN_8cd> {
  typedef complexdouble type;
  typedef PacketN_16cd half;
  enum {size=SIMD_N_8, alignment=ALIGN_N_2, vectorizable=true, masked_load_available=false, masked_store_available=false};
};

template <> EIGEN_STRONG_INLINE PacketN_8cd pset1<PacketN_8cd>(const complexdouble &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtdouble *tp0 = (xt_reinterpret_const_cast<xtdouble,complexdouble >(&from)); 
  xtdouble treal, timag;
  treal = BBE_LSRN_4XF64_I(tp0, 0*sizeof(xtdouble) /* real */);
  timag = BBE_LSRN_4XF64_I(tp0, 1*sizeof(xtdouble) /* imag */);
  xtcomplexdouble tcplx = BBE_CMPLXF64(timag, treal);
  xb_vecN_8xcf64 tvcplx = BBE_LSRN_8XCF64_I(&tcplx, 0);
  return PacketN_8cd(tvcplx);
}

template <> EIGEN_STRONG_INLINE PacketN_8cd padd<PacketN_8cd>(const PacketN_8cd &a, const PacketN_8cd &b) {
  FUNC_ENTRY;
  return PacketN_8cd(a.v + b.v); 
}
template <> EIGEN_STRONG_INLINE PacketN_8cd psub<PacketN_8cd>(const PacketN_8cd &a, const PacketN_8cd &b) {
  FUNC_ENTRY;
  return PacketN_8cd(a.v - b.v); 
}
template <> EIGEN_STRONG_INLINE PacketN_8cd pnegate(const PacketN_8cd &a) {
  FUNC_ENTRY;
  return PacketN_8cd(BBE_NEGN_8XCF64(a.v)); 
}
template <> EIGEN_STRONG_INLINE PacketN_8cd pconj(const PacketN_8cd &a) {
  FUNC_ENTRY;
  return PacketN_8cd(BBE_CONJN_8XCF64(a.v)); 
}
template <> EIGEN_STRONG_INLINE PacketN_8cd pmul<PacketN_8cd>(const PacketN_8cd &a, const PacketN_8cd &b) {
  FUNC_ENTRY;
  return PacketN_8cd(BBE_MULN_8XCF64(a.v, b.v)); 
}
template <> EIGEN_STRONG_INLINE PacketN_8cd pdiv<PacketN_8cd>(const PacketN_8cd &a, const PacketN_8cd &b) {
  FUNC_ENTRY;
  return PacketN_8cd(BBE_DIVN_8XCF64(a.v, b.v)); 
}
template <> EIGEN_STRONG_INLINE PacketN_8cd pand(const PacketN_8cd &a, const PacketN_8cd &b) {
  FUNC_ENTRY;
  return PacketN_8cd(VEC_XCF64_FROM_VEC_X32(BBE_ANDN_2X32(VEC_X32_FROM_VEC_XCF64(a.v), VEC_X32_FROM_VEC_XCF64(b.v))));
}
template <> EIGEN_STRONG_INLINE PacketN_8cd por(const PacketN_8cd &a, const PacketN_8cd &b) {
  FUNC_ENTRY;
  return PacketN_8cd(VEC_XCF64_FROM_VEC_X32(BBE_ORN_2X32(VEC_X32_FROM_VEC_XCF64(a.v), VEC_X32_FROM_VEC_XCF64(b.v))));
}
template <> EIGEN_STRONG_INLINE PacketN_8cd pxor(const PacketN_8cd &a, const PacketN_8cd &b) {
  FUNC_ENTRY;
  return PacketN_8cd(VEC_XCF64_FROM_VEC_X32(BBE_XORN_2X32(VEC_X32_FROM_VEC_XCF64(a.v), VEC_X32_FROM_VEC_XCF64(b.v))));
}
#if XCHAL_HAVE_BBENEP
  template <> EIGEN_STRONG_INLINE PacketN_8cd pandnot(const PacketN_8cd &a, const PacketN_8cd &b) {
    FUNC_ENTRY;
    return PacketN_8cd(VEC_XCF64_FROM_VEC_X32(BBE_ANDNOTN_2X32(VEC_X32_FROM_VEC_XCF64(a.v), VEC_X32_FROM_VEC_XCF64(b.v))));
  }
#endif

template <> EIGEN_STRONG_INLINE PacketN_8cd pload<PacketN_8cd>(const complexdouble *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  return *(xt_reinterpret_const_cast<PacketN_8cd,complexdouble >(from));
}

template <> EIGEN_STRONG_INLINE PacketN_8cd ploadu<PacketN_8cd>(const complexdouble *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_LOAD;
  xb_vecN_8xcf64     tv0;
  valign         talign;
  xb_vecN_8xcf64 *tpv0 = xt_reinterpret_const_cast<xb_vecN_8xcf64,complexdouble >(from);
  talign              = BBE_LAN_8XCF64_PP(tpv0);
  BBE_LAN_8XCF64_IP(tv0, talign, tpv0);
  return PacketN_8cd(tv0);
}

#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX || XCHAL_HAVE_VISION
  /** \internal \returns a packet with elements of \a *from duplicated.
   * For instance, for a packet of 8 elements, 4 scalars will be read from \a *from and
   * duplicated to form: {from[0],from[0],from[1],from[1],from[2],from[2],from[3],from[3]}
   * Currently, this function is only used for scalar * complex products.
   */
  template <> EIGEN_STRONG_INLINE PacketN_8cd ploaddup<PacketN_8cd>(const complexdouble *from) {
    FUNC_ENTRY;
    EIGEN_DEBUG_ALIGNED_LOAD;
    #if SIMD_N_2_EQ_8()
    return pset1<PacketN_8cd>(*from);
    #elif SIMD_N_2_EQ_16()
    PacketN_8cd tv0 = ploadu<PacketN_8cd>(from);
    xb_vecN_8xcf64 tv1 = (BBE_SELN_8XCF64I(tv0.v, tv0.v, BBE_SELI_MMC2X4X4X4_M2_STEP_1));
    xb_vecN_8xcf64 tvzero = BBE_CONSTN_8XCF64(0);
    xb_vecN_8xcf64 tv2 = (BBE_SELN_8XCF64I(tvzero, tv0.v, BBE_SELI_64B_ROTATE_RIGHT_2));
    xb_vecN_8xcf64 tv3 = (BBE_SELN_8XCF64I(tv2, tv2, BBE_SELI_MMC2X4X4X4_M2_STEP_1));
    xb_vecN_8xcf64 tv4 = (BBE_SELN_8XCF64I(tv3, tv1, BBE_SELI_64B_ROTATE_RIGHT_4));
    return PacketN_8cd(tv4);
    #elif SIMD_N_2_EQ_32()
    PacketN_8cd tv0 = ploadu<PacketN_8cd>(from);
    xb_vecN_8xcf64 tv1 = (BBE_SELN_8XCF64I(tv0.v, tv0.v, BBE_SELI_64B_INTERLEAVE_1_EVEN));
    xb_vecN_8xcf64 tv2 = (BBE_SELN_8XCF64I(tv0.v, tv0.v, BBE_SELI_64B_INTERLEAVE_1_ODD));
    xb_vecN_8xcf64 tv3 = (BBE_SELN_8XCF64I(tv2, tv1, BBE_SELI_64B_INTERLEAVE_1_LO));
    return PacketN_8cd(tv3);
    #endif
  }
#endif

#if SIMD_N_4_EQ_8() || SIMD_N_4_EQ_16()
  #if XCHAL_HAVE_BBENEP
    /** \internal \returns a packet with elements of \a *from quadrupled.
      * For instance, for a packet of 8 elements, 2 scalars will be read from \a *from and
      * replicated to form: {from[0],from[0],from[0],from[0],from[1],from[1],from[1],from[1]}
      * Currently, this function is only used in matrix products.
      * For packet-size smaller or equal to 4, this function is equivalent to pload1 
      */
    template <> EIGEN_STRONG_INLINE PacketN_8cd ploadquad<PacketN_8cd>(const complexdouble *from) {
      FUNC_ENTRY;
      EIGEN_DEBUG_ALIGNED_LOAD;
      #if SIMD_N_2_EQ_16()
      PacketN_8cd tv0 = ploadu<PacketN_8cd>(from);
      xb_vecN_8xcf64 tv1 = BBE_SELN_8XCF64I(tv0.v, tv0.v, BBE_SELI_MMC2X4X4X4_M2_STEP_1);
      return PacketN_8cd(tv1);
      #elif SIMD_N_2_EQ_32()
      PacketN_8cd tv0 = ploadu<PacketN_8cd>(from);
      xb_vecN_8xcf64 tv1 = BBE_CONSTN_8XCF64(0);
      xb_vecN_8xcf64 tv2 = BBE_CONSTN_8XCF64(0);
      xb_vecN_8xcf64 tv3 = BBE_CONSTN_8XCF64(0);
      xb_vecN_8xcf64 tv4 = BBE_CONSTN_8XCF64(0);
      BBE_DSELN_8XCF64I(tv2, tv1, tv0.v, tv0.v, BBE_DSELI_64B_INTERLEAVE_2);
      BBE_DSELN_8XCF64I(tv4, tv3, tv1, tv1, BBE_DSELI_64B_INTERLEAVE_2);
      return PacketN_8cd(tv3);
      #endif
    }
  #endif
#endif

template <> EIGEN_STRONG_INLINE void pstore<complexdouble >(complexdouble *to, const PacketN_8cd &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_STORE;
  *(xt_reinterpret_cast<PacketN_8cd,complexdouble >(to)) = from;
}

template <> EIGEN_STRONG_INLINE void pstoreu<complexdouble >(complexdouble *to, const PacketN_8cd &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_STORE;
  valign         talign;
  xb_vecN_8xcf64 *tpv0 = xt_reinterpret_cast<xb_vecN_8xcf64,complexdouble>(to);
  talign           = BBE_ZALIGN();
  BBE_SAN_8XCF64_IP(from.v, talign, tpv0);
  BBE_SAN_8XCF64POS_FP(talign, tpv0);
}

#if XCHAL_HAVE_BBENEP
  template <> EIGEN_DEVICE_FUNC inline PacketN_8cd pgather<complexdouble, PacketN_8cd>(const complexdouble *from, Index stride) {
    FUNC_ENTRY;
    EIGEN_DEBUG_ALIGNED_LOAD;
    xtcomplexdouble *tp = xt_reinterpret_const_cast<xtcomplexdouble,complexdouble>(from);
    xb_vecN_8xcf64 ta;
    xtcomplexdouble *tpf = xt_reinterpret_cast<xtcomplexdouble,xb_vecN_8xcf64>(&ta);
    int    i;
    int    stride_bytes = sizeof(xtcomplexdouble) * stride;
    xtcomplexdouble tvf;

    for (i = 0; i < SIMD_N_8; i++) {
      BBE_LSN_8XCF64_XP(tvf, tp, stride_bytes);
      BBE_SSN_8XCF64_IP(BBE_MOVN_8XCF64_FROMCF64(tvf), tpf, sizeof(xtcomplexdouble));
    }
    return PacketN_8cd(ta);
  }
#endif

#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX
  template <> EIGEN_DEVICE_FUNC inline void pscatter<complexdouble, PacketN_8cd>(complexdouble *to, const PacketN_8cd &from, Index stride) {
    FUNC_ENTRY;
    EIGEN_DEBUG_ALIGNED_LOAD;
    int i;
    xtcomplexdouble *tpf = xt_reinterpret_const_cast<xtcomplexdouble,PacketN_8cd>(&from);
    xtcomplexdouble *tpt = xt_reinterpret_cast<xtcomplexdouble,complexdouble>(to);
    int    stride_bytes = sizeof(xtcomplexdouble) * stride;
    xtcomplexdouble tvf;
    for (i = 0; i < SIMD_N_8; i++)
    {
      BBE_LSN_8XCF64_IP(tvf, tpf, sizeof(xtcomplexdouble));
      BBE_SSN_8XCF64_XP(BBE_MOVN_8XCF64_FROMCF64(tvf), tpt, stride_bytes);
    }
  }
#endif

template <> EIGEN_STRONG_INLINE complexdouble pfirst<PacketN_8cd>(const PacketN_8cd &a) {
  FUNC_ENTRY;
  xtcomplexdouble t0 = BBE_SELSN_8XCF64(a.v, 0 /*Start element*/); 
  return *(xt_reinterpret_cast<complexdouble,xtcomplexdouble>(&t0));
}
#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX || XCHAL_HAVE_VISION
  template <> EIGEN_STRONG_INLINE PacketN_8cd preverse(const PacketN_8cd &a) {
    FUNC_ENTRY;
    #if SIMD_N_2_EQ_8()
    xb_vecN_8xcf64 tv1 = (BBE_SHFLN_8XCF64I(a.v, BBE_SHFLI_128B_SWAP_1));
    return PacketN_8cd(tv1); 
    #elif SIMD_N_2_EQ_16()
    xb_vecN_8xcf64 tv1 = (BBE_SHFLN_8XCF64I(a.v, BBE_SHFLI_128B_REVERSE_1)); 
    xb_vecN_8xcf64 tv2 = (BBE_SELN_8XCF64I(tv1, tv1, BBE_SELI_64B_ROTATE_RIGHT_4));
    return PacketN_8cd(tv2); 
    #elif SIMD_N_2_EQ_32()
    xb_vecN_8xcf64 tv1 = (BBE_SHFLN_8XCF64I(a.v, BBE_SHFLI_128B_REVERSE_1));
    xb_vecN_8xcf64 tv2 = (BBE_SHFLN_8XCF64I(tv1, BBE_SHFLI_128B_SWAP_2));
    xb_vecN_8xcf64 tv3 = (BBE_SHFLN_8XCF64I(tv2, BBE_SHFLI_128B_SWAP_4));
    return PacketN_8cd(tv3); 
    #endif
  }
#endif

template <> EIGEN_STRONG_INLINE complexdouble predux<PacketN_8cd>(const PacketN_8cd &a) {
  FUNC_ENTRY;
  xtcomplexdouble t0 = BBE_RADDN_8XCF64(a.v);
  return *(xt_reinterpret_cast<complexdouble,xtcomplexdouble>(&t0));
}

#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_VISION
  #if SIMD_N_4_EQ_16()
    /** \internal \returns the sum of the elements of \a a by block of 4 elements.
      * For a packet {a0, a1, a2, a3, a4, a5, a6, a7}, it returns a half packet {a0+a4, a1+a5, a2+a6, a3+a7}
      * For packet-size smaller or equal to 4, this boils down to a noop.
      */
    template<> EIGEN_DEVICE_FUNC inline PacketN_16cd predux_half_dowto4(const PacketN_8cd& a) {
      FUNC_ENTRY;
        xb_vecN_8xcf64 tv0, tv1, tv2;
        vboolN_8 tvb;
        tvb = BBE_LTRN_8I(SIMD_N_8/2);
        tv0 = a.v;
        tv1 = BBE_CONSTN_8XCF64(0);
        tv2 = BBE_SELN_8XCF64I(tv1, tv0, BBE_SELI_64B_ROTATE_RIGHT_8);
        tv2 = tv0 + tv2;
        tv0 = BBE_MOVN_8XCF64T(tv2, tv1, tvb);
        return PacketN_16cd(tv0);
    }
  #endif
#endif

#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX || XCHAL_HAVE_VISION
  /** \internal \returns the product of the elements of \a a*/
  template <> EIGEN_STRONG_INLINE complexdouble predux_mul<PacketN_8cd>(const PacketN_8cd &a) {
    FUNC_ENTRY;
    #if SIMD_N_2_EQ_8()
      xb_vecN_8xcf64 tv0, tv1;
      tv0 = a.v;
      tv1 = BBE_MULN_8XCF64(tv0, BBE_SELN_8XCF64I(tv0, tv0, BBE_SELI_64B_ROTATE_LEFT_2));
      xtcomplexdouble t0 = BBE_SELSN_8XCF64(tv1, 0);
      return *(xt_reinterpret_cast<complexdouble,xtcomplexdouble>(&t0));
    #elif SIMD_N_2_EQ_16()
      xb_vecN_8xcf64 tv0, tv1;
      tv0 = a.v;
      tv1 = BBE_MULN_8XCF64(tv0, BBE_SELN_8XCF64I(tv0, tv0, BBE_SELI_64B_ROTATE_LEFT_4));
      tv0 = BBE_MULN_8XCF64(tv1, BBE_SELN_8XCF64I(tv1, tv1, BBE_SELI_64B_ROTATE_LEFT_2));
      xtcomplexdouble t0 = BBE_SELSN_8XCF64(tv0, 0);
      return *(xt_reinterpret_cast<complexdouble,xtcomplexdouble>(&t0));
    #elif SIMD_N_2_EQ_32()
      xb_vecN_8xcf64 tv0, tv1;
      tv0 = a.v;
      tv1 = BBE_MULN_8XCF64(tv0, BBE_SELN_8XCF64I(tv0, tv0, BBE_SELI_64B_ROTATE_RIGHT_8));
      tv0 = BBE_MULN_8XCF64(tv1, BBE_SELN_8XCF64I(tv1, tv1, BBE_SELI_64B_ROTATE_LEFT_4));
      tv1 = BBE_MULN_8XCF64(tv0, BBE_SELN_8XCF64I(tv0, tv0, BBE_SELI_64B_ROTATE_LEFT_2));
      xtcomplexdouble t0 = BBE_SELSN_8XCF64(tv1, 0);
      return *(xt_reinterpret_cast<complexdouble,xtcomplexdouble>(&t0));
    #endif
  }
#endif

#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX
  #if SIMD_N_4_EQ_4()
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8cd, CONST_2> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_8xcf64 _var##_num = BBE_CONSTN_8XCF64(0)
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
        BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                               \
        BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##2, _inp##0, _pat);     \

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_64B_INTERLEAVE_2, tvs, tvf);
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
  #elif SIMD_N_4_EQ_8()
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8cd, CONST_4> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_8xcf64 _var##_num = BBE_CONSTN_8XCF64(0)
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
        BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
        BBE_DSELN_8XCF64I(_out##3, _out##2, _inp##3, _inp##2, _pat);     \

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                               \
        BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        BBE_DSELN_8XCF64I(_out##3, _out##2, _inp##3, _inp##1, _pat);    \

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_64B_INTERLEAVE_2, tvs, tvf);
      NEXT_STEP(BBE_DSELI_64B_INTERLEAVE_4, tvf, tvs);
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
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8cd, CONST_8> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_8xcf64 _var##_num = BBE_CONSTN_8XCF64(0)
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
        BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
        BBE_DSELN_8XCF64I(_out##3, _out##2, _inp##3, _inp##2, _pat);     \
        BBE_DSELN_8XCF64I(_out##5, _out##4, _inp##5, _inp##4, _pat);     \
        BBE_DSELN_8XCF64I(_out##7, _out##6, _inp##7, _inp##6, _pat);     \

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                               \
        BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        BBE_DSELN_8XCF64I(_out##3, _out##2, _inp##6, _inp##4, _pat);    \
        BBE_DSELN_8XCF64I(_out##5, _out##4, _inp##3, _inp##1, _pat);   \
        BBE_DSELN_8XCF64I(_out##7, _out##6, _inp##7, _inp##5, _pat);  \

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
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8cd, CONST_4> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_8xcf64 _var##_num = BBE_CONSTN_8XCF64(0)
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
        BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
        BBE_DSELN_8XCF64I(_out##3, _out##2, _inp##3, _inp##2, _pat);     \

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                               \
        BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        BBE_DSELN_8XCF64I(_out##3, _out##2, _inp##3, _inp##1, _pat);    \

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_64B_INTERLEAVE_2, tvs, tvf);
      NEXT_STEP(BBE_DSELI_64B_INTERLEAVE_4, tvf, tvs);
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
    EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8cd, CONST_8> &kernel) {
      FUNC_ENTRY;
      // Declare vec
      #define DR(_var,_num) xb_vecN_8xcf64 _var##_num = BBE_CONSTN_8XCF64(0)
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
        BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
        BBE_DSELN_8XCF64I(_out##3, _out##2, _inp##3, _inp##2, _pat);     \
        BBE_DSELN_8XCF64I(_out##5, _out##4, _inp##5, _inp##4, _pat);     \
        BBE_DSELN_8XCF64I(_out##7, _out##6, _inp##7, _inp##6, _pat);     \

      /* This is subsequent step in transpose operation. The only difference between
       * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
       * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
      #define NEXT_STEP(_pat, _out, _inp)                              \
        BBE_DSELN_8XCF64I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        BBE_DSELN_8XCF64I(_out##3, _out##2, _inp##6, _inp##4, _pat);    \
        BBE_DSELN_8XCF64I(_out##5, _out##4, _inp##3, _inp##1, _pat);   \
        BBE_DSELN_8XCF64I(_out##7, _out##6, _inp##7, _inp##5, _pat);  \

      DECL_VEC(tvf);
      DECL_VEC(tvs);
      LOAD_VEC(tvf);
      FIRST_STEP(BBE_DSELI_64B_INTERLEAVE_2, tvs, tvf);
      NEXT_STEP(BBE_DSELI_64B_INTERLEAVE_4, tvf, tvs);
      NEXT_STEP(BBE_DSELI_64B_INTERLEAVE_8, tvs, tvf);
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

#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_PDX || XCHAL_HAVE_VISION
/** \internal \returns \a a with real and imaginary part flipped (for complex type only) */
template<> EIGEN_STRONG_INLINE PacketN_8cd pcplxflip<PacketN_8cd>(const PacketN_8cd& x) {
  FUNC_ENTRY;
  return PacketN_8cd(BBE_SHFLN_8XCF64I(x.v, BBE_SHFLI_64B_SWAP_1));
}
#endif

template<> struct conj_helper<PacketN_8cd, PacketN_8cd, false,true> {
  EIGEN_STRONG_INLINE PacketN_8cd pmadd(const PacketN_8cd& x, const PacketN_8cd& y, const PacketN_8cd& c) const {
    return padd(pmul(x,y),c);
  }

  EIGEN_STRONG_INLINE PacketN_8cd pmul(const PacketN_8cd& a, const PacketN_8cd& b) const {
    return internal::pmul(a, pconj(b));
  }
};

template<> struct conj_helper<PacketN_8cd, PacketN_8cd, true,false> {
  EIGEN_STRONG_INLINE PacketN_8cd pmadd(const PacketN_8cd& x, const PacketN_8cd& y, const PacketN_8cd& c) const {
    return padd(pmul(x,y),c);
  }

  EIGEN_STRONG_INLINE PacketN_8cd pmul(const PacketN_8cd& a, const PacketN_8cd& b) const {
    return internal::pmul(pconj(a), b);
  }
};

template<> struct conj_helper<PacketN_8cd, PacketN_8cd, true,true> {
  EIGEN_STRONG_INLINE PacketN_8cd pmadd(const PacketN_8cd& x, const PacketN_8cd& y, const PacketN_8cd& c) const {
    return padd(pmul(x,y),c);
  }

  EIGEN_STRONG_INLINE PacketN_8cd pmul(const PacketN_8cd& a, const PacketN_8cd& b) const {
    return pconj(internal::pmul(a, b));
  }
};

// EIGEN_MAKE_CONJ_HELPER_CPLX_REAL(PacketN_8cd,PacketN_4d)
template <> struct conj_helper<PacketN_4d, PacketN_8cd, false, false> {
  EIGEN_STRONG_INLINE PacketN_8cd pmadd(const PacketN_4d &x, const PacketN_8cd &y, const PacketN_8cd &c) const {
    return padd(c, pmul(x,y));
  }                                                                                
  EIGEN_STRONG_INLINE PacketN_8cd pmul(const PacketN_4d& x, const PacketN_8cd& y) const
  { PacketN_4d yy = VEC_XF64_FROM_VEC_XCF64(y.v); return PacketN_8cd(Eigen::internal::pmul<PacketN_4d>(x, yy)); }                                           
};                                                                                                              

template <> struct conj_helper<PacketN_8cd, PacketN_4d, false, false> {
  EIGEN_STRONG_INLINE PacketN_8cd pmadd(const PacketN_8cd &x, const PacketN_4d &y, const PacketN_8cd &c) const {
    return padd(c, pmul(x,y));
  }                                                                                
  EIGEN_STRONG_INLINE PacketN_8cd pmul(const PacketN_8cd& x, const PacketN_4d& y) const                        
  { PacketN_4d xx = VEC_XF64_FROM_VEC_XCF64(x.v); return PacketN_8cd(Eigen::internal::pmul<PacketN_4d>(xx, y)); }                                           
};

template<> EIGEN_STRONG_INLINE PacketN_8cd pcmp_eq(const PacketN_8cd& a, const PacketN_8cd& b) {
  FUNC_ENTRY;
  xb_vecN_4x64 tv0 = 0x0000000000000000u;
  xb_vecN_4x64 tv1 = 0xffffffffffffffffu;
  vboolN_4 tb0, tb1, tb2;
  xb_vecN_8xcf64 tva, tvb;
  xb_vecN_4xcf64 tvaa, tvbb;
  xb_vecN_4xf64 tvaa_r, tvaa_i;
  xb_vecN_4xf64 tvbb_r, tvbb_i;
  tva = a.v;
  tvb = b.v;
  tb0 = BBE_OEQN_4XF64(VEC_XF64_FROM_VEC_XCF64(tva),VEC_XF64_FROM_VEC_XCF64(tvb));

  tvaa = BBE_JOINN_4XCF64_FROMN_8XCF64(tva,tva);
  tvbb = BBE_JOINN_4XCF64_FROMN_8XCF64(tvb,tvb);

  tvaa_r = BBE_CREALN_4XCF64(tvaa); tvaa_i = BBE_CIMAGN_4XCF64(tvaa);
  tvbb_r = BBE_CREALN_4XCF64(tvbb); tvbb_i = BBE_CIMAGN_4XCF64(tvbb);

  tvaa = BBE_CMPLXN_4XF64(tvaa_r,tvaa_i);
  tvbb = BBE_CMPLXN_4XF64(tvbb_r,tvbb_i);

  tva = BBE_EXTRACTN_8XCF64_FROMN_4XCF64_V0(tvaa);
  tvb = BBE_EXTRACTN_8XCF64_FROMN_4XCF64_V0(tvbb);

  tb1 = BBE_OEQN_4XF64(VEC_XF64_FROM_VEC_XCF64(tva),VEC_XF64_FROM_VEC_XCF64(tvb));
  tb2 = BBE_ANDBN_4(tb0,tb1);

  return PacketN_8cd(BBE_MOVN_4XF64_FROMN_4X64(BBE_MOVN_4X64UT(tv1, tv0, tb2)));
}

#if (F64_HALF_PACKET_SUPPORT)
// EIGEN_MAKE_CONJ_HELPER_CPLX_REAL(PacketN_8cd,PacketN_8d)
template <> struct conj_helper<PacketN_8d, PacketN_8cd, false, false> {
  EIGEN_STRONG_INLINE PacketN_8cd pmadd(const PacketN_8d &x, const PacketN_8cd &y, const PacketN_8cd &c) const {
    return padd(c, pmul(x,y));
  }                                                                                
  EIGEN_STRONG_INLINE PacketN_8cd pmul(const PacketN_8d& x, const PacketN_8cd& y) const                        
  { PacketN_8d yy = PacketN_8d(VEC_XF64_FROM_VEC_XCF64(y.v)); return PacketN_8cd(Eigen::internal::pmul<PacketN_8d>(x, yy)); }                                           
};                                                                                                              

template <> struct conj_helper<PacketN_8cd, PacketN_8d, false, false> {
  EIGEN_STRONG_INLINE PacketN_8cd pmadd(const PacketN_8cd &x, const PacketN_8d &y, const PacketN_8cd &c) const {
    return padd(c, pmul(x,y));
  }                                                                                
  EIGEN_STRONG_INLINE PacketN_8cd pmul(const PacketN_8cd& x, const PacketN_8d& y) const                        
  { PacketN_8d xx = PacketN_8d(VEC_XF64_FROM_VEC_XCF64(x.v)); return PacketN_8cd(Eigen::internal::pmul<PacketN_8d>(xx, y)); }                                           
};
#endif
} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_CF64_KX_XTENSA_H
