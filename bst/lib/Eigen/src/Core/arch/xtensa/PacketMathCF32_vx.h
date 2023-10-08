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

#ifndef EIGEN_PACKET_MATH_CF32_VX_XTENSA_H
#define EIGEN_PACKET_MATH_CF32_VX_XTENSA_H

#if CF32_FULL_PACKET_SUPPORT

#ifndef ALIGN128
  #define ALIGN128  __attribute__((aligned(128)))
#endif

namespace Eigen {

namespace internal {
#if CF32_HALF_PACKET_SUPPORT
  template <> EIGEN_STRONG_INLINE PacketN_4cf extract_N_8cf(const PacketN_4cf& a, const PacketN_4cf& b){
    return PacketN_4cf(IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(b.v), IVP_MOVNX16_FROMN_4XCF32(a.v), IVP_SELI_32B_EXTRACT_LO_HALVES)));
  }
#endif

template <> EIGEN_STRONG_INLINE PacketN_4cf pandnot(const PacketN_4cf &a, const PacketN_4cf &b) {
  FUNC_ENTRY;
  return PacketN_4cf(IVP_MOVN_4XCF32_FROMNX16(IVP_MOVNX16_FROMN_2X32(IVP_ANDN_2X32(IVP_MOVN_2X32_FROMNX16(IVP_MOVNX16_FROMN_4XCF32(a.v)), IVP_NOTN_2X32(IVP_MOVN_2X32_FROMNX16(IVP_MOVNX16_FROMN_4XCF32(b.v)))))));
}

/** \internal \returns a packet with elements of \a *from duplicated.
 * For instance, for a packet of 8 elements, 4 scalars will be read from \a *from and
 * duplicated to form: {from[0],from[0],from[1],from[1],from[2],from[2],from[3],from[3]}
 * Currently, this function is only used for scalar * complex products.
 */
template <> EIGEN_STRONG_INLINE PacketN_4cf ploaddup<PacketN_4cf>(const complexfloat *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  PacketN_4cf tv0 = ploadu<PacketN_4cf>(from);
  return PacketN_4cf(IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv0.v), IVP_MOVNX16_FROMN_4XCF32(tv0.v), IVP_SELI_32B_INTERLEAVE_2_LO)));
}

#if SIMD_N_4_EQ_8() || SIMD_N_4_EQ_16()
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
    xb_vecN_4xcf32 tv1 = IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv0.v), IVP_MOVNX16_FROMN_4XCF32(tv0.v), IVP_SELI_64B_INTERLEAVE_1_LO));
    return PacketN_4cf(IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_SELI_64B_INTERLEAVE_1_LO)));
  }
#endif

template <> EIGEN_DEVICE_FUNC inline PacketN_4cf pgather<complexfloat, PacketN_4cf>(const complexfloat *from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  #if SIMD_N_4_EQ_2()
    xtcomplexfloat *tp = xt_reinterpret_const_cast<xtcomplexfloat,complexfloat>(from);
    xb_vecN_4xcf32 tv0, tv1, tv2;
    tv0 = IVP_LSN_4XCF32_X(tp, 0 * sizeof(xtcomplexfloat) * stride);
    tv1 = IVP_LSN_4XCF32_X(tp, 1 * sizeof(xtcomplexfloat) * stride);
    tv2 = IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_MOVNX16_FROMN_4XCF32(tv0), IVP_SELI_64B_INTERLEAVE_1_EVEN));
    return PacketN_4cf(tv2);
  #else
    xtcomplexfloat *tp = xt_reinterpret_const_cast<xtcomplexfloat,complexfloat>(from);
    xb_vecN_4xcf32 ta;
    xtcomplexfloat *tpf = xt_reinterpret_cast<xtcomplexfloat,xb_vecN_4xcf32>(&ta);
    int    i;
    int    stride_bytes = sizeof(xtcomplexfloat) * stride;
    xb_vecN_4xcf32 tvf;

    for (i = 0; i < SIMD_N_4; i++) {
      IVP_LSRN_4XCF32_XP(tvf, tp, stride_bytes);
      IVP_SSN_4XCF32_IP(tvf, tpf, sizeof(xtcomplexfloat));
    }
    return PacketN_4cf(ta);
  #endif
}

template <> EIGEN_DEVICE_FUNC inline void pscatter<complexfloat, PacketN_4cf>(complexfloat *to, const PacketN_4cf &from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  #if SIMD_N_4_EQ_2()
    xtcomplexfloat *tpt = xt_reinterpret_cast<xtcomplexfloat,complexfloat>(to);
    tpt[stride * 0] = IVP_SELSN_4XCF32(from.v, 0);
    tpt[stride * 1] = IVP_SELSN_4XCF32(from.v, 1);
  #else
    int i;
    xtcomplexfloat *tpf = xt_reinterpret_const_cast<xtcomplexfloat,PacketN_4cf>(&from);
    xtcomplexfloat *tpt = xt_reinterpret_cast<xtcomplexfloat,complexfloat>(to);
    int    stride_bytes = sizeof(xtcomplexfloat) * stride;
    xb_vecN_4xcf32 tvf;
    for (i = 0; i < SIMD_N_4; i++)
    {
      IVP_LSRN_4XCF32_IP(tvf, tpf, sizeof(xtcomplexfloat));
      IVP_SSN_4XCF32_XP(tvf, tpt, stride_bytes);
    }
  #endif
}

template <> EIGEN_STRONG_INLINE PacketN_4cf preverse(const PacketN_4cf &a) {
  FUNC_ENTRY;
  return PacketN_4cf(IVP_MOVN_4XCF32_FROMNX16(IVP_SHFLNX16I(IVP_MOVNX16_FROMN_4XCF32(a.v), IVP_SHFLI_64B_REVERSE_1)));
}

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
    tv1 = IVP_CONSTN_4XCF32(0);
    tv2 = IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv0), IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_SELI_64B_ROTATE_RIGHT_4));
    tv2 = tv0 + tv2;
    tv0 = IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_MOVNX16_FROMN_4XCF32(tv2), IVP_SELI_64B_ROTATE_LEFT_4));
    return PacketN_8cf(tv0);
  #elif SIMD_N_4_EQ_16()
    xb_vecN_4xcf32 tv0, tv1, tv2;
    vboolN_4 tvb;
    tvb = IVP_LTRN_4I(SIMD_N_4/2);
    tv0 = a.v;
    tv1 = IVP_CONSTN_4XCF32(0);
    tv2 = IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_MOVNX16_FROMN_4XCF32(tv0), IVP_SELI_64B_ROTATE_RIGHT_8));
    tv2 = tv0 + tv2;
    tv0 = IVP_MOVN_4XCF32T(tv2, tv1, tvb);
    return PacketN_8cf(tv0);
  #endif
}
#endif

/** \internal \returns the product of the elements of \a a*/
template <> EIGEN_STRONG_INLINE complexfloat predux_mul<PacketN_4cf>(const PacketN_4cf &a) {
  FUNC_ENTRY;
  #if SIMD_N_4_EQ_2()
    xb_vecN_4xcf32 tv1 = a.v;
    xb_vecN_4xcf32 tv2 = IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_SELI_64B_ROTATE_LEFT_1));
    xb_vecN_4xcf32 tv3 = IVP_MULN_4XCF32(tv1, tv2);
    xtcomplexfloat t0 = IVP_SELSN_4XCF32(tv3, 0);
    return *(xt_reinterpret_cast<complexfloat,xtcomplexfloat>(&t0));
  #elif SIMD_N_4_EQ_8()
    xb_vecN_4xcf32 tv0, tv1;
    tv0 = a.v;
    tv1 = IVP_MULN_4XCF32(tv0, IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv0), IVP_MOVNX16_FROMN_4XCF32(tv0), IVP_SELI_64B_ROTATE_LEFT_4)));
    tv0 = IVP_MULN_4XCF32(tv1, IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_SELI_64B_ROTATE_LEFT_2)));
    tv1 = IVP_MULN_4XCF32(tv0, IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv0), IVP_MOVNX16_FROMN_4XCF32(tv0), IVP_SELI_64B_ROTATE_LEFT_1)));
    xtcomplexfloat t0 = IVP_SELSN_4XCF32(tv1, 0);
    return *(xt_reinterpret_cast<complexfloat,xtcomplexfloat>(&t0));
  #elif SIMD_N_4_EQ_16()
    xb_vecN_4xcf32 tv0, tv1;
    tv0 = a.v;
    tv1 = IVP_MULN_4XCF32(tv0, IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv0), IVP_MOVNX16_FROMN_4XCF32(tv0), IVP_SELI_64B_ROTATE_RIGHT_8)));
    tv0 = IVP_MULN_4XCF32(tv1, IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_SELI_64B_ROTATE_LEFT_4)));
    tv1 = IVP_MULN_4XCF32(tv0, IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv0), IVP_MOVNX16_FROMN_4XCF32(tv0), IVP_SELI_64B_ROTATE_LEFT_2)));
    tv0 = IVP_MULN_4XCF32(tv1, IVP_MOVN_4XCF32_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_MOVNX16_FROMN_4XCF32(tv1), IVP_SELI_64B_ROTATE_LEFT_1)));
    xtcomplexfloat t0 = IVP_SELSN_4XCF32(tv0, 0);
    return *(xt_reinterpret_cast<complexfloat,xtcomplexfloat>(&t0));
  #endif
}


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
    #define VR16(_num) xb_vecNx16 t##_num;
    #define DECL_VEC16 VR16(0); VR16(1); VR16(2);
    #define DECL_VEC(_var) DR(_var,0); DR(_var,1);

    // Load vectors
    #define LOAD_VEC(_var) LR(_var,0); LR(_var,1);

    // Store vectors
    #define STORE_VEC(_var) SR(_var,0,0); SR(_var,1,1);

    signed char ALIGN128 sel_32b_interleave_2[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 64, 96, 65, 97, 66, 98, 67, 99, 4, 36, 5, 37, 6, 38, 7, 39, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 72, 104, 73, 105, 74, 106, 75, 107, 12, 44, 13, 45, 14, 46, 15, 47, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 80, 112, 81, 113, 82, 114, 83, 115, 20, 52, 21, 53, 22, 54, 23, 55, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 88, 120, 89, 121, 90, 122, 91, 123, 28, 60, 29, 61, 30, 62, 31, 63, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_2)); // interleave 2

    #define FIRST_STEP(_pat, _out, _inp)       \
      t0 = IVP_MOVNX16_FROMN_4XCF32(_out##0);  \
      t1 = IVP_MOVNX16_FROMN_4XCF32(_out##1);  \
      t2 = IVP_MOVNX16_FROMN_4XCF32(_out##2);  \
      IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_4XCF32(_inp##1), IVP_MOVNX16_FROMN_4XCF32(_inp##0), _pat);  \
      _out##0 = IVP_MOVN_4XCF32_FROMNX16(t0);  \
      _out##1 = IVP_MOVN_4XCF32_FROMNX16(t1);  \
      _out##2 = IVP_MOVN_4XCF32_FROMNX16(t2);  \

    #define NEXT_STEP(_pat, _out, _inp)        \
      t0 = IVP_MOVNX16_FROMN_4XCF32(_out##0);  \
      t1 = IVP_MOVNX16_FROMN_4XCF32(_out##1);  \
      t2 = IVP_MOVNX16_FROMN_4XCF32(_out##2);  \
      IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_4XCF32(_inp##2), IVP_MOVNX16_FROMN_4XCF32(_inp##0), _pat);     \
      _out##0 = IVP_MOVN_4XCF32_FROMNX16(t0);  \
      _out##1 = IVP_MOVN_4XCF32_FROMNX16(t1);  \
      _out##2 = IVP_MOVN_4XCF32_FROMNX16(t2);  \

    DECL_VEC(tvf);
    DECL_VEC(tvs);
    DECL_VEC16;
    LOAD_VEC(tvf);
    FIRST_STEP(vecselpat_32b_interleave_2, tvs, tvf);
    STORE_VEC(tvs);

    #undef DR
    #undef LR
    #undef SR
    #undef DECL_VEC
    #undef DECL_VEC16
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
    #define DECL_VEC(_var) DR(_var,0); DR(_var,1); DR(_var,2); DR(_var,3);

    #define VR16(_num) xb_vecNx16 t##_num;

    #define DECL_VEC16 VR16(0); VR16(1); VR16(2); VR16(3);

    // Load vectors
    #define LOAD_VEC(_var) LR(_var,0); LR(_var,1); LR(_var,2); LR(_var,3);

    // Store vectors
    #define STORE_VEC(_var) SR(_var,0,0); SR(_var,1,1); SR(_var,2,2); SR(_var,3,3);

    signed char ALIGN128 sel_32b_interleave_4[128] =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_4)); // interleave 4

    signed char ALIGN128 sel_32b_interleave_2[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 64, 96, 65, 97, 66, 98, 67, 99, 4, 36, 5, 37, 6, 38, 7, 39, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 72, 104, 73, 105, 74, 106, 75, 107, 12, 44, 13, 45, 14, 46, 15, 47, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 80, 112, 81, 113, 82, 114, 83, 115, 20, 52, 21, 53, 22, 54, 23, 55, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 88, 120, 89, 121, 90, 122, 91, 123, 28, 60, 29, 61, 30, 62, 31, 63, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_2)); // interleave 2

    /* This is first step in transpose operation. In first step, input vectors and
     * output vectors have same sequence. This will change in 'NEXT_STEP', where
     * output vectors will have the same sequence but input vector sequence will be
     * different. */
    #define FIRST_STEP(_pat, _out, _inp)       \
      t0 = IVP_MOVNX16_FROMN_4XCF32(_out##0);  \
      t1 = IVP_MOVNX16_FROMN_4XCF32(_out##1);  \
      t2 = IVP_MOVNX16_FROMN_4XCF32(_out##2);  \
      t3 = IVP_MOVNX16_FROMN_4XCF32(_out##3);  \
      IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_4XCF32(_inp##1), IVP_MOVNX16_FROMN_4XCF32(_inp##0), _pat);     \
      IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_4XCF32(_inp##3), IVP_MOVNX16_FROMN_4XCF32(_inp##2), _pat);     \
      _out##0 = IVP_MOVN_4XCF32_FROMNX16(t0);  \
      _out##1 = IVP_MOVN_4XCF32_FROMNX16(t1);  \
      _out##2 = IVP_MOVN_4XCF32_FROMNX16(t2);  \
      _out##3 = IVP_MOVN_4XCF32_FROMNX16(t3);  \

    /* This is subsequent step in transpose operation. The only difference between
     * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
     * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
    #define NEXT_STEP(_pat, _out, _inp)        \
      t0 = IVP_MOVNX16_FROMN_4XCF32(_out##0);  \
      t1 = IVP_MOVNX16_FROMN_4XCF32(_out##1);  \
      t2 = IVP_MOVNX16_FROMN_4XCF32(_out##2);  \
      t3 = IVP_MOVNX16_FROMN_4XCF32(_out##3);  \
      IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_4XCF32(_inp##2), IVP_MOVNX16_FROMN_4XCF32(_inp##0), _pat);    \
      IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_4XCF32(_inp##3), IVP_MOVNX16_FROMN_4XCF32(_inp##1), _pat);    \
      _out##0 = IVP_MOVN_4XCF32_FROMNX16(t0);  \
      _out##1 = IVP_MOVN_4XCF32_FROMNX16(t1);  \
      _out##2 = IVP_MOVN_4XCF32_FROMNX16(t2);  \
      _out##3 = IVP_MOVN_4XCF32_FROMNX16(t3);  \

    DECL_VEC(tvf);
    DECL_VEC(tvs);
    DECL_VEC16;
    LOAD_VEC(tvf);
    FIRST_STEP(vecselpat_32b_interleave_2, tvs, tvf);
    NEXT_STEP(vecselpat_32b_interleave_4, tvf, tvs);
    STORE_VEC(tvf);

    #undef DR
    #undef LR
    #undef SR
    #undef DECL_VEC
    #undef DECL_VEC16
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

    #define VR16(_num) xb_vecNx16 t##_num;

    #define DECL_VEC16 \
      VR16(0)  ; VR16(1)  ; VR16(2)  ; VR16(3)  ; \
      VR16(4)  ; VR16(5)  ; VR16(6)  ; VR16(7)  ; \

    signed char ALIGN128 sel_32b_interleave_4[128] =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_4)); // interleave 4

    signed char ALIGN128 sel_32b_interleave_2[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 64, 96, 65, 97, 66, 98, 67, 99, 4, 36, 5, 37, 6, 38, 7, 39, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 72, 104, 73, 105, 74, 106, 75, 107, 12, 44, 13, 45, 14, 46, 15, 47, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 80, 112, 81, 113, 82, 114, 83, 115, 20, 52, 21, 53, 22, 54, 23, 55, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 88, 120, 89, 121, 90, 122, 91, 123, 28, 60, 29, 61, 30, 62, 31, 63, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_2)); // interleave 2
        
    signed char ALIGN128 sel_32b_interleave_8[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111, 
      16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_8 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_8)); // interleave 8

    /* This is first step in transpose operation. In first step, input vectors and
     * output vectors have same sequence. This will change in 'NEXT_STEP', where
     * output vectors will have the same sequence but input vector sequence will be
     * different. */
    #define FIRST_STEP(_pat, _out, _inp)                              \
      t0 = IVP_MOVNX16_FROMN_4XCF32(_out##0);  \
      t1 = IVP_MOVNX16_FROMN_4XCF32(_out##1);  \
      t2 = IVP_MOVNX16_FROMN_4XCF32(_out##2);  \
      t3 = IVP_MOVNX16_FROMN_4XCF32(_out##3);  \
      t4 = IVP_MOVNX16_FROMN_4XCF32(_out##4);  \
      t5 = IVP_MOVNX16_FROMN_4XCF32(_out##5);  \
      t6 = IVP_MOVNX16_FROMN_4XCF32(_out##6);  \
      t7 = IVP_MOVNX16_FROMN_4XCF32(_out##7);  \
      IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_4XCF32(_inp##1), IVP_MOVNX16_FROMN_4XCF32(_inp##0), _pat);     \
      IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_4XCF32(_inp##3), IVP_MOVNX16_FROMN_4XCF32(_inp##2), _pat);     \
      IVP_DSELNX16(t5, t4, IVP_MOVNX16_FROMN_4XCF32(_inp##5), IVP_MOVNX16_FROMN_4XCF32(_inp##4), _pat);     \
      IVP_DSELNX16(t7, t6, IVP_MOVNX16_FROMN_4XCF32(_inp##7), IVP_MOVNX16_FROMN_4XCF32(_inp##6), _pat);     \
      _out##0 = IVP_MOVN_4XCF32_FROMNX16(t0);  \
      _out##1 = IVP_MOVN_4XCF32_FROMNX16(t1);  \
      _out##2 = IVP_MOVN_4XCF32_FROMNX16(t2);  \
      _out##3 = IVP_MOVN_4XCF32_FROMNX16(t3);  \
      _out##4 = IVP_MOVN_4XCF32_FROMNX16(t4);  \
      _out##5 = IVP_MOVN_4XCF32_FROMNX16(t5);  \
      _out##6 = IVP_MOVN_4XCF32_FROMNX16(t6);  \
      _out##7 = IVP_MOVN_4XCF32_FROMNX16(t7);  \

    /* This is subsequent step in transpose operation. The only difference between
     * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
     * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
    #define NEXT_STEP(_pat, _out, _inp)                               \
      t0 = IVP_MOVNX16_FROMN_4XCF32(_out##0);  \
      t1 = IVP_MOVNX16_FROMN_4XCF32(_out##1);  \
      t2 = IVP_MOVNX16_FROMN_4XCF32(_out##2);  \
      t3 = IVP_MOVNX16_FROMN_4XCF32(_out##3);  \
      t4 = IVP_MOVNX16_FROMN_4XCF32(_out##4);  \
      t5 = IVP_MOVNX16_FROMN_4XCF32(_out##5);  \
      t6 = IVP_MOVNX16_FROMN_4XCF32(_out##6);  \
      t7 = IVP_MOVNX16_FROMN_4XCF32(_out##7);  \
      IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_4XCF32(_inp##2), IVP_MOVNX16_FROMN_4XCF32(_inp##0), _pat); \
      IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_4XCF32(_inp##6), IVP_MOVNX16_FROMN_4XCF32(_inp##4), _pat); \
      IVP_DSELNX16(t5, t4, IVP_MOVNX16_FROMN_4XCF32(_inp##3), IVP_MOVNX16_FROMN_4XCF32(_inp##1), _pat); \
      IVP_DSELNX16(t7, t6, IVP_MOVNX16_FROMN_4XCF32(_inp##7), IVP_MOVNX16_FROMN_4XCF32(_inp##5), _pat); \
      _out##0 = IVP_MOVN_4XCF32_FROMNX16(t0);  \
      _out##1 = IVP_MOVN_4XCF32_FROMNX16(t1);  \
      _out##2 = IVP_MOVN_4XCF32_FROMNX16(t2);  \
      _out##3 = IVP_MOVN_4XCF32_FROMNX16(t3);  \
      _out##4 = IVP_MOVN_4XCF32_FROMNX16(t4);  \
      _out##5 = IVP_MOVN_4XCF32_FROMNX16(t5);  \
      _out##6 = IVP_MOVN_4XCF32_FROMNX16(t6);  \
      _out##7 = IVP_MOVN_4XCF32_FROMNX16(t7);  \

    DECL_VEC(tvf);
    DECL_VEC(tvs);
    DECL_VEC16;
    LOAD_VEC(tvf);
    FIRST_STEP(vecselpat_32b_interleave_2, tvs, tvf);
    NEXT_STEP(vecselpat_32b_interleave_4, tvf, tvs);
    NEXT_STEP(vecselpat_32b_interleave_8, tvs, tvf);
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
    #define DECL_VEC(_var) DR(_var,0); DR(_var,1); DR(_var,2); DR(_var,3);

    #define VR16(_num) xb_vecNx16 t##_num;

    #define DECL_VEC16 VR16(0); VR16(1); VR16(2); VR16(3);

    // Load vectors
    #define LOAD_VEC(_var) LR(_var,0); LR(_var,1); LR(_var,2); LR(_var,3);

    // Store vectors
    #define STORE_VEC(_var) SR(_var,0,0); SR(_var,1,1); SR(_var,2,2); SR(_var,3,3);

    signed char ALIGN128 sel_32b_interleave_4[128] =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_4)); // interleave 4

    signed char ALIGN128 sel_32b_interleave_2[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 64, 96, 65, 97, 66, 98, 67, 99, 4, 36, 5, 37, 6, 38, 7, 39, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 72, 104, 73, 105, 74, 106, 75, 107, 12, 44, 13, 45, 14, 46, 15, 47, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 80, 112, 81, 113, 82, 114, 83, 115, 20, 52, 21, 53, 22, 54, 23, 55, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 88, 120, 89, 121, 90, 122, 91, 123, 28, 60, 29, 61, 30, 62, 31, 63, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_2)); // interleave 2


    /* This is first step in transpose operation. In first step, input vectors and
     * output vectors have same sequence. This will change in 'NEXT_STEP', where
     * output vectors will have the same sequence but input vector sequence will be
     * different. */
    #define FIRST_STEP(_pat, _out, _inp)       \
      t0 = IVP_MOVNX16_FROMN_4XCF32(_out##0);  \
      t1 = IVP_MOVNX16_FROMN_4XCF32(_out##1);  \
      t2 = IVP_MOVNX16_FROMN_4XCF32(_out##2);  \
      t3 = IVP_MOVNX16_FROMN_4XCF32(_out##3);  \
      IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_4XCF32(_inp##1), IVP_MOVNX16_FROMN_4XCF32(_inp##0), _pat);     \
      IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_4XCF32(_inp##3), IVP_MOVNX16_FROMN_4XCF32(_inp##2), _pat);     \
      _out##0 = IVP_MOVN_4XCF32_FROMNX16(t0);  \
      _out##1 = IVP_MOVN_4XCF32_FROMNX16(t1);  \
      _out##2 = IVP_MOVN_4XCF32_FROMNX16(t2);  \
      _out##3 = IVP_MOVN_4XCF32_FROMNX16(t3);  \

    /* This is subsequent step in transpose operation. The only difference between
     * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
     * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
    #define NEXT_STEP(_pat, _out, _inp)        \
      t0 = IVP_MOVNX16_FROMN_4XCF32(_out##0);  \
      t1 = IVP_MOVNX16_FROMN_4XCF32(_out##1);  \
      t2 = IVP_MOVNX16_FROMN_4XCF32(_out##2);  \
      t3 = IVP_MOVNX16_FROMN_4XCF32(_out##3);  \
      IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_4XCF32(_inp##2), IVP_MOVNX16_FROMN_4XCF32(_inp##0), _pat);    \
      IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_4XCF32(_inp##3), IVP_MOVNX16_FROMN_4XCF32(_inp##1), _pat);    \
      _out##0 = IVP_MOVN_4XCF32_FROMNX16(t0);  \
      _out##1 = IVP_MOVN_4XCF32_FROMNX16(t1);  \
      _out##2 = IVP_MOVN_4XCF32_FROMNX16(t2);  \
      _out##3 = IVP_MOVN_4XCF32_FROMNX16(t3);  \

    DECL_VEC(tvf);
    DECL_VEC(tvs);
    DECL_VEC16;
    LOAD_VEC(tvf);
    FIRST_STEP(vecselpat_32b_interleave_2, tvs, tvf);
    NEXT_STEP(vecselpat_32b_interleave_4, tvf, tvs);
    STORE_VEC(tvf);

    #undef DR
    #undef LR
    #undef SR
    #undef DECL_VEC
    #undef DECL_VEC16
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

    #define VR16(_num) xb_vecNx16 t##_num;

    #define DECL_VEC16 \
      VR16(0)  ; VR16(1)  ; VR16(2)  ; VR16(3)  ; \
      VR16(4)  ; VR16(5)  ; VR16(6)  ; VR16(7)  ; \
      VR16(8)  ; VR16(9)  ; VR16(10) ; VR16(11) ; \
      VR16(12) ; VR16(13) ; VR16(14) ; VR16(15) ; \

    signed char ALIGN128 sel_32b_interleave_4[128] =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_4)); // interleave 4
        
    signed char ALIGN128 sel_32b_interleave_8[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111, 
      16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_8 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_8)); // interleave 8

    signed char ALIGN128 sel_32b_interleave_16[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63,
      64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_16 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_16)); // interleave 16
        
    signed char ALIGN128 sel_32b_interleave_2[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 64, 96, 65, 97, 66, 98, 67, 99, 4, 36, 5, 37, 6, 38, 7, 39, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 72, 104, 73, 105, 74, 106, 75, 107, 12, 44, 13, 45, 14, 46, 15, 47, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 80, 112, 81, 113, 82, 114, 83, 115, 20, 52, 21, 53, 22, 54, 23, 55, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 88, 120, 89, 121, 90, 122, 91, 123, 28, 60, 29, 61, 30, 62, 31, 63, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_2)); // interleave 2

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
    #define FIRST_STEP(_pat, _out, _inp)       \
      t0 = IVP_MOVNX16_FROMN_4XCF32(_out##0);  \
      t1 = IVP_MOVNX16_FROMN_4XCF32(_out##1);  \
      t2 = IVP_MOVNX16_FROMN_4XCF32(_out##2);  \
      t3 = IVP_MOVNX16_FROMN_4XCF32(_out##3);  \
      t4 = IVP_MOVNX16_FROMN_4XCF32(_out##4);  \
      t5 = IVP_MOVNX16_FROMN_4XCF32(_out##5);  \
      t6 = IVP_MOVNX16_FROMN_4XCF32(_out##6);  \
      t7 = IVP_MOVNX16_FROMN_4XCF32(_out##7);  \
      t8 = IVP_MOVNX16_FROMN_4XCF32(_out##8);  \
      t9 = IVP_MOVNX16_FROMN_4XCF32(_out##9);  \
      t10= IVP_MOVNX16_FROMN_4XCF32(_out##10); \
      t11= IVP_MOVNX16_FROMN_4XCF32(_out##11); \
      t12= IVP_MOVNX16_FROMN_4XCF32(_out##12); \
      t13= IVP_MOVNX16_FROMN_4XCF32(_out##13); \
      t14= IVP_MOVNX16_FROMN_4XCF32(_out##14); \
      t15= IVP_MOVNX16_FROMN_4XCF32(_out##15); \
      IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_4XCF32(_inp##1), IVP_MOVNX16_FROMN_4XCF32(_inp##0), _pat);     \
      IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_4XCF32(_inp##3), IVP_MOVNX16_FROMN_4XCF32(_inp##2), _pat);     \
      IVP_DSELNX16(t5, t4, IVP_MOVNX16_FROMN_4XCF32(_inp##5), IVP_MOVNX16_FROMN_4XCF32(_inp##4), _pat);     \
      IVP_DSELNX16(t7, t6, IVP_MOVNX16_FROMN_4XCF32(_inp##7), IVP_MOVNX16_FROMN_4XCF32(_inp##6), _pat);     \
      IVP_DSELNX16(t9, t8, IVP_MOVNX16_FROMN_4XCF32(_inp##9), IVP_MOVNX16_FROMN_4XCF32(_inp##8), _pat);     \
      IVP_DSELNX16(t11, t10, IVP_MOVNX16_FROMN_4XCF32(_inp##11), IVP_MOVNX16_FROMN_4XCF32(_inp##10), _pat); \
      IVP_DSELNX16(t13, t12, IVP_MOVNX16_FROMN_4XCF32(_inp##13), IVP_MOVNX16_FROMN_4XCF32(_inp##12), _pat); \
      IVP_DSELNX16(t15, t14, IVP_MOVNX16_FROMN_4XCF32(_inp##15), IVP_MOVNX16_FROMN_4XCF32(_inp##14), _pat); \
      _out##0 = IVP_MOVN_4XCF32_FROMNX16(t0);  \
      _out##1 = IVP_MOVN_4XCF32_FROMNX16(t1);  \
      _out##2 = IVP_MOVN_4XCF32_FROMNX16(t2);  \
      _out##3 = IVP_MOVN_4XCF32_FROMNX16(t3);  \
      _out##4 = IVP_MOVN_4XCF32_FROMNX16(t4);  \
      _out##5 = IVP_MOVN_4XCF32_FROMNX16(t5);  \
      _out##6 = IVP_MOVN_4XCF32_FROMNX16(t6);  \
      _out##7 = IVP_MOVN_4XCF32_FROMNX16(t7);  \
      _out##8 = IVP_MOVN_4XCF32_FROMNX16(t8);  \
      _out##9 = IVP_MOVN_4XCF32_FROMNX16(t9);  \
      _out##10= IVP_MOVN_4XCF32_FROMNX16(t10); \
      _out##11= IVP_MOVN_4XCF32_FROMNX16(t11); \
      _out##12= IVP_MOVN_4XCF32_FROMNX16(t12); \
      _out##13= IVP_MOVN_4XCF32_FROMNX16(t13); \
      _out##14= IVP_MOVN_4XCF32_FROMNX16(t14); \
      _out##15= IVP_MOVN_4XCF32_FROMNX16(t15); \

    /* This is subsequent step in transpose operation. The only difference between
     * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
     * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
    #define NEXT_STEP(_pat, _out, _inp)        \
      t0 = IVP_MOVNX16_FROMN_4XCF32(_out##0);  \
      t1 = IVP_MOVNX16_FROMN_4XCF32(_out##1);  \
      t2 = IVP_MOVNX16_FROMN_4XCF32(_out##2);  \
      t3 = IVP_MOVNX16_FROMN_4XCF32(_out##3);  \
      t4 = IVP_MOVNX16_FROMN_4XCF32(_out##4);  \
      t5 = IVP_MOVNX16_FROMN_4XCF32(_out##5);  \
      t6 = IVP_MOVNX16_FROMN_4XCF32(_out##6);  \
      t7 = IVP_MOVNX16_FROMN_4XCF32(_out##7);  \
      t8 = IVP_MOVNX16_FROMN_4XCF32(_out##8);  \
      t9 = IVP_MOVNX16_FROMN_4XCF32(_out##9);  \
      t10= IVP_MOVNX16_FROMN_4XCF32(_out##10); \
      t11= IVP_MOVNX16_FROMN_4XCF32(_out##11); \
      t12= IVP_MOVNX16_FROMN_4XCF32(_out##12); \
      t13= IVP_MOVNX16_FROMN_4XCF32(_out##13); \
      t14= IVP_MOVNX16_FROMN_4XCF32(_out##14); \
      t15= IVP_MOVNX16_FROMN_4XCF32(_out##15); \
      IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_4XCF32(_inp##2), IVP_MOVNX16_FROMN_4XCF32(_inp##0), _pat);    \
      IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_4XCF32(_inp##6), IVP_MOVNX16_FROMN_4XCF32(_inp##4), _pat);    \
      IVP_DSELNX16(t5, t4, IVP_MOVNX16_FROMN_4XCF32(_inp##10), IVP_MOVNX16_FROMN_4XCF32(_inp##8), _pat);   \
      IVP_DSELNX16(t7, t6, IVP_MOVNX16_FROMN_4XCF32(_inp##14), IVP_MOVNX16_FROMN_4XCF32(_inp##12), _pat);  \
      IVP_DSELNX16(t9, t8, IVP_MOVNX16_FROMN_4XCF32(_inp##3), IVP_MOVNX16_FROMN_4XCF32(_inp##1), _pat);    \
      IVP_DSELNX16(t11, t10, IVP_MOVNX16_FROMN_4XCF32(_inp##7), IVP_MOVNX16_FROMN_4XCF32(_inp##5), _pat);  \
      IVP_DSELNX16(t13, t12, IVP_MOVNX16_FROMN_4XCF32(_inp##11), IVP_MOVNX16_FROMN_4XCF32(_inp##9), _pat); \
      IVP_DSELNX16(t15, t14, IVP_MOVNX16_FROMN_4XCF32(_inp##15), IVP_MOVNX16_FROMN_4XCF32(_inp##13), _pat);\
      _out##0 = IVP_MOVN_4XCF32_FROMNX16(t0);  \
      _out##1 = IVP_MOVN_4XCF32_FROMNX16(t1);  \
      _out##2 = IVP_MOVN_4XCF32_FROMNX16(t2);  \
      _out##3 = IVP_MOVN_4XCF32_FROMNX16(t3);  \
      _out##4 = IVP_MOVN_4XCF32_FROMNX16(t4);  \
      _out##5 = IVP_MOVN_4XCF32_FROMNX16(t5);  \
      _out##6 = IVP_MOVN_4XCF32_FROMNX16(t6);  \
      _out##7 = IVP_MOVN_4XCF32_FROMNX16(t7);  \
      _out##8 = IVP_MOVN_4XCF32_FROMNX16(t8);  \
      _out##9 = IVP_MOVN_4XCF32_FROMNX16(t9);  \
      _out##10= IVP_MOVN_4XCF32_FROMNX16(t10); \
      _out##11= IVP_MOVN_4XCF32_FROMNX16(t11); \
      _out##12= IVP_MOVN_4XCF32_FROMNX16(t12); \
      _out##13= IVP_MOVN_4XCF32_FROMNX16(t13); \
      _out##14= IVP_MOVN_4XCF32_FROMNX16(t14); \
      _out##15= IVP_MOVN_4XCF32_FROMNX16(t15); \

    DECL_VEC(tvf);
    DECL_VEC(tvs);
    DECL_VEC16;
    LOAD_VEC(tvf);
    FIRST_STEP(vecselpat_32b_interleave_2, tvs, tvf);
    NEXT_STEP(vecselpat_32b_interleave_4, tvf, tvs);
    NEXT_STEP(vecselpat_32b_interleave_8, tvs, tvf);
    NEXT_STEP(vecselpat_32b_interleave_16, tvf, tvs);
    STORE_VEC(tvf);

    #undef DR
    #undef LR
    #undef SR
    #undef DECL_VEC
    #undef DECL_VEC16
    #undef LOAD_VEC
    #undef STORE_VEC
    #undef FIRST_STEP
    #undef NEXT_STEP
  }
#endif

/** \internal \returns \a a with real and imaginary part flipped (for complex type only) */
template<> EIGEN_STRONG_INLINE PacketN_4cf pcplxflip<PacketN_4cf>(const PacketN_4cf& x) {
  FUNC_ENTRY;
  return PacketN_4cf(IVP_MOVN_4XCF32_FROMNX16(IVP_SHFLNX16I(IVP_MOVNX16_FROMN_4XCF32(x.v), IVP_SHFLI_32B_SWAP_1)));
}

} // end namespace internal

} // end namespace Eigen

#endif
#endif // EIGEN_PACKET_MATH_CF32_VX_XTENSA_H
