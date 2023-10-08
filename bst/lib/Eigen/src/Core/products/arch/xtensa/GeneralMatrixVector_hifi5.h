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
/*
 Copyright (c) 2011, Intel Corporation. All rights reserved.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of Intel Corporation nor the names of its contributors may
   be used to endorse or promote products derived from this software without
   specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ********************************************************************************
 *   Content : Eigen bindings to BLAS F77
 *   General matrix-vector product functionality based on ?GEMV.
 ********************************************************************************
*/

#ifndef EIGEN_GENERAL_MATRIX_VECTOR_HIFI5_H
#define EIGEN_GENERAL_MATRIX_VECTOR_HIFI5_H

#include <iomanip>
#include "mtxOps_f32_hifi5.h"

#define USE_STRIDED_MATVEC_KERNELS 1

namespace Eigen { 

namespace internal {

/**********************************************************************
* This file implements general matrix-vector multiplication using BLAS
* gemv function via partial specialization of
* general_matrix_vector_product::run(..) method for float, double,
* std::complex<float> and std::complex<double> types
**********************************************************************/

// gemv specialization
template<typename Index, typename LhsScalar, int StorageOrder, bool ConjugateLhs, typename RhsScalar, bool ConjugateRhs>
struct general_matrix_vector_product_gemv;

#define EIGEN_BLAS_GEMV_SPECIALIZE(Scalar) \
template<typename Index, bool ConjugateLhs, bool ConjugateRhs> \
struct general_matrix_vector_product<Index,Scalar,const_blas_data_mapper<Scalar,Index,ColMajor>,ColMajor,ConjugateLhs,Scalar,const_blas_data_mapper<Scalar,Index,RowMajor>,ConjugateRhs,Specialized> { \
static void run( \
  Index rows, Index cols, \
  const const_blas_data_mapper<Scalar,Index,ColMajor> &lhs, \
  const const_blas_data_mapper<Scalar,Index,RowMajor> &rhs, \
  Scalar* res, Index resIncr, Scalar alpha) \
{ \
  if (ConjugateLhs) { \
    general_matrix_vector_product<Index,Scalar,const_blas_data_mapper<Scalar,Index,ColMajor>,ColMajor,ConjugateLhs,Scalar,const_blas_data_mapper<Scalar,Index,RowMajor>,ConjugateRhs,BuiltIn>::run( \
      rows, cols, lhs, rhs, res, resIncr, alpha); \
  } else { \
    general_matrix_vector_product_gemv<Index,Scalar,ColMajor,ConjugateLhs,Scalar,ConjugateRhs>::run( \
      rows, cols, lhs.data(), lhs.stride(), rhs.data(), rhs.stride(), res, resIncr, alpha); \
  } \
} \
}; \
template<typename Index, bool ConjugateLhs, bool ConjugateRhs> \
struct general_matrix_vector_product<Index,Scalar,const_blas_data_mapper<Scalar,Index,RowMajor>,RowMajor,ConjugateLhs,Scalar,const_blas_data_mapper<Scalar,Index,ColMajor>,ConjugateRhs,Specialized> { \
static void run( \
  Index rows, Index cols, \
  const const_blas_data_mapper<Scalar,Index,RowMajor> &lhs, \
  const const_blas_data_mapper<Scalar,Index,ColMajor> &rhs, \
  Scalar* res, Index resIncr, Scalar alpha) \
{ \
    general_matrix_vector_product_gemv<Index,Scalar,RowMajor,ConjugateLhs,Scalar,ConjugateRhs>::run( \
      rows, cols, lhs.data(), lhs.stride(), rhs.data(), rhs.stride(), res, resIncr, alpha); \
} \
}; \

EIGEN_BLAS_GEMV_SPECIALIZE(float)

#define EIGEN_BLAS_GEMV_SPECIALIZATION(EIGTYPE,BLASTYPE,BLASFUNC) \
template<typename Index, int LhsStorageOrder, bool ConjugateLhs, bool ConjugateRhs> \
struct general_matrix_vector_product_gemv<Index,EIGTYPE,LhsStorageOrder,ConjugateLhs,EIGTYPE,ConjugateRhs> \
{ \
typedef Matrix<EIGTYPE,Dynamic,1,ColMajor> GEMVVector;\
\
static void run( \
  Index rows, Index cols, \
  const EIGTYPE* lhs, Index lhsStride, \
  const EIGTYPE* rhs, Index rhsIncr, \
  EIGTYPE* res, Index resIncr, EIGTYPE alpha) \
  { \
    int M,N,P;\
    float * t;\
    float * z;\
    float * x;\
    float * y;\
    if (LhsStorageOrder==ColMajor){\
      M=1;\
      N=cols;\
      P=rows;\
      y = (BLASTYPE*)lhs;\
      x = (BLASTYPE*)rhs;\
      z = (BLASTYPE*)res;\
      if ((rows == lhsStride) && (1 == rhsIncr) && (1 == resIncr)) {\
        if ((0 == (P%4)) && (0 == (N%4))){\
          /* Fast kernel */ \
          vec_mtxmpyf_fast(\
              z,\
              y,\
              x,\
              P,N);\
        } else {\
          vec_mtxmpyf(\
              z,\
              y,\
              x,\
              P,N);\
        }\
      } else {\
        vec_mtxmpyf_stride(\
            z,\
            y,\
            x,\
            lhsStride,\
            rhsIncr,\
            resIncr,\
            P,N);\
      }\
    } else {\
      M=rows;\
      N=cols;\
      P=1;\
      x = (BLASTYPE*)lhs;\
      y = (BLASTYPE*)rhs;\
      z = (BLASTYPE*)res;\
      if ((cols == lhsStride) && (1 == rhsIncr) && (1 == resIncr)) {\
        if ((0 == (M%4)) && (0 == (N%4))){\
          /* Fast kernel */ \
          mtx_vecmpyf_fast(\
              z,\
              x,\
              y,\
              M,N);\
        } else {\
          mtx_vecmpyf(\
              z,\
              x,\
              y,\
              M,N);\
        }\
      } else {\
        mtx_vecmpyf_stride(\
            z,\
            x,\
            y,\
            lhsStride,\
            rhsIncr,\
            resIncr,\
            M,N);\
      }\
    }\
    int j;\
    for (j = 0; j < rows; j++) {\
      z[(j*resIncr)] = alpha * z[(j*resIncr)];\
    }\
  }\
};

EIGEN_BLAS_GEMV_SPECIALIZATION(float,    float,  sgemv_)

} // end namespase internal

} // end namespace Eigen

#endif
