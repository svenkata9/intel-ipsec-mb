/*******************************************************************************
  Copyright (c) 2009-2019, Intel Corporation

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

      * Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
      * Neither the name of Intel Corporation nor the names of its contributors
        may be used to endorse or promote products derived from this software
        without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#ifndef _SNOW3G_INTERNAL_H_
#define _SNOW3G_INTERNAL_H_

#include "intel-ipsec-mb.h"
#include <immintrin.h>
#include <smmintrin.h>
#include <wmmintrin.h>
#include <xmmintrin.h>

#define MAX_KEY_LEN (16)
#define SNOW3G_4_BYTES (4)
#define SNOW3G_8_BYTES (8)
#define SNOW3G_8_BITS (8)
#define SNOW3G_16_BYTES (16)
#define SNOW3G_16_BITS (16)

#define SNOW3G_BLOCK_SIZE (8)

#define SNOW3G_KEY_LEN_IN_BYTES (16) /* 128b */
#define SNOW3G_IV_LEN_IN_BYTES (16)  /* 128b */

#define SNOW3GCONSTANT (0x1b)

#define NUM_PACKETS_1 1
#define NUM_PACKETS_2 2
#define NUM_PACKETS_3 3
#define NUM_PACKETS_4 4
#define NUM_PACKETS_8 8
#define NUM_PACKETS_16 16

#define ComplementaryMask64(x) ((~(x) % 64) + 1)
#define ComplementaryMask32(x) ((~(x) % 32) + 1)

#ifdef _WIN32
#define _BSWAP32(x) _byteswap_ulong(x)
#define _BSWAP64(x) _byteswap_uint64(x)
#else
#define _BSWAP32(x) __builtin_bswap32(x)
#define _BSWAP64(x) __builtin_bswap64(x)
#endif

typedef union _m128_u {
        uint8_t byte[16];
        uint16_t word[8];
        uint32_t dword[4];
        uint64_t qword[2];
        __m128i m;
} m128_t;

typedef union _m64_u {
        uint8_t byte[8];
        uint16_t word[4];
        uint32_t dword[2];
        __m64 m;
} m64_t;

typedef struct snow3gKeyState1_s {
        /* 16 LFSR stages */
        uint32_t LFSR_S[16];
        /* 3 FSM states */
        uint32_t FSM_R3;
        uint32_t FSM_R2;
        uint32_t FSM_R1;
} DECLARE_ALIGNED(snow3gKeyState1_t, 16);

typedef struct snow3gKeyState4_s {
        /* 16 LFSR stages */
        __m128i LFSR_X[16];
        /* 3 FSM states */
        __m128i FSM_X[3];
        uint32_t iLFSR_X;

} snow3gKeyState4_t;


#ifdef _WIN32
#pragma pack(push,1)
#define DECLARE_PACKED_UINT32(x) uint32_t x
#else
#define DECLARE_PACKED_UINT32(x) uint32_t x __attribute__((__packed__))
#endif

typedef union snow3gTableEntry_u {
        uint64_t v;
        struct {
                uint8_t shift[3];
                DECLARE_PACKED_UINT32(v);
        } w3;
        struct {
                uint8_t shift[2];
                DECLARE_PACKED_UINT32(v);
        } w2;
        struct {
                uint8_t shift[1];
                DECLARE_PACKED_UINT32(v);
        } w1;
        struct {
                uint8_t shift[4];
                DECLARE_PACKED_UINT32(v);
        } w0;
} snow3gTableEntry_t;
#ifdef _WIN32
#pragma pack(pop)
#endif

#define rotl32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define rotr32(x, n) (((x) << (32 - (n))) | ((x) >> (n)))

#define rotl8(x, n) (((x) << (n)) | ((x) >> (8 - (n))))

#define rotr8(x, n) (((x) << (8 - (n))) | ((x) >> (n)))

/*************************************************************************
 * @description - snow3g internal tables
 *************************************************************************/

extern const int snow3g_table_A_mul[256];
extern const int snow3g_table_A_div[256];
extern snow3gTableEntry_t snow3g_table_S1[256];
extern snow3gTableEntry_t snow3g_table_S2[256];
extern const int S1_T0[256];
extern const int S1_T1[256];
extern const int S1_T2[256];
extern const int S1_T3[256];
extern const int S2_T0[256];
extern const int S2_T1[256];
extern const int S2_T2[256];
extern const int S2_T3[256];

/* -------------------------------------------------------------------
 * combined S-Box processing for reduced instruction dependencies
 *
 * S1_S2_1    : 2 S-Box , 1 packet at a time
 * S1_S2_S3_1 : 3 S-Box at the same time
 *
 * S1_S2_4    : 2 S-Box , 4 packets at a time
 *
 * ------------------------------------------------------------------ */
#ifdef AVX2
#ifndef __INTEL_COMPILER
#define _mm256_set_m128i(/* __m128i */ hi, /* __m128i */ lo)                   \
        _mm256_insertf128_si256(_mm256_castsi128_si256(lo), (hi), 0x1)

#ifndef _mm256_loadu2_m128i
#define _mm256_loadu2_m128i(hi, lo)                                            \
        _mm256_inserti128_si256(                                               \
            _mm256_castsi128_si256(_mm_loadu_si128((const __m128i *)lo)),      \
            _mm_loadu_si128((const __m128i *)hi), 1)
#endif /* _mm256_loadu2_m128i */
#endif
typedef struct snow3gKeyState8_s {
        /* 16 LFSR stages */
        __m256i LFSR_X[16];
        /* 3 FSM states */
        __m256i FSM_X[3];
        uint32_t iLFSR_X;

} snow3gKeyState8_t;

/* Sbox Snow3g_S1 and Snow3g_S2 with dependency unrolling
 * for n in [0..3]
 *     w[n-1] = k; y[n] = Snow3g_S2(w[n]); k = Snow3g_S1(x[n])
 *
 *
 */
#define S1_S2_8(y, w, x, k, l, n)                                              \
        do {                                                                   \
                uint8_t w0, w1, w2, w3;                                        \
                uint8_t x0, x1, x2, x3;                                        \
                uint32_t ty = l;                                               \
                w3 = _mm256_extract_epi8(w, (4 * n + 0));                      \
                w2 = _mm256_extract_epi8(w, (4 * n + 1));                      \
                w1 = _mm256_extract_epi8(w, (4 * n + 2));                      \
                w0 = _mm256_extract_epi8(w, (4 * n + 3));                      \
                l = snow3g_table_S2[w3].w3.v ^ snow3g_table_S2[w2].w2.v ^      \
                    snow3g_table_S2[w1].w1.v ^ snow3g_table_S2[w0].w0.v;       \
                if (n != 0)                                                    \
                        w = _mm256_insert_epi32(w, k, (n - 1));                \
                if (n != 0)                                                    \
                        y = _mm256_insert_epi32(y, ty, (n - 1));               \
                x3 = _mm256_extract_epi8(x, (4 * n + 0));                      \
                x2 = _mm256_extract_epi8(x, (4 * n + 1));                      \
                x1 = _mm256_extract_epi8(x, (4 * n + 2));                      \
                x0 = _mm256_extract_epi8(x, (4 * n + 3));                      \
                k = snow3g_table_S1[x3].w3.v ^ snow3g_table_S1[x2].w2.v ^      \
                    snow3g_table_S1[x1].w1.v ^ snow3g_table_S1[x0].w0.v;       \
                if (n == 7)                                                    \
                        w = _mm256_insert_epi32(w, k, n);                      \
                if (n == 7)                                                    \
                        y = _mm256_insert_epi32(y, l, n);                      \
        } while (0)
#endif /* AVX2 */

#ifdef NO_AESNI

/* help compilers to interleave the
 * operations and table access latencies
 */

/* Sbox Snow3g_S1 and Snow3g_S2, simple C code
 *  y = Snow3g_S2(w); w = Snow3g_S1(x);
 */
#define S1_S2_1(y, w, x)                                                       \
        do {                                                                   \
                uint32_t w0, w1, w2, w3;                                       \
                uint32_t x0, x1, x2, x3;                                       \
                uint32_t tw, tx;                                               \
                w3 = w & 0xff;                                                 \
                x3 = x & 0xff;                                                 \
                tw = snow3g_table_S2[w3].w3.v;                                 \
                tx = snow3g_table_S1[x3].w3.v;                                 \
                w0 = w >> 24;                                                  \
                x0 = x >> 24;                                                  \
                tw ^= snow3g_table_S2[w0].w0.v;                                \
                tx ^= snow3g_table_S1[x0].w0.v;                                \
                w1 = (w >> 16) & 0xff;                                         \
                x1 = (x >> 16) & 0xff;                                         \
                tw ^= snow3g_table_S2[w1].w1.v;                                \
                tx ^= snow3g_table_S1[x1].w1.v;                                \
                w2 = (w >> 8) & 0xff;                                          \
                x2 = (x >> 8) & 0xff;                                          \
                y = tw ^ snow3g_table_S2[w2].w2.v;                             \
                w = tx ^ snow3g_table_S1[x2].w2.v;                             \
        } while (0)

/* Sbox Snow3g_S1 and Snow3g_S2, simple C code
 *  y = Snow3g_S2(w); w = Snow3g_S1(x); u = Snow3g_S1(z);
 */
#define S1_S2_S3_1(y, w, x, u, z)                                              \
        do {                                                                   \
                unsigned w0, w1, w2, w3;                                       \
                unsigned x0, x1, x2, x3;                                       \
                unsigned z0, z1, z2, z3;                                       \
                uint32_t tw, tx, tz;                                           \
                w3 = w & 0xff;                                                 \
                x3 = x & 0xff;                                                 \
                z3 = z & 0xff;                                                 \
                tw = snow3g_table_S2[w3].w3.v;                                 \
                tx = snow3g_table_S1[x3].w3.v;                                 \
                tz = snow3g_table_S1[z3].w3.v;                                 \
                w0 = w >> 24;                                                  \
                x0 = x >> 24;                                                  \
                z0 = z >> 24;                                                  \
                tw ^= snow3g_table_S2[w0].w0.v;                                \
                tx ^= snow3g_table_S1[x0].w0.v;                                \
                tz ^= snow3g_table_S1[z0].w0.v;                                \
                w1 = (w >> 16) & 0xff;                                         \
                x1 = (x >> 16) & 0xff;                                         \
                z1 = (z >> 16) & 0xff;                                         \
                tw ^= snow3g_table_S2[w1].w1.v;                                \
                tx ^= snow3g_table_S1[x1].w1.v;                                \
                tz ^= snow3g_table_S1[z1].w1.v;                                \
                w2 = (w >> 8) & 0xff;                                          \
                x2 = (x >> 8) & 0xff;                                          \
                z2 = (z >> 8) & 0xff;                                          \
                y = tw ^ snow3g_table_S2[w2].w2.v;                             \
                w = tx ^ snow3g_table_S1[x2].w2.v;                             \
                u = tz ^ snow3g_table_S1[z2].w2.v;                             \
        } while (0)

/* Sbox Snow3g_S1 and Snow3g_S2 with dependency unrolling
 * for n in [0..3]
 *     w[n-1] = k; y[n] = Snow3g_S2(w[n]); k = Snow3g_S1(x[n])
 *
 *
 */
#define S1_S2_4(y, w, x, k, l, n)                                              \
        do {                                                                   \
                unsigned w0, w1, w2, w3;                                       \
                unsigned x0, x1, x2, x3;                                       \
                uint32_t ty = l;                                               \
                w3 = _mm_extract_epi8(w, (4 * n + 0));                         \
                w2 = _mm_extract_epi8(w, (4 * n + 1));                         \
                w1 = _mm_extract_epi8(w, (4 * n + 2));                         \
                w0 = _mm_extract_epi8(w, (4 * n + 3));                         \
                l = snow3g_table_S2[w3].w3.v ^ snow3g_table_S2[w2].w2.v ^      \
                    snow3g_table_S2[w1].w1.v ^ snow3g_table_S2[w0].w0.v;       \
                if (n != 0)                                                    \
                        w = _mm_insert_epi32(w, k, (n - 1));                   \
                if (n != 0)                                                    \
                        y = _mm_insert_epi32(y, ty, (n - 1));                  \
                x3 = _mm_extract_epi8(x, (4 * n + 0));                         \
                x2 = _mm_extract_epi8(x, (4 * n + 1));                         \
                x1 = _mm_extract_epi8(x, (4 * n + 2));                         \
                x0 = _mm_extract_epi8(x, (4 * n + 3));                         \
                k = snow3g_table_S1[x3].w3.v ^ snow3g_table_S1[x2].w2.v ^      \
                    snow3g_table_S1[x1].w1.v ^ snow3g_table_S1[x0].w0.v;       \
                if (n == 3)                                                    \
                        w = _mm_insert_epi32(w, k, n);                         \
                if (n == 3)                                                    \
                        y = _mm_insert_epi32(y, l, n);                         \
        } while (0)

#else /* SSE/AVX */

/* use AES-NI Rijndael for Snow3G Sbox, overlap the latency
 * of AESENC with Snow3g_S2 sbox calculations
 */

/* Sbox Snow3g_S1 and Snow3g_S2, simple C code
 *  y = Snow3g_S2(w); w = rijndael Snow3g_S1(x);
 */
#define S1_S2_1(y, w, x)                                                       \
        do {                                                                   \
                __m128i m10, m11;                                              \
                m11 = _mm_cvtsi32_si128(x);                                    \
                m10 = _mm_setzero_si128();                                     \
                m11 = _mm_shuffle_epi32(m11, 0x0);                             \
                m11 = _mm_aesenc_si128(m11, m10);                              \
                y = Snow3g_S2(w);                                              \
                w = _mm_cvtsi128_si32(m11);                                    \
        } while (0)

/* Sbox Snow3g_S1 and Snow3g_S2
 *  y = Snow3g_S2(w); w = rijndael Snow3g_S1(x); u = rijndael Snow3g_S1(z);
 */
#define S1_S2_S3_1(y, w, x, v, z)                                              \
        do {                                                                   \
                __m128i m10, m11, m12;                                         \
                m11 = _mm_cvtsi32_si128(x);                                    \
                m10 = _mm_setzero_si128();                                     \
                m11 = _mm_shuffle_epi32(m11, 0x0);                             \
                m11 = _mm_aesenc_si128(m11, m10);                              \
                m12 = _mm_cvtsi32_si128(z);                                    \
                m12 = _mm_shuffle_epi32(m12, 0x0);                             \
                m12 = _mm_aesenc_si128(m12, m10);                              \
                y = Snow3g_S2(w);                                              \
                w = _mm_cvtsi128_si32(m11);                                    \
                v = _mm_cvtsi128_si32(m12);                                    \
        } while (0)
/* Sbox Snow3g_S1 and Snow3g_S2
 * for n in [0..3]
 *     extract packet data
 *     y = Snow3g_S2(w); w = rijndael Snow3g_S1(x)
 *     insert the result data
 */
#define S1_S2_4(y, w, x, k, n)                                                 \
        do {                                                                   \
                uint32_t ty;                                                   \
                unsigned w0, w1, w2, w3;                                       \
                __m128i m10, m11;                                              \
                m10 = _mm_setzero_si128();                                     \
                m11 = _mm_shuffle_epi32(                                       \
                    x, ((n << 6) | (n << 4) | (n << 2) | (n << 0)));           \
                m11 = _mm_aesenc_si128(m11, m10);                              \
                w3 = _mm_extract_epi8(w, (4 * n + 0));                         \
                w2 = _mm_extract_epi8(w, (4 * n + 1));                         \
                w1 = _mm_extract_epi8(w, (4 * n + 2));                         \
                w0 = _mm_extract_epi8(w, (4 * n + 3));                         \
                ty = snow3g_table_S2[w3].w3.v ^ snow3g_table_S2[w1].w1.v ^     \
                     snow3g_table_S2[w2].w2.v ^ snow3g_table_S2[w0].w0.v;      \
                if (n != 0)                                                    \
                        w = _mm_insert_epi32(w, k, (n - 1));                   \
                k = _mm_cvtsi128_si32(m11);                                    \
                if (n == 3)                                                    \
                        w = _mm_insert_epi32(w, k, n);                         \
                y = _mm_insert_epi32(y, ty, n);                                \
        } while (0)

#endif /* NO_AESNI */

/* -------------------------------------------------------------------
 * Sbox Snow3g_S1 maps a 32bit input to a 32bit output
 * ------------------------------------------------------------------ */
static inline uint32_t Snow3g_S1(uint32_t w)
{
        uint32_t w0, w1, w2, w3;

        w3 = w & 0xff;
        w1 = (w >> 16) & 0xff;
        w2 = (w >> 8) & 0xff;
        w0 = w >> 24;
        return snow3g_table_S1[w3].w3.v ^ snow3g_table_S1[w1].w1.v ^
               snow3g_table_S1[w2].w2.v ^ snow3g_table_S1[w0].w0.v;
}

/* -------------------------------------------------------------------
 * Sbox Snow3g_S2 maps a 32bit input to a 32bit output
 * ------------------------------------------------------------------ */
static inline uint32_t Snow3g_S2(uint32_t w)
{
        uint32_t w0, w1, w2, w3;

        w3 = w & 0xff;
        w1 = (w >> 16) & 0xff;
        w2 = (w >> 8) & 0xff;
        w0 = w >> 24;

        return snow3g_table_S2[w3].w3.v ^ snow3g_table_S2[w1].w1.v ^
               snow3g_table_S2[w2].w2.v ^ snow3g_table_S2[w0].w0.v;
}

/* -------------------------------------------------------------------
 * LFSR array shift by 1 position
 * ------------------------------------------------------------------ */
static inline void ShiftLFSR_1(snow3gKeyState1_t *pCtx)
{
        uint32_t i;

        for (i = 0; i < 15; i++)
                pCtx->LFSR_S[i] = pCtx->LFSR_S[i + 1];
}

/* -------------------------------------------------------------------
 * LFSR array shift by 2 positions
 * ------------------------------------------------------------------ */
static inline void ShiftTwiceLFSR_1(snow3gKeyState1_t *pCtx)
{
        int i;

        for (i = 0; i < 14; i++)
                pCtx->LFSR_S[i] = pCtx->LFSR_S[i + 2];
}

/* -------------------------------------------------------------------
 * ClockFSM function as defined in snow3g standard
 * The FSM has 2 input words S5 and S15 from the LFSR
 * produces a 32 bit output word F
 * ------------------------------------------------------------------ */
static inline void ClockFSM_1(snow3gKeyState1_t *pCtx, uint32_t *data)
{
        uint32_t F, R;

        F = pCtx->LFSR_S[15] + pCtx->FSM_R1;
        R = pCtx->FSM_R3 ^ pCtx->LFSR_S[5];
        *data = F ^ pCtx->FSM_R2;
        R += pCtx->FSM_R2;
        S1_S2_1(pCtx->FSM_R3, pCtx->FSM_R2, pCtx->FSM_R1);
        pCtx->FSM_R1 = R;
}

/* -------------------------------------------------------------------
 * ClockLFSR functin as defined in snow3g standard
 * ------------------------------------------------------------------ */
static inline void ClockLFSR_1(snow3gKeyState1_t *pCtx)
{
        uint32_t V = pCtx->LFSR_S[2];
        uint32_t S0 = pCtx->LFSR_S[0];
        uint32_t S11 = pCtx->LFSR_S[11];

        V ^= snow3g_table_A_mul[S0 >> 24];
        V ^= snow3g_table_A_div[S11 & 0xff];
        V ^= S0 << 8;
        V ^= S11 >> 8;

        ShiftLFSR_1(pCtx);

        pCtx->LFSR_S[15] = V;
}

/*************************************************************************
 * @description - this function is used to copy the right number of bytes
 *                from the source to destination buffer
 *
 * @param pSrc [IN] - pointer to an input Byte array (at least len bytes
 *available)
 * @param pDst [IN] - pointer to the output buffer (at least len bytes
 *available)
 * @param len  [IN] - length in bytes to copy (0 to 4)
 *
 *************************************************************************/
static inline void memcpy_keystream_32(uint8_t *pDst, const uint8_t *pSrc,
                                       const uint32_t len)
{
        switch (len) {
        case 4:
                *(uint32_t *)pDst = *(const uint32_t *)pSrc;
                break;
        case 3:
                pDst[2] = pSrc[2];
                /* fall-through */
        case 2:
                pDst[1] = pSrc[1];
                /* fall-through */
        case 1:
                pDst[0] = pSrc[0];
        }
}

/*************************************************************************
 * @description - this function is used to XOR the right number of bytes
 *                from a keystrea and a source into a destination buffer
 *
 * @param pSrc [IN] - pointer to an input Byte array (at least 4 bytes
 *available)
 * @param pDst [IN] - pointer to the output buffer (at least 4 bytes available)
 * @param KS  [IN]  - 4 bytes of keystream number, must be reversed
 *                    into network byte order before XOR
 *
 *************************************************************************/
static inline void xor_keystream_reverse_32(uint8_t *pDst, const uint8_t *pSrc,
                                            const uint32_t KS)
{
        const uint32_t *pSrc32 = (const uint32_t *)pSrc;
        uint32_t *pDst32 = (uint32_t *)pDst;
        *pDst32 = *pSrc32 ^ _BSWAP32(KS);
}

/******************************************************************************
 * @description - this function is used to do a keystream operation
 * @param pSrc [IN] - pointer to an input Byte array (at least 8 bytes
 *available)
 * @param pDst [IN] - pointer to the output buffer (at least 8 bytes available)
 * @param keyStream [IN] -  the Keystream value (8 bytes)
 ******************************************************************************/
static inline const uint8_t *
xor_keystrm_rev(uint8_t *pDst, const uint8_t *pSrc, uint64_t keyStream)
{
        /* default: XOR ONLY, read the input buffer, update the output buffer */
        const uint64_t *pSrc64 = (const uint64_t *)pSrc;
        uint64_t *pDst64 = (uint64_t *)pDst;
        *pDst64 = *pSrc64 ^ _BSWAP64(keyStream);
        return (const uint8_t *)(pSrc64 + 1);
}

/******************************************************************************
 * @description - this function is used to copy the right number of bytes
 *                from the source to destination buffer
 * @param pSrc [IN] - pointer to an input Byte array (at least len bytes
 *available)
 * @param pDst [IN] - pointer to the output buffer (at least len bytes
 *available)
 * @param len  [IN] - length in bytes to copy
 ******************************************************************************/
static inline void memcpy_keystrm(uint8_t *pDst,
                                  const uint8_t *pSrc,
                                  const uint32_t len)
{
        switch (len) {
        case 8:
                *(uint64_t *)pDst = *(const uint64_t *)pSrc;
                break;
        case 7:
                pDst[6] = pSrc[6];
                /* fall-through */
        case 6:
                pDst[5] = pSrc[5];
                /* fall-through */
        case 5:
                pDst[4] = pSrc[4];
                /* fall-through */
        case 4:
                *(uint32_t *)pDst = *(const uint32_t *)pSrc;
                break;
        case 3:
                pDst[2] = pSrc[2];
                /* fall-through */
        case 2:
                pDst[1] = pSrc[1];
                /* fall-through */
        case 1:
                pDst[0] = pSrc[0];
        }
}

/**
 *******************************************************************************
 * @description
 * This function initializes the key schedule for 1 buffer for snow3g f8/f9.
 *
 * @param[in]       pCtx        Context where the scheduled keys are stored
 * @param [in]      pKeySched    Key schedule
 * @param [in]      pIV          IV
 *
 ******************************************************************************/
static inline void
snow3gStateInitialize_1(snow3gKeyState1_t *pCtx,
                        const snow3g_key_schedule_t *pKeySched,
                        const void *pIV)
{
        uint32_t K, L;
        int i;
        uint32_t V0, V1;
        uint32_t F0, F1;
        uint32_t L0, L1, L11, L12;
        uint32_t R0, R1;
        uint32_t FSM2, FSM3, FSM4;
        const uint32_t *pIV32 = pIV;

        /* LFSR initialisation */
        for (i = 0; i < 4; i++) {
                K = pKeySched->k[i];
                L = ~K;
                pCtx->LFSR_S[i + 4] = K;
                pCtx->LFSR_S[i + 12] = K;
                pCtx->LFSR_S[i + 0] = L;
                pCtx->LFSR_S[i + 8] = L;
        }

        pCtx->LFSR_S[15] ^= _BSWAP32(pIV32[3]);
        pCtx->LFSR_S[12] ^= _BSWAP32(pIV32[2]);
        pCtx->LFSR_S[10] ^= _BSWAP32(pIV32[1]);
        pCtx->LFSR_S[9] ^= _BSWAP32(pIV32[0]);

        /* FSM initialialization */
        FSM2 = 0x0;
        FSM3 = 0x0;
        FSM4 = 0x0;
        R1 = 0x0;
        V1 = pCtx->LFSR_S[15];

        for (i = 0; i < 16; i++) {
                /* clock FSM + clock LFSR + clockFSM + clock LFSR */
                L0 = pCtx->LFSR_S[0];
                L1 = pCtx->LFSR_S[1];
                V0 = pCtx->LFSR_S[2];
                F0 = V1 + R1; /**  (s15 +  R1) **/
                V1 = pCtx->LFSR_S[3];
                V0 ^= snow3g_table_A_mul[L0 >> 24]; /* MUL(s0,0 ) */
                F0 ^= FSM2;                         /** (s15 + R1) ^ R2 **/
                V1 ^= snow3g_table_A_mul[L1 >> 24];
                L11 = pCtx->LFSR_S[11];
                L12 = pCtx->LFSR_S[12];
                R0 = FSM3 ^ pCtx->LFSR_S[5];          /*** (R3 ^ s5 ) ***/
                V0 ^= snow3g_table_A_div[L11 & 0xff]; /* DIV(s11,3 )*/
                R0 += FSM2;                           /*** R2 + (R3 ^ s5 ) ***/
                V1 ^= snow3g_table_A_div[L12 & 0xff];
                V0 ^= L0 << 8; /*  (s0,1 || s0,2 || s0,3 || 0x00) */
                V1 ^= L1 << 8;
                V0 ^= L11 >> 8; /* (0x00 || s11,0 || s11,1 || s11,2 ) */
                V1 ^= L12 >> 8;
                S1_S2_S3_1(FSM3, FSM2, R1, FSM4, R0);
                V0 ^= F0; /* ^F */
                R1 = FSM3 ^ pCtx->LFSR_S[6];
                F1 = V0 + R0;
                F1 ^= FSM2;
                R1 += FSM2;
                FSM3 = Snow3g_S2(FSM2);
                FSM2 = FSM4;
                V1 ^= F1;

                /* shift LFSR twice */
                ShiftTwiceLFSR_1(pCtx);

                pCtx->LFSR_S[14] = V0;
                pCtx->LFSR_S[15] = V1;
        }

        /* set FSM into scheduling structure */
        pCtx->FSM_R3 = FSM3;
        pCtx->FSM_R2 = FSM2;
        pCtx->FSM_R1 = R1;
}

/**
 *******************************************************************************
 * @description
 * This function generates 5 words of keystream used in the initial stages
 * of snow3g F9.
 *
 * @param[in]       pCtx                         Context where the scheduled
 *keys are stored
 * @param[in/out]   pKeyStream          Pointer to the generated keystream
 *
 ******************************************************************************/
static inline void snow3g_f9_keystream_words(snow3gKeyState1_t *pCtx,
                                             uint32_t *pKeyStream)
{
        uint32_t F, XX;
        int i;

        ClockFSM_1(pCtx, &XX);
        ClockLFSR_1(pCtx);

        for (i = 0; i < 5; i++) {
                ClockFSM_1(pCtx, &F);
                pKeyStream[i] = F ^ pCtx->LFSR_S[0];
                ClockLFSR_1(pCtx);
        }
}

#endif /* _SNOW3G_INTERNAL_H_  */
