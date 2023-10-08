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

#ifndef EIGEN_GENERAL_MATRIX_VECTOR_BLAS_XTENSA_H
#define EIGEN_GENERAL_MATRIX_VECTOR_BLAS_XTENSA_H
  #if XCHAL_HAVE_HIFI5
    #include "GeneralMatrixVector_hifi5.h"
  #else
    #error "[ERROR] DSPLIB not supported for this DSP core"
  #endif
#endif // EIGEN_GENERAL_MATRIX_VECTOR_BLAS_XTENSA_H
