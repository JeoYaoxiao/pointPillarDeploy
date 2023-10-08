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

#ifndef EIGEN_PACKET_MATH_CF32_HALF_VX_XTENSA_H
#define EIGEN_PACKET_MATH_CF32_HALF_VX_XTENSA_H

#if CF32_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

#if CF32_HALF_PACKET_SUPPORT
template <> EIGEN_DEVICE_FUNC inline PacketN_8cf pgather<complexfloat, PacketN_8cf>(const complexfloat *from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtcomplexfloat *tp = xt_reinterpret_const_cast<xtcomplexfloat,complexfloat>(from);
  xb_vecN_4xcf32 ta;
  xtcomplexfloat *tpf = xt_reinterpret_cast<xtcomplexfloat,xb_vecN_4xcf32>(&ta);
  int    i;
  int    stride_bytes = sizeof(xtcomplexfloat) * stride;
  xb_vecN_4xcf32 tvf;

  for (i = 0; i < SIMD_N_8; i++) {
    IVP_LSRN_4XCF32_XP(tvf, tp, stride_bytes);
    IVP_SSN_4XCF32_IP(tvf, tpf, sizeof(xtcomplexfloat));
  }
  return PacketN_8cf(ta);
}

template <> EIGEN_DEVICE_FUNC inline void pscatter<complexfloat, PacketN_8cf>(complexfloat *to, const PacketN_8cf &from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  int i;
  xtcomplexfloat *tpf = xt_reinterpret_const_cast<xtcomplexfloat,xb_vecN_4xcf32>(&(from.v.v));
  xtcomplexfloat *tpt = xt_reinterpret_cast<xtcomplexfloat,complexfloat>(to);
  int    stride_bytes = sizeof(xtcomplexfloat) * stride;
  xb_vecN_4xcf32 tvf;
  for (i = 0; i < SIMD_N_8; i++)
  {
    IVP_LSRN_4XCF32_IP(tvf, tpf, sizeof(xtcomplexfloat));
    IVP_SSN_4XCF32_XP(tvf, tpt, stride_bytes);
  }
}

#if SIMD_N_EQ(8,8)
  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8cf, CONST_4> &kernel) {
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

    signed char ALIGN128 sel_64b_interleave_8[128]  =
    {
      0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63,
      64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
    };

    xb_vec2Nx8 vecselpat_64b_interleave_8 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_8)); // 64b interleave 8

    #define LV(_num) xb_vecN_4xcf32 tvf##_num = kernel.packet[_num].v.v
    #define DV16(_num) xb_vecNx16 t_inp##_num, t_out##_num;

    DV16(0); DV16(1); DV16(2); DV16(3);
    LV(0); LV(1); LV(2); LV(3);

    /* Macro for converting input vector to NX16 */
    #define XINP(_iv, _num)                                   \
      t_inp##_num = IVP_MOVNX16_FROMN_4XCF32(_iv##_num);

    #define CONVERT_TO_NX16_FROMN_4XCF32(_iv)                  \
      XINP(_iv,0);  XINP(_iv,1);  XINP(_iv,2);  XINP(_iv,3);  \

    /* Macro for converting output vector to N_4XCF32 */
    #define XOUT(_ov,_num)                                    \
      _ov##_num   = IVP_MOVN_4XCF32_FROMNX16(t_out##_num);     \

    #define CONVERT_TO_N_4XCF32_FROMNX16(_ov)                  \
      XOUT(_ov,0);  XOUT(_ov,1);  XOUT(_ov,2);  XOUT(_ov,3);  \

    #define FIRST_STEP(_pat, _out, _inp)                             \
      IVP_DSELNX16(_out##1, _out##0, _inp##1, _inp##0, _pat);   \
      IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##2, _pat);   \

    #define SECOND_STEP(_pat, _out, _inp)                            \
      IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);   \
      _out##2 = _inp##2; _out##3 = _inp##3;
    
    #define LAST_STEP(_pat, _out, _inp)                              \
      IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);   \
      IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##1, _pat);   \

    CONVERT_TO_NX16_FROMN_4XCF32(tvf);
    FIRST_STEP(vecselpat_64b_interleave_1, t_out, t_inp);
    SECOND_STEP(vecselpat_64b_interleave_2, t_inp, t_out);
    LAST_STEP(vecselpat_64b_interleave_8, t_out, t_inp);
    CONVERT_TO_N_4XCF32_FROMNX16(tvf);
    
    #define SV(_num) kernel.packet[_num] = PacketN_8cf(tvf##_num)
    SV(0); SV(1); SV(2); SV(3);

    #undef DV16
    #undef LV
    #undef FIRST_STEP
    #undef SECOND_STEP
    #undef LAST_STEP
    #undef SV
    #undef CONVERT_TO_N_4XCF32_FROMNX16
    #undef CONVERT_TO_NX16_FROMN_4XCF32
    #undef XINP
    #undef XOUT
  }

  EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8cf, CONST_8> &kernel) {
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

    #define LV(_num) xb_vecN_4xcf32 tvf##_num = kernel.packet[_num].v.v
    #define DV16(_num) xb_vecNx16 t_inp##_num, t_out##_num;

    DV16(0); DV16(1); DV16(2); DV16(3); DV16(4); DV16(5); DV16(6); DV16(7);
    LV(0); LV(1); LV(2); LV(3); LV(4); LV(5); LV(6); LV(7);

    /* Macro for converting input vector to NX16 */
    #define XINP(_iv, _num)                                   \
      t_inp##_num = IVP_MOVNX16_FROMN_4XCF32(_iv##_num);

    #define CONVERT_TO_NX16_FROMN_4XCF32(_iv)                  \
      XINP(_iv,0);  XINP(_iv,1);  XINP(_iv,2);  XINP(_iv,3);  \
      XINP(_iv,4);  XINP(_iv,5);  XINP(_iv,6);  XINP(_iv,7);  \

    /* Macro for converting output vector to N_4XCF32 */
    #define XOUT(_ov,_num)                                    \
      _ov##_num   = IVP_MOVN_4XCF32_FROMNX16(t_inp##_num);     \

    #define CONVERT_TO_N_4XCF32_FROMNX16(_ov)                  \
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

    CONVERT_TO_NX16_FROMN_4XCF32(tvf);
    FIRST_STEP(vecselpat_64b_interleave_1, t_out, t_inp);
    SECOND_STEP(vecselpat_64b_interleave_2, t_inp, t_out);
    THIRD_STEP(vecselpat_64b_interleave_4, t_out, t_inp);
    LAST_STEP(vecselpat_64b_interleave_8, t_inp, t_out);
    CONVERT_TO_N_4XCF32_FROMNX16(tvf);

    #define SV(_num) kernel.packet[_num] = PacketN_8cf(tvf##_num)
    SV(0); SV(1); SV(2); SV(3); SV(4); SV(5); SV(6); SV(7);

    #undef DV
    #undef LV
    #undef FIRST_STEP
    #undef SECOND_STEP
    #undef LAST_STEP
    #undef SV
    #undef CONVERT_TO_N_4XCF32_FROMNX16
    #undef CONVERT_TO_NX16_FROMN_4XCF32
    #undef XINP
    #undef XOUT
  }
#endif

#endif

} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_CF32_HALF_VX_XTENSA_H
