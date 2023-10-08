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

#ifndef EIGEN_PACKET_MATH_CF64_HALF_VX_XTENSA_H
#define EIGEN_PACKET_MATH_CF64_HALF_VX_XTENSA_H

#if CF64_FULL_PACKET_SUPPORT

namespace Eigen {

namespace internal {

#if CF64_HALF_PACKET_SUPPORT

EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_16cd, CONST_2> &kernel) {
  FUNC_ENTRY;
  #define DV(_num) xb_vecNx16 tvs##_num = IVP_MOVNX16_FROMN_8XCF64(IVP_CONSTN_8XCF64(0))
  #define LV(_num) xb_vecNx16 tvf##_num = IVP_MOVNX16_FROMN_8XCF64(kernel.packet[_num].v.v)

  DV(0); DV(1);
  LV(0); LV(1);

  signed char ALIGN128 sel_64b_interleave_2[128] =
  {
    0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111,
    16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
  };

  xb_vec2Nx8 vecselpat_64b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_2)); // interleave 2

  signed char ALIGN128 sel_64b_interleave_8[128]  =
  {
    0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63,
    64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
  };

  xb_vec2Nx8 vecselpat_64b_interleave_8 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_8)); // interleave 8
      
  #define FIRST_STEP(_pat, _out, _inp)                              \
    IVP_DSELNX16(_out##1, _out##0, _inp##1, _inp##0, _pat);    \


  FIRST_STEP(vecselpat_64b_interleave_2, tvs, tvf);
  FIRST_STEP(vecselpat_64b_interleave_8, tvf, tvs);
  
  #define SV(_num) kernel.packet[_num] = PacketN_16cd(IVP_MOVN_8XCF64_FROMNX16(tvf##_num))
  SV(0); SV(1);

  #undef DV
  #undef LV
  #undef FIRST_STEP
  #undef SV
}

EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_16cd, CONST_4> &kernel) {
  FUNC_ENTRY;

  #define DV(_num) xb_vecNx16 tvs##_num = IVP_MOVNX16_FROMN_8XCF64(IVP_CONSTN_8XCF64(0))
  #define LV(_num) xb_vecNx16 tvf##_num = IVP_MOVNX16_FROMN_8XCF64(kernel.packet[_num].v.v)

  DV(0); DV(1); DV(2); DV(3);
  LV(0); LV(1); LV(2); LV(3);

  signed char ALIGN128 sel_64b_interleave_2[128] =
  {
    0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111,
    16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
  };

  xb_vec2Nx8 vecselpat_64b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_2)); // interleave 2

  signed char ALIGN128 sel_64b_interleave_4[128]  =
  {
    0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111, 
    16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
  };

  xb_vec2Nx8 vecselpat_64b_interleave_4 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_4)); // interleave 4

  signed char ALIGN128 sel_64b_interleave_8[128]  =
  {
    0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63,
    64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
  };

  xb_vec2Nx8 vecselpat_64b_interleave_8 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_8)); // interleave 8
      
  #define FIRST_STEP(_pat, _out, _inp)                             \
    IVP_DSELNX16(_out##1, _out##0, _inp##1, _inp##0, _pat);   \
    IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##2, _pat);   \

  #define SECOND_STEP(_pat, _out, _inp)                            \
    IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);   \
    _out##2 = _inp##2; _out##3 = _inp##3;
  
  #define LAST_STEP(_pat, _out, _inp)                              \
    IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);   \
    IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##1, _pat);   \

  FIRST_STEP(vecselpat_64b_interleave_2, tvs, tvf);
  SECOND_STEP(vecselpat_64b_interleave_4, tvf, tvs);
  LAST_STEP(vecselpat_64b_interleave_8, tvs, tvf);
  
  #define SV(_num) kernel.packet[_num] = PacketN_16cd(IVP_MOVN_8XCF64_FROMNX16(tvs##_num))
  SV(0); SV(1); SV(2); SV(3);

  #undef DV
  #undef LV
  #undef FIRST_STEP
  #undef SECOND_STEP
  #undef LAST_STEP
  #undef SV
}

template <> EIGEN_DEVICE_FUNC inline PacketN_16cd pgather<complexdouble, PacketN_16cd>(const complexdouble *from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  xtcomplexdouble *tp = xt_reinterpret_const_cast<xtcomplexdouble,complexdouble>(from);
  xb_vecN_8xcf64 ta;
  xtcomplexdouble *tpf = xt_reinterpret_cast<xtcomplexdouble,xb_vecN_8xcf64>(&ta);
  int    i;
  int    stride_bytes = sizeof(xtcomplexdouble) * stride;
  xtcomplexdouble tvf;

  for (i = 0; i < SIMD_N_16; i++) {
    IVP_LSN_8XCF64_XP(tvf, tp, stride_bytes);
    IVP_SSN_8XCF64_IP(IVP_MOVN_8XCF64_FROMCF64(tvf), tpf, sizeof(xtcomplexdouble));
  }
  return PacketN_16cd(ta);
}

template <> EIGEN_DEVICE_FUNC inline void pscatter<complexdouble, PacketN_16cd>(complexdouble *to, const PacketN_16cd &from, Index stride) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  int i;
  xtcomplexdouble *tpf = xt_reinterpret_const_cast<xtcomplexdouble,PacketN_16cd>(&from);
  xtcomplexdouble *tpt = xt_reinterpret_cast<xtcomplexdouble,complexdouble>(to);
  int    stride_bytes = sizeof(xtcomplexdouble) * stride;
  xtcomplexdouble tvf;
  for (i = 0; i < SIMD_N_16; i++)
  {
    IVP_LSN_8XCF64_IP(tvf, tpf, sizeof(xtcomplexdouble));
    IVP_SSN_8XCF64_XP(IVP_MOVN_8XCF64_FROMCF64(tvf), tpt, stride_bytes);
  }
}

#endif

} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_CF64_HALF_VX_XTENSA_H
