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

#ifndef EIGEN_PACKET_MATH_CF32_HIFI1_1_XTENSA_H
#define EIGEN_PACKET_MATH_CF32_HIFI1_1_XTENSA_H

#if SIMD_N_4_EQ_1()
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

#if CF32_FULL_PACKET_SUPPORT
namespace Eigen {

namespace internal {

typedef std::complex<float> complexfloat;

template <> struct is_arithmetic<xtcomplexfloat> {
  enum { value = true };
};

#ifndef CF32_HALF_PACKET_SUPPORT
#define CF32_HALF_PACKET_SUPPORT 0
#endif
#if CF32_HALF_PACKET_SUPPORT
#  pragma message("Incorrect value set for CF32_HALF_PACKET_SUPPORT. Resetting it back")
#  undef CF32_HALF_PACKET_SUPPORT
#  define CF32_HALF_PACKET_SUPPORT 0
#endif

struct PacketN_4cf {
  EIGEN_STRONG_INLINE PacketN_4cf() {}
  EIGEN_STRONG_INLINE explicit PacketN_4cf(const PacketN_2f& a) {v =AE_MOVXTCOMPLEXFLOAT_FROMXTFLOATX2(a);}
  EIGEN_STRONG_INLINE explicit PacketN_4cf(xtfloatx2& a) {v = AE_MOVXTCOMPLEXFLOAT_FROMXTFLOATX2(a);}

  EIGEN_STRONG_INLINE explicit PacketN_4cf(const xtcomplexfloat& a) : v(a) {}
  EIGEN_STRONG_INLINE explicit PacketN_4cf(xtcomplexfloat& a) : v(a) {}
  EIGEN_STRONG_INLINE operator PacketN_2f() {return AE_MOVXTFLOATX2_FROMXTCOMPLEXFLOAT(v);}

  #ifdef __XTENSA__
  EIGEN_STRONG_INLINE friend xtbool2 operator==(const PacketN_4cf& a, const PacketN_4cf& b) {
    return (a.v == b.v) ;
  }
  #else
  EIGEN_STRONG_INLINE friend bool operator==(const PacketN_4cf& a, const PacketN_4cf& b) {
    xtbool vb1 = OEQ_CS(a.v, b.v);
	unsigned int ar = AE_MOVAB1(vb1);
	bool bl1 = ar;
	return bl1;
  }
  #endif
  xtcomplexfloat v;
};


typedef PacketN_4cf PacketN_8cf;

template <> struct packet_traits<complexfloat > : default_packet_traits {
  typedef PacketN_4cf type;
  typedef PacketN_8cf half;
  enum {
    Vectorizable    = 0,
    AlignedOnScalar = 0,
    size            = SIMD_N_4,
    HasHalfPacket   = 0,
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

template <> struct unpacket_traits<PacketN_4cf> {
  typedef complexfloat type;
  typedef PacketN_8cf half;
  enum {size=SIMD_N_4, alignment=ALIGN_N_2, vectorizable=true, masked_load_available=false, masked_store_available=false};
};

template <> EIGEN_STRONG_INLINE PacketN_4cf pset1<PacketN_4cf>(const complexfloat &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtcomplexfloat out = CMPLX_CS(from.imag(), from.real());
  return PacketN_4cf(out);
}

template <> EIGEN_STRONG_INLINE PacketN_4cf padd<PacketN_4cf>(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  #ifdef __XTENSA__
  return PacketN_4cf(a.v + b.v); 
  #else
  return PacketN_4cf(ADD_CS(a.v, b.v)); 
  #endif
}
template <> EIGEN_STRONG_INLINE PacketN_4cf psub<PacketN_4cf>(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  #ifdef __XTENSA__
  return PacketN_4cf(a.v - b.v); 
  #else
  return PacketN_4cf(SUB_CS(a.v, b.v)); 
  #endif
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pnegate(const PacketN_4cf &a) {
  FUNC_ENTRY;
  #ifdef __XTENSA__
  return PacketN_4cf(-a.v); 
  #else
  return PacketN_4cf(NEG_CS(a.v)); 
  #endif
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pconj(const PacketN_4cf &a) {
  FUNC_ENTRY;
  return PacketN_4cf(CONJC_CS(a.v)); 
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pmul<PacketN_4cf>(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  #ifdef __XTENSA__
  return PacketN_4cf(a.v * b.v); 
  #else
  return PacketN_4cf(MULC_CS(a.v, b.v)); 
  #endif
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pmadd<PacketN_4cf>(const PacketN_4cf &a, const PacketN_4cf &b, const PacketN_4cf &c) {
  FUNC_ENTRY;
  #ifdef __XTENSA__
  return PacketN_4cf((a.v * b.v) + c.v); 
  #else
  return PacketN_4cf(ADD_CS(MULC_CS(a.v, b.v), c.v)); 
  #endif
}

template <> EIGEN_STRONG_INLINE PacketN_4cf pdiv<PacketN_4cf>(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  #ifdef __XTENSA__
  return PacketN_4cf((a.v)/(b.v));
  #else
  return PacketN_4cf(DIV_CS(a.v, b.v));
  #endif
}

template <> EIGEN_STRONG_INLINE PacketN_4cf pand(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  ae_int32x2 ai = AE_MOVINT32X2_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMXTCOMPLEXFLOAT(a.v));
  ae_int32x2 bi = AE_MOVINT32X2_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMXTCOMPLEXFLOAT(b.v));
  ai = AE_INT32X2_AND_INT32X2(ai, bi);
  xtcomplexfloat out = AE_MOVXTCOMPLEXFLOAT_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMINT32X2(ai));
  return PacketN_4cf(out);
}
template <> EIGEN_STRONG_INLINE PacketN_4cf por(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  ae_int32x2 ai = AE_MOVINT32X2_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMXTCOMPLEXFLOAT(a.v));
  ae_int32x2 bi = AE_MOVINT32X2_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMXTCOMPLEXFLOAT(b.v));
  ai = AE_INT32X2_OR_INT32X2(ai, bi);
  xtcomplexfloat out = AE_MOVXTCOMPLEXFLOAT_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMINT32X2(ai));
  return PacketN_4cf(out);
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pxor(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  ae_int32x2 ai = AE_MOVINT32X2_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMXTCOMPLEXFLOAT(a.v));
  ae_int32x2 bi = AE_MOVINT32X2_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMXTCOMPLEXFLOAT(b.v));
  ai = AE_INT32X2_XOR_INT32X2(ai, bi);
  xtcomplexfloat out = AE_MOVXTCOMPLEXFLOAT_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMINT32X2(ai));
  return PacketN_4cf(out);
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pandnot(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  ae_int32x2 ai = AE_MOVINT32X2_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMXTCOMPLEXFLOAT(a.v));
  ae_int32x2 bi = AE_MOVINT32X2_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMXTCOMPLEXFLOAT(b.v));
  bi = AE_INT32X2_XOR_INT32X2(bi, bi);
  ai = AE_INT32X2_AND_INT32X2(ai, bi);
  xtcomplexfloat out = AE_MOVXTCOMPLEXFLOAT_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMINT32X2(ai));
  return PacketN_4cf(out);
}

template <> EIGEN_STRONG_INLINE PacketN_4cf pload<PacketN_4cf>(const complexfloat *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;

  xtfloat *tp0 = (xt_reinterpret_const_cast<xtfloat,complexfloat >(from)); 
  xtfloat treal, timag;
  treal = AE_LSI(tp0, 0*sizeof(xtfloat) /* real */);
  timag = AE_LSI(tp0, 1*sizeof(xtfloat) /* imag */);
  xtcomplexfloat out = CMPLX_CS(timag, treal); 
  return PacketN_4cf(out);
}

template <> EIGEN_STRONG_INLINE PacketN_4cf ploadu<PacketN_4cf>(const complexfloat *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;

  xtfloat *tp0 = (xt_reinterpret_const_cast<xtfloat,complexfloat >(from)); 
  xtfloat treal, timag;
  treal = AE_LSI(tp0, 0*sizeof(xtfloat) /* real */);
  timag = AE_LSI(tp0, 1*sizeof(xtfloat) /* imag */);
  xtcomplexfloat out = CMPLX_CS(timag, treal); 
  return PacketN_4cf(out);
}

/*
template <> EIGEN_STRONG_INLINE PacketN_4cf ploadu<PacketN_4cf>(const complexfloat *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_LOAD;
  xtfloatx2     tv0;
  ae_valign         talign;
  xtfloatx2 *tpv0 = xt_reinterpret_const_cast<xtfloatx2,complexfloat >(from);
  talign              = AE_LASX2PP(tpv0);
  AE_LASX2IP(tv0, talign, tpv0);
  return PacketN_4cf(AE_MOVXTCOMPLEXFLOAT_FROMXTFLOATX2(tv0));
}
*/

  /** \internal \returns a packet with elements of \a *from duplicated.
   * For instance, for a packet of 8 elements, 4 scalars will be read from \a *from and
   * duplicated to form: {from[0],from[0],from[1],from[1],from[2],from[2],from[3],from[3]}
   * Currently, this function is only used for scalar * complex products.
   */
template <> EIGEN_STRONG_INLINE PacketN_4cf ploaddup<PacketN_4cf>(const complexfloat *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtfloat *from_x = xt_reinterpret_const_cast<xtfloat, complexfloat>(from);
  xtfloat tv0 = from_x[0];
  xtfloat tv1 = from_x[1];
  xtcomplexfloat ctv = CMPLX_CS(tv1, tv0);
  return PacketN_4cf(ctv);
}

template <> EIGEN_STRONG_INLINE void pstore<complexfloat >(complexfloat *to, const PacketN_4cf &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_STORE;
  xtfloat *to_b = xt_reinterpret_cast<xtfloat, complexfloat>(to);
  to_b[0] = CREAL_CS(from.v);
  to_b[1] = CIMAG_CS(from.v);
}

template <> EIGEN_STRONG_INLINE void pstoreu<complexfloat >(complexfloat *to, const PacketN_4cf &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_STORE;
  xtfloat *from_s = xt_reinterpret_const_cast<xtfloat, xtcomplexfloat>(&(from.v));
  xtfloat *to_s = xt_reinterpret_cast<xtfloat, complexfloat>(to);
  to_s[0] = from_s[0];
  to_s[1] = from_s[1];
}

template <> EIGEN_DEVICE_FUNC inline PacketN_4cf pgather<complexfloat, PacketN_4cf>(const complexfloat *from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtcomplexfloat *tp = xt_reinterpret_const_cast<xtcomplexfloat,complexfloat>(from);
  xtcomplexfloat tv0; 
  tv0 = xtcomplexfloat_loadx(tp, 0 * sizeof(xtcomplexfloat) * stride);
  return PacketN_4cf(tv0);
}

template <> EIGEN_DEVICE_FUNC inline void pscatter<complexfloat, PacketN_4cf>(complexfloat *to, const PacketN_4cf &from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtcomplexfloat *tpt = xt_reinterpret_cast<xtcomplexfloat,complexfloat>(to);
  tpt[stride * 0] = from.v;
}

inline std::ostream & operator <<(std::ostream & s, const PacketN_4cf & v) {
  complexfloat *ptr = xt_reinterpret_const_cast<complexfloat,xtcomplexfloat>(&v.v);
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
  complexfloat out = *xt_reinterpret_const_cast<complexfloat, xtcomplexfloat>(&(a.v));
  return out;
}

template <> EIGEN_STRONG_INLINE PacketN_4cf preverse(const PacketN_4cf &a) {
  FUNC_ENTRY;
  return a;
}

template <> EIGEN_STRONG_INLINE complexfloat predux<PacketN_4cf>(const PacketN_4cf &a) {
  FUNC_ENTRY;
  complexfloat out = *xt_reinterpret_const_cast<complexfloat, xtcomplexfloat>(&(a.v));
  return out;
}


/** \internal \returns the product of the elements of \a a*/
template <> EIGEN_STRONG_INLINE complexfloat predux_mul<PacketN_4cf>(const PacketN_4cf &a) {
  FUNC_ENTRY;
  complexfloat out = *xt_reinterpret_const_cast<complexfloat, xtcomplexfloat>(&(a.v));
  return out;
}

/** \internal \returns \a a with real and imaginary part flipped (for complex type only) */
template<> EIGEN_STRONG_INLINE PacketN_4cf pcplxflip<PacketN_4cf>(const PacketN_4cf& x) {
  FUNC_ENTRY;
/* FIXME */
  xtcomplexfloat out;
  ae_int32x2 y = AE_MOVINT32X2_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMXTCOMPLEXFLOAT(x.v));
  out = AE_MOVXTCOMPLEXFLOAT_FROMXTFLOATX2(AE_MOVXTFLOATX2_FROMINT32X2(AE_SEL32_LH(y, y)));
  return PacketN_4cf(out);
}


template<> struct conj_helper<PacketN_4cf, PacketN_4cf, false,true> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf& x, const PacketN_4cf& y, const PacketN_4cf& c) const {
    FUNC_ENTRY;
    xtcomplexfloat out = c.v;
    MADDCCONJ_CS(out,x.v,y.v);
    return PacketN_4cf(out);
  }

  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& a, const PacketN_4cf& b) const {
    FUNC_ENTRY;
    return PacketN_4cf(MULCCONJ_CS(a.v, b.v));
  }
};

template<> struct conj_helper<PacketN_4cf, PacketN_4cf, true,false> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf& x, const PacketN_4cf& y, const PacketN_4cf& c) const {
    FUNC_ENTRY;
    xtcomplexfloat out = c.v;
    MADDCCONJ_CS(out,y.v,x.v);
    return PacketN_4cf(out);
  }

  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& a, const PacketN_4cf& b) const {
    FUNC_ENTRY;
    return PacketN_4cf(MULCCONJ_CS(b.v, a.v));
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
  { 
    PacketN_2f yy = AE_MOVXTFLOATX2_FROMXTCOMPLEXFLOAT(y.v); 
    return PacketN_4cf(Eigen::internal::pmul<PacketN_2f>(x, yy));
  }                 
};                                                                                                              

template <> struct conj_helper<PacketN_4cf, PacketN_2f, false, false> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf &x, const PacketN_2f &y, const PacketN_4cf &c) const {
    return padd(c, pmul(x,y));
  }                                                                                
  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& x, const PacketN_2f& y) const                        
  { 
    PacketN_2f xx = AE_MOVXTFLOATX2_FROMXTCOMPLEXFLOAT(x.v); 
    return PacketN_4cf(Eigen::internal::pmul<PacketN_2f>(xx, y));
  }                                           
};

template<> EIGEN_DEVICE_FUNC inline PacketN_4cf pselect(const PacketN_4cf& mask, const PacketN_4cf& a, const PacketN_4cf& b) {
  FUNC_ENTRY;
  xtcomplexfloat c;
  #ifdef __XTENSA__
  c = (mask.v==0) ? b.v : a.v;
  #else
  xtcomplexfloat zero = AE_MOVXTCOMPLEXFLOAT_FROMXTFLOATX2(CONST_SX2(0));
  xtbool vb1 = OEQ_CS(mask.v, zero) ;
  unsigned int ar = AE_MOVAB1(vb1);
  bool bl1 = ar;
  c = bl1 ? b.v : a.v;
  #endif
  return PacketN_4cf(c);
}

} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_CF32_HIFI1_1_XTENSA_H
