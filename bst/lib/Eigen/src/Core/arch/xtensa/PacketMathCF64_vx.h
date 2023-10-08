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

#ifndef EIGEN_PACKET_MATH_CF64_VX_XTENSA_H
#define EIGEN_PACKET_MATH_CF64_VX_XTENSA_H

#if CF64_FULL_PACKET_SUPPORT
namespace Eigen {

namespace internal {

template <> EIGEN_STRONG_INLINE PacketN_8cd pandnot(const PacketN_8cd &a, const PacketN_8cd &b) {
  FUNC_ENTRY;
  return PacketN_8cd(VEC_XCF64_FROM_VEC_X32(IVP_ANDN_2X32(VEC_X32_FROM_VEC_XCF64(a.v), IVP_NOTN_2X32(VEC_X32_FROM_VEC_XCF64(b.v)))));
}

/** \internal \returns a packet with elements of \a *from quadrupled.
  * For instance, for a packet of 8 elements, 2 scalars will be read from \a *from and
  * replicated to form: {from[0],from[0],from[0],from[0],from[1],from[1],from[1],from[1]}
  * Currently, this function is only used in matrix products.
  * For packet-size smaller or equal to 4, this function is equivalent to pload1 
  */
template <> EIGEN_STRONG_INLINE PacketN_8cd ploadquad<PacketN_8cd>(const complexdouble *from) {
  FUNC_ENTRY;
  EIGEN_DEBUG_ALIGNED_LOAD;
  PacketN_8cd tv0 = ploadu<PacketN_8cd>(from);
  xb_vecNx16 tv1 = IVP_MOVNX16_FROMN_8XCF64(IVP_CONSTN_8XCF64(0));
  xb_vecNx16 tv2 = IVP_MOVNX16_FROMN_8XCF64(IVP_CONSTN_8XCF64(0));
  xb_vecNx16 tv3 = IVP_MOVNX16_FROMN_8XCF64(IVP_CONSTN_8XCF64(0));
  xb_vecNx16 tv4 = IVP_MOVNX16_FROMN_8XCF64(IVP_CONSTN_8XCF64(0));

  signed char ALIGN128 sel_64b_interleave_2[128] =
  {
    0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 64, 96, 65, 97, 66, 98, 67, 99, 68, 100, 69, 101, 70, 102, 71, 103, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 72, 104, 73, 105, 74, 106, 75, 107, 76, 108, 77, 109, 78, 110, 79, 111,
    16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 80, 112, 81, 113, 82, 114, 83, 115, 84, 116, 85, 117, 86, 118, 87, 119, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63, 88, 120, 89, 121, 90, 122, 91, 123, 92, 124, 93, 125, 94, 126, 95, 127
  };

  xb_vec2Nx8 vecselpat_64b_interleave_2 = *(xt_reinterpret_cast<xb_vec2Nx8,signed char>(sel_64b_interleave_2)); // interleave 4

  IVP_DSELNX16(tv2, tv1, IVP_MOVNX16_FROMN_8XCF64(tv0.v), IVP_MOVNX16_FROMN_8XCF64(tv0.v), vecselpat_64b_interleave_2);
  IVP_DSELNX16(tv4, tv3, tv1, tv1, vecselpat_64b_interleave_2);
  return PacketN_8cd(IVP_MOVN_8XCF64_FROMNX16(tv3));
}

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
    IVP_LSN_8XCF64_XP(tvf, tp, stride_bytes);
    IVP_SSN_8XCF64_IP(IVP_MOVN_8XCF64_FROMCF64(tvf), tpf, sizeof(xtcomplexdouble));
  }
  return PacketN_8cd(ta);
}

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
    IVP_LSN_8XCF64_IP(tvf, tpf, sizeof(xtcomplexdouble));
    IVP_SSN_8XCF64_XP(IVP_MOVN_8XCF64_FROMCF64(tvf), tpt, stride_bytes);
  }
}

EIGEN_DEVICE_FUNC inline void ptranspose(PacketBlock<PacketN_8cd, CONST_4> &kernel) {
  FUNC_ENTRY;
  // Declare vec
  #define DR(_var,_num) xb_vecNx16 _var##_num = IVP_MOVNX16_FROMN_8XCF64(IVP_CONSTN_8XCF64(0))
  // Load vec
  #define LR(_var,_num) _var##_num = IVP_MOVNX16_FROMN_8XCF64(kernel.packet[_num].v)
  // Store vec
  #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num].v = IVP_MOVN_8XCF64_FROMNX16(_var##_src_num)

  // Declare vectors
  #define DECL_VEC(_var) \
    DR(_var,0); DR(_var,1); DR(_var,2); DR(_var,3);

  // Load vectors
  #define LOAD_VEC(_var) \
    LR(_var,0); LR(_var,1); LR(_var,2); LR(_var,3);

  // Store vectors
  #define STORE_VEC(_var) \
    SR(_var,0,0); SR(_var,1,1); SR(_var,2,2); SR(_var,3,3);

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


  /* This is first step in transpose operation. In first step, input vectors and
   * output vectors have same sequence. This will change in 'NEXT_STEP', where
   * output vectors will have the same sequence but input vector sequence will be
   * different. */
  #define FIRST_STEP(_pat, _out, _inp)                              \
    IVP_DSELNX16(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
    IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##2, _pat);     \

  /* This is subsequent step in transpose operation. The only difference between
   * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
   * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
  #define NEXT_STEP(_pat, _out, _inp)                               \
    IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
    IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##1, _pat);    \

  DECL_VEC(tvf);
  DECL_VEC(tvs);
  LOAD_VEC(tvf);
  FIRST_STEP(vecselpat_64b_interleave_2, tvs, tvf);
  NEXT_STEP(vecselpat_64b_interleave_4, tvf, tvs);
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
  #define DR(_var,_num) xb_vecNx16 _var##_num = IVP_MOVNX16_FROMN_8XCF64(IVP_CONSTN_8XCF64(0))
  // Load vec
  #define LR(_var,_num) _var##_num = IVP_MOVNX16_FROMN_8XCF64(kernel.packet[_num].v)
  // Store vec
  #define SR(_var,_dst_num,_src_num) kernel.packet[_dst_num].v = IVP_MOVN_8XCF64_FROMNX16(_var##_src_num)

  // Declare vectors
  #define DECL_VEC(_var) \
    DR(_var,0); DR(_var,1); DR(_var,2); DR(_var,3); DR(_var,4); DR(_var,5); DR(_var,6); DR(_var,7);

  // Load vectors
  #define LOAD_VEC(_var) \
    LR(_var,0); LR(_var,1); LR(_var,2); LR(_var,3); LR(_var,4); LR(_var,5); LR(_var,6); LR(_var,7);

  // Store vectors
  #define STORE_VEC(_var) \
    SR(_var,0,0); SR(_var,1,1); SR(_var,2,4); SR(_var,3,5); SR(_var,4,2); SR(_var,5,3); SR(_var,6,6); SR(_var,7,7);

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
      
  /* This is first step in transpose operation. In first step, input vectors and
   * output vectors have same sequence. This will change in 'NEXT_STEP', where
   * output vectors will have the same sequence but input vector sequence will be
   * different. */
  #define FIRST_STEP(_pat, _out, _inp)                              \
    IVP_DSELNX16(_out##1, _out##0, _inp##1, _inp##0, _pat);     \
    IVP_DSELNX16(_out##3, _out##2, _inp##3, _inp##2, _pat);     \
    IVP_DSELNX16(_out##5, _out##4, _inp##5, _inp##4, _pat);     \
    IVP_DSELNX16(_out##7, _out##6, _inp##7, _inp##6, _pat);     \

  /* This is subsequent step in transpose operation. The only difference between
   * this in and 'FIRST_STEP' is the sequence used for input vectors. Output
   * vectors sequence is same with both 'FIRST_STEP' and this 'NEXT_STEP' */
  #define NEXT_STEP(_pat, _out, _inp)                              \
    IVP_DSELNX16(_out##1, _out##0, _inp##2, _inp##0, _pat);    \
    IVP_DSELNX16(_out##3, _out##2, _inp##6, _inp##4, _pat);    \
    IVP_DSELNX16(_out##5, _out##4, _inp##3, _inp##1, _pat);   \
    IVP_DSELNX16(_out##7, _out##6, _inp##7, _inp##5, _pat);  \

  DECL_VEC(tvf);
  DECL_VEC(tvs);
  LOAD_VEC(tvf);
  FIRST_STEP(vecselpat_64b_interleave_2, tvs, tvf);
  NEXT_STEP(vecselpat_64b_interleave_4, tvf, tvs);
  NEXT_STEP(vecselpat_64b_interleave_8, tvs, tvf);
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
} // end namespace internal

} // end namespace Eigen

#endif

#endif // EIGEN_PACKET_MATH_CF64_VX_XTENSA_H
