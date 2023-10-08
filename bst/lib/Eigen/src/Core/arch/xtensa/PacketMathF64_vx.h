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

#ifndef EIGEN_PACKET_MATH_F64_VX_XTENSA_H
#define EIGEN_PACKET_MATH_F64_VX_XTENSA_H

#if F64_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

#if F64_HALF_PACKET_SUPPORT
  template <> EIGEN_STRONG_INLINE PacketN_4d extract_N_8d(const PacketN_4d& a, const PacketN_4d& b){
    FUNC_ENTRY;
  #if SIMD_N_4_EQ_8()
    PacketN_4d vzero = PacketN_4d(0);
    PacketN_4d va = IVP_SELN_4XF64I(a, vzero, IVP_SELI_64B_ROTATE_RIGHT_4);
    PacketN_4d va2 = IVP_SELN_4XF64I(vzero, va, IVP_SELI_64B_ROTATE_LEFT_4);
    PacketN_4d vb = IVP_SELN_4XF64I(b, vzero, IVP_SELI_64B_ROTATE_RIGHT_4);
    PacketN_4d vout = va2 + vb;
  #elif SIMD_N_4_EQ_16()
    PacketN_4d vzero = PacketN_4d(0);
    PacketN_4d va = IVP_SELN_4XF64I(a, vzero, IVP_SELI_64B_ROTATE_RIGHT_8);
    PacketN_4d va2 = IVP_SELN_4XF64I(vzero, va, IVP_SELI_64B_ROTATE_LEFT_8);
    PacketN_4d vb = IVP_SELN_4XF64I(b, vzero, IVP_SELI_64B_ROTATE_RIGHT_8);
    PacketN_4d vout = va2 + vb;
  #endif
    return vout;
  }
  template <> EIGEN_STRONG_INLINE PacketN_4d replicate_N_8d(const PacketN_4d& a){
    return extract_N_8d<PacketN_4d,PacketN_4d>(a, a);
  }
#endif

template <> EIGEN_DEVICE_FUNC inline PacketN_4d pgather<double, PacketN_4d>(const double *from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  #if SIMD_N_4_EQ_2()
    xtdouble *tp = xt_reinterpret_const_cast<xtdouble,double>(from);
    xb_vecN_4xf64 tv0, tv1, tv2;
    tv0 = IVP_LSN_4XF64_X(tp, 0 * sizeof(xtdouble) * stride);
    tv1 = IVP_LSN_4XF64_X(tp, 1 * sizeof(xtdouble) * stride);
    tv2 = IVP_MOVN_4XF64_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XF64(tv1), IVP_MOVNX16_FROMN_4XF64(tv0), IVP_SELI_64B_INTERLEAVE_1_EVEN));
    return PacketN_4d(tv2);
  #else
    xtdouble *tp = xt_reinterpret_const_cast<xtdouble,double>(from);
    xb_vecN_4xf64 ta;
    xtdouble *tpf = xt_reinterpret_cast<xtdouble,xb_vecN_4xf64>(&ta);
    int    i;
    int    stride_bytes = sizeof(xtdouble) * stride;
    xb_vecN_4xf64 tvf;

    for (i = 0; i < SIMD_N_4; i++) {
      IVP_LSRN_4XF64_XP(tvf, tp, stride_bytes);
      IVP_SSN_4XF64_IP(tvf, tpf, sizeof(xtdouble));
    }
    return PacketN_4d(ta);
  #endif
}

template <> EIGEN_DEVICE_FUNC inline void pscatter<double, PacketN_4d>(double *to, const PacketN_4d &from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  #if SIMD_N_4_EQ_2()
    xtdouble *tpt = xt_reinterpret_cast<xtdouble,double>(to);
    tpt[stride * 0] = IVP_SELSN_4XF64(from.v, 0);
    tpt[stride * 1] = IVP_SELSN_4XF64(from.v, 1);
  #else
    int i;
    xtdouble *tpf = xt_reinterpret_const_cast<xtdouble,PacketN_4d>(&from);
    xtdouble *tpt = xt_reinterpret_cast<xtdouble,double>(to);
    int    stride_bytes = sizeof(xtdouble) * stride;
    xb_vecN_4xf64 tvf;
    for (i = 0; i < SIMD_N_4; i++)
    {
      IVP_LSRN_4XF64_IP(tvf, tpf, sizeof(xtdouble));
      IVP_SSN_4XF64_XP(tvf, tpt, stride_bytes);
    }
  #endif
}

/** \internal \returns the product of the elements of \a a*/
template <> EIGEN_STRONG_INLINE double predux_mul<PacketN_4d>(const PacketN_4d &a) {
  FUNC_ENTRY;
  #if SIMD_N_4_EQ_2()
    xb_vecN_4xf64 tv1 = a;
    xb_vecN_4xf64 tv2 = IVP_MOVN_4XF64_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XF64(tv1), IVP_MOVNX16_FROMN_4XF64(tv1), IVP_SELI_64B_ROTATE_LEFT_1));
    xb_vecN_4xf64 tv3 = IVP_MULN_4XF64(tv1, tv2);
    xtdouble t0 = IVP_SELSN_4XF64(tv3, 0);
    return *(xt_reinterpret_cast<double,xtdouble>(&t0));
  #elif SIMD_N_4_EQ_8()
    xb_vecN_4xf64 tv0, tv1;
    tv0 = a;
    tv1 = IVP_MULN_4XF64(tv0, IVP_MOVN_4XF64_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XF64(tv0), IVP_MOVNX16_FROMN_4XF64(tv0), IVP_SELI_64B_ROTATE_LEFT_4)));
    tv0 = IVP_MULN_4XF64(tv1, IVP_MOVN_4XF64_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XF64(tv1), IVP_MOVNX16_FROMN_4XF64(tv1), IVP_SELI_64B_ROTATE_LEFT_2)));
    tv1 = IVP_MULN_4XF64(tv0, IVP_MOVN_4XF64_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XF64(tv0), IVP_MOVNX16_FROMN_4XF64(tv0), IVP_SELI_64B_ROTATE_LEFT_1)));
    xtdouble t0 = IVP_SELSN_4XF64(tv1, 0);
    return *(xt_reinterpret_cast<double,xtdouble>(&t0));
  #elif SIMD_N_4_EQ_16()
    xb_vecN_4xf64 tv0, tv1;
    tv0 = a;
    tv1 = IVP_MULN_4XF64(tv0, IVP_MOVN_4XF64_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XF64(tv0), IVP_MOVNX16_FROMN_4XF64(tv0), IVP_SELI_64B_ROTATE_RIGHT_8)));
    tv0 = IVP_MULN_4XF64(tv1, IVP_MOVN_4XF64_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XF64(tv1), IVP_MOVNX16_FROMN_4XF64(tv1), IVP_SELI_64B_ROTATE_LEFT_4)));
    tv1 = IVP_MULN_4XF64(tv0, IVP_MOVN_4XF64_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XF64(tv0), IVP_MOVNX16_FROMN_4XF64(tv0), IVP_SELI_64B_ROTATE_LEFT_2)));
    tv0 = IVP_MULN_4XF64(tv1, IVP_MOVN_4XF64_FROMNX16(IVP_SELNX16I(IVP_MOVNX16_FROMN_4XF64(tv1), IVP_MOVNX16_FROMN_4XF64(tv1), IVP_SELI_64B_ROTATE_LEFT_1)));
    xtdouble t0 = IVP_SELSN_4XF64(tv0, 0);
    return *(xt_reinterpret_cast<double,xtdouble>(&t0));
  #endif
}

#if SIMD_N_2_EQ_16()
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4d, CONST_4> &kernel) {
    FUNC_ENTRY;
    // Declare vec
    #define DR(_var,_num) xb_vecNx16 _var##_num = BBE_MOVNX16_FROMN_4XF64(BBE_CONSTN_4XF64(0))
    // Load vec
    #define LR(_var,_num) _var##_num = BBE_MOVNX16_FROMN_4XF64(kernel.packet[_num])
    // Store vec
    #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = BBE_MOVN_4XF64_FROMNX16(_var##_src_num)

    // Declare vectors
    #define DECL_VEC(_var) DR(_var,0); DR(_var,1); DR(_var,2); DR(_var,3);

    #define VR16(_num) xb_vecNx16 t##_num;

    #define DECL_VEC16 VR16(0); VR16(1); VR16(2); VR16(3);

    // Load vectors
    #define LOAD_VEC(_var) LR(_var,0); LR(_var,1); LR(_var,2); LR(_var,3);

    // Store vectors
    #define STORE_VEC(_var) SR(_var,0,0); SR(_var,1,1); SR(_var,2,2); SR(_var,3,3);

    signed char ALIGN64 sel_32b_interleave_2[64] =
    {
      0,  16, 1,  17, 2,  18, 3,  19,
      32, 48, 33, 49, 34, 50, 35, 51,
      4,  20, 5,  21, 6,  22, 7,  23,
      36, 52, 37, 53, 38, 54, 39, 55,
      8,  24, 9,  25, 10, 26, 11, 27,
      40, 56, 41, 57, 42, 58, 43, 59,
      12, 28, 13, 29, 14, 30, 15, 31,
      44, 60, 45, 61, 46, 62, 47, 63
    };

    xb_vec2Nx8 vecselpat_32b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_2)); // interleave 2

    signed char ALIGN64 sel_32b_interleave_4[64] =
    {
      0,16,1,17,2,18,3,19,4,20,5,21,6,22,7,23,32,48,33,49,34,50,35,51,36,52,37,53,38,54,39,55,
      8,24,9,25,10,26,11,27,12,28,13,29,14,30,15,31,40,56,41,57,42,58,43,59,44,60,45,61,46,62,47,63
    };

    xb_vec2Nx8 vecselpat_32b_interleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_4)); // interleave 4


    /* This is first step in transpose operation. In first step, input vectors and
     * output vectors have same sequence. This will change in 'NEXT_STEP', where
     * output vectors will have the same sequence but input vector sequence will be
     * different. */
    #define FIRST_STEP(_pat, _out, _inp)       \
      t0 = (_out##0);  \
      t1 = (_out##1);  \
      t2 = (_out##2);  \
      t3 = (_out##3);  \
      IVP_DSELNX16(t1, t0, (_inp##1), (_inp##0), _pat);     \
      IVP_DSELNX16(t3, t2, (_inp##3), (_inp##2), _pat);     \
      _out##0 = (t0);  \
      _out##1 = (t1);  \
      _out##2 = (t2);  \
      _out##3 = (t3);  \

    /* This is subsequent step in transpose operation. The only difference between
     * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
     * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
    #define NEXT_STEP(_pat, _out, _inp)        \
      t0 = (_out##0);  \
      t1 = (_out##1);  \
      t2 = (_out##2);  \
      t3 = (_out##3);  \
      IVP_DSELNX16(t1, t0, (_inp##2), (_inp##0), _pat);    \
      IVP_DSELNX16(t3, t2, (_inp##3), (_inp##1), _pat);    \
      _out##0 = (t0);  \
      _out##1 = (t1);  \
      _out##2 = (t2);  \
      _out##3 = (t3);  \

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
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4d, CONST_8> &kernel) {
    FUNC_ENTRY;
    // Declare vec
    #define DR(_var,_num) xb_vecNx16 _var##_num = BBE_MOVNX16_FROMN_4XF64(BBE_CONSTN_4XF64(0))
    // Load vec
    #define LR(_var,_num) _var##_num = BBE_MOVNX16_FROMN_4XF64(kernel.packet[_num])
    // Store vec
    #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = BBE_MOVN_4XF64_FROMNX16(_var##_src_num)

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

    signed char ALIGN64 sel_32b_interleave_4[64] =
    {
      0,  16, 1,  17, 2,  18, 3,  19, 4,  20, 5,  21, 6,  22, 7,  23,
      32, 48, 33, 49, 34, 50, 35, 51, 36, 52, 37, 53, 38, 54, 39, 55,
      8,  24, 9,  25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31,
      40, 56, 41, 57, 42, 58, 43, 59, 44, 60, 45, 61, 46, 62, 47, 63
    };

    xb_vec2Nx8 vecselpat_32b_interleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_4)); // interleave 4

    signed char ALIGN64 sel_32b_interleave_2[64] =
    {
      0,  16, 1,  17, 2,  18, 3,  19,
      32, 48, 33, 49, 34, 50, 35, 51,
      4,  20, 5,  21, 6,  22, 7,  23,
      36, 52, 37, 53, 38, 54, 39, 55,
      8,  24, 9,  25, 10, 26, 11, 27,
      40, 56, 41, 57, 42, 58, 43, 59,
      12, 28, 13, 29, 14, 30, 15, 31,
      44, 60, 45, 61, 46, 62, 47, 63
    };

    xb_vec2Nx8 vecselpat_32b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_2)); // interleave 2
        
    signed char ALIGN64 sel_32b_interleave_8[64] =
    {
      0,  16, 1,  17, 2,  18, 3,  19, 4,  20, 5,  21, 6,  22, 7,  23, 8,  24, 9,  25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31,
      32, 48, 33, 49, 34, 50, 35, 51, 36, 52, 37, 53, 38, 54, 39, 55, 40, 56, 41, 57, 42, 58, 43, 59, 44, 60, 45, 61, 46, 62, 47, 63
    };

    xb_vec2Nx8 vecselpat_32b_interleave_8 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_8)); // interleave 8

    /* This is first step in transpose operation. In first step, input vectors and
     * output vectors have same sequence. This will change in 'NEXT_STEP', where
     * output vectors will have the same sequence but input vector sequence will be
     * different. */
    #define FIRST_STEP(_pat, _out, _inp)                              \
      t0 = (_out##0);  \
      t1 = (_out##1);  \
      t2 = (_out##2);  \
      t3 = (_out##3);  \
      t4 = (_out##4);  \
      t5 = (_out##5);  \
      t6 = (_out##6);  \
      t7 = (_out##7);  \
      IVP_DSELNX16(t1, t0, (_inp##1), (_inp##0), _pat);     \
      IVP_DSELNX16(t3, t2, (_inp##3), (_inp##2), _pat);     \
      IVP_DSELNX16(t5, t4, (_inp##5), (_inp##4), _pat);     \
      IVP_DSELNX16(t7, t6, (_inp##7), (_inp##6), _pat);     \
      _out##0 = (t0);  \
      _out##1 = (t1);  \
      _out##2 = (t2);  \
      _out##3 = (t3);  \
      _out##4 = (t4);  \
      _out##5 = (t5);  \
      _out##6 = (t6);  \
      _out##7 = (t7);  \

    /* This is subsequent step in transpose operation. The only difference between
     * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
     * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
    #define NEXT_STEP(_pat, _out, _inp)                               \
      t0 = (_out##0);  \
      t1 = (_out##1);  \
      t2 = (_out##2);  \
      t3 = (_out##3);  \
      t4 = (_out##4);  \
      t5 = (_out##5);  \
      t6 = (_out##6);  \
      t7 = (_out##7);  \
      IVP_DSELNX16(t1, t0, (_inp##2), (_inp##0), _pat); \
      IVP_DSELNX16(t3, t2, (_inp##6), (_inp##4), _pat); \
      IVP_DSELNX16(t5, t4, (_inp##3), (_inp##1), _pat); \
      IVP_DSELNX16(t7, t6, (_inp##7), (_inp##5), _pat); \
      _out##0 = (t0);  \
      _out##1 = (t1);  \
      _out##2 = (t2);  \
      _out##3 = (t3);  \
      _out##4 = (t4);  \
      _out##5 = (t5);  \
      _out##6 = (t6);  \
      _out##7 = (t7);  \

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
#elif SIMD_N_2_EQ_32()
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4d, CONST_4> &kernel) {
    FUNC_ENTRY;
    // Declare vec
    #define DR(_var,_num) xb_vecNx16 _var##_num = BBE_MOVNX16_FROMN_4XF64(BBE_CONSTN_4XF64(0))
    // Load vec
    #define LR(_var,_num) _var##_num = BBE_MOVNX16_FROMN_4XF64(kernel.packet[_num])
    // Store vec
    #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = BBE_MOVN_4XF64_FROMNX16(_var##_src_num)

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
      t0 = (_out##0);  \
      t1 = (_out##1);  \
      t2 = (_out##2);  \
      t3 = (_out##3);  \
      IVP_DSELNX16(t1, t0, (_inp##1), (_inp##0), _pat);     \
      IVP_DSELNX16(t3, t2, (_inp##3), (_inp##2), _pat);     \
      _out##0 = (t0);  \
      _out##1 = (t1);  \
      _out##2 = (t2);  \
      _out##3 = (t3);  \

    /* This is subsequent step in transpose operation. The only difference between
     * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
     * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
    #define NEXT_STEP(_pat, _out, _inp)        \
      t0 = (_out##0);  \
      t1 = (_out##1);  \
      t2 = (_out##2);  \
      t3 = (_out##3);  \
      IVP_DSELNX16(t1, t0, (_inp##2), (_inp##0), _pat);    \
      IVP_DSELNX16(t3, t2, (_inp##3), (_inp##1), _pat);    \
      _out##0 = (t0);  \
      _out##1 = (t1);  \
      _out##2 = (t2);  \
      _out##3 = (t3);  \

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
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4d, CONST_16> &kernel) {
    FUNC_ENTRY;
    // Declare vec
    #define DR(_var,_num) xb_vecNx16 _var##_num = BBE_MOVNX16_FROMN_4XF64(BBE_CONSTN_4XF64(0))
    // Load vec
    #define LR(_var,_num) _var##_num = BBE_MOVNX16_FROMN_4XF64(kernel.packet[_num])
    // Store vec
    #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num] = BBE_MOVN_4XF64_FROMNX16(_var##_src_num)

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
      t0 = (_out##0);  \
      t1 = (_out##1);  \
      t2 = (_out##2);  \
      t3 = (_out##3);  \
      t4 = (_out##4);  \
      t5 = (_out##5);  \
      t6 = (_out##6);  \
      t7 = (_out##7);  \
      t8 = (_out##8);  \
      t9 = (_out##9);  \
      t10= (_out##10); \
      t11= (_out##11); \
      t12= (_out##12); \
      t13= (_out##13); \
      t14= (_out##14); \
      t15= (_out##15); \
      IVP_DSELNX16(t1, t0, (_inp##1), (_inp##0), _pat);     \
      IVP_DSELNX16(t3, t2, (_inp##3), (_inp##2), _pat);     \
      IVP_DSELNX16(t5, t4, (_inp##5), (_inp##4), _pat);     \
      IVP_DSELNX16(t7, t6, (_inp##7), (_inp##6), _pat);     \
      IVP_DSELNX16(t9, t8, (_inp##9), (_inp##8), _pat);     \
      IVP_DSELNX16(t11, t10, (_inp##11), (_inp##10), _pat); \
      IVP_DSELNX16(t13, t12, (_inp##13), (_inp##12), _pat); \
      IVP_DSELNX16(t15, t14, (_inp##15), (_inp##14), _pat); \
      _out##0 = (t0);  \
      _out##1 = (t1);  \
      _out##2 = (t2);  \
      _out##3 = (t3);  \
      _out##4 = (t4);  \
      _out##5 = (t5);  \
      _out##6 = (t6);  \
      _out##7 = (t7);  \
      _out##8 = (t8);  \
      _out##9 = (t9);  \
      _out##10= (t10); \
      _out##11= (t11); \
      _out##12= (t12); \
      _out##13= (t13); \
      _out##14= (t14); \
      _out##15= (t15); \

    /* This is subsequent step in transpose operation. The only difference between
     * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
     * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
    #define NEXT_STEP(_pat, _out, _inp)        \
      t0 = (_out##0);  \
      t1 = (_out##1);  \
      t2 = (_out##2);  \
      t3 = (_out##3);  \
      t4 = (_out##4);  \
      t5 = (_out##5);  \
      t6 = (_out##6);  \
      t7 = (_out##7);  \
      t8 = (_out##8);  \
      t9 = (_out##9);  \
      t10= (_out##10); \
      t11= (_out##11); \
      t12= (_out##12); \
      t13= (_out##13); \
      t14= (_out##14); \
      t15= (_out##15); \
      IVP_DSELNX16(t1, t0, (_inp##2), (_inp##0), _pat);    \
      IVP_DSELNX16(t3, t2, (_inp##6), (_inp##4), _pat);    \
      IVP_DSELNX16(t5, t4, (_inp##10), (_inp##8), _pat);   \
      IVP_DSELNX16(t7, t6, (_inp##14), (_inp##12), _pat);  \
      IVP_DSELNX16(t9, t8, (_inp##3), (_inp##1), _pat);    \
      IVP_DSELNX16(t11, t10, (_inp##7), (_inp##5), _pat);  \
      IVP_DSELNX16(t13, t12, (_inp##11), (_inp##9), _pat); \
      IVP_DSELNX16(t15, t14, (_inp##15), (_inp##13), _pat);\
      _out##0 = (t0);  \
      _out##1 = (t1);  \
      _out##2 = (t2);  \
      _out##3 = (t3);  \
      _out##4 = (t4);  \
      _out##5 = (t5);  \
      _out##6 = (t6);  \
      _out##7 = (t7);  \
      _out##8 = (t8);  \
      _out##9 = (t9);  \
      _out##10= (t10); \
      _out##11= (t11); \
      _out##12= (t12); \
      _out##13= (t13); \
      _out##14= (t14); \
      _out##15= (t15); \

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

} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_F64_VX_XTENSA_H
