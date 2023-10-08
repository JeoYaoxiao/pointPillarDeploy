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

#ifndef EIGEN_XTENSA_VERSION_STRING_H
#define EIGEN_XTENSA_VERSION_STRING_H

static EIGEN_STRONG_INLINE std::string getEigenBaseVersion(){
  std::string eigenBaseVersion = std::to_string(EIGEN_WORLD_VERSION) + "." + std::to_string(EIGEN_MAJOR_VERSION) + "." + std::to_string(EIGEN_MINOR_VERSION);
  return eigenBaseVersion;
}

static EIGEN_STRONG_INLINE std::string getEigenXtensaVersion(){
  std::string eigenXtensaVersion = std::to_string(EIGEN_XTENSA_WORLD_VERSION) + "." + std::to_string(EIGEN_XTENSA_MAJOR_VERSION) + "." + std::to_string(EIGEN_XTENSA_MINOR_VERSION);
  return eigenXtensaVersion;
}

#endif // EIGEN_XTENSA_VERSION_STRING_H
