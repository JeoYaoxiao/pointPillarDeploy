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

#ifndef EIGEN_MATH_FUNCTIONS_F32_XTENSA_H
  #define EIGEN_MATH_FUNCTIONS_F32_XTENSA_H
  #if XCHAL_HAVE_HIFI5_3 || XCHAL_HAVE_HIFI5 || XCHAL_HAVE_HIFI3Z || XCHAL_HAVE_HIFI1
    #include "MathFunctionsF32_hifi5.h"
  #elif XCHAL_HAVE_PDX
    #include "MathFunctionsF32_gx.h"
  #else
    #include "MathFunctionsF32_kx.h"
  #endif
#endif
