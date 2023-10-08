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
#ifndef EIGEN_PACKET_MATH_COMMON_XTENSA_H
#define EIGEN_PACKET_MATH_COMMON_XTENSA_H

#define SIMD_N_2_EQ_2() (2 == SIMD_N_2)
#define SIMD_N_2_EQ_4() (4 == SIMD_N_2)
#define SIMD_N_2_EQ_8() (8 == SIMD_N_2)
#define SIMD_N_2_EQ_16() (16 == SIMD_N_2)
#define SIMD_N_2_EQ_32() (32 == SIMD_N_2)

#define SIMD_N_4_EQ_1() (1 == SIMD_N_4)
#define SIMD_N_4_EQ_2() (2 == SIMD_N_4)
#define SIMD_N_4_EQ_4() (4 == SIMD_N_4)
#define SIMD_N_4_EQ_8() (8 == SIMD_N_4)
#define SIMD_N_4_EQ_16() (16 == SIMD_N_4)

#define SIMD_N_EQ(_simd_div_factor,_compare_val) (((SIMD_N)/(_simd_div_factor)) == (_compare_val))
#define SIMD_N_GE(_simd_div_factor,_compare_val) (((SIMD_N)/(_simd_div_factor)) >= (_compare_val))
#define SIMD_N_GT(_simd_div_factor,_compare_val) (((SIMD_N)/(_simd_div_factor)) >  (_compare_val))
#define SIMD_N_LE(_simd_div_factor,_compare_val) (((SIMD_N)/(_simd_div_factor)) <= (_compare_val))
#define SIMD_N_LT(_simd_div_factor,_compare_val) (((SIMD_N)/(_simd_div_factor)) <  (_compare_val))

#if defined(DEBUG_FUNC_ENTRY) && !defined(FUNC_ENTRY)
  #if defined(GPM_FUNC_ENTRY)
    #define FUNC_ENTRY GPM_FUNC_ENTRY
  #else
    #define FUNC_ENTRY std::cout<<__func__<<" "<<__FILE__<<":"<<__LINE__<<std::endl
  #endif
#elif !defined(FUNC_ENTRY)
  #define FUNC_ENTRY
#endif

// Used at places for refererring a const values
#define CONST_2 2
#define CONST_4 4
#define CONST_8 8
#define CONST_16 16
#define CONST_32 32

#if SIMD_N_2_EQ_2()
#define ALIGN_N_2 Aligned8
#elif SIMD_N_2_EQ_4()
#define ALIGN_N_2 Aligned16
#elif SIMD_N_2_EQ_8()
#define ALIGN_N_2 Aligned32
#elif SIMD_N_2_EQ_16()
#define ALIGN_N_2 Aligned64
#elif SIMD_N_2_EQ_32()
#define ALIGN_N_2 Aligned128
#else
// Default value
#define ALIGN_N_2 Aligned128
#endif

#ifndef EIGEN_ARCH_DEFAULT_NUMBER_OF_REGISTERS
#define EIGEN_ARCH_DEFAULT_NUMBER_OF_REGISTERS (2 * sizeof(void *))
#endif

#if defined(__XTENSA__) && defined(__XCLIB__)
#define XCLIB_SUPPORT 1
#else
#define XCLIB_SUPPORT 0
#endif

// Used at places for move operations
#if XCHAL_HAVE_BBENEP || XCHAL_HAVE_VISION
  #define VEC_XCF32_FROM_VEC_XF32(_a) BBE_MOVN_4XCF32_FROMNX16(BBE_MOVNX16_FROMN_2XF32(_a))
  #define VEC_XCF32_FROM_VEC_X32(_a)  BBE_MOVN_4XCF32_FROMNX16(BBE_MOVNX16_FROMN_2X32(_a))
  #define VEC_X32_FROM_VEC_XCF32(_a)  BBE_MOVN_2X32_FROMNX16(BBE_MOVNX16_FROMN_4XCF32(_a))
  #define VEC_XF32_FROM_VEC_XCF32(_a) BBE_MOVN_2XF32_FROMNX16(BBE_MOVNX16_FROMN_4XCF32(_a))
  #define VEC_XF32_FROM_VEC_X32U(_a) BBE_MOVN_2XF32_FROMNX16(BBE_MOVNX16_FROMN_2X32U(_a))
  #define VEC_XF64_FROM_VEC_XCF32(_a) BBE_MOVN_4XF64_FROMNX16(BBE_MOVNX16_FROMN_4XCF32(_a))
  #define VEC_XF64_FROM_VEC_X64U(_a) BBE_MOVN_4XF64_FROMNX16(BBE_MOVNX16_FROMN_4X64U(_a))
  #define VEC_XCF64_FROM_VEC_XF64(_a) BBE_MOVN_8XCF64_FROMNX16(BBE_MOVNX16_FROMN_4XF64(_a))
  #define VEC_XF64_FROM_VEC_XCF64(_a) BBE_MOVN_4XF64_FROMNX16(BBE_MOVNX16_FROMN_8XCF64(_a))
  #define VEC_X32_FROM_VEC_XCF64(_a)  BBE_MOVN_2X32_FROMNX16(BBE_MOVNX16_FROMN_8XCF64(_a))
  #define VEC_XCF64_FROM_VEC_X32(_a)  BBE_MOVN_8XCF64_FROMNX16(BBE_MOVNX16_FROMN_2X32(_a))
#elif XCHAL_HAVE_PDX
  #define VEC_XCF32_FROM_VEC_XF32(_a) PDX_MOV_M2XCF32_FROM_4MX8(PDX_MOV_4MX8_FROM_MXF32(_a))
  #define VEC_XCF32_FROM_VEC_X32(_a)  PDX_MOV_M2XCF32_FROM_4MX8(PDX_MOV_4MX8_FROM_MX32(_a))
  #define VEC_X32_FROM_VEC_XCF32(_a)  PDX_MOV_MX32_FROM_4MX8(PDX_MOV_4MX8_FROM_M2XCF32(_a))
  #define VEC_XF32_FROM_VEC_XCF32(_a) PDX_MOV_MXF32_FROM_4MX8(PDX_MOV_4MX8_FROM_M2XCF32(_a))
  #define VEC_XF32_FROM_VEC_X32U(_a) PDX_MOV_MXF32_FROM_4MX8(PDX_MOV_4MX8_FROM_MXU32(_a))
  #define VEC_XF64_FROM_VEC_XCF32(_a) PDX_MOV_M2XF64_FROM_4MX8(PDX_MOV_4MX8_FROM_M2XCF32(_a))

  #if defined(PDX_MULAMNW_MX32)
    #define HAVE_FUSIONG3FP 0
  #else
    #define HAVE_FUSIONG3FP 1
  #endif
#endif 

#ifndef BBE_MOVN_4XCF32_FROMN_4XF64
  #define BBE_MOVN_4XCF32_FROMN_4XF64(_v) BBE_MOVN_4XCF32_FROMNX16(BBE_MOVNX16_FROMN_4XF64(_v))
#endif

#ifndef BBE_MOVN_4XF64_FROMN_4XCF32
  #define BBE_MOVN_4XF64_FROMN_4XCF32(_v) BBE_MOVN_4XF64_FROMNX16(BBE_MOVNX16_FROMN_4XCF32(_v))
#endif

#ifndef BBE_MOVN_4X64_FROMN_4XF64
  #define BBE_MOVN_4X64_FROMN_4XF64(_v) BBE_MOVN_4X64_FROMNX16(BBE_MOVNX16_FROMN_4XF64(_v))
#endif

#ifndef BBE_MOVN_4XF64_FROMN_4X64
  #define BBE_MOVN_4XF64_FROMN_4X64(_v) BBE_MOVN_4XF64_FROMNX16(BBE_MOVNX16_FROMN_4X64(_v))
#endif

// #ifndef EIGEN_HAS_SINGLE_INSTRUCTION_MADD
// #define EIGEN_HAS_SINGLE_INSTRUCTION_MADD 1
// #endif

// Used at ptranspose in VX cores (where the dsel/sel patterns are not available)
#ifndef ALIGN64
  #define ALIGN64  __attribute__((aligned(64)))
#endif

#ifndef ALIGN128
  #define ALIGN128  __attribute__((aligned(128)))
#endif

template <typename DestT, typename SrcT> EIGEN_STRONG_INLINE DestT * xt_reinterpret_cast(SrcT *src) {
  return reinterpret_cast<DestT*>(src);
}

template <typename DestT, typename SrcT> EIGEN_STRONG_INLINE DestT * xt_reinterpret_const_cast(const SrcT *src) {
  return reinterpret_cast<DestT*>(const_cast<SrcT*>(src));
}

#define rep_T(_a,_b,_Tfull,_Thalf,_func_intr,_func_rep) \
  _Thalf(_func_intr<_Tfull>(_func_rep<_Tfull>(_a.v), _func_rep<_Tfull>(_b.v)));

typedef union {
  unsigned int ui32;
  float f32;
} ui32_f32;

typedef union {
  unsigned long long int ui64;
  double f64;
} ui64_f64;

#ifndef PVAR
    #define PVAR(_v) std::cout<<"[DBG] "<<__FILE__<<":"<<__LINE__<<std::endl<<#_v<<std::endl<<_v<<std::endl<<std::endl
#endif

#endif // EIGEN_PACKET_MATH_COMMON_XTENSA_H
