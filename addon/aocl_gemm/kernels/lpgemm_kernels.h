/*

   BLIS
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    - Neither the name(s) of the copyright holder(s) nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef BLIS_LPGEMM_KERN_H
#define BLIS_LPGEMM_KERN_H

#include "lpgemm_post_ops.h"

#define LPGEMM_MAIN_KERN(A_type,B_type,C_type,LP_SFX) \
void lpgemm_rowvar_ ## LP_SFX \
     ( \
       const dim_t     m0, \
       const dim_t     n0, \
       const dim_t     k0, \
       const A_type*   a, \
       const dim_t     rs_a, \
       const dim_t     cs_a, \
       const dim_t     ps_a, \
       const B_type*   b, \
       const dim_t     rs_b, \
       const dim_t     cs_b, \
       C_type*         c, \
       const dim_t     rs_c, \
       const dim_t     cs_c, \
       const C_type    alpha, \
       const C_type    beta, \
       bool            is_last_k, \
       dim_t           post_op_c_i, \
       dim_t           post_op_c_j, \
       lpgemm_post_op* post_ops_list \
     ) \

LPGEMM_MAIN_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_6x64);
LPGEMM_MAIN_KERN(uint8_t,int8_t,int16_t,u8s8s16o16_6x32);

#define LPGEMM_M_FRINGE_KERN(A_type,B_type,C_type,LP_SFX) \
void lpgemm_rowvar_ ## LP_SFX \
     ( \
       const dim_t     k0, \
       const A_type*   a, \
       const dim_t     rs_a, \
       const dim_t     cs_a, \
       const B_type*   b, \
       const dim_t     rs_b, \
       const dim_t     cs_b, \
       C_type*         c, \
       const dim_t     rs_c, \
       const C_type    alpha, \
       const C_type    beta, \
       bool            is_last_k, \
       dim_t           post_op_c_i, \
       dim_t           post_op_c_j, \
       lpgemm_post_op* post_ops_list \
     ) \

LPGEMM_M_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_5x64);
LPGEMM_M_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_4x64);
LPGEMM_M_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_3x64);
LPGEMM_M_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_2x64);
LPGEMM_M_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_1x64);

LPGEMM_M_FRINGE_KERN(uint8_t,int8_t,int16_t,u8s8s16o16_4x32);
LPGEMM_M_FRINGE_KERN(uint8_t,int8_t,int16_t,u8s8s16o16_2x32);
LPGEMM_M_FRINGE_KERN(uint8_t,int8_t,int16_t,u8s8s16o16_1x32);

#define LPGEMM_N_FRINGE_KERN(A_type,B_type,C_type,LP_SFX) \
void lpgemm_rowvar_ ## LP_SFX \
     ( \
       const dim_t     m0, \
       const dim_t     k0, \
       const A_type*   a, \
       const dim_t     rs_a, \
       const dim_t     cs_a, \
       const dim_t     ps_a, \
       const B_type*   b, \
       const dim_t     rs_b, \
       const dim_t     cs_b, \
       C_type*         c, \
       const dim_t     rs_c, \
       const C_type    alpha, \
       const C_type    beta, \
       bool            is_last_k, \
       dim_t           post_op_c_i, \
       dim_t           post_op_c_j, \
       lpgemm_post_op* post_ops_list \
     ) \

LPGEMM_N_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_6x16);
LPGEMM_N_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_6x32);
LPGEMM_N_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_6x48);

LPGEMM_N_FRINGE_KERN(uint8_t,int8_t,int16_t,u8s8s16o16_6x16);

#define LPGEMM_N_LT_NR0_FRINGE_KERN(A_type,B_type,C_type,LP_SFX) \
void lpgemm_rowvar_ ## LP_SFX \
     ( \
       const dim_t     m0, \
       const dim_t     k0, \
       const A_type*   a, \
       const dim_t     rs_a, \
       const dim_t     cs_a, \
       const dim_t     ps_a, \
       const B_type*   b, \
       const dim_t     rs_b, \
       const dim_t     cs_b, \
       C_type*         c, \
       const dim_t     rs_c, \
       const C_type    alpha, \
       const C_type    beta, \
       const dim_t     n0_rem, \
       bool            is_last_k, \
       dim_t           post_op_c_i, \
       dim_t           post_op_c_j, \
       lpgemm_post_op* post_ops_list \
     ) \

LPGEMM_N_LT_NR0_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_6xlt16);

LPGEMM_N_LT_NR0_FRINGE_KERN(uint8_t,int8_t,int16_t,u8s8s16o16_6xlt16);

#define LPGEMM_MN_FRINGE_KERN(A_type,B_type,C_type,LP_SFX) \
void lpgemm_rowvar_ ## LP_SFX \
     ( \
       const dim_t     k0, \
       const A_type*   a, \
       const dim_t     rs_a, \
       const dim_t     cs_a, \
       const B_type*   b, \
       const dim_t     rs_b, \
       const dim_t     cs_b, \
       C_type*         c, \
       const dim_t     rs_c, \
       const C_type    alpha, \
       const C_type    beta, \
       bool            is_last_k, \
       dim_t           post_op_c_i, \
       dim_t           post_op_c_j, \
       lpgemm_post_op* post_ops_list \
     ) \

LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_5x16);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_4x16);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_3x16);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_2x16);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_1x16);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_5x32);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_4x32);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_3x32);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_2x32);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_1x32);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_5x48);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_4x48);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_3x48);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_2x48);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_1x48);

LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int16_t,u8s8s16o16_4x16);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int16_t,u8s8s16o16_2x16);
LPGEMM_MN_FRINGE_KERN(uint8_t,int8_t,int16_t,u8s8s16o16_1x16);

#define LPGEMM_MN_LT_NR0_FRINGE_KERN(A_type,B_type,C_type,LP_SFX) \
void lpgemm_rowvar_ ## LP_SFX \
     ( \
       const dim_t     k0, \
       const A_type*   a, \
       const dim_t     rs_a, \
       const dim_t     cs_a, \
       const B_type*   b, \
       const dim_t     rs_b, \
       const dim_t     cs_b, \
       C_type*         c, \
       const dim_t     rs_c, \
       const C_type    alpha, \
       const C_type    beta, \
       const dim_t     n0_rem, \
       bool            is_last_k, \
       dim_t           post_op_c_i, \
       dim_t           post_op_c_j, \
       lpgemm_post_op* post_ops_list \
     ) \

LPGEMM_MN_LT_NR0_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_5xlt16);
LPGEMM_MN_LT_NR0_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_4xlt16);
LPGEMM_MN_LT_NR0_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_3xlt16);
LPGEMM_MN_LT_NR0_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_2xlt16);
LPGEMM_MN_LT_NR0_FRINGE_KERN(uint8_t,int8_t,int32_t,u8s8s32o32_1xlt16);

LPGEMM_MN_LT_NR0_FRINGE_KERN(uint8_t,int8_t,int16_t,u8s8s16o16_4xlt16);
LPGEMM_MN_LT_NR0_FRINGE_KERN(uint8_t,int8_t,int16_t,u8s8s16o16_2xlt16);
LPGEMM_MN_LT_NR0_FRINGE_KERN(uint8_t,int8_t,int16_t,u8s8s16o16_1xlt16);

#endif //BLIS_LPGEMM_KERN_H
