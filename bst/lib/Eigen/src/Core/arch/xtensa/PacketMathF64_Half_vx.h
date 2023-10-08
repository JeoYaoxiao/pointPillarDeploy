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

#ifndef EIGEN_PACKET_MATH_F64_HALF_VX_XTENSA_H
#define EIGEN_PACKET_MATH_F64_HALF_VX_XTENSA_H

#if F64_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

#if F64_HALF_PACKET_SUPPORT
#if SIMD_N_EQ(8,4) || SIMD_N_EQ(8,8)
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8d, CONST_4> &kernel) {
  FUNC_ENTRY;
  #if SIMD_N_EQ(8,4)
    #define DV(_num) xb_vecNx16 tvs##_num = BBE_MOVNX16_FROMN_4XF64(IVP_CONSTN_4XF64(0))
    #define LV(_num) xb_vecNx16 tvf##_num = BBE_MOVNX16_FROMN_4XF64(kernel.packet[_num].v)
    
    DV(0); DV(1); DV(2); DV(3);
    LV(0); LV(1); LV(2); LV(3);

    #define FIRST_STEP(_pat, _out, _inp)                             \
      IVP_DSELNX16(_out##2, _out##0, _inp##1, _inp##0, _pat);    \
      IVP_DSELNX16(_out##3, _out##1, _inp##3, _inp##2, _pat);    \

    #define SECOND_STEP(_pat, _out, _inp)                            \
      IVP_DSELNX16(_out##1, _out##0, _inp##1, _inp##0, _pat);    \
      _out##2 = _inp##2; _out##3 = _inp##3;
    
    #define LAST_STEP(_pat, _out, _inp)                              \
      IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
      IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##1, _pat);    \

    signed char ALIGN128 sel_64b_interleave_1[64]  =
    {
      0,16,1,17,2,18,3,19,
      32,48,33,49,34,50,35,51,
      4,20,5,21,6,22,7,23,
      36,52,37,53,38,54,39,55,
      8,24,9,25,10,26,11,27,
      40,56,41,57,42,58,43,59,
      12,28,13,29,14,30,15,31,
      44,60,45,61,46,62,47,63,
    };
    xb_vec2Nx8 vecselpat_64b_interleave_1 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_1)); // 64b interleave 1

    signed char ALIGN64 sel_64b_interleave_2[64] =
    {
      0,16,1,17,2,18,3,19,
      4,20,5,21,6,22,7,23,
      32,48,33,49,34,50,35,51,
      36,52,37,53,38,54,39,55,
      8,24,9,25,10,26,11,27,
      12,28,13,29,14,30,15,31,
      40,56,41,57,42,58,43,59,
      44,60,45,61,46,62,47,63,
    };
    xb_vec2Nx8 vecselpat_64b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_2)); // 64b interleave 2

    signed char ALIGN64 sel_64b_interleave_4[64] =
    {
      0,16,1,17,2,18,3,19,
      4,20,5,21,6,22,7,23,
      8,24,9,25,10,26,11,27,
      12,28,13,29,14,30,15,31,
      32,48,33,49,34,50,35,51,
      36,52,37,53,38,54,39,55,
      40,56,41,57,42,58,43,59,
      44,60,45,61,46,62,47,63,
    };
    xb_vec2Nx8 vecselpat_64b_interleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_4)); // 64b interleave 4

    FIRST_STEP(vecselpat_64b_interleave_1, tvs, tvf);
    SECOND_STEP(vecselpat_64b_interleave_2, tvf, tvs);
    LAST_STEP(vecselpat_64b_interleave_4, tvs, tvf);
    
    #define SV(_num) kernel.packet[_num] = PacketN_8d(BBE_MOVN_4XF64_FROMNX16(tvs##_num))
    SV(0); SV(1); SV(2); SV(3);

    #undef DV
    #undef LV
    #undef FIRST_STEP
    #undef SECOND_STEP
    #undef LAST_STEP
    #undef SV
  #elif SIMD_N_EQ(8,8)
    #define DV(_num) xb_vecNx16 tvs##_num = BBE_MOVNX16_FROMN_4XF64(IVP_CONSTN_4XF64(0))
    #define LV(_num) xb_vecNx16 tvf##_num = BBE_MOVNX16_FROMN_4XF64(kernel.packet[_num].v)
    
    DV(0); DV(1); DV(2); DV(3);
    LV(0); LV(1); LV(2); LV(3);

    #define FIRST_STEP(_pat, _out, _inp)                             \
      IVP_DSELNX16(_out##2, _out##0, _inp##1, _inp##0, _pat);    \
      IVP_DSELNX16(_out##3, _out##1, _inp##3, _inp##2, _pat);    \

    #define SECOND_STEP(_pat, _out, _inp)                            \
      IVP_DSELNX16(_out##1, _out##0, _inp##1, _inp##0, _pat);    \
      _out##2 = _inp##2; _out##3 = _inp##3;
    
    #define LAST_STEP(_pat, _out, _inp)                              \
      IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
      IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##1, _pat);    \

    signed char ALIGN128 sel_64b_interleave_1[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 64, 96, 65, 97, 66, 98, 67, 99, 4, 36, 5, 37, 6, 38, 7, 39, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 72, 104, 73, 105, 74, 106, 75, 107, 12, 44, 13, 45, 14, 46, 15, 47, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 80, 112, 81, 113, 82, 114, 83, 115, 20, 52, 21, 53, 22, 54, 23, 55, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 88, 120, 89, 121, 90, 122, 91, 123, 28, 60, 29, 61, 30, 62, 31, 63, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_64b_interleave_1 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_1)); // 64b interleave 1

    signed char ALIGN128 sel_64b_interleave_2[128] =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_64b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_2)); // 64b interleave 2

    signed char ALIGN128 sel_64b_interleave_8[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63,
      64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_64b_interleave_8 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_8)); // 64b interleave 8

    FIRST_STEP(vecselpat_64b_interleave_1, tvs, tvf);
    SECOND_STEP(vecselpat_64b_interleave_2, tvf, tvs);
    LAST_STEP(vecselpat_64b_interleave_8, tvs, tvf);
    
    #define SV(_num) kernel.packet[_num] = PacketN_8d(BBE_MOVN_4XF64_FROMNX16(tvs##_num))
    SV(0); SV(1); SV(2); SV(3);

    #undef DV
    #undef LV
    #undef FIRST_STEP
    #undef SECOND_STEP
    #undef LAST_STEP
    #undef SV
  #endif
    
}
#endif
#if SIMD_N_EQ(8,8)
EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8d, CONST_8> &kernel) {
    FUNC_ENTRY;

    signed char ALIGN128 sel_64b_interleave_1[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 64, 96, 65, 97, 66, 98, 67, 99, 4, 36, 5, 37, 6, 38, 7, 39, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 72, 104, 73, 105, 74, 106, 75, 107, 12, 44, 13, 45, 14, 46, 15, 47, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 80, 112, 81, 113, 82, 114, 83, 115, 20, 52, 21, 53, 22, 54, 23, 55, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 88, 120, 89, 121, 90, 122, 91, 123, 28, 60, 29, 61, 30, 62, 31, 63, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_64b_interleave_1 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_1)); // 64b interleave 1

    signed char ALIGN128 sel_64b_interleave_2[128] =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111,
      16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_64b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_2)); // 64b interleave 2

    signed char ALIGN128 sel_64b_interleave_4[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111, 
      16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_64b_interleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_4)); // 64b interleave 4

    signed char ALIGN128 sel_64b_interleave_8[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63,
      64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_64b_interleave_8 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_8)); // 64b interleave 8

    #define LV(_num) xb_vecNx16 tvf##_num = BBE_MOVNX16_FROMN_4XF64(kernel.packet[_num].v)
    #define DV16(_num) xb_vecNx16 t_inp##_num, t_out##_num;

    DV16(0); DV16(1); DV16(2); DV16(3); DV16(4); DV16(5); DV16(6); DV16(7);
    LV(0); LV(1); LV(2); LV(3); LV(4); LV(5); LV(6); LV(7);

    /* Macro for converting input vector */
    #define XINP(_iv, _num)                                   \
      t_inp##_num = (_iv##_num);

    #define CONVERT_INP_NX16(_iv)                  \
      XINP(_iv,0);  XINP(_iv,1);  XINP(_iv,2);  XINP(_iv,3);  \
      XINP(_iv,4);  XINP(_iv,5);  XINP(_iv,6);  XINP(_iv,7);  \

    /* Macro for converting output vector */
    #define XOUT(_ov,_num)                                    \
      _ov##_num   = (t_inp##_num);     \

    #define CONVERT_OUT_NX16(_ov)                  \
      XOUT(_ov,0);  XOUT(_ov,1);  XOUT(_ov,2);  XOUT(_ov,3);  \
      XOUT(_ov,4);  XOUT(_ov,5);  XOUT(_ov,6);  XOUT(_ov,7);  \

    #define FIRST_STEP(_pat, _out, _inp)                             \
      IVP_DSELNX16(_out##1, _out##0, _inp##1, _inp##0, _pat);   \
      IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##2, _pat);   \
      IVP_DSELNX16(_out##5, _out##4, _inp##5, _inp##4, _pat);   \
      IVP_DSELNX16(_out##7, _out##6, _inp##7, _inp##6, _pat);   \

    #define SECOND_STEP(_pat, _out, _inp)                            \
      IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);   \
      IVP_DSELNX16(_out##3, _out##2, _inp##6, _inp##4, _pat);   \
      _out##4 = _inp##1; _out##5 = _inp##3;                          \
      _out##6 = _inp##5; _out##7 = _inp##7;                          \

    #define THIRD_STEP(_pat, _out, _inp)                             \
      IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);   \
      IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##1, _pat);   \
      _out##4 = _inp##4; _out##5 = _inp##5;                          \
      _out##6 = _inp##6; _out##7 = _inp##7;                          \

    #define LAST_STEP(_pat, _out, _inp)                              \
      IVP_DSELNX16(_out##1, _out##0, _inp##4, _inp##0, _pat);   \
      IVP_DSELNX16(_out##3, _out##2, _inp##5, _inp##1, _pat);   \
      IVP_DSELNX16(_out##5, _out##4, _inp##6, _inp##2, _pat);   \
      IVP_DSELNX16(_out##7, _out##6, _inp##7, _inp##3, _pat);   \

    CONVERT_INP_NX16(tvf);
    FIRST_STEP(vecselpat_64b_interleave_1, t_out, t_inp);
    SECOND_STEP(vecselpat_64b_interleave_2, t_inp, t_out);
    THIRD_STEP(vecselpat_64b_interleave_4, t_out, t_inp);
    LAST_STEP(vecselpat_64b_interleave_8, t_inp, t_out);
    CONVERT_OUT_NX16(tvf);

    #define SV(_num) kernel.packet[_num] = PacketN_8d(BBE_MOVN_4XF64_FROMNX16(tvf##_num))
    SV(0); SV(1); SV(2); SV(3); SV(4); SV(5); SV(6); SV(7);

    #undef DV
    #undef LV
    #undef FIRST_STEP
    #undef SECOND_STEP
    #undef LAST_STEP
    #undef SV
    #undef CONVERT_OUT_NX16
    #undef CONVERT_INP_NX16
    #undef XINP
    #undef XOUT
}
#endif
template <> EIGEN_DEVICE_FUNC inline void pscatter<double, PacketN_8d>(double *to, const PacketN_8d &from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  int i;
  double *tpf = (double*)(&from.v);
  double *tpt = (double*)(to);
  int    stride_bytes = sizeof(double) * stride;
  xb_vecN_4xf64 tvf;
  for (i = 0; i < SIMD_N_8; i++)
  {
    IVP_LSRN_4XF64_IP(tvf, tpf, sizeof(double));
    IVP_SSN_4XF64_XP(tvf, tpt, stride_bytes);
  }
}

template <> EIGEN_DEVICE_FUNC inline PacketN_8d pgather<double, PacketN_8d>(const double *from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtdouble *tp = xt_reinterpret_const_cast<xtdouble,double>(from);
  xb_vecN_4xf64 ta;
  xtdouble *tpf = xt_reinterpret_cast<xtdouble,xb_vecN_4xf64>(&ta);
  int    i;
  int    stride_bytes = sizeof(xtdouble) * stride;
  xb_vecN_4xf64 tvf;

  for (i = 0; i < SIMD_N_8; i++) {
    IVP_LSRN_4XF64_XP(tvf, tp, stride_bytes);
    IVP_SSN_4XF64_IP(tvf, tpf, sizeof(xtdouble));
  }
  return PacketN_8d(ta);
}


#endif
} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_F64_HALF_VX_XTENSA_H
