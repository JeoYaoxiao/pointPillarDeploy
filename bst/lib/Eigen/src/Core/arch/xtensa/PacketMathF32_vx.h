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

#ifndef EIGEN_PACKET_MATH_F32_VX_XTENSA_H
#define EIGEN_PACKET_MATH_F32_VX_XTENSA_H

#if F32_FULL_PACKET_SUPPORT
namespace Eigen {

namespace internal {

template <> EIGEN_DEVICE_FUNC inline PacketN_2f pgather<float, PacketN_2f>(const float *from, Index stride) {
  FUNC_ENTRY;
  float *tp = (float *)(from);
  xb_vecN_2xf32  ta;
  float *tpf = (float *)(&ta);
  int    i;
  int    stride_bytes = sizeof(float) * stride;
  float tvf;
  for (i = 0; i < SIMD_N_2; i++) {
    IVP_LSN_2XF32_XP(tvf, tp, stride_bytes);
    IVP_SSN_2XF32_IP(tvf, tpf, sizeof(float));
  }
  return ta;
}
template <> EIGEN_DEVICE_FUNC inline void pscatter<float, PacketN_2f>(float *to, const PacketN_2f &from, Index stride) {
  FUNC_ENTRY;
  float *tpf = (float*)(&from);
  float *tpt = (float*)(to);
  int    stride_bytes = sizeof(float) * stride;
  float tvf;
  int i;
  for (i = 0; i < SIMD_N_2; i++)
  {
    IVP_LSN_2XF32_IP(tvf, tpf, sizeof(float));
    IVP_SSN_2XF32_XP(tvf, tpt, stride_bytes);
  } 
}

#if SIMD_N_2_EQ_4()
  template <> EIGEN_STRONG_INLINE float predux_mul<PacketN_2f>(const PacketN_2f &a) {
    FUNC_ENTRY;
    xb_vecN_2xf32 vecout;
    vecout = a;
    xb_vecN_2xf32 veczero = IVP_ZERON_2XF32();
    xb_vecN_2xf32 vecout_1 = IVP_MULN_2XF32(vecout, IVP_SELN_2XF32I(vecout, veczero, IVP_SELI_32B_ROTATE_LEFT_1));
    xb_vecN_2xf32 vecout_2 = IVP_MULN_2XF32(vecout_1, IVP_SELN_2XF32I(vecout_1, veczero, IVP_SELI_32B_ROTATE_LEFT_2));
    return IVP_SELSN_2XF32(vecout_2, 3);
  }
#elif SIMD_N_2_EQ_16()
  /** \internal \returns the product of the elements of \a a*/
  template <> EIGEN_STRONG_INLINE float predux_mul<PacketN_2f>(const PacketN_2f &a) {
    FUNC_ENTRY;
    xb_vecN_2xf32 vecout;
    vecout = a;
    xb_vecN_2xf32 veczero = IVP_ZERON_2XF32();
    xb_vecN_2xf32 vecout_1 = IVP_MULN_2XF32(vecout, IVP_SELN_2XF32I(vecout, veczero, IVP_SELI_32B_ROTATE_LEFT_1));
    xb_vecN_2xf32 vecout_2 = IVP_MULN_2XF32(vecout_1, IVP_SELN_2XF32I(vecout_1, veczero, IVP_SELI_32B_ROTATE_LEFT_2));
    xb_vecN_2xf32 vecout_3 = IVP_MULN_2XF32(vecout_2, IVP_SELN_2XF32I(vecout_2, veczero, IVP_SELI_32B_ROTATE_LEFT_4));
    xb_vecN_2xf32 vecout_4 = IVP_MULN_2XF32(vecout_3, IVP_SELN_2XF32I(vecout_3, veczero, IVP_SELI_32B_ROTATE_LEFT_8));
    return IVP_SELSN_2XF32(vecout_4, 15);
  }
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_4> &kernel) {
    FUNC_ENTRY;
    xb_vecN_2xf32  tv0, tv1, tv2, tv3;
    xb_vecN_2xf32  tvT0, tvT1, tvT2, tvT3;
    xb_vecN_2xf32  tvS0, tvS1, tvS2, tvS3;

    tvT0 = kernel.packet[0];
    tvT1 = kernel.packet[1];
    tvT2 = kernel.packet[2];
    tvT3 = kernel.packet[3];

    signed char ALIGN64 sel_32b_interleave_4[64] =
    {
      0,16,1,17,2,18,3,19,4,20,5,21,6,22,7,23,32,48,33,49,34,50,35,51,36,52,37,53,38,54,39,55,
      8,24,9,25,10,26,11,27,12,28,13,29,14,30,15,31,40,56,41,57,42,58,43,59,44,60,45,61,46,62,47,63
    };
    xb_vec2Nx8 vecselpat_32b_interleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_4)); // interleave 4
    
    signed char ALIGN64 sel_32b_interleave_8[64]  =
    {
      0,16,1,17,2,18,3,19,4,20,5,21,6,22,7,23,8,24,9,25,10,26,11,27,12,28,13,29,14,30,15,31,
      32,48,33,49,34,50,35,51,36,52,37,53,38,54,39,55,40,56,41,57,42,58,43,59,44,60,45,61,46,62,47,63
    };
    xb_vec2Nx8 vecselpat_32b_interleave_8 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_8)); // interleave 8

    signed char ALIGN64 sel_32b_deinterleave_4[64]  =
    {
      0,8,1,9,2,10,3,11,4,12,5,13,6,14,7,15,16,24,17,25,18,26,19,27,20,28,21,29,22,30,23,31,
      32,40,33,41,34,42,35,43,36,44,37,45,38,46,39,47,48,56,49,57,50,58,51,59,52,60,53,61,54,62,55,63
    };
   
    xb_vec2Nx8 vecselpat_32b_deinterleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_deinterleave_4)); // de-interleave 4
    xb_vecNx16 t0,t1,t2,t3;
    IVP_DSELN_2XF32I(tv1, tv0, tvT1, tvT0, IVP_DSELI_32B_INTERLEAVE_1);
    IVP_DSELNX16(t1, t0,IVP_MOVNX16_FROMN_2XF32(tv1), IVP_MOVNX16_FROMN_2XF32(tv0), vecselpat_32b_deinterleave_4);

    IVP_DSELN_2XF32I(tv3, tv2, tvT3, tvT2, IVP_DSELI_32B_INTERLEAVE_1);
    IVP_DSELNX16(t3, t2,IVP_MOVNX16_FROMN_2XF32(tv3), IVP_MOVNX16_FROMN_2XF32(tv2), vecselpat_32b_deinterleave_4);

    tvT0 = IVP_MOVN_2XF32_FROMNX16(t0);
    tvT1 = IVP_MOVN_2XF32_FROMNX16(t1);
    tvT2 = IVP_MOVN_2XF32_FROMNX16(t2);
    tvT3 = IVP_MOVN_2XF32_FROMNX16(t3);

    IVP_DSELN_2XF32I(tv1, tv0, tvT2, tvT0, IVP_DSELI_32B_INTERLEAVE_2);
    IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_2XF32(tv1), IVP_MOVNX16_FROMN_2XF32(tv0), vecselpat_32b_deinterleave_4);
    IVP_DSELN_2XF32I(tv3, tv2, tvT3, tvT1, IVP_DSELI_32B_INTERLEAVE_2);
    IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_2XF32(tv3),  IVP_MOVNX16_FROMN_2XF32(tv2), vecselpat_32b_deinterleave_4);

    tvS0 = IVP_MOVN_2XF32_FROMNX16(t0);
    tvS1 = IVP_MOVN_2XF32_FROMNX16(t1);
    tvS2 = IVP_MOVN_2XF32_FROMNX16(t2);
    tvS3 = IVP_MOVN_2XF32_FROMNX16(t3);

    IVP_DSELNX16(t1, t0,IVP_MOVNX16_FROMN_2XF32(tvS1), IVP_MOVNX16_FROMN_2XF32(tvS0), vecselpat_32b_interleave_4);
    IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_2XF32(tvS3), IVP_MOVNX16_FROMN_2XF32(tvS2), vecselpat_32b_interleave_4);

    tv0 = IVP_MOVN_2XF32_FROMNX16(t0);
    tv1 = IVP_MOVN_2XF32_FROMNX16(t1);
    tv2 = IVP_MOVN_2XF32_FROMNX16(t2);
    tv3 = IVP_MOVN_2XF32_FROMNX16(t3);

    IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_2XF32(tv2), IVP_MOVNX16_FROMN_2XF32(tv0), vecselpat_32b_interleave_8);
    IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_2XF32(tv3), IVP_MOVNX16_FROMN_2XF32(tv1), vecselpat_32b_interleave_8);

    tvS0 = IVP_MOVN_2XF32_FROMNX16(t0);
    tvS1 = IVP_MOVN_2XF32_FROMNX16(t1);
    tvS2 = IVP_MOVN_2XF32_FROMNX16(t2);
    tvS3 = IVP_MOVN_2XF32_FROMNX16(t3);

    kernel.packet[0] = tvS0;
    kernel.packet[1] = tvS1;
    kernel.packet[2] = tvS2;
    kernel.packet[3] = tvS3;
  }
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_16> &kernel) {
    FUNC_ENTRY;
    // Declare vectors
    #define VR(_num)         \
      xb_vecN_2xf32 d##_num; \
      xb_vecN_2xf32 tmp_d##_num;

    #define VR16(_num) xb_vecNx16 t##_num;  

    #define DECL_VEC                      \
      VR(0)  ; VR(1)  ; VR(2)  ; VR(3)  ; \
      VR(4)  ; VR(5)  ; VR(6)  ; VR(7)  ; \
      VR(8)  ; VR(9)  ; VR(10) ; VR(11) ; \
      VR(12) ; VR(13) ; VR(14) ; VR(15) ; \

    #define DECL_VEC16 \
      VR16(0)  ; VR16(1)  ; VR16(2)  ; VR16(3)  ; \
      VR16(4)  ; VR16(5)  ; VR16(6)  ; VR16(7)  ; \
      VR16(8)  ; VR16(9)  ; VR16(10) ; VR16(11) ; \
      VR16(12) ; VR16(13) ; VR16(14) ; VR16(15) ; \
     
    DECL_VEC;
    DECL_VEC16;
    
    signed char ALIGN64 sel_32b_interleave_4[64] =
    {
      0,16,1,17,2,18,3,19,4,20,5,21,6,22,7,23,32,48,33,49,34,50,35,51,36,52,37,53,38,54,39,55,
      8,24,9,25,10,26,11,27,12,28,13,29,14,30,15,31,40,56,41,57,42,58,43,59,44,60,45,61,46,62,47,63
    };
    xb_vec2Nx8 vecselpat_32b_interleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_4));

    signed char ALIGN64 sel_32b_interleave_8[64]  =
    {
      0,16,1,17,2,18,3,19,4,20,5,21,6,22,7,23,8,24,9,25,10,26,11,27,12,28,13,29,14,30,15,31,
      32,48,33,49,34,50,35,51,36,52,37,53,38,54,39,55,40,56,41,57,42,58,43,59,44,60,45,61,46,62,47,63
    };
    xb_vec2Nx8 vecselpat_32b_interleave_8 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_8));


    // Load vec and increment pointer to next row
    #define XR(_num) d##_num = kernel.packet[_num]

    #define LOAD_VEC                      \
      XR(0)  ; XR(1)  ; XR(2)  ; XR(3)  ; \
      XR(4)  ; XR(5)  ; XR(6)  ; XR(7)  ; \
      XR(8)  ; XR(9)  ; XR(10) ; XR(11) ; \
      XR(12) ; XR(13) ; XR(14) ; XR(15) ; \
    
    LOAD_VEC;

    /* First step in transpose operation. Since this first step, thus input vectors
    * and output vectors have same sequence. This will change in 'NEXT_STEP',
    * where output vectors will have the same sequence but input vector sequence
    * will be different. */
    #define FIRST_STEP(_pat, _out_vec, _inp_vec)                                      \
      IVP_DSELN_2XF32I(_out_vec##1, _out_vec##0, _inp_vec##1, _inp_vec##0, _pat);     \
      IVP_DSELN_2XF32I(_out_vec##3, _out_vec##2, _inp_vec##3, _inp_vec##2, _pat);     \
      IVP_DSELN_2XF32I(_out_vec##5, _out_vec##4, _inp_vec##5, _inp_vec##4, _pat);     \
      IVP_DSELN_2XF32I(_out_vec##7, _out_vec##6, _inp_vec##7, _inp_vec##6, _pat);     \
      IVP_DSELN_2XF32I(_out_vec##9, _out_vec##8, _inp_vec##9, _inp_vec##8, _pat);     \
      IVP_DSELN_2XF32I(_out_vec##11, _out_vec##10, _inp_vec##11, _inp_vec##10, _pat); \
      IVP_DSELN_2XF32I(_out_vec##13, _out_vec##12, _inp_vec##13, _inp_vec##12, _pat); \
      IVP_DSELN_2XF32I(_out_vec##15, _out_vec##14, _inp_vec##15, _inp_vec##14, _pat);  

    FIRST_STEP(IVP_DSELI_32B_INTERLEAVE_1, tmp_d, d);

    /* Subsequent step in transpose operation. The only difference between this in
     * and 'FIRST_STEP' is the sequence used for input vectors. Output vectors
     * sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
    #define NEXT_STEP(_pat, _out_vec, _inp_vec)                                      \
      IVP_DSELN_2XF32I(_out_vec##1, _out_vec##0, _inp_vec##2, _inp_vec##0, _pat);    \
      IVP_DSELN_2XF32I(_out_vec##3, _out_vec##2, _inp_vec##6, _inp_vec##4, _pat);    \
      IVP_DSELN_2XF32I(_out_vec##5, _out_vec##4, _inp_vec##10, _inp_vec##8, _pat);   \
      IVP_DSELN_2XF32I(_out_vec##7, _out_vec##6, _inp_vec##14, _inp_vec##12, _pat);  \
      IVP_DSELN_2XF32I(_out_vec##9, _out_vec##8, _inp_vec##3, _inp_vec##1, _pat);    \
      IVP_DSELN_2XF32I(_out_vec##11, _out_vec##10, _inp_vec##7, _inp_vec##5, _pat);  \
      IVP_DSELN_2XF32I(_out_vec##13, _out_vec##12, _inp_vec##11, _inp_vec##9, _pat); \
      IVP_DSELN_2XF32I(_out_vec##15, _out_vec##14, _inp_vec##15, _inp_vec##13, _pat);

    NEXT_STEP(IVP_DSELI_32B_INTERLEAVE_2, d, tmp_d);

    #define NEXT_STEP1(_pat, _out_vec, _inp_vec)   \
      t0  = IVP_MOVNX16_FROMN_2XF32(_out_vec##0);  \
      t1  = IVP_MOVNX16_FROMN_2XF32(_out_vec##1);  \
      t2  = IVP_MOVNX16_FROMN_2XF32(_out_vec##2);  \
      t3  = IVP_MOVNX16_FROMN_2XF32(_out_vec##3);  \
      t4  = IVP_MOVNX16_FROMN_2XF32(_out_vec##4);  \
      t5  = IVP_MOVNX16_FROMN_2XF32(_out_vec##5);  \
      t6  = IVP_MOVNX16_FROMN_2XF32(_out_vec##6);  \
      t7  = IVP_MOVNX16_FROMN_2XF32(_out_vec##7);  \
      t8  = IVP_MOVNX16_FROMN_2XF32(_out_vec##8);  \
      t9  = IVP_MOVNX16_FROMN_2XF32(_out_vec##9);  \
      t10  = IVP_MOVNX16_FROMN_2XF32(_out_vec##10);\
      t11  = IVP_MOVNX16_FROMN_2XF32(_out_vec##11);\
      t12  = IVP_MOVNX16_FROMN_2XF32(_out_vec##12);\
      t13  = IVP_MOVNX16_FROMN_2XF32(_out_vec##13);\
      t14  = IVP_MOVNX16_FROMN_2XF32(_out_vec##14);\
      t15  = IVP_MOVNX16_FROMN_2XF32(_out_vec##15);\
      IVP_DSELNX16(t1, t0  ,IVP_MOVNX16_FROMN_2XF32(_inp_vec##2 ), IVP_MOVNX16_FROMN_2XF32(_inp_vec##0 ), _pat);\
      IVP_DSELNX16(t3, t2  ,IVP_MOVNX16_FROMN_2XF32(_inp_vec##6 ), IVP_MOVNX16_FROMN_2XF32(_inp_vec##4 ), _pat);\
      IVP_DSELNX16(t5, t4  ,IVP_MOVNX16_FROMN_2XF32(_inp_vec##10), IVP_MOVNX16_FROMN_2XF32(_inp_vec##8 ), _pat);\
      IVP_DSELNX16(t7, t6  ,IVP_MOVNX16_FROMN_2XF32(_inp_vec##14), IVP_MOVNX16_FROMN_2XF32(_inp_vec##12), _pat);\
      IVP_DSELNX16(t9, t8  ,IVP_MOVNX16_FROMN_2XF32(_inp_vec##3 ), IVP_MOVNX16_FROMN_2XF32(_inp_vec##1 ), _pat);\
      IVP_DSELNX16(t11,t10 ,IVP_MOVNX16_FROMN_2XF32(_inp_vec##7 ), IVP_MOVNX16_FROMN_2XF32(_inp_vec##5 ), _pat);\
      IVP_DSELNX16(t13,t12 ,IVP_MOVNX16_FROMN_2XF32(_inp_vec##11), IVP_MOVNX16_FROMN_2XF32(_inp_vec##9 ), _pat);\
      IVP_DSELNX16(t15,t14 ,IVP_MOVNX16_FROMN_2XF32(_inp_vec##15), IVP_MOVNX16_FROMN_2XF32(_inp_vec##13), _pat);\
      _out_vec##0   = IVP_MOVN_2XF32_FROMNX16(t0);  \
      _out_vec##1   = IVP_MOVN_2XF32_FROMNX16(t1);  \
      _out_vec##2   = IVP_MOVN_2XF32_FROMNX16(t2);  \
      _out_vec##3   = IVP_MOVN_2XF32_FROMNX16(t3);  \
      _out_vec##4   = IVP_MOVN_2XF32_FROMNX16(t4);  \
      _out_vec##5   = IVP_MOVN_2XF32_FROMNX16(t5);  \
      _out_vec##6   = IVP_MOVN_2XF32_FROMNX16(t6);  \
      _out_vec##7   = IVP_MOVN_2XF32_FROMNX16(t7);  \
      _out_vec##8   = IVP_MOVN_2XF32_FROMNX16(t8);  \
      _out_vec##9   = IVP_MOVN_2XF32_FROMNX16(t9);  \
      _out_vec##10  = IVP_MOVN_2XF32_FROMNX16(t10); \
      _out_vec##11  = IVP_MOVN_2XF32_FROMNX16(t11); \
      _out_vec##12  = IVP_MOVN_2XF32_FROMNX16(t12); \
      _out_vec##13  = IVP_MOVN_2XF32_FROMNX16(t13); \
      _out_vec##14  = IVP_MOVN_2XF32_FROMNX16(t14); \
      _out_vec##15  = IVP_MOVN_2XF32_FROMNX16(t15);
    
    NEXT_STEP1(vecselpat_32b_interleave_4, tmp_d, d);
    NEXT_STEP1(vecselpat_32b_interleave_8, d, tmp_d);

    // Store vec and increment pointer to next row
    #define SR(_nums,_num) kernel.packet[_nums] = d##_num

    #define STORE_VEC                   \
      SR(0,0) ; SR(1,1) ; SR(2,8)   ; SR(3,9)  ; \
      SR(4,4) ; SR(5,5) ; SR(6,12)  ; SR(7,13) ; \
      SR(8,2) ; SR(9,3) ; SR(10,10) ; SR(11,11); \
      SR(12,6); SR(13,7); SR(14,14) ; SR(15,15); \

    STORE_VEC;
    #undef VR
    #undef VR16
    #undef DECL_VEC
    #undef DECL_VEC16
    #undef XR
    #undef LOAD_VEC
    #undef FIRST_STEP
    #undef NEXT_STEP
    #undef NEXT_STEP1
    #undef SR
    #undef STORE_VEC
  }
#elif SIMD_N_2_EQ_32()
  template <> EIGEN_STRONG_INLINE float predux_mul<PacketN_2f>(const PacketN_2f &a) {
    FUNC_ENTRY;
    xb_vecN_2xf32 vecout;
    vecout = a;
    
    xb_vecN_2xf32 veczero = IVP_ZERON_2XF32();
    xb_vecN_2xf32 vecout_1 = IVP_MULN_2XF32(vecout, IVP_SELN_2XF32I(vecout, veczero, IVP_SELI_32B_ROTATE_LEFT_1));
    xb_vecN_2xf32 vecout_2 = IVP_MULN_2XF32(vecout_1, IVP_SELN_2XF32I(vecout_1, veczero, IVP_SELI_32B_ROTATE_LEFT_2));
    xb_vecN_2xf32 vecout_3 = IVP_MULN_2XF32(vecout_2, IVP_SELN_2XF32I(vecout_2, veczero, IVP_SELI_32B_ROTATE_LEFT_4));
    xb_vecN_2xf32 vecout_4 = IVP_MULN_2XF32(vecout_3, IVP_SELN_2XF32I(vecout_3, veczero, IVP_SELI_32B_ROTATE_LEFT_8));
    xb_vecN_2xf32 vecout_5 = IVP_MULN_2XF32(vecout_4, IVP_SELN_2XF32I(vecout_4, veczero, IVP_SELI_32B_ROTATE_LEFT_16));
    return IVP_SELSN_2XF32(vecout_5, 31);

  }
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_4> &kernel) {
    FUNC_ENTRY;
    xb_vecNx16 t0,t1,t2,t3, tv1, tv0, tv3, tv2, t4;
    xb_vecN_2xf32  tvT0, tvT1, tvT2, tvT3;

    tvT0 = kernel.packet[0];
    tvT1 = kernel.packet[1];
    tvT2 = kernel.packet[2];
    tvT3 = kernel.packet[3];

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

    signed char ALIGN128 sel_32b_deinterleave_4[128]  =
    {
      0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15, 16, 24, 17, 25, 18, 26, 19, 27, 20, 28, 21, 29, 22, 30, 23, 31, 32, 40, 33, 41, 34, 42, 35, 43, 36, 44, 37, 45, 38, 46, 39, 47, 48, 56, 49, 57, 50, 58, 51, 59, 52, 60, 53, 61, 54, 62, 55, 63,
      64, 72, 65, 73, 66, 74, 67, 75, 68, 76, 69, 77, 70, 78, 71, 79, 80, 88, 81, 89, 82, 90, 83, 91, 84, 92, 85, 93, 86, 94, 87, 95, 96, 104, 97, 105, 98, 106, 99, 107, 100, 108, 101, 109, 102, 110, 103, 111, 112, 120, 113, 121, 114, 122, 115, 123, 116, 124, 117, 125, 118, 126, 119, 127
    };

    xb_vec2Nx8 vecselpat_32b_deinterleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_deinterleave_4)); // de-interleave 4


    signed char ALIGN128 sel_32b_interleave_2[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 64, 96, 65, 97, 66, 98, 67, 99, 4, 36, 5, 37, 6, 38, 7, 39, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 72, 104, 73, 105, 74, 106, 75, 107, 12, 44, 13, 45, 14, 46, 15, 47, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 80, 112, 81, 113, 82, 114, 83, 115, 20, 52, 21, 53, 22, 54, 23, 55, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 88, 120, 89, 121, 90, 122, 91, 123, 28, 60, 29, 61, 30, 62, 31, 63, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_2)); // interleave 2
    
    signed char ALIGN128 sel_32b_interleave_1[128]  =
    {
      0, 32, 1, 33, 64, 96, 65, 97, 2, 34, 3, 35, 66, 98, 67, 99, 4, 36, 5, 37, 68, 100, 69, 101, 6, 38, 7, 39, 70, 102, 71, 103, 8, 40, 9, 41, 72, 104, 73, 105, 10, 42, 11, 43, 74, 106, 75, 107, 12, 44, 13, 45, 76, 108, 77, 109, 14, 46, 15, 47, 78, 110, 79, 111,
      16, 48, 17, 49, 80, 112, 81, 113, 18, 50, 19, 51, 82, 114, 83, 115, 20, 52, 21, 53, 84, 116, 85, 117, 22, 54, 23, 55, 86, 118, 87, 119, 24, 56, 25, 57, 88, 120, 89, 121, 26, 58, 27, 59, 90, 122, 91, 123, 28, 60, 29, 61, 92, 124, 93, 125, 30, 62, 31, 63, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_1 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_1)); // interleave 1


    IVP_DSELNX16(tv1, tv0, IVP_MOVNX16_FROMN_2XF32(tvT1), IVP_MOVNX16_FROMN_2XF32(tvT0), vecselpat_32b_interleave_1);
    IVP_DSELNX16(t1, t0, tv1, tv0, vecselpat_32b_deinterleave_4);
    
    IVP_DSELNX16(tv3, tv2, IVP_MOVNX16_FROMN_2XF32(tvT3), IVP_MOVNX16_FROMN_2XF32(tvT2), vecselpat_32b_interleave_1);
    IVP_DSELNX16(t3, t2, tv3, tv2, vecselpat_32b_deinterleave_4);

    IVP_DSELNX16(tv1, tv0, t2, t0, vecselpat_32b_interleave_2);
    IVP_DSELNX16(t4, t0, tv1, tv0, vecselpat_32b_deinterleave_4);
    
    IVP_DSELNX16(tv3, tv2, t3, t1, vecselpat_32b_interleave_2);
    IVP_DSELNX16(t3, t2, tv3, tv2, vecselpat_32b_deinterleave_4);
    t1=t4;

    IVP_DSELNX16(tv1, tv0, t1, t0, vecselpat_32b_interleave_4);
    IVP_DSELNX16(tv3, tv2, t3, t2, vecselpat_32b_interleave_4);

    IVP_DSELNX16(t1, t0, tv2, tv0, vecselpat_32b_interleave_8);
    IVP_DSELNX16(t3, t2, tv3, tv1, vecselpat_32b_interleave_8);

    kernel.packet[0] = IVP_MOVN_2XF32_FROMNX16(t0);
    kernel.packet[1] = IVP_MOVN_2XF32_FROMNX16(t1);
    kernel.packet[2] = IVP_MOVN_2XF32_FROMNX16(t2);
    kernel.packet[3] = IVP_MOVN_2XF32_FROMNX16(t3);

  }
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_2f, CONST_32> &kernel) {
    FUNC_ENTRY;
    // Declare vectors
    #define VR(_num)             \
      xb_vecN_2xf32 d##_num;     \
      xb_vecN_2xf32 tmp_d##_num; \

    #define VR16(_num)       \
      xb_vecNx16 t_inp##_num;\
      xb_vecNx16 t_out##_num;\
      

    #define DECL_VEC                      \
      VR(0)  ; VR(1)  ; VR(2)  ; VR(3)  ; \
      VR(4)  ; VR(5)  ; VR(6)  ; VR(7)  ; \
      VR(8)  ; VR(9)  ; VR(10) ; VR(11) ; \
      VR(12) ; VR(13) ; VR(14) ; VR(15) ; \
      VR(16) ; VR(17) ; VR(18) ; VR(19) ; \
      VR(20) ; VR(21) ; VR(22) ; VR(23) ; \
      VR(24) ; VR(25) ; VR(26) ; VR(27) ; \
      VR(28) ; VR(29) ; VR(30) ; VR(31) ; \
   
    #define DECL_VEC16                            \
      VR16(0)  ; VR16(1)  ; VR16(2)  ; VR16(3)  ; \
      VR16(4)  ; VR16(5)  ; VR16(6)  ; VR16(7)  ; \
      VR16(8)  ; VR16(9)  ; VR16(10) ; VR16(11) ; \
      VR16(12) ; VR16(13) ; VR16(14) ; VR16(15) ; \
      VR16(16) ; VR16(17) ; VR16(18) ; VR16(19) ; \
      VR16(20) ; VR16(21) ; VR16(22) ; VR16(23) ; \
      VR16(24) ; VR16(25) ; VR16(26) ; VR16(27) ; \
      VR16(28) ; VR16(29) ; VR16(30) ; VR16(31) ; \

    DECL_VEC;
    DECL_VEC16;
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
    
    signed char ALIGN128 sel_32b_interleave_1[128]  =
    {
      0, 32, 1, 33, 64, 96, 65, 97, 2, 34, 3, 35, 66, 98, 67, 99, 4, 36, 5, 37, 68, 100, 69, 101, 6, 38, 7, 39, 70, 102, 71, 103, 8, 40, 9, 41, 72, 104, 73, 105, 10, 42, 11, 43, 74, 106, 75, 107, 12, 44, 13, 45, 76, 108, 77, 109, 14, 46, 15, 47, 78, 110, 79, 111,
      16, 48, 17, 49, 80, 112, 81, 113, 18, 50, 19, 51, 82, 114, 83, 115, 20, 52, 21, 53, 84, 116, 85, 117, 22, 54, 23, 55, 86, 118, 87, 119, 24, 56, 25, 57, 88, 120, 89, 121, 26, 58, 27, 59, 90, 122, 91, 123, 28, 60, 29, 61, 92, 124, 93, 125, 30, 62, 31, 63, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_32b_interleave_1 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_1)); // interleave 1
  
    
    // Load vec and increment pointer to next row
    #define XR(_num) d##_num = kernel.packet[_num]

    #define LOAD_VEC                      \
      XR(0)  ; XR(1)  ; XR(2)  ; XR(3)  ; \
      XR(4)  ; XR(5)  ; XR(6)  ; XR(7)  ; \
      XR(8)  ; XR(9)  ; XR(10) ; XR(11) ; \
      XR(12) ; XR(13) ; XR(14) ; XR(15) ; \
      XR(16) ; XR(17) ; XR(18) ; XR(19) ; \
      XR(20) ; XR(21) ; XR(22) ; XR(23) ; \
      XR(24) ; XR(25) ; XR(26) ; XR(27) ; \
      XR(28) ; XR(29) ; XR(30) ; XR(31) ; \

    /* Macro for converting input vector to NX16 */
    #define XINP(_iv, _num)                                   \
      t_inp##_num = IVP_MOVNX16_FROMN_2XF32(_iv##_num);

    #define CONVERT_TO_NX16_FROMN_2XF32(_iv)                  \
      XINP(_iv,0);  XINP(_iv,1);  XINP(_iv,2);  XINP(_iv,3);  \
      XINP(_iv,4);  XINP(_iv,5);  XINP(_iv,6);  XINP(_iv,7);  \
      XINP(_iv,8);  XINP(_iv,9);  XINP(_iv,10); XINP(_iv,11); \
      XINP(_iv,12); XINP(_iv,13); XINP(_iv,14); XINP(_iv,15); \
      XINP(_iv,16); XINP(_iv,17); XINP(_iv,18); XINP(_iv,19); \
      XINP(_iv,20); XINP(_iv,21); XINP(_iv,22); XINP(_iv,23); \
      XINP(_iv,24); XINP(_iv,25); XINP(_iv,26); XINP(_iv,27); \
      XINP(_iv,28); XINP(_iv,29); XINP(_iv,30); XINP(_iv,31); \
        

    /* Macro for converting output vector to N_2XF32 */
    #define XOUT(_ov,_num)                                    \
      _ov##_num   = IVP_MOVN_2XF32_FROMNX16(t_out##_num);     \

    #define CONVERT_TO_N_2XF32_FROMNX16(_ov)                  \
      XOUT(_ov,0);  XOUT(_ov,1);  XOUT(_ov,2);  XOUT(_ov,3);  \
      XOUT(_ov,4);  XOUT(_ov,5);  XOUT(_ov,6);  XOUT(_ov,7);  \
      XOUT(_ov,8);  XOUT(_ov,9);  XOUT(_ov,10); XOUT(_ov,11); \
      XOUT(_ov,12); XOUT(_ov,13); XOUT(_ov,14); XOUT(_ov,15); \
      XOUT(_ov,16); XOUT(_ov,17); XOUT(_ov,18); XOUT(_ov,19); \
      XOUT(_ov,20); XOUT(_ov,21); XOUT(_ov,22); XOUT(_ov,23); \
      XOUT(_ov,24); XOUT(_ov,25); XOUT(_ov,26); XOUT(_ov,27); \
      XOUT(_ov,28); XOUT(_ov,29); XOUT(_ov,30); XOUT(_ov,31); \

    /* First step in transpose operation. Since this first step, thus input vectors
     * and output vectors have same sequence. This will change in 'NEXT_STEP',
     * where output vectors will have the same sequence but input vector sequence
     * will be different. */

    #define FIRST_STEP(_pat, _out_vec, _inp_vec)                                  \
      IVP_DSELNX16(_out_vec##1,  _out_vec##0,  _inp_vec##1,  _inp_vec##0,  _pat); \
      IVP_DSELNX16(_out_vec##3,  _out_vec##2,  _inp_vec##3,  _inp_vec##2,  _pat); \
      IVP_DSELNX16(_out_vec##5,  _out_vec##4,  _inp_vec##5,  _inp_vec##4,  _pat); \
      IVP_DSELNX16(_out_vec##7,  _out_vec##6,  _inp_vec##7,  _inp_vec##6,  _pat); \
      IVP_DSELNX16(_out_vec##9,  _out_vec##8,  _inp_vec##9,  _inp_vec##8,  _pat); \
      IVP_DSELNX16(_out_vec##11, _out_vec##10, _inp_vec##11, _inp_vec##10, _pat); \
      IVP_DSELNX16(_out_vec##13, _out_vec##12, _inp_vec##13, _inp_vec##12, _pat); \
      IVP_DSELNX16(_out_vec##15, _out_vec##14, _inp_vec##15, _inp_vec##14, _pat); \
      IVP_DSELNX16(_out_vec##17, _out_vec##16, _inp_vec##17, _inp_vec##16, _pat); \
      IVP_DSELNX16(_out_vec##19, _out_vec##18, _inp_vec##19, _inp_vec##18, _pat); \
      IVP_DSELNX16(_out_vec##21, _out_vec##20, _inp_vec##21, _inp_vec##20, _pat); \
      IVP_DSELNX16(_out_vec##23, _out_vec##22, _inp_vec##23, _inp_vec##22, _pat); \
      IVP_DSELNX16(_out_vec##25, _out_vec##24, _inp_vec##25, _inp_vec##24, _pat); \
      IVP_DSELNX16(_out_vec##27, _out_vec##26, _inp_vec##27, _inp_vec##26, _pat); \
      IVP_DSELNX16(_out_vec##29, _out_vec##28, _inp_vec##29, _inp_vec##28, _pat); \
      IVP_DSELNX16(_out_vec##31, _out_vec##30, _inp_vec##31, _inp_vec##30, _pat); \
      
    #define NEXT_STEP(_pat, _out_vec, _inp_vec)                                   \
      IVP_DSELNX16(_out_vec##1,  _out_vec##0,  _inp_vec##2,  _inp_vec##0,  _pat); \
      IVP_DSELNX16(_out_vec##3,  _out_vec##2,  _inp_vec##6,  _inp_vec##4,  _pat); \
      IVP_DSELNX16(_out_vec##5,  _out_vec##4,  _inp_vec##10, _inp_vec##8,  _pat); \
      IVP_DSELNX16(_out_vec##7,  _out_vec##6,  _inp_vec##14, _inp_vec##12, _pat); \
      IVP_DSELNX16(_out_vec##9,  _out_vec##8,  _inp_vec##18, _inp_vec##16, _pat); \
      IVP_DSELNX16(_out_vec##11, _out_vec##10, _inp_vec##22, _inp_vec##20, _pat); \
      IVP_DSELNX16(_out_vec##13, _out_vec##12, _inp_vec##26, _inp_vec##24, _pat); \
      IVP_DSELNX16(_out_vec##15, _out_vec##14, _inp_vec##30, _inp_vec##28, _pat); \
      IVP_DSELNX16(_out_vec##17, _out_vec##16, _inp_vec##3,  _inp_vec##1,  _pat); \
      IVP_DSELNX16(_out_vec##19, _out_vec##18, _inp_vec##7,  _inp_vec##5,  _pat); \
      IVP_DSELNX16(_out_vec##21, _out_vec##20, _inp_vec##11, _inp_vec##9,  _pat); \
      IVP_DSELNX16(_out_vec##23, _out_vec##22, _inp_vec##15, _inp_vec##13, _pat); \
      IVP_DSELNX16(_out_vec##25, _out_vec##24, _inp_vec##19, _inp_vec##17, _pat); \
      IVP_DSELNX16(_out_vec##27, _out_vec##26, _inp_vec##23, _inp_vec##21, _pat); \
      IVP_DSELNX16(_out_vec##29, _out_vec##28, _inp_vec##27, _inp_vec##25, _pat); \
      IVP_DSELNX16(_out_vec##31, _out_vec##30, _inp_vec##31, _inp_vec##29, _pat); \

    #define SR(_nums,_num) kernel.packet[_nums] = tmp_d##_num

    #define STORE_VEC                     \
      SR(0,0)  ; SR(1,1)  ; SR(2,16) ; SR(3,17) ; \
      SR(4,8)  ; SR(5,9)  ; SR(6,24) ; SR(7,25) ; \
      SR(8,4)  ; SR(9,5)  ; SR(10,20); SR(11,21); \
      SR(12,12); SR(13,13); SR(14,28); SR(15,29); \
      SR(16,2) ; SR(17,3) ; SR(18,18); SR(19,19); \
      SR(20,10); SR(21,11); SR(22,26); SR(23,27); \
      SR(24,6) ; SR(25,7) ; SR(26,22); SR(27,23); \
      SR(28,14); SR(29,15); SR(30,30); SR(31,31); \
     
    LOAD_VEC;
    CONVERT_TO_NX16_FROMN_2XF32(d);
    FIRST_STEP(vecselpat_32b_interleave_1, t_out, t_inp);
    NEXT_STEP(vecselpat_32b_interleave_2, t_inp, t_out);
    NEXT_STEP(vecselpat_32b_interleave_4, t_out, t_inp);
    NEXT_STEP(vecselpat_32b_interleave_8,t_inp, t_out);
    NEXT_STEP(vecselpat_32b_interleave_16, t_out, t_inp);
    CONVERT_TO_N_2XF32_FROMNX16(tmp_d);
    STORE_VEC;
    #undef CONVERT_TO_N_2XF32_FROMNX16
    #undef CONVERT_TO_NX16_FROMN_2XF32
    #undef VR
    #undef VR16
    #undef DECL_VEC
    #undef DECL_VEC16
    #undef XR
    #undef LOAD_VEC
    #undef FIRST_STEP
    #undef NEXT_STEP
    #undef NEXT_STEP1
    #undef SR
    #undef STORE_VEC
    #undef XOUT
    #undef XINP
  }
#endif

} // end namespace internal

} // end namespace Eigen
#endif
#endif
