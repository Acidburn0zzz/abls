/*

   BLIS
   An object-based framework for developing high-performance BLAS-like
   libraries.

  Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.

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

//Zero-out the given ZMM accumulator registers
#define ZERO_ACC_ZMM_4_REG(zmm0,zmm1,zmm2,zmm3) \
      zmm0 = _mm512_setzero_ps(); \
      zmm1 = _mm512_setzero_ps(); \
      zmm2 = _mm512_setzero_ps(); \
      zmm3 = _mm512_setzero_ps();

//Zero-out the given YMM accumulator registers
#define ZERO_ACC_YMM_4_REG(ymm0,ymm1,ymm2,ymm3) \
      ymm0 = _mm256_setzero_ps(); \
      ymm1 = _mm256_setzero_ps(); \
      ymm2 = _mm256_setzero_ps(); \
      ymm3 = _mm256_setzero_ps();

//Zero-out the given XMM accumulator registers
#define ZERO_ACC_XMM_4_REG(xmm0,xmm1,xmm2,xmm3) \
      xmm0 = _mm_setzero_ps(); \
      xmm1 = _mm_setzero_ps(); \
      xmm2 = _mm_setzero_ps(); \
      xmm3 = _mm_setzero_ps();

/*Multiply alpha with accumulator registers and store back*/
#define ALPHA_MUL_ACC_ZMM_4_REG(zmm0,zmm1,zmm2,zmm3,alpha) \
      zmm0 = _mm512_mul_ps(zmm0,alpha); \
      zmm1 = _mm512_mul_ps(zmm1,alpha); \
      zmm2 = _mm512_mul_ps(zmm2,alpha); \
      zmm3 = _mm512_mul_ps(zmm3,alpha);
      
/*Multiply alpha with accumulator registers and store back*/
#define ALPHA_MUL_ACC_YMM_4_REG(ymm0,ymm1,ymm2,ymm3,alpha) \
      ymm0 = _mm256_mul_ps(ymm0,alpha); \
      ymm1 = _mm256_mul_ps(ymm1,alpha); \
      ymm2 = _mm256_mul_ps(ymm2,alpha); \
      ymm3 = _mm256_mul_ps(ymm3,alpha);

/*Multiply alpha with accumulator registers and store back*/
#define ALPHA_MUL_ACC_XMM_4_REG(xmm0,xmm1,xmm2,xmm3,alpha) \
      xmm0 = _mm_mul_ps(xmm0,alpha); \
      xmm1 = _mm_mul_ps(xmm1,alpha); \
      xmm2 = _mm_mul_ps(xmm2,alpha); \
      xmm3 = _mm_mul_ps(xmm3,alpha);
      
/*Load C, Multiply with beta and add with A*B and store*/
#define F32_C_STORE_BNZ_8(cbuf,rs_c,ymm0,beta,ymm2) \
      ymm0 = _mm256_load_ps(cbuf); \
      ymm2 = _mm256_fmadd_ps(ymm0, beta, ymm2); \
      _mm256_storeu_ps(cbuf, ymm2);

/*Load C, Multiply with beta and add with A*B and store*/
#define F32_C_STORE_BNZ_4(cbuf,rs_c,xmm0,beta,xmm2) \
      xmm0 = _mm_load_ps(cbuf); \
      xmm2 = _mm_fmadd_ps(xmm0, beta, xmm2); \
      _mm_storeu_ps(cbuf, xmm2);     

/*Load C, Multiply with beta and add with A*B and store*/
#define F32_C_STORE_BNZ_2(cbuf,rs_c,xmm0,beta,xmm2) \
      xmm0 = _mm_load_sd((const double*)cbuf); \
      xmm2 = _mm_fmadd_ps(xmm0, beta, xmm2); \
      _mm_store_sd((double*)cbuf, xmm2);

/*Load C, Multiply with beta and add with A*B and store*/
#define F32_C_STORE_BNZ_1(cbuf,rs_c,xmm0,beta,xmm2) \
      xmm0 = _mm_load_ss(cbuf); \
      xmm2 = _mm_fmadd_ps(xmm0, beta, xmm2); \
      _mm_store_ss(cbuf, xmm2);

