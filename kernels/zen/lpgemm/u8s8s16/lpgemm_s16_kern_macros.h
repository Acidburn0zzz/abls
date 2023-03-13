/*

   BLIS
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2022-23, Advanced Micro Devices, Inc. All rights reserved.

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

#ifndef LPGEMM_S16_KERN_MACROS_H
#define LPGEMM_S16_KERN_MACROS_H

#include "../gelu_avx2.h"
#include "../math_utils_avx2.h"

#define S8_MIN  (-128)
#define S8_MAX  (+127)

/* ReLU scale (Parametric ReLU):  f(x) = x, when x > 0 and f(x) = a*x when x <= 0 */
#define RELU_SCALE_OP_S16_AVX2(reg) \
	selector1 = _mm256_setzero_si256();\
	selector1 = _mm256_cmpgt_epi16 ( selector1, reg ); \
 \
	/* Only < 0 elements in b0. */ \
	b0 = _mm256_and_si256 ( selector1, reg ); \
\
	/* Only >= 0 elements in c_int16_0p0. */ \
	reg = _mm256_andnot_si256( selector1, reg ); \
 \
	/* Only scaling for < 0 elements. */ \
	b0 = _mm256_mullo_epi16( b0, selector2 ); \
 \
	/* Combine the scaled < 0 and >= 0 elements. */ \
	reg = _mm256_or_si256( b0, reg ); \
 \

//--------------------------------------------------------------------------

#define BLI_MM256_S16_DOWNSCALE(c_int16__p0, c_int16__p1, vec_loc)\
\
  /* Extract the first 128 bits of the register*/\
  temp[0] = _mm256_extractf128_si256(c_int16__p0, 0);\
  /* Extract the second 128 bits of the register*/\
  temp[1] = _mm256_extractf128_si256(c_int16__p0, 1);\
\
  temp_32[0] = _mm256_cvtepi16_epi32(temp[0]);\
  temp_32[1] = _mm256_cvtepi16_epi32(temp[1]);\
  temp_float[0] = _mm256_cvtepi32_ps(temp_32[0]);\
  temp_float[1] = _mm256_cvtepi32_ps(temp_32[1]);\
\
  /* Multiply the C matrix by the scale value*/\
  res_1 = _mm256_mul_ps(temp_float[0], scale_1);\
  res_2 = _mm256_mul_ps(temp_float[1], scale_2);\
\
  /* Round the resultant value to the nearest float value and clip the values between [-128, 127] */\
  res_1 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps(res_1, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps(( float )S8_MIN)), _mm256_set1_ps(( float )S8_MAX));\
  res_2 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps (res_2, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps(( float )S8_MIN)), _mm256_set1_ps(( float )S8_MAX));\
\
  /* Convert the clipped float32 scaled rounded value to int32 */\
  temp_32[0] = _mm256_cvtps_epi32(res_1);\
  temp_32[1] = _mm256_cvtps_epi32(res_2);\
\
  /* Convert the s32 to s16 */\
	c_int16__p0 = _mm256_packs_epi32(temp_32[0], temp_32[1]);\
\
  /*Permute to make sure the order is correct*/\
	c_int16__p0 = _mm256_permute4x64_epi64(c_int16__p0, 0XD8);\
\
   /* Extract the first 128 bits of the register*/\
	temp[0] = _mm256_extractf128_si256(c_int16__p1, 0);\
\
  /* Extract the second 128 bits of the register*/\
	temp[1] = _mm256_extractf128_si256(c_int16__p1, 1);\
\
  temp_32[0] = _mm256_cvtepi16_epi32(temp[0]);\
  temp_32[1] = _mm256_cvtepi16_epi32(temp[1]);\
  temp_float[0] = _mm256_cvtepi32_ps(temp_32[0]);\
  temp_float[1] = _mm256_cvtepi32_ps(temp_32[1]);\
\
   /* Multiply the C matrix by the scale value*/\
  res_1 = _mm256_mul_ps(temp_float[0], scale_1);\
  res_2 = _mm256_mul_ps(temp_float[1], scale_2);\
\
  /* Round the resultant value to the nearest float value and clip the values between [-128, 127] */\
  res_1 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps (res_1, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps(( float )S8_MIN)), _mm256_set1_ps(( float )S8_MAX));\
  res_2 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps (res_2, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps(( float )S8_MIN)), _mm256_set1_ps(( float )S8_MAX));\
\
  /* Convert the clipped float32 scaled rounded value to int32 */\
  temp_32[0] = _mm256_cvtps_epi32(res_1);\
  temp_32[1] = _mm256_cvtps_epi32(res_2);\
\
  /* Convert the s32 to s16 */\
	c_int16__p1 = _mm256_packs_epi32(temp_32[0], temp_32[1]);\
\
  /*Permute to make sure the order is correct*/\
	c_int16__p1 = _mm256_permute4x64_epi64(c_int16__p1, 0XD8);\
\
   /* Convert the s16 to s8 */\
	store_reg = _mm256_packs_epi16(c_int16__p0, c_int16__p1);\
	store_reg = _mm256_permute4x64_epi64(store_reg, 0XD8);\
\
  /* Store the result in s8 form */\
	_mm256_storeu_si256 \
	( \
	  (__m256i *)(( int8_t* )post_ops_list_temp->op_args3 + \
	  ( post_ops_attr.rs_c_downscale * ( post_ops_attr.post_op_c_i + vec_loc ) ) + \
	  post_ops_attr.post_op_c_j), store_reg \
	);\
\

//--------------------------------------------------------------------------

#define BLI_MM256_S16_DOWNSCALE2(c_int16__p0, c_int16__p1, vec_loc1, vec_loc2)\
\
  /* Extract the first 128 bits of the register*/\
  temp[0] = _mm256_extractf128_si256(c_int16__p0, 0);\
  /* Extract the second 128 bits of the register*/\
  temp[1] = _mm256_extractf128_si256(c_int16__p0, 1);\
\
  temp_32[0] = _mm256_cvtepi16_epi32(temp[0]);\
  temp_32[1] = _mm256_cvtepi16_epi32(temp[1]);\
  temp_float[0] = _mm256_cvtepi32_ps(temp_32[0]);\
  temp_float[1] = _mm256_cvtepi32_ps(temp_32[1]);\
\
  /* Multiply the C matrix by the scale value*/\
  res_1 = _mm256_mul_ps(temp_float[0], scale_1);\
  res_2 = _mm256_mul_ps(temp_float[1], scale_2);\
\
  /* Round the resultant value to the nearest float value and clip the values between [-128, 127] */\
  res_1 = _mm256_min_ps(_mm256_max_ps \
         (_mm256_round_ps(res_1, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps(( float )S8_MIN)), _mm256_set1_ps(( float )S8_MAX));\
  res_2 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps(res_2, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps(( float )S8_MIN)), _mm256_set1_ps(( float )S8_MAX));\
\
  /* Convert the clipped float32 scaled rounded value to int32 */\
  temp_32[0] = _mm256_cvtps_epi32(res_1);\
  temp_32[1] = _mm256_cvtps_epi32(res_2);\
\
  /* Convert the s32 to s16 */\
	c_int16__p0 = _mm256_packs_epi32(temp_32[0], temp_32[1]);\
\
  /*Permute to make sure the order is correct*/\
	c_int16__p0 = _mm256_permute4x64_epi64(c_int16__p0, 0XD8);\
\
   /* Extract the first 128 bits of the register*/\
	temp[0] = _mm256_extractf128_si256(c_int16__p1, 0);\
\
  /* Extract the second 128 bits of the register*/\
	temp[1] = _mm256_extractf128_si256(c_int16__p1, 1);\
\
  temp_32[0] = _mm256_cvtepi16_epi32(temp[0]);\
  temp_32[1] = _mm256_cvtepi16_epi32(temp[1]);\
  temp_float[0] = _mm256_cvtepi32_ps(temp_32[0]);\
  temp_float[1] = _mm256_cvtepi32_ps(temp_32[1]);\
\
   /* Multiply the C matrix by the scale value*/\
  res_1 = _mm256_mul_ps(temp_float[0], scale_1);\
  res_2 = _mm256_mul_ps(temp_float[1], scale_2);\
\
  /* Round the resultant value to the nearest float value and clip the values between [-128, 127] */\
  res_1 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps(res_1, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps(( float )S8_MIN)), _mm256_set1_ps(( float )S8_MAX));\
  res_2 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps(res_2, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps(( float )S8_MIN)), _mm256_set1_ps(( float )S8_MAX));\
\
  /* Convert the clipped float32 scaled rounded value to int32 */\
  temp_32[0] = _mm256_cvtps_epi32(res_1);\
  temp_32[1] = _mm256_cvtps_epi32(res_2);\
\
  /* Convert the s32 to s16 */\
	c_int16__p1 = _mm256_packs_epi32(temp_32[0], temp_32[1]);\
\
  /*Permute to make sure the order is correct*/\
	c_int16__p1 = _mm256_permute4x64_epi64(c_int16__p1, 0XD8);\
\
   /* Convert the s16 to s8 */\
	store_reg = _mm256_packs_epi16(c_int16__p0, c_int16__p1);\
  store_reg = _mm256_permute4x64_epi64(store_reg, 0XD8);\
  /* Extract the first 128 bits of the register*/\
  temp[0] = _mm256_extractf128_si256(store_reg, 0);\
  /* Extract the second 128 bits of the register*/\
  temp[1] = _mm256_extractf128_si256(store_reg, 1);\
\
  /* Store the result in s8 form */\
	_mm_storeu_si128 \
	( \
	  (__m128i *)(( int8_t* )post_ops_list_temp->op_args3 + \
	  ( post_ops_attr.rs_c_downscale * ( post_ops_attr.post_op_c_i + vec_loc1 ) ) + \
	  post_ops_attr.post_op_c_j), temp[0] \
	);\
	_mm_storeu_si128 \
	( \
	  (__m128i *)(( int8_t* )post_ops_list_temp->op_args3 + \
	  ( post_ops_attr.rs_c_downscale * ( post_ops_attr.post_op_c_i + vec_loc2 ) ) + \
	  post_ops_attr.post_op_c_j), temp[1] \
	);\
\

//--------------------------------------------------------------------------

#define BLI_MM256_S16_DOWNSCALE2_LT16(c_int16__p0, c_int16__p1, vec_loc1, vec_loc2)\
\
  /* Extract the first 128 bits of the register*/\
  temp[0] = _mm256_extractf128_si256(c_int16__p0, 0);\
  /* Extract the second 128 bits of the register*/\
  temp[1] = _mm256_extractf128_si256(c_int16__p0, 1);\
\
  temp_32[0] = _mm256_cvtepi16_epi32(temp[0]);\
  temp_32[1] = _mm256_cvtepi16_epi32(temp[1]);\
  temp_float[0] = _mm256_cvtepi32_ps(temp_32[0]);\
  temp_float[1] = _mm256_cvtepi32_ps(temp_32[1]);\
\
  /* Multiply the C matrix by the scale value*/\
  res_1 = _mm256_mul_ps(temp_float[0], scale_1);\
  res_2 = _mm256_mul_ps(temp_float[1], scale_2);\
\
  /* Round the resultant value to the nearest float value and clip the values between [-128, 127] */\
  res_1 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps(res_1, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps (( float )S8_MIN)), _mm256_set1_ps (( float )S8_MAX));\
  res_2 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps(res_2, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps (( float )S8_MIN)), _mm256_set1_ps (( float )S8_MAX));\
\
  /* Convert the clipped float32 scaled rounded value to int32 */\
  temp_32[0] = _mm256_cvtps_epi32(res_1);\
  temp_32[1] = _mm256_cvtps_epi32(res_2);\
\
  /* Convert the s32 to s16 */\
	c_int16__p0 = _mm256_packs_epi32(temp_32[0], temp_32[1]);\
\
  /*Permute to make sure the order is correct*/\
	c_int16__p0 = _mm256_permute4x64_epi64(c_int16__p0, 0XD8);\
\
   /* Extract the first 128 bits of the register*/\
	temp[0] = _mm256_extractf128_si256(c_int16__p1, 0);\
\
  /* Extract the second 128 bits of the register*/\
	temp[1] = _mm256_extractf128_si256(c_int16__p1, 1);\
\
  temp_32[0] = _mm256_cvtepi16_epi32(temp[0]);\
  temp_32[1] = _mm256_cvtepi16_epi32(temp[1]);\
  temp_float[0] = _mm256_cvtepi32_ps(temp_32[0]);\
  temp_float[1] = _mm256_cvtepi32_ps(temp_32[1]);\
\
   /* Multiply the C matrix by the scale value*/\
  res_1 = _mm256_mul_ps(temp_float[0], scale_1);\
  res_2 = _mm256_mul_ps(temp_float[1], scale_2);\
\
  /* Round the resultant value to the nearest float value and clip the values between [-128, 127] */\
  res_1 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps(res_1, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps (( float )S8_MIN)), _mm256_set1_ps (( float )S8_MAX));\
  res_2 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps(res_2, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps (( float )S8_MIN)), _mm256_set1_ps (( float )S8_MAX));\
\
  /* Convert the clipped float32 scaled rounded value to int32 */\
  temp_32[0] = _mm256_cvtps_epi32(res_1);\
  temp_32[1] = _mm256_cvtps_epi32(res_2);\
\
  /* Convert the s32 to s16 */\
	c_int16__p1 = _mm256_packs_epi32(temp_32[0], temp_32[1]);\
\
  /*Permute to make sure the order is correct*/\
	c_int16__p1 = _mm256_permute4x64_epi64(c_int16__p1, 0XD8);\
\
   /* Convert the s16 to s8 */\
	store_reg = _mm256_packs_epi16(c_int16__p0, c_int16__p1);\
  store_reg = _mm256_permute4x64_epi64(store_reg, 0XD8);\
  /* Extract the first 128 bits of the register*/\
  temp[0] = _mm256_extractf128_si256(store_reg, 0);\
  /* Extract the second 128 bits of the register*/\
  temp[1] = _mm256_extractf128_si256(store_reg, 1);\
\
  /* Store the result in s8 form */\
  _mm_storeu_si128((__m128i *)store_buf, temp[0]);\
  memcpy( \
  ( \
    int8_t* )post_ops_list_temp->op_args3 + \
    ( post_ops_attr.rs_c_downscale * ( post_ops_attr.post_op_c_i + vec_loc1 ) ) + \
    post_ops_attr.post_op_c_j, store_buf, ( n0_rem * sizeof( int8_t ) ) \
  ); \
\
  _mm_storeu_si128((__m128i *)store_buf, temp[1]);\
  memcpy \
  ( \
    ( int8_t* )post_ops_list_temp->op_args3 + \
    ( post_ops_attr.rs_c_downscale * ( post_ops_attr.post_op_c_i + vec_loc2 ) ) + \
    post_ops_attr.post_op_c_j, store_buf, ( n0_rem * sizeof( int8_t ) ) \
  ); \
\

//--------------------------------------------------------------------------

#define BLI_MM256_S16_DOWNSCALE2_EDGE(c_int16__p0, vec_ind)\
\
  /* Extract the first 128 bits of the register*/\
  temp[0] = _mm256_extractf128_si256(c_int16__p0, 0);\
  /* Extract the second 128 bits of the register*/\
  temp[1] = _mm256_extractf128_si256(c_int16__p0, 1);\
\
  temp_32[0] = _mm256_cvtepi16_epi32(temp[0]);\
  temp_32[1] = _mm256_cvtepi16_epi32(temp[1]);\
  temp_float[0] = _mm256_cvtepi32_ps(temp_32[0]);\
  temp_float[1] = _mm256_cvtepi32_ps(temp_32[1]);\
\
  /* Multiply the C matrix by the scale value*/\
  res_1 = _mm256_mul_ps(temp_float[0], scale_1);\
  res_2 = _mm256_mul_ps(temp_float[1], scale_2);\
\
  /* Round the resultant value to the nearest float value and clip the values between [-128, 127] */\
  res_1 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps(res_1, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps (( float )S8_MIN)), _mm256_set1_ps (( float )S8_MAX));\
  res_2 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps(res_2, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps (( float )S8_MIN)), _mm256_set1_ps (( float )S8_MAX));\
\
  /* Convert the clipped float32 scaled rounded value to int32 */\
  temp_32[0] = _mm256_cvtps_epi32(res_1);\
  temp_32[1] = _mm256_cvtps_epi32(res_2);\
\
  /* Convert the s32 to s16 */\
	c_int16__p0 = _mm256_packs_epi32(temp_32[0], temp_32[1]);\
\
  /*Permute to make sure the order is correct*/\
	c_int16__p0 = _mm256_permute4x64_epi64(c_int16__p0, 0XD8);\
\
   /* Convert the s16 to s8 */\
	store_reg = _mm256_packs_epi16(c_int16__p0, zero_reg);\
  store_reg = _mm256_permute4x64_epi64(store_reg, 0XD8);\
  /* Extract the first 128 bits of the register*/\
  temp[0] = _mm256_extractf128_si256(store_reg, 0);\
\
  /* Store the result in s8 form */\
	_mm_storeu_si128 \
	( \
	  (__m128i *)(( int8_t* )post_ops_list_temp->op_args3 + \
	  ( post_ops_attr.rs_c_downscale * ( post_ops_attr.post_op_c_i + vec_ind ) ) + \
	  post_ops_attr.post_op_c_j), temp[0] \
	);\
\

//--------------------------------------------------------------------------

#define BLI_MM256_S16_DOWNSCALE2_EDGE_LT16(c_int16__p0, vec_ind)\
\
  /* Extract the first 128 bits of the register*/\
  temp[0] = _mm256_extractf128_si256(c_int16__p0, 0);\
  /* Extract the second 128 bits of the register*/\
  temp[1] = _mm256_extractf128_si256(c_int16__p0, 1);\
\
  temp_32[0] = _mm256_cvtepi16_epi32(temp[0]);\
  temp_32[1] = _mm256_cvtepi16_epi32(temp[1]);\
  temp_float[0] = _mm256_cvtepi32_ps(temp_32[0]);\
  temp_float[1] = _mm256_cvtepi32_ps(temp_32[1]);\
\
  /* Multiply the C matrix by the scale value*/\
  res_1 = _mm256_mul_ps(temp_float[0], scale_1);\
  res_2 = _mm256_mul_ps(temp_float[1], scale_2);\
\
  /* Round the resultant value to the nearest float value and clip the values between [-128, 127] */\
  res_1 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps(res_1, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps (( float )S8_MIN)), _mm256_set1_ps (( float )S8_MAX));\
  res_2 = _mm256_min_ps(_mm256_max_ps \
          (_mm256_round_ps(res_2, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)), \
          _mm256_set1_ps (( float )S8_MIN)), _mm256_set1_ps (( float )S8_MAX));\
\
  /* Convert the clipped float32 scaled rounded value to int32 */\
  temp_32[0] = _mm256_cvtps_epi32(res_1);\
  temp_32[1] = _mm256_cvtps_epi32(res_2);\
\
  /* Convert the s32 to s16 */\
	c_int16__p0 = _mm256_packs_epi32(temp_32[0], temp_32[1]);\
\
  /*Permute to make sure the order is correct*/\
	c_int16__p0 = _mm256_permute4x64_epi64(c_int16__p0, 0XD8);\
\
   /* Convert the s16 to s8 */\
	store_reg = _mm256_packs_epi16(c_int16__p0, zero_reg);\
  store_reg = _mm256_permute4x64_epi64(store_reg, 0XD8);\
  /* Extract the first 128 bits of the register*/\
  temp[0] = _mm256_extractf128_si256(store_reg, 0);\
\
  /* Store the result in s8 form */\
  _mm_storeu_si128((__m128i *)store_buf, temp[0]);\
  memcpy \
  ( \
    (( int8_t* )post_ops_list_temp->op_args3 + \
    ( post_ops_attr.rs_c_downscale * ( post_ops_attr.post_op_c_i + vec_ind ) ) + \
    post_ops_attr.post_op_c_j), store_buf, ( n0_rem * sizeof( int8_t ) ) \
  ); \
\

//--------------------------------------------------------------------------
/* GeLU (x) = 0.5* x * (1 + tanh ( 0.797884 * ( x + ( 0.044715 * x^3 ) ) ) )  */
#define GELU_TANH_S16_AVX2(reg, y1, y2, r, r2, x, z, dn, x_tanh, q) \
\
	y1 = _mm256_cvtepi32_ps( _mm256_cvtepi16_epi32(_mm256_extractf128_si256(reg, 0)) ); \
	y2 = _mm256_cvtepi32_ps( _mm256_cvtepi16_epi32(_mm256_extractf128_si256(reg, 1)) ); \
\
	GELU_TANH_F32_AVX2_DEF(y1, r, r2, x, z, dn, x_tanh, q); \
\
	GELU_TANH_F32_AVX2_DEF(y2, r, r2, x, z, dn, x_tanh, q); \
\
	reg = _mm256_packs_epi32(_mm256_cvtps_epi32(y1), _mm256_cvtps_epi32(y2));\
	reg = _mm256_permute4x64_epi64(reg, 0XD8);\


/* ERF GeLU (x) = 0.5* x * (1 + erf (x * 0.707107 ))  */
#define GELU_ERF_S16_AVX2(reg, y1, y2, r, x, x_erf) \
\
	y1 = _mm256_cvtepi32_ps( _mm256_cvtepi16_epi32(_mm256_extractf128_si256(reg, 0)) ); \
	y2 = _mm256_cvtepi32_ps( _mm256_cvtepi16_epi32(_mm256_extractf128_si256(reg, 1)) ); \
\
	GELU_ERF_F32_AVX2_DEF(y1, r, x, x_erf); \
\
	GELU_ERF_F32_AVX2_DEF(y2, r, x, x_erf); \
\
	reg = _mm256_packs_epi32(_mm256_cvtps_epi32(y1), _mm256_cvtps_epi32(y2));\
	reg = _mm256_permute4x64_epi64(reg, 0XD8);\

#endif //LPGEMM_S16_KERN_MACROS_H
