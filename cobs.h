/*
 * Copyright (c) 2024 I.G. BAEK
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef COBS_LIB_H
#define COBS_LIB_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
#define _RESTRICT __restrict
#else
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define _RESTRICT restrict
#else
#define _RESTRICT
#endif
#endif

/**
 * @brief Encode data with consistent overhead byte stuffing algorithm.
 *
 * @param dest a pointer to the destination buffer location where the encoded data will be stored.
 * @param src a pointer to the source buffer location from where the data is to be encoded.
 * @param len the length of source data bytes.
 * @return the length of encoded data bytes.
 */
size_t cobs_encode(void *_RESTRICT dest, const void *_RESTRICT src, size_t len);

/**
 * @brief Decode data with consistent overhead byte stuffing algorithm
 *
 * @param dest a pointer to the destination buffer location where the decoded data will be stored.
 * @param src a pointer to the source buffer location from where the data is to be decoded.
 * @param len the length of source data bytes.
 * @return the length of decoded data bytes.
 */
size_t cobs_decode(void *_RESTRICT dest, const void *_RESTRICT src, size_t len);

#ifdef __cplusplus
}
#endif

#endif
