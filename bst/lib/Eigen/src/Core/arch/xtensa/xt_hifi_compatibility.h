/*
 * Copyright (c) 2022 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of
 * Cadence Design Systems Inc.  They may be adapted and modified by bona fide
 * purchasers for internal use, but neither the original nor any adapted
 * or modified version may be disclosed or distributed to third parties
 * in any manner, medium, or form, in whole or in part, without the prior
 * written consent of Cadence Design Systems Inc.  This software and its
 * derivatives are to be executed solely on products incorporating a Cadence
 * Design Systems processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _XTENSA_XT_COMPAT_HIFI_H_HEADER
#define _XTENSA_XT_COMPAT_HIFI_H_HEADER

#if XCHAL_HAVE_HIFI3Z
/* Below Mapping is from HiFi5 to HiFi3z */

#define HIGH_S                      XT_HIGH_S
#define AE_LASX2IP                  XT_LASX2IP
#define AE_SASX2IP                  XT_SASX2IP
#define DIV_SX2                     XT_DIV_SX2
#define FIROUND_SX2                 XT_FIROUND_SX2
#define FICEIL_SX2                  XT_FICEIL_SX2
#define FIFLOOR_SX2                 XT_FIFLOOR_SX2
#define FIROUND_SX2                 XT_FIROUND_SX2
#define AE_SSX2I                    XT_SSX2I
#define AE_SASX2IP                  XT_SASX2IP
#define AE_LSXP                     XT_LSXP
#define AE_MOVINT32X2_FROMXTFLOATX2 XT_AE_MOVINT32X2_FROMXTFLOATX2
#define AE_MOVXTFLOATX2_FROMINT32X2 XT_AE_MOVXTFLOATX2_FROMINT32X2
#define ABS_SX2                     XT_ABS_SX2
#define RADD_SX2                    XT_RADD_SX2
#define AE_LSIP                     XT_LSIP
#define AE_SSXP                     XT_SSXP
#define AE_SSIP                     XT_SSIP
#define LOW_S                       XT_LOW_S
#define MUL_S                       XT_MUL_S
#define RMIN_S                      XT_RMIN_S
#define RMAX_S                      XT_RMAX_S
#define OEQ_SX2                     XT_OEQ_SX2
#define OLE_SX2                     XT_OLE_SX2
#define ULT_SX2                     XT_ULT_SX2
#define SQRT_SX2                    XT_SQRT_SX2
#define RSQRT_SX2                   XT_RSQRT_SX2
#define MAXNUM_SX2                  XT_MAX_SX2
#define MINNUM_SX2                  XT_MIN_SX2
#define OLT_SX2                     XT_OLT_SX2
#define MOVT_SX2                    XT_MOVT_SX2
#define CONST_S                     XT_CONST_S
#define AE_SEL32_LH_SX2				      XT_SEL32_LH_SX2
#define AE_SEL32_HH_SX2				      XT_SEL32_HH_SX2
#define AE_SEL32_LL_SX2				      XT_SEL32_LL_SX2

#endif

#endif /* !defined(_XTENSA_XT_COMPAT_HIFI_H_HEADER) */
