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

#ifndef EIGEN_PACKET_MATH_F32_HALF_VX_XTENSA_H
#define EIGEN_PACKET_MATH_F32_HALF_VX_XTENSA_H

#if F32_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

#ifndef ALIGN64
  #define ALIGN64  __attribute__((aligned(64)))
#endif

#ifndef ALIGN128
  #define ALIGN128  __attribute__((aligned(128)))
#endif

#if F32_HALF_PACKET_SUPPORT
template <> EIGEN_DEVICE_FUNC inline PacketN_4f pgather<float, PacketN_4f>(const float *from, Index stride) {
  FUNC_ENTRY;
  float *tp = (float *)(from);
  xb_vecN_2xf32  ta;
  float *tpf = (float *)(&ta);
  int    i;
  int    stride_bytes = sizeof(float) * stride;
  float tvf;
  for (i = 0; i < SIMD_N_4; i++) {
    IVP_LSN_2XF32_XP(tvf, tp, stride_bytes);
    IVP_SSN_2XF32_IP(tvf, tpf, sizeof(float));
  }
  return PacketN_4f(ta);
}
template <> EIGEN_DEVICE_FUNC inline void pscatter<float, PacketN_4f>(float *to, const PacketN_4f &from, Index stride) {
  FUNC_ENTRY;
  float *tpf = (float*)(&from.v);
  float *tpt = (float*)(to);
  int    stride_bytes = sizeof(float) * stride;
  float tvf;
  int i;
  for (i = 0; i < SIMD_N_4; i++)
  {
    IVP_LSN_2XF32_IP(tvf, tpf, sizeof(float));
    IVP_SSN_2XF32_XP(tvf, tpt, stride_bytes);
  } 
}

#if SIMD_N_2_EQ_16() || SIMD_N_2_EQ_32()
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4f, CONST_4> &kernel) {
  FUNC_ENTRY;
  #if SIMD_N_2_EQ_16()
      signed char ALIGN64 sel_32b_interleave_8[64]  =
      {
        0,16,1,17,2,18,3,19,4,20,5,21,6,22,7,23,8,24,9,25,10,26,11,27,12,28,13,29,14,30,15,31,
        32,48,33,49,34,50,35,51,36,52,37,53,38,54,39,55,40,56,41,57,42,58,43,59,44,60,45,61,46,62,47,63
      };
      xb_vec2Nx8 vecselpat_32b_interleave_8 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_8)); // interleave 8

      #define DV(_num) xb_vecN_2xf32 tvs##_num = IVP_CONSTN_2XF32(0)
      #define LV(_num) xb_vecN_2xf32 tvf##_num = kernel.packet[_num].v
      #define DV16(_num) xb_vecNx16 t##_num;
      
      DV(0); DV(1); DV(2); DV(3);
      DV16(0); DV16(1); DV16(2); DV16(3);
      LV(0); LV(1); LV(2); LV(3);

      #define FIRST_STEP(_pat, _out, _inp)                             \
        IVP_DSELN_2XF32I(_out##2, _out##0, _inp##1, _inp##0, _pat);    \
        IVP_DSELN_2XF32I(_out##3, _out##1, _inp##3, _inp##2, _pat);    \

      #define SECOND_STEP(_pat, _out, _inp)                            \
        IVP_DSELN_2XF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);    \
        _out##2 = _inp##2; _out##3 = _inp##3;

      #define F32TO16(_out,_num)                                            \
        t##_num  = IVP_MOVNX16_FROMN_2XF32(_out##_num);                \

      #define N16TOF32(_out,_num)                                            \
        _out##_num   = PacketN_4f(IVP_MOVN_2XF32_FROMNX16(t##_num));   \
      
      #define LAST_STEP(_pat, _out, _inp)                              \
        F32TO16(_out,0); F32TO16(_out,1); F32TO16(_out,2); F32TO16(_out,3);                \
        IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_2XF32(_inp##2), IVP_MOVNX16_FROMN_2XF32(_inp##0), _pat);    \
        IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_2XF32(_inp##3), IVP_MOVNX16_FROMN_2XF32(_inp##1), _pat);    \
        N16TOF32(_out,0); N16TOF32(_out,1); N16TOF32(_out,2); N16TOF32(_out,3);

      FIRST_STEP(IVP_DSELI_32B_INTERLEAVE_1, tvs, tvf);
      SECOND_STEP(IVP_DSELI_32B_INTERLEAVE_2, tvf, tvs);
      LAST_STEP(vecselpat_32b_interleave_8, tvs, tvf);
      
      #define SV(_num) kernel.packet[_num] = PacketN_4f(tvs##_num)
      SV(0); SV(1); SV(2); SV(3);

      #undef DV
      #undef DV16
      #undef LV
      #undef FIRST_STEP
      #undef SECOND_STEP
      #undef LAST_STEP
      #undef SV
      #undef F32TO16
      #undef N16TOF32
  #elif SIMD_N_2_EQ_32()
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

      signed char ALIGN128 sel_32b_interleave_16[128]  =
      {
        0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63,
        64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
      };

      xb_vec2Nx8 vecselpat_32b_interleave_16 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_16)); // interleave 16
      
      #define LV(_num) xb_vecN_2xf32 tvf##_num = kernel.packet[_num].v
      #define DV16(_num) xb_vecNx16 t_inp##_num, t_out##_num;
      
      DV16(0); DV16(1); DV16(2); DV16(3);
      LV(0); LV(1); LV(2); LV(3);

      /* Macro for converting input vector to NX16 */
      #define XINP(_iv, _num)                                   \
        t_inp##_num = IVP_MOVNX16_FROMN_2XF32(_iv##_num);

      #define CONVERT_TO_NX16_FROMN_2XF32(_iv)                  \
        XINP(_iv,0);  XINP(_iv,1);  XINP(_iv,2);  XINP(_iv,3);  \

      /* Macro for converting output vector to N_2XF32 */
      #define XOUT(_ov,_num)                                    \
        _ov##_num   = PacketN_4f(IVP_MOVN_2XF32_FROMNX16(t_out##_num));     \

      #define CONVERT_TO_N_2XF32_FROMNX16(_ov)                  \
        XOUT(_ov,0);  XOUT(_ov,1);  XOUT(_ov,2);  XOUT(_ov,3);  \

      #define FIRST_STEP(_pat, _out, _inp)                             \
        IVP_DSELNX16(_out##2, _out##0, _inp##1, _inp##0, _pat);    \
        IVP_DSELNX16(_out##3, _out##1, _inp##3, _inp##2, _pat);    \

      #define SECOND_STEP(_pat, _out, _inp)                            \
        IVP_DSELNX16(_out##1, _out##0, _inp##1, _inp##0, _pat);    \
        _out##2 = _inp##2; _out##3 = _inp##3;
      
      #define LAST_STEP(_pat, _out, _inp)                              \
        IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##1, _pat);    \
      
      CONVERT_TO_NX16_FROMN_2XF32(tvf);
      FIRST_STEP(vecselpat_32b_interleave_1, t_out, t_inp);
      SECOND_STEP(vecselpat_32b_interleave_2, t_inp, t_out);
      LAST_STEP(vecselpat_32b_interleave_16, t_out, t_inp);
      CONVERT_TO_N_2XF32_FROMNX16(tvf);
      
      #define SV(_num) kernel.packet[_num] = PacketN_4f(tvf##_num)
      SV(0); SV(1); SV(2); SV(3);

      #undef DV16
      #undef LV
      #undef FIRST_STEP
      #undef SECOND_STEP
      #undef LAST_STEP
      #undef SV
      #undef CONVERT_TO_N_2XF32_FROMNX16
      #undef CONVERT_TO_NX16_FROMN_2XF32
      #undef XINP
      #undef XOUT
  #endif

}
#endif

#if SIMD_N_2_EQ_16()
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4f, CONST_8> &kernel) {
    FUNC_ENTRY;
      signed char ALIGN64 sel_32b_interleave_4[64] =
      {
        0,16,1,17,2,18,3,19,4,20,5,21,6,22,7,23,32,48,33,49,34,50,35,51,36,52,37,53,38,54,39,55,
        8,24,9,25,10,26,11,27,12,28,13,29,14,30,15,31,40,56,41,57,42,58,43,59,44,60,45,61,46,62,47,63
      };
      xb_vec2Nx8 vecselpat_32b_interleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_32b_interleave_4));

      #define DV(_num) xb_vecN_2xf32 tvs##_num = IVP_CONSTN_2XF32(0)
      #define LV(_num) xb_vecN_2xf32 tvf##_num = kernel.packet[_num].v
      #define DV16(_num) xb_vecNx16 t##_num;

      DV(0); DV(1); DV(2); DV(3); DV(4); DV(5); DV(6); DV(7);
      DV16(0); DV16(1); DV16(2); DV16(3); DV16(4); DV16(5); DV16(6); DV16(7);
      LV(0); LV(1); LV(2); LV(3); LV(4); LV(5); LV(6); LV(7);

      #define FIRST_STEP(_pat, _out, _inp)                             \
        IVP_DSELN_2XF32I(_out##1, _out##0, _inp##1, _inp##0, _pat);    \
        IVP_DSELN_2XF32I(_out##3, _out##2, _inp##3, _inp##2, _pat);    \
        IVP_DSELN_2XF32I(_out##5, _out##4, _inp##5, _inp##4, _pat);    \
        IVP_DSELN_2XF32I(_out##7, _out##6, _inp##7, _inp##6, _pat);    \

      #define SECOND_STEP(_pat, _out, _inp)                            \
        IVP_DSELN_2XF32I(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
        IVP_DSELN_2XF32I(_out##3, _out##2, _inp##6, _inp##4, _pat);    \
        _out##4 = _inp##1; _out##5 = _inp##3;                          \
        _out##6 = _inp##5; _out##7 = _inp##7;                          \

      #define F32TO16(_out,_num)                                            \
        t##_num  = IVP_MOVNX16_FROMN_2XF32(_out##_num);                \

      #define N16TOF32(_out,_num)                                            \
        _out##_num   = PacketN_4f(IVP_MOVN_2XF32_FROMNX16(t##_num));   \
      

      #define LAST_STEP(_pat, _out, _inp)                              \
        F32TO16(_out,0); F32TO16(_out,1); F32TO16(_out,2); F32TO16(_out,3);  \
        F32TO16(_out,4); F32TO16(_out,5); F32TO16(_out,6); F32TO16(_out,7);  \
        IVP_DSELNX16(t1, t0, IVP_MOVNX16_FROMN_2XF32(_inp##4), IVP_MOVNX16_FROMN_2XF32(_inp##0), _pat);    \
        IVP_DSELNX16(t3, t2, IVP_MOVNX16_FROMN_2XF32(_inp##5), IVP_MOVNX16_FROMN_2XF32(_inp##1), _pat);    \
        IVP_DSELNX16(t5, t4, IVP_MOVNX16_FROMN_2XF32(_inp##6), IVP_MOVNX16_FROMN_2XF32(_inp##2), _pat);    \
        IVP_DSELNX16(t7, t6, IVP_MOVNX16_FROMN_2XF32(_inp##7), IVP_MOVNX16_FROMN_2XF32(_inp##3), _pat);    \
        N16TOF32(_out,0); N16TOF32(_out,1); N16TOF32(_out,2); N16TOF32(_out,3);  \
        N16TOF32(_out,4); N16TOF32(_out,5); N16TOF32(_out,6); N16TOF32(_out,7);  \

      FIRST_STEP(IVP_DSELI_32B_INTERLEAVE_1, tvs, tvf);
      SECOND_STEP(IVP_DSELI_32B_INTERLEAVE_2, tvf, tvs);
      LAST_STEP(vecselpat_32b_interleave_4, tvs, tvf);
      LAST_STEP(vecselpat_32b_interleave_4, tvf, tvs);

      #define SV(_num) kernel.packet[_num] = PacketN_4f(tvf##_num)
      SV(0); SV(1); SV(2); SV(3); SV(4); SV(5); SV(6); SV(7);

      #undef DV
      #undef DV16
      #undef LV
      #undef FIRST_STEP
      #undef SECOND_STEP
      #undef LAST_STEP
      #undef SV
      #undef F32TO16
      #undef N16TOF32
    
  }
#endif

#if SIMD_N_2_EQ_32()
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_4f, CONST_16> &kernel) {
    FUNC_ENTRY;
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

      #define LV(_num) xb_vecN_2xf32 tvf##_num = kernel.packet[_num].v
      #define DV16(_num) xb_vecNx16 t_inp##_num, t_out##_num;

      DV16(0); DV16(1); DV16(2); DV16(3); DV16(4); DV16(5); DV16(6); DV16(7); DV16(8); DV16(9); DV16(10); DV16(11); DV16(12); DV16(13); DV16(14); DV16(15);
      LV(0); LV(1); LV(2); LV(3); LV(4); LV(5); LV(6); LV(7); LV(8); LV(9); LV(10); LV(11); LV(12); LV(13); LV(14); LV(15);

      /* Macro for converting input vector to NX16 */
      #define XINP(_iv, _num)                                   \
        t_inp##_num = IVP_MOVNX16_FROMN_2XF32(_iv##_num);

      #define CONVERT_TO_NX16_FROMN_2XF32(_iv)                  \
        XINP(_iv,0);  XINP(_iv,1);  XINP(_iv,2);  XINP(_iv,3);  \
        XINP(_iv,4);  XINP(_iv,5);  XINP(_iv,6);  XINP(_iv,7);  \
        XINP(_iv,8);  XINP(_iv,9);  XINP(_iv,10); XINP(_iv,11); \
        XINP(_iv,12); XINP(_iv,13); XINP(_iv,14); XINP(_iv,15); \

      /* Macro for converting output vector to N_2XF32 */
      #define XOUT(_ov,_num)                                    \
        _ov##_num   = PacketN_4f(IVP_MOVN_2XF32_FROMNX16(t_out##_num));     \

      #define CONVERT_TO_N_2XF32_FROMNX16(_ov)                  \
        XOUT(_ov,0);  XOUT(_ov,1);  XOUT(_ov,2);  XOUT(_ov,3);  \
        XOUT(_ov,4);  XOUT(_ov,5);  XOUT(_ov,6);  XOUT(_ov,7);  \
        XOUT(_ov,8);  XOUT(_ov,9);  XOUT(_ov,10); XOUT(_ov,11); \
        XOUT(_ov,12); XOUT(_ov,13); XOUT(_ov,14); XOUT(_ov,15); \

      #define FIRST_STEP(_pat, _out, _inp)                              \
        IVP_DSELNX16(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
        IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##2, _pat);     \
        IVP_DSELNX16(_out##5, _out##4, _inp##5, _inp##4, _pat);     \
        IVP_DSELNX16(_out##7, _out##6, _inp##7, _inp##6, _pat);     \
        IVP_DSELNX16(_out##9, _out##8, _inp##9, _inp##8, _pat);     \
        IVP_DSELNX16(_out##11, _out##10, _inp##11, _inp##10, _pat); \
        IVP_DSELNX16(_out##13, _out##12, _inp##13, _inp##12, _pat); \
        IVP_DSELNX16(_out##15, _out##14, _inp##15, _inp##14, _pat); \

      #define SECOND_STEP(_pat, _out, _inp)                             \
        IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);     \
        IVP_DSELNX16(_out##3, _out##2, _inp##6, _inp##4, _pat);     \
        IVP_DSELNX16(_out##5, _out##4, _inp##10, _inp##8, _pat);    \
        IVP_DSELNX16(_out##7, _out##6, _inp##14, _inp##12, _pat);   \
        _out##8 = _inp##1;   _out##9 = _inp##3;                         \
        _out##10 = _inp##5;  _out##11 = _inp##7;                        \
        _out##12 = _inp##9;  _out##13 = _inp##11;                       \
        _out##14 = _inp##13; _out##15 = _inp##15;                       \

      #define THIRD_STEP(_pat, _out, _inp)                              \
        IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);     \
        IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##1, _pat);     \
        IVP_DSELNX16(_out##5, _out##4, _inp##6, _inp##4, _pat);     \
        IVP_DSELNX16(_out##7, _out##6, _inp##7, _inp##5, _pat);     \
        _out##8 = _inp##8;   _out##9 = _inp##9;                         \
        _out##10 = _inp##10; _out##11 = _inp##11;                       \
        _out##12 = _inp##12; _out##13 = _inp##13;                       \
        _out##14 = _inp##14; _out##15 = _inp##15;                       \

      #define LAST_STEP(_pat, _out, _inp)                               \
        IVP_DSELNX16(_out##1, _out##0, _inp##8, _inp##0, _pat);     \
        IVP_DSELNX16(_out##3, _out##2, _inp##9, _inp##1, _pat);     \
        IVP_DSELNX16(_out##5, _out##4, _inp##10, _inp##2, _pat);    \
        IVP_DSELNX16(_out##7, _out##6, _inp##11, _inp##3, _pat);    \
        IVP_DSELNX16(_out##9, _out##8, _inp##12, _inp##4, _pat);    \
        IVP_DSELNX16(_out##11, _out##10, _inp##13, _inp##5, _pat);  \
        IVP_DSELNX16(_out##13, _out##12, _inp##14, _inp##6, _pat);  \
        IVP_DSELNX16(_out##15, _out##14, _inp##15, _inp##7, _pat);  \

      CONVERT_TO_NX16_FROMN_2XF32(tvf);
      FIRST_STEP(vecselpat_32b_interleave_1, t_out, t_inp);
      SECOND_STEP(vecselpat_32b_interleave_2, t_inp, t_out);
      THIRD_STEP(vecselpat_32b_interleave_4, t_out, t_inp);
      LAST_STEP(vecselpat_32b_interleave_8, t_inp, t_out);
      LAST_STEP(vecselpat_32b_interleave_8, t_out, t_inp);
      CONVERT_TO_N_2XF32_FROMNX16(tvf);

      #define SV(_num) kernel.packet[_num] = PacketN_4f(tvf##_num)
      SV(0); SV(1); SV(2); SV(3); SV(4); SV(5); SV(6); SV(7); SV(8); SV(9); SV(10); SV(11); SV(12); SV(13); SV(14); SV(15);
      
      #undef DV16
      #undef LV
      #undef FIRST_STEP
      #undef SECOND_STEP
      #undef THIRD_STEP
      #undef LAST_STEP
      #undef SV
      #undef CONVERT_TO_N_2XF32_FROMNX16
      #undef CONVERT_TO_NX16_FROMN_2XF32
      #undef XINP
      #undef XOUT
  }

#endif

#endif

} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_F32_HALF_VX_XTENSA_H
