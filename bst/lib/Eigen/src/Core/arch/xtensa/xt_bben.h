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

#ifndef _XTENSA_XT_BBEN_H_HEADER
#define _XTENSA_XT_BBEN_H_HEADER

#if XCHAL_HAVE_VISION
/* Below Mapping for float32 */
#define BBE_FLOATN_2X32                   IVP_FLOATN_2X32
#define BBE_FLOAT64N_4X64                 IVP_FLOAT64N_4X64
#define BBE_SEQN_2X32                     IVP_SEQN_2X32
#define BBE_SEQN_4X64                     IVP_SEQN_4X64
#define BBE_DIVN_2XF32                    IVP_DIVN_2XF32
#define BBE_DIVN_4XF64                    IVP_DIVN_4XF64
#define BBE_MULAN_2XF32                   IVP_MULAN_2XF32
#define BBE_MULAN_4XF64                   IVP_MULAN_4XF64
#define BBE_MOVN_2XF32_FROMN_2X32         IVP_MOVN_2XF32_FROMN_2X32
#define BBE_ANDN_2X32                     IVP_ANDN_2X32
#define BBE_ANDN_4X64                     IVP_ANDN_4X64
#define BBE_MOVN_2X32_FROMN_2XF32         IVP_MOVN_2X32_FROMN_2XF32
#define BBE_ORN_2X32                      IVP_ORN_2X32
#define BBE_ORN_4X64                      IVP_ORN_4X64
#define BBE_MINN_2XF32                    IVP_MINN_2XF32
#define BBE_MINN_4XF64                    IVP_MINN_4XF64
#define BBE_MAXN_2XF32                    IVP_MAXN_2XF32
#define BBE_MAXN_4XF64                    IVP_MAXN_4XF64
#define BBE_FIROUNDN_2XF32                IVP_FIROUNDN_2XF32
#define BBE_FIROUNDN_4XF64                IVP_FIROUNDN_4XF64
#define BBE_FICEILN_2XF32                 IVP_FICEILN_2XF32
#define BBE_FICEILN_4XF64                 IVP_FICEILN_4XF64
#define BBE_FIFLOORN_2XF32                IVP_FIFLOORN_2XF32
#define BBE_FIFLOORN_4XF64                IVP_FIFLOORN_4XF64
#define BBE_LAN_2XF32_PP                  IVP_LAN_2XF32_PP
#define BBE_LAVN_2XF32_XP                 IVP_LAVN_2XF32_XP
#define BBE_LAN_2XF32_IP                  IVP_LAN_2XF32_IP
#define BBE_SELN_2XF32I                   IVP_SELN_2XF32I
#define BBE_LAN_4XF64_PP                  IVP_LAN_4XF64_PP
#define BBE_LAVN_4XF64_XP                 IVP_LAVN_4XF64_XP
#define BBE_LAN_4XF64_IP                  IVP_LAN_4XF64_IP
#define BBE_SELN_4XF64I                   IVP_SELN_4XF64I
#define BBE_SELI_32B_INTERLEAVE_1_LO      IVP_SELI_32B_INTERLEAVE_1_LO
#define BBE_ZALIGN                        IVP_ZALIGN
#define BBE_SAVN_2XF32_XP                 IVP_SAVN_2XF32_XP
#define BBE_SAN_2XF32_IP                  IVP_SAN_2XF32_IP
#define BBE_SAN_2XF32POS_FP               IVP_SAPOSN_2XF32_FP
#define BBE_LSN_2XF32_XP                  IVP_LSN_2XF32_XP
#define BBE_SSN_2XF32_IP                  IVP_SSN_2XF32_IP
#define BBE_SELSN_2XF32                   IVP_SELSN_2XF32
#define BBE_SHFLN_2XF32I                  IVP_SHFLN_2XF32I
#define BBE_SAVN_4XF64_XP                 IVP_SAVN_4XF64_XP
#define BBE_SAN_4XF64_IP                  IVP_SAN_4XF64_IP
#define BBE_SAN_4XF64POS_FP               IVP_SAPOSN_4XF64_FP
#define BBE_LSN_4XF64_XP                  IVP_LSN_4XF64_XP
#define BBE_SSN_4XF64_IP                  IVP_SSN_4XF64_IP
#define BBE_SELSN_4XF64                   IVP_SELSN_4XF64
#define BBE_SHFLN_4XF64I                  IVP_SHFLN_4XF64I
#define BBE_SHFLI_32B_REVERSE_1           IVP_SHFLI_32B_REVERSE_1
#define BBE_ABSN_2XF32                    IVP_ABSN_2XF32
#define BBE_CONSTN_2XF32                  IVP_CONSTN_2XF32
#define BBE_RADDN_2XF32                   IVP_RADDN_2XF32
#define BBE_ABSN_4XF64                    IVP_ABSN_4XF64
#define BBE_CONSTN_4XF64                  IVP_CONSTN_4XF64
#define BBE_RADDN_4XF64                   IVP_RADDN_4XF64
#define BBE_SELI_32B_EXTRACT_1_OF_2_OFF_0 IVP_SELI_32B_EXTRACT_1_OF_2_OFF_0
#define BBE_SELI_32B_ROTATE_RIGHT_8       IVP_SELI_32B_ROTATE_RIGHT_8
#define BBE_SELI_32B_ROTATE_LEFT_8        IVP_SELI_32B_ROTATE_LEFT_8
#define BBE_LTRN_2I                       IVP_LTRN_2I
#define BBE_MOVN_2XF32T                   IVP_MOVN_2XF32T
#define BBE_RMINN_2XF32                   IVP_RMINN_2XF32
#define BBE_RMAXN_2XF32                   IVP_RMAXN_2XF32
#define BBE_SELN_2XF32                    IVP_SELN_2XF32
#define BBE_DSELN_2XF32I                  IVP_DSELN_2XF32I
#define BBE_MOVN_4XF64T                   IVP_MOVN_4XF64T
#define BBE_RMINN_4XF64                   IVP_RMINN_4XF64
#define BBE_RMAXN_4XF64                   IVP_RMAXN_4XF64
#define BBE_SELN_4XF64                    IVP_SELN_4XF64
#define BBE_DSELN_4XF64I                  IVP_DSELN_4XF64I
#define BBE_DSELI_32B_INTERLEAVE_1        IVP_DSELI_32B_INTERLEAVE_1
#define BBE_DSELI_32B_INTERLEAVE_2        IVP_DSELI_32B_INTERLEAVE_2
#define BBE_NOTBN_2                       IVP_NOTBN_2
#define BBE_DSELN_2X32I                   IVP_DSELN_2X32I
#define BBE_SSN_2XF32_XP                  IVP_SSN_2XF32_XP
#define BBE_LSN_2XF32_IP                  IVP_LSN_2XF32_IP
#define BBE_MOVNX16_FROMN_2XF32           IVP_MOVNX16_FROMN_2XF32
#define BBE_RSQRTN_2XF32                  IVP_RSQRTN_2XF32
#define BBE_SQRTN_2XF32                   IVP_SQRTN_2XF32
#define BBE_SSN_4XF64_XP                  IVP_SSN_4XF64_XP
#define BBE_LSN_4XF64_IP                  IVP_LSN_4XF64_IP
#define BBE_MOVNX16_FROMN_4XF64           IVP_MOVNX16_FROMN_4XF64
#define BBE_RSQRTN_4XF64                  IVP_RSQRTN_4XF64
#define BBE_SQRTN_4XF64                   IVP_SQRTN_4XF64
#define BBE_DSELI_32B_DEINTERLEAVE_2      IVP_DSELI_32B_INTERLEAVE_2
#define BBE_NOTN_2X32                     IVP_NOTN_2X32
#define BBE_NOTN_4X64                     IVP_NOTN_4X64
#define BBE_SELI_32B_EXTRACT_LO_HALVES    IVP_SELI_32B_EXTRACT_LO_HALVES
#define BBE_SELI_32B_ROTATE_RIGHT_16      IVP_SELI_32B_ROTATE_LEFT_16
#define BBE_MOVNX16_FROMNX16U             IVP_MOVNX16_FROMNX16U
#define BBE_ADDN_2XF32T                   IVP_ADDN_2XF32T
#define BBE_OEQN_2XF32                    IVP_OEQN_2XF32
#define BBE_OLEN_2XF32                    IVP_OLEN_2XF32
#define BBE_OLTN_2XF32                    IVP_OLTN_2XF32
#define BBE_RMAXNUMN_2XF32                IVP_RMAXNUMN_2XF32
#define BBE_RMINNUMN_2XF32                IVP_RMINNUMN_2XF32
#define BBE_ULTN_2XF32                    IVP_ULTN_2XF32
#define BBE_ADDN_4XF64T                   IVP_ADDN_4XF64T
#define BBE_OEQN_4XF64                    IVP_OEQN_4XF64
#define BBE_OLEN_4XF64                    IVP_OLEN_4XF64
#define BBE_OLTN_4XF64                    IVP_OLTN_4XF64
#define BBE_RMAXNUMN_4XF64                IVP_RMAXNUMN_4XF64
#define BBE_RMINNUMN_4XF64                IVP_RMINNUMN_4XF64
#define BBE_ULTN_4XF64                    IVP_ULTN_4XF64
#define BBE_MOVN_2X32UT                   IVP_MOVN_2X32UT
#define BBE_MOVNX16_FROMN_2X32U           IVP_MOVNX16_FROMN_2X32U
#define BBE_MOVN_4X64UT                   IVP_MOVN_4X64UT
#define BBE_MOVNX16_FROMN_4X64U           IVP_MOVNX16_FROMN_4X64U
#define BBE_ADDF32T                       IVP_ADDF32T
#define BBE_ADDF64T                       IVP_ADDF64T

/* Below Mapping for complexfloat */
#define BBE_MOVN_4XCF32_FROMNX16          IVP_MOVN_4XCF32_FROMNX16
#define BBE_LSRN_2XF32_I                  IVP_LSRN_2XF32_I
#define BBE_LSRN_4XF64_I                  IVP_LSRN_4XF64_I
#define BBE_CMPLXF32                      IVP_CMPLXF32
#define BBE_LSRN_4XCF32_I                 IVP_LSRN_4XCF32_I
#define BBE_NEGN_4XCF32                   IVP_NEGN_4XCF32
#define BBE_CONJN_4XCF32                  IVP_CONJN_4XCF32
#define BBE_DIVN_4XCF32                   IVP_DIVN_4XCF32
#define BBE_MOVNX16_FROMN_2X32            IVP_MOVNX16_FROMN_2X32
#define BBE_MOVNX16_FROMN_4X64            IVP_MOVNX16_FROMN_4X64
#define BBE_MOVN_4X64_FROMNX16            IVP_MOVN_4X64_FROMNX16
#define BBE_MOVN_2X32_FROMNX16            IVP_MOVN_2X32_FROMNX16
#define BBE_MOVNX16_FROMN_4XCF32          IVP_MOVNX16_FROMN_4XCF32
#define BBE_XORN_2X32                     IVP_XORN_2X32
#define BBE_LAN_4XCF32_PP                 IVP_LAN_4XCF32_PP
#define BBE_LAVN_4XCF32_XP                IVP_LAVN_4XCF32_XP
#define BBE_LAN_4XCF32_IP                 IVP_LAN_4XCF32_IP
#define BBE_SELI_32B_INTERLEAVE_2_LO      IVP_SELI_32B_INTERLEAVE_2_LO
#define BBE_SELI_64B_INTERLEAVE_1_LO      IVP_SELI_64B_INTERLEAVE_1_LO
#define BBE_SAVN_4XCF32_XP                IVP_SAVN_4XCF32_XP
#define BBE_SAN_4XCF32_IP                 IVP_SAN_4XCF32_IP
#define BBE_SELI_64B_INTERLEAVE_1_EVEN    IVP_SELI_64B_INTERLEAVE_1_EVEN
#define BBE_LSN_4XCF32_XP                 IVP_LSN_4XCF32_XP
#define BBE_SSN_4XCF32_IP                 IVP_SSN_4XCF32_IP
#define BBE_SELSN_4XCF32                  IVP_SELSN_4XCF32
#define BBE_SHFLI_64B_REVERSE_1           IVP_SHFLI_64B_REVERSE_1
#define BBE_SELI_64B_EXTRACT_1_OF_2_OFF_0 BBE_SELI_64B_EXTRACT_1_OF_2_OFF_0
#define BBE_SELI_64B_EXTRACT_1_OF_2_OFF_1 IVP_SELI_64B_EXTRACT_1_OF_2_OFF_1
#define BBE_SELI_64B_ROTATE_RIGHT_4       IVP_SELI_64B_ROTATE_RIGHT_4
#define BBE_SELI_64B_ROTATE_LEFT_4        IVP_SELI_64B_ROTATE_LEFT_4
#define BBE_LTRN_4I                       IVP_LTRN_4I
#define BBE_CONSTN_4XCF32                 IVP_CONSTN_4XCF32
#define BBE_SELI_64B_ROTATE_RIGHT_8       IVP_SELI_64B_ROTATE_RIGHT_8
#define BBE_MOVN_4XCF32T                  IVP_MOVN_4XCF32T
#define BBE_SELI_64B_ROTATE_LEFT_1        IVP_SELI_64B_ROTATE_LEFT_1
#define BBE_MULN_4XCF32                   IVP_MULN_4XCF32
#define BBE_SELI_64B_ROTATE_LEFT_2        IVP_SELI_64B_ROTATE_LEFT_2
#define BBE_MOVN_2XF32_FROMNX16           IVP_MOVN_2XF32_FROMNX16
#define BBE_MOVN_4XF64_FROMNX16           IVP_MOVN_4XF64_FROMNX16
#define BBE_NOTBN_4                       IVP_NOTBN_4
#define BBE_SHFLI_32B_SWAP_1              IVP_SHFLI_32B_SWAP_1
#define BBE_SSN_4XCF32_XP                 IVP_SSN_4XCF32_XP
#define BBE_RADDN_4XCF32                  IVP_RADDN_4XCF32
#define BBE_SAN_4XCF32POS_FP              IVP_SAPOSN_4XCF32_FP
#define BBE_OEQN_4XF64                    IVP_OEQN_4XF64
#define BBE_CREALN_2XCF32                 IVP_CREALN_2XCF32
#define BBE_CIMAGN_2XCF32                 IVP_CIMAGN_2XCF32
#define BBE_CMPLXN_2XF32                  IVP_CMPLXN_2XF32
#define BBE_CMPLXN_4XF64                  IVP_CMPLXN_4XF64
#define BBE_ANDBN_2                       IVP_ANDBN_2
#define BBE_MOV2N_FROMN_2                 IVP_MOV2N_FROMN_2
#define BBE_MOVN_4_FROM2N                 IVP_MOVN_4_FROM2N

#if defined(IVP_OEQN_4XCF32)
  #define BBE_OEQN_4XCF32                 IVP_OEQN_4XCF32
#endif


#define BBE_EXTRACTN_4XCF32_FROMN_2XCF32_V0 IVP_EXTRACTN_4XCF32_FROMN_2XCF32_V0
#define BBE_JOINN_2XCF32_FROMN_4XCF32       IVP_JOINN_2XCF32_FROMN_4XCF32

/* Below Mapping for complexdouble */
#define BBE_MOVN_8XCF64_FROMNX16            IVP_MOVN_8XCF64_FROMNX16
#define BBE_MOVNX16_FROMN_8XCF64            IVP_MOVNX16_FROMN_8XCF64
#define BBE_CMPLXF64                        IVP_CMPLXF64
#define BBE_LSRN_8XCF64_I                   IVP_LSRN_8XCF64_I
#define BBE_NEGN_8XCF64                     IVP_NEGN_8XCF64
#define BBE_CONJN_8XCF64                    IVP_CONJN_8XCF64
#define BBE_MULN_8XCF64                     IVP_MULN_8XCF64
#define BBE_DIVN_8XCF64                     IVP_DIVN_8XCF64
#define BBE_LAN_8XCF64_PP                   IVP_LAN_8XCF64_PP
#define BBE_LAN_8XCF64_IP                   IVP_LAN_8XCF64_IP
#define BBE_SELI_64B_INTERLEAVE_1_ODD       IVP_SELI_64B_INTERLEAVE_1_ODD
#define BBE_CONSTN_8XCF64                   IVP_CONSTN_8XCF64
#define BBE_SAN_8XCF64_IP                   IVP_SAN_8XCF64_IP
#define BBE_SAN_8XCF64POS_FP                IVP_SAPOSN_8XCF64_FP
#define BBE_SELSN_8XCF64                    IVP_SELSN_8XCF64
#define BBE_SHFLI_128B_REVERSE_1            IVP_SHFLI_128B_REVERSE_1
#define BBE_SHFLI_128B_SWAP_2               IVP_SHFLI_128B_SWAP_2
#define BBE_SHFLI_128B_SWAP_4               IVP_SHFLI_128B_SWAP_4
#define BBE_RADDN_8XCF64                    IVP_RADDN_8XCF64
#define BBE_LTRN_8I                         IVP_LTRN_8I
#define BBE_MOVN_8XCF64T                    IVP_MOVN_8XCF64T
#define BBE_SHFLI_64B_SWAP_1                IVP_SHFLI_64B_SWAP_1
#define BBE_JOINN_4XCF64_FROMN_8XCF64       IVP_JOINN_4XCF64_FROMN_8XCF64
#define BBE_CREALN_4XCF64                   IVP_CREALN_4XCF64
#define BBE_CIMAGN_4XCF64                   IVP_CIMAGN_4XCF64
#define BBE_EXTRACTN_8XCF64_FROMN_4XCF64_V0 IVP_EXTRACTN_8XCF64_FROMN_4XCF64_V0
#define BBE_ANDBN_4                         IVP_ANDBN_4
#define BBE_MOVN_4XF64_FROMN_4X64           IVP_MOVN_4XF64_FROMN_4X64
#define BBE_SELI_64B_EXTRACT_LO_HALVES      IVP_SELI_EXTRACT_LO_HALVES
#define BBE_MOVN_8_FROM2N                   IVP_MOVN_8_FROM2N
#define BBE_MOV2N_FROMN_4                   IVP_MOV2N_FROMN_4
#define BBE_SAVN_8XCF64_XP                  IVP_SAVN_8XCF64_XP
#define BBE_LAVN_8XCF64_XP                  IVP_LAVN_8XCF64_XP

#if defined(IVP_OEQN_8XCF64)
#define BBE_OEQN_8XCF64                     IVP_OEQN_8XCF64
#endif

// Indirect ISA Mapping
#define BBE_SELN_8XCF64I(_p,_a,_z)          VEC_XCF64_FROM_VEC_XF64(IVP_SELN_4XF64I(VEC_XF64_FROM_VEC_XCF64(_p),VEC_XF64_FROM_VEC_XCF64(_a),_z))
#define BBE_SHFLN_8XCF64I(_p,_z)            VEC_XCF64_FROM_VEC_XF64(IVP_SHFLN_4XF64I(VEC_XF64_FROM_VEC_XCF64(_p),_z))

#elif XCHAL_HAVE_PDX
  /* Below Mapping for datatypes */
  #define xb_vecN_2xf32 xb_vecMxf32
  #define vboolN_2 vboolM
  #define xb_vecN_2x32v xb_vecMx32
  #define xb_vecN_2x32Uv xb_vecMxu32
  
  /* Below Mapping for float32 */
  #define BBE_SELN_2XF32I                   PDX_SELI_MXF32
  #define BBE_SELN_2XF32                    PDX_SEL_MXF32
  #define BBE_NOTBN_2                       PDX_NOT_BM
  #define BBE_SQRTN_2XF32                   PDX_SQRT_MXF32
  #define BBE_RSQRTN_2XF32                  PDX_RSQRT_MXF32  
  #define BBE_SELI_32B_EXTRACT_LO_HALVES    PDX_SELI_32B_EXTRACT_LO_HALVES
  #define BBE_SEQN_2X32                     PDX_SEQ_MX32
  #define BBE_FLOATN_2X32                   PDX_FLOATF32_MX32
  #define BBE_DIVN_2XF32                    PDX_DIV_MXF32
  #define BBE_MULAN_2XF32                   PDX_MULA_MXF32
  #define BBE_MOVN_2X32_FROMN_2XF32         PDX_MOV_MX32_FROM_MXF32
  #define BBE_MOVN_2XF32_FROMN_2X32         PDX_MOV_MXF32_FROM_MX32
  #define BBE_ANDN_2X32                     PDX_AND_MX32
  #define BBE_ORN_2X32                      PDX_OR_MX32
  #define BBE_NOTN_2X32                     PDX_NOT_MX32
  #define BBE_MINN_2XF32                    PDX_MIN_MXF32
  #define BBE_MAXN_2XF32                    PDX_MAX_MXF32
  #define BBE_FIROUNDN_2XF32                PDX_FIROUND_MXF32
  #define BBE_FICEILN_2XF32                 PDX_FICEIL_MXF32
  #define BBE_FIFLOORN_2XF32                PDX_FIFLOOR_MXF32
  #define BBE_LAN_2XF32_PP                  PDX_LA_MXF32_PP
  #define BBE_LAVN_2XF32_XP                 PDX_LAV_MXF32_XP
  #define BBE_LAN_2XF32_IP                  PDX_LA_MXF32_IP
  #define BBE_SELI_32B_INTERLEAVE_1_LO      PDX_SELI_32B_INTERLEAVE_1_LO
  #define BBE_SAVN_2XF32_XP                 PDX_SAV_MXF32_XP
  #define BBE_SAN_2XF32_IP                  PDX_SA_MXF32_IP
  #define BBE_SAN_2XF32POS_FP               PDX_SAPOS_MXF32_FP
  #define BBE_SELSN_2XF32                   PDX_SELS_MXF32
  #define BBE_SHFLN_2XF32I                  PDX_SHFLI_MXF32
  #define BBE_SHFLI_32B_REVERSE_1           PDX_SHFLI_32B_REVERSE_1
  #define BBE_ABSN_2XF32                    PDX_ABS_MXF32
  #define BBE_CONSTN_2XF32                  PDX_CONST_MXF32
  #define BBE_RADDN_2XF32                   PDX_RADD_MXF32
  #define BBE_MULMN_2XF32                   PDX_MULMN_MXF32
  #define BBE_SELI_32B_EXTRACT_1_OF_2_OFF_0 PDX_SELI_32B_EXTRACT_1_OF_2_OFF_0
  #define BBE_RMINN_2XF32                   PDX_RMIN_MXF32
  #define BBE_RMAXN_2XF32                   PDX_RMAX_MXF32
  #define BBE_DSELN_2XF32I                  PDX_DSELI_MXF32
  #define BBE_DSELI_32B_INTERLEAVE_1        PDX_DSELI_32B_INTERLEAVE_1
  #define BBE_DSELI_32B_INTERLEAVE_2        PDX_DSELI_32B_INTERLEAVE_2
  #define BBE_LTRN_2I                       PDX_LTRI_BM
  #define BBE_MOVN_2XF32T                   PDX_MOV_MXF32_T
  #define BBE_ZALIGN                        PDX_Z_ALIGN
  #define BBE_ADDN_2XF32T                   PDX_ADD_MXF32_T
  #define BBE_OEQN_2XF32                    PDX_OEQ_MXF32
  #define BBE_OLEN_2XF32                    PDX_OLE_MXF32
  #define BBE_OLTN_2XF32                    PDX_OLT_MXF32
  #define BBE_RMAXNUMN_2XF32                PDX_RMAXNUM_MXF32
  #define BBE_RMINNUMN_2XF32                PDX_RMINNUM_MXF32
  #define BBE_ULTN_2XF32                    PDX_ULT_MXF32
  #define BBE_MOVN_2X32UT                   PDX_MOV_MXU32_T
  #define BBE_ADDF32T                       PDX_ADD_F32_T
  
  /* Below Mapping for complexfloat */
  #define xb_vecN_4xcf32                    xb_vecM2xcf32
  #define xb_vecN_2xcf32                    xb_vecMxcf32
  #define vboolN_4                          vboolM2

  #define BBE_SELN_4XCF32I                  PDX_SELI_M2XCF32
  #define BBE_SELI_64B_EXTRACT_LO_HALVES    PDX_SELI_64B_EXTRACT_LO_HALVES
  #define BBE_LSRN_2XF32_I                  PDX_LSR_F32_I
  #define BBE_CMPLXF32                      PDX_CMPLXCF32_F32
  #define BBE_LSRN_4XCF32_I                 PDX_LSR_CF32_I
  #define BBE_NEGN_4XCF32                   PDX_NEG_M2XCF32
  #define BBE_CONJN_4XCF32                  PDX_CONJ_M2XCF32
  #define BBE_DIVN_4XCF32                   PDX_DIV_M2XCF32
  #define BBE_XORN_2X32                     PDX_XOR_MX32
  #define BBE_LAN_4XCF32_PP                 PDX_LA_M2XCF32_PP
  #define BBE_LAVN_4XCF32_XP                PDX_LAV_M2XCF32_XP
  #define BBE_LAN_4XCF32_IP                 PDX_LA_M2XCF32_IP
  #define BBE_SAVN_4XCF32_XP                PDX_SAV_M2XCF32_XP
  #define BBE_SAN_4XCF32_IP                 PDX_SA_M2XCF32_IP
  #define BBE_SAN_4XCF32POS_FP              PDX_SAPOS_M2XCF32_FP
  #define BBE_SELI_32B_INTERLEAVE_2_LO      PDX_SELI_32B_INTERLEAVE_2_LO
  #define BBE_SELSN_4XCF32                  PDX_SELS_M2XCF32
  #define BBE_SHFLN_4XCF32I                 PDX_SHFLI_M2XCF32
  #define BBE_SHFLI_64B_REVERSE_1           PDX_SHFLI_64B_REVERSE_1
  #define BBE_SELI_64B_EXTRACT_1_OF_2_OFF_0 PDX_SELI_64B_EXTRACT_1_OF_2_OFF_0
  #define BBE_SELI_64B_EXTRACT_1_OF_2_OFF_1 PDX_SELI_64B_EXTRACT_1_OF_2_OFF_1
  #define BBE_RADDN_4XCF32                  PDX_RADD_M2XCF32
  #define BBE_SELI_64B_ROTATE_LEFT_1        PDX_SELI_64B_ROTATE_LEFT_1
  #define BBE_MULN_4XCF32                   PDX_MUL_M2XCF32
  #define BBE_DSELN_4XCF32I                 PDX_DSELI_M2XCF32
  #define BBE_LTRN_4I                       PDX_LTRI_BM2
  #define BBE_MOVN_4XCF32T                  PDX_MOV_M2XCF32_T
  #define BBE_NOTBN_4                       PDX_NOT_BM2
  #define BBE_SHFLI_32B_SWAP_1              PDX_SHFLI_32B_SWAP_1

  #define BBE_CONSTN_4XCF32                 PDX_CONST_M2XCF32
  #define BBE_OEQN_4XCF32                   PDX_OEQ_M2XCF32
  #define BBE_OEQN_4XCF32T                  PDX_OEQ_M2XCF32_T
  #define BBE_OEQN_4XF64                    PDX_OEQ_M2XF64
  #define BBE_MOVN_4XF64T                   PDX_MOV_M2XF64_T
  #define BBE_CREALN_2XCF32                 PDX_CREALF32_MXCF32
  #define BBE_CIMAGN_2XCF32                 PDX_CIMAGF32_MXCF32
  #define BBE_CMPLXN_2XF32                  PDX_CMPLXCF32_MXF32
  #define BBE_ANDBN_2                       PDX_AND_BM

  #define BBE_EXTRACTN_4XCF32_FROMN_2XCF32_V0 PDX_SPLIT_M2XCF32_MXCF32_L
  #define BBE_JOINN_2XCF32_FROMN_4XCF32       PDX_COMBINE_MXCF32_M2XCF32D

#endif

#endif /* !defined(_XTENSA_XT_BBEN_H_HEADER) */
