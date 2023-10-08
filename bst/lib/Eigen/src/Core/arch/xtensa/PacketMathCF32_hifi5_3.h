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

#ifndef EIGEN_PACKET_MATH_CF32_HIFI5_3_XTENSA_H
#define EIGEN_PACKET_MATH_CF32_HIFI5_3_XTENSA_H

#if SIMD_N_4_EQ_2()
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

template <> struct is_arithmetic<xtcomplexfloatx2> {
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
  EIGEN_STRONG_INLINE explicit PacketN_4cf(const PacketN_2f& a) {v = AE_MOVXTCOMPLEXFLOATX2_FROMXTFLOATX4(a);}
  #if (F32_HALF_PACKET_SUPPORT)
  EIGEN_STRONG_INLINE explicit PacketN_4cf(const PacketN_4f& a) {xtfloatx2 zero = 0.0f; v = AE_MOVXTCOMPLEXFLOATX2_FROMXTFLOATX4(AE_JOINSX4_FROMSX2(a.v, zero));}
  #endif 
  EIGEN_STRONG_INLINE explicit PacketN_4cf(xtfloatx4& a) {v = AE_MOVXTCOMPLEXFLOATX2_FROMXTFLOATX4(a);}

  EIGEN_STRONG_INLINE explicit PacketN_4cf(const xtcomplexfloatx2& a) : v(a) {}
  EIGEN_STRONG_INLINE explicit PacketN_4cf(xtcomplexfloatx2& a) : v(a) {}
  EIGEN_STRONG_INLINE operator PacketN_2f() {return AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(v);}
  #if (F32_HALF_PACKET_SUPPORT)
  EIGEN_STRONG_INLINE operator PacketN_4f() {return PacketN_4f(AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(v));}
  #endif 

  EIGEN_STRONG_INLINE friend xtbool2 operator==(const PacketN_4cf& a, const PacketN_4cf& b) {
    return OEQ_CSX2(a.v, b.v);
  }
  xtcomplexfloatx2 v;
};

typedef PacketN_4cf PacketN_8cf;

template <> struct packet_traits<complexfloat > : default_packet_traits {
  typedef PacketN_4cf type;
  typedef PacketN_8cf half;
  enum {
    Vectorizable    = 1,
    AlignedOnScalar = 1,
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
  return PacketN_4cf(xtcomplexfloat_rtor_xtcomplexfloatx2(out));
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
  return PacketN_4cf(-a.v); 
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pconj(const PacketN_4cf &a) {
  FUNC_ENTRY;
  return PacketN_4cf(CONJC_CSX2(a.v)); 
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pmul<PacketN_4cf>(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  return PacketN_4cf(a.v * b.v); 
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pmadd<PacketN_4cf>(const PacketN_4cf &a, const PacketN_4cf &b, const PacketN_4cf &c) {
  FUNC_ENTRY;
  xtcomplexfloatx2 out = c.v;
  MADDC_CSX2(out, a.v, b.v);
  return PacketN_4cf(out);
}

template <> EIGEN_STRONG_INLINE PacketN_4cf pdiv<PacketN_4cf>(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  return PacketN_4cf(DIV_CSX2(a.v, b.v)); 
}

template <> EIGEN_STRONG_INLINE PacketN_4cf pand(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  ae_int32x4 ai = AE_MOVINT32X4_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(a.v));
  ae_int32x4 bi = AE_MOVINT32X4_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(b.v));
  ai = AE_INT32X4_AND(ai, bi);
  xtcomplexfloatx2 out = AE_MOVXTCOMPLEXFLOATX2_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMINT32X4(ai));
  return PacketN_4cf(out);
}
template <> EIGEN_STRONG_INLINE PacketN_4cf por(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  ae_int32x4 ai = AE_MOVINT32X4_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(a.v));
  ae_int32x4 bi = AE_MOVINT32X4_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(b.v));
  ai = AE_INT32X4_OR(ai, bi);
  xtcomplexfloatx2 out = AE_MOVXTCOMPLEXFLOATX2_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMINT32X4(ai));
  return PacketN_4cf(out);
}
template <> EIGEN_STRONG_INLINE PacketN_4cf pxor(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  ae_int32x4 ai = AE_MOVINT32X4_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(a.v));
  ae_int32x4 bi = AE_MOVINT32X4_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(b.v));
  ai = AE_INT32X4_XOR(ai, bi);
  xtcomplexfloatx2 out = AE_MOVXTCOMPLEXFLOATX2_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMINT32X4(ai));
  return PacketN_4cf(out);
}

template <> EIGEN_STRONG_INLINE PacketN_4cf pandnot(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  ae_int32x4 ai = AE_MOVINT32X4_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(a.v));
  ae_int32x4 bi = AE_MOVINT32X4_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(b.v));
  bi = AE_INT32X4_XOR(bi, bi);
  ai = AE_INT32X4_AND(ai, bi);
  xtcomplexfloatx2 out = AE_MOVXTCOMPLEXFLOATX2_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMINT32X4(ai));
  return PacketN_4cf(out);
}

template <> EIGEN_STRONG_INLINE PacketN_4cf pload<PacketN_4cf>(const complexfloat *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  return *(xt_reinterpret_const_cast<PacketN_4cf,complexfloat >(from));
}

template <> EIGEN_STRONG_INLINE PacketN_4cf ploadu<PacketN_4cf>(const complexfloat *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_LOAD;
  xtcomplexfloatx2     tv0;
  ae_valignx2         talign;
  xtcomplexfloatx2 *tpv0 = xt_reinterpret_const_cast<xtcomplexfloatx2,complexfloat >(from);
  talign              = xtcomplexfloatx2_aligning_load_prime(tpv0);
  xtcomplexfloatx2_aligning_load_post_update_positive(tv0, talign, tpv0);
  return PacketN_4cf(tv0);
}

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
  xtcomplexfloatx2 out = xtcomplexfloat_rtor_xtcomplexfloatx2(ctv);
  return PacketN_4cf(out);
}

template <> EIGEN_STRONG_INLINE void pstore<complexfloat >(complexfloat *to, const PacketN_4cf &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_STORE;
  *(xt_reinterpret_cast<PacketN_4cf,complexfloat >(to)) = from;
}

template <> EIGEN_STRONG_INLINE void pstoreu<complexfloat >(complexfloat *to, const PacketN_4cf &from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_UNALIGNED_STORE;
  ae_valignx2 talign;
  xtcomplexfloatx2 *tpv0 = xt_reinterpret_cast<xtcomplexfloatx2,complexfloat>(to);
  talign = AE_ZALIGN128();
  xtcomplexfloatx2_aligning_store_post_update_positive(from.v, talign, tpv0);
  xtcomplexfloatx2_aligning_store_flush_positive(talign, tpv0);
}

template <> EIGEN_DEVICE_FUNC inline PacketN_4cf pgather<complexfloat, PacketN_4cf>(const complexfloat *from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtcomplexfloat *tp = xt_reinterpret_const_cast<xtcomplexfloat,complexfloat>(from);
  xtcomplexfloat tv0, tv1;
  xtcomplexfloatx2 tv2;
  xtcomplexfloat *to = xt_reinterpret_cast<xtcomplexfloat,xtcomplexfloatx2>(&tv2);
  tv0 = xtcomplexfloat_loadx(tp, 0 * sizeof(xtcomplexfloat) * stride);
  tv1 = xtcomplexfloat_loadx(tp, 1 * sizeof(xtcomplexfloat) * stride);
  to[0] = tv0;
  to[1] = tv1;
  return PacketN_4cf(tv2);
}

template <> EIGEN_DEVICE_FUNC inline void pscatter<complexfloat, PacketN_4cf>(complexfloat *to, const PacketN_4cf &from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtcomplexfloat *tpt = xt_reinterpret_cast<xtcomplexfloat,complexfloat>(to);
  xtcomplexfloat *from_v = xt_reinterpret_const_cast<xtcomplexfloat,xtcomplexfloatx2>(&from.v);
  tpt[stride * 0] = from_v[0];
  tpt[stride * 1] = from_v[1];
}

inline std::ostream & operator <<(std::ostream & s, const PacketN_4cf & v) {
  complexfloat *ptr = xt_reinterpret_const_cast<complexfloat,xtcomplexfloatx2>(&v.v);
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
  complexfloat *from = xt_reinterpret_const_cast<complexfloat, xtcomplexfloatx2>(&a.v);
  return *from;
}

template <> EIGEN_STRONG_INLINE PacketN_4cf preverse(const PacketN_4cf &a) {
  FUNC_ENTRY;
  xtcomplexfloatx2 out = AE_JOINCSX2_FROMCS(AE_EXTRACTCS_FROMCSX2_L(a.v), AE_EXTRACTCS_FROMCSX2_H(a.v));
  return PacketN_4cf(out);
}

template <> EIGEN_STRONG_INLINE complexfloat predux<PacketN_4cf>(const PacketN_4cf &a) {
  FUNC_ENTRY;
  xtcomplexfloat t0 = RADD_CSX2(a.v);
  return *(xt_reinterpret_cast<complexfloat,xtcomplexfloat>(&t0));
}


/** \internal \returns the product of the elements of \a a*/
template <> EIGEN_STRONG_INLINE complexfloat predux_mul<PacketN_4cf>(const PacketN_4cf &a) {
  FUNC_ENTRY;
  xtcomplexfloat tv1 = AE_EXTRACTCS_FROMCSX2_H(a.v);
  xtcomplexfloat tv2 = AE_EXTRACTCS_FROMCSX2_L(a.v);
  xtcomplexfloat out = MULC_CS(tv1, tv2);
  return *(xt_reinterpret_cast<complexfloat,xtcomplexfloat>(&out));
}

EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4cf, CONST_2> &kernel) {
  xtfloatx4 kv0 = AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(kernel.packet[0].v);
  xtfloatx4 kv1 = AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(kernel.packet[1].v);
  xtfloatx2 tmp = AE_EXTRACTSX2_FROMSX4_L(kv0);
  kernel.packet[0].v = AE_MOVXTCOMPLEXFLOATX2_FROMXTFLOATX4(AE_JOINSX4_FROMSX2(AE_EXTRACTSX2_FROMSX4_H(kv0), AE_EXTRACTSX2_FROMSX4_H(kv1)));
  kernel.packet[1].v = AE_MOVXTCOMPLEXFLOATX2_FROMXTFLOATX4(AE_JOINSX4_FROMSX2(tmp, AE_EXTRACTSX2_FROMSX4_L(kv1)));
}

/** \internal \returns \a a with real and imaginary part flipped (for complex type only) */
template<> EIGEN_STRONG_INLINE PacketN_4cf pcplxflip<PacketN_4cf>(const PacketN_4cf& x) {
  FUNC_ENTRY;
  xtfloatx2 real = CREAL_CSX2(x.v);
  xtfloatx2 imag = CIMAG_CSX2(x.v);
  return PacketN_4cf(CMPLX_CSX2(real, imag));
}


template<> struct conj_helper<PacketN_4cf, PacketN_4cf, false,true> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf& x, const PacketN_4cf& y, const PacketN_4cf& c) const {
    FUNC_ENTRY;
    xtcomplexfloatx2 out = c.v;
    MADDCCONJ_CSX2(out,x.v,y.v);
    return PacketN_4cf(out);
  }

  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& a, const PacketN_4cf& b) const {
    FUNC_ENTRY;
    return PacketN_4cf(MULCCONJ_CSX2(a.v, b.v));
  }
};

template<> struct conj_helper<PacketN_4cf, PacketN_4cf, true,false> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf& x, const PacketN_4cf& y, const PacketN_4cf& c) const {
    FUNC_ENTRY;
    xtcomplexfloatx2 out = c.v;
    MADDCCONJ_CSX2(out,y.v,x.v);
    return PacketN_4cf(out);
  }

  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& a, const PacketN_4cf& b) const {
    FUNC_ENTRY;
    return PacketN_4cf(MULCCONJ_CSX2(b.v, a.v));
  }
};

template<> struct conj_helper<PacketN_4cf, PacketN_4cf, true,true> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf& x, const PacketN_4cf& y, const PacketN_4cf& c) const {
    return padd(pmul(x,y),c);
  }

  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& a, const PacketN_4cf& b) const {
    FUNC_ENTRY;
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
    PacketN_2f yy = AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(y.v); 
    return PacketN_4cf(Eigen::internal::pmul<PacketN_2f>(x, yy));
  }                 
};                                                                                                              

template <> struct conj_helper<PacketN_4cf, PacketN_2f, false, false> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf &x, const PacketN_2f &y, const PacketN_4cf &c) const {
    return padd(c, pmul(x,y));
  }                                                                                
  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& x, const PacketN_2f& y) const                        
  { 
    PacketN_2f xx = AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(x.v); 
    return PacketN_4cf(Eigen::internal::pmul<PacketN_2f>(xx, y));
  }                                           
};

template<> EIGEN_DEVICE_FUNC inline PacketN_4cf pselect(const PacketN_4cf& mask, const PacketN_4cf& a, const PacketN_4cf& b) {
  FUNC_ENTRY;
  xtcomplexfloatx2 c;
  c = a.v;
  MOVT_CSX2(c, b.v, OEQ_CSX2(mask.v, ZERO_CSX2()));
  return PacketN_4cf(c);
}

template<> EIGEN_STRONG_INLINE PacketN_4cf pcmp_eq(const PacketN_4cf& a, const PacketN_4cf& b) {
  ae_int32x4 aone = 0xffffffffu;
  ae_int32x4 azero = 0x0u;
  xtcomplexfloatx2 aonef = AE_MOVXTCOMPLEXFLOATX2_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMINT32X4(aone));
  xtcomplexfloatx2 out = AE_MOVXTCOMPLEXFLOATX2_FROMXTFLOATX4(AE_MOVXTFLOATX4_FROMINT32X4(azero));
  MOVT_CSX2(out, aonef, OEQ_CSX2(a.v, b.v));
  return PacketN_4cf(out);
}
#if (F32_HALF_PACKET_SUPPORT)
// EIGEN_MAKE_CONJ_HELPER_CPLX_REAL(PacketN_4cf,PacketN_4f)
template <> struct conj_helper<PacketN_4f, PacketN_4cf, false, false> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4f &x, const PacketN_4cf &y, const PacketN_4cf &c) const {
    return padd(c, pmul(x,y));
  }                                                                                
  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4f& x, const PacketN_4cf& y) const                        
  { PacketN_4f yy = PacketN_4f(AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(y.v)); return PacketN_4cf(Eigen::internal::pmul<PacketN_4f>(x, yy)); }                                           
};                                                                                                              

template <> struct conj_helper<PacketN_4cf, PacketN_4f, false, false> {
  EIGEN_STRONG_INLINE PacketN_4cf pmadd(const PacketN_4cf &x, const PacketN_4f &y, const PacketN_4cf &c) const {
    return padd(c, pmul(x,y));
  }                                                                                
  EIGEN_STRONG_INLINE PacketN_4cf pmul(const PacketN_4cf& x, const PacketN_4f& y) const                        
  { PacketN_4f xx = PacketN_4f(AE_MOVXTFLOATX4_FROMXTCOMPLEXFLOATX2(x.v)); return PacketN_4cf(Eigen::internal::pmul<PacketN_4f>(xx, y)); }                                           
};
#endif
} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_CF32_HIFI5_3_XTENSA_H
