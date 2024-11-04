/*
 * Copyright (c) 2024 I.G. BAEK
 *
 * SPDX-License-Identifier: MIT
 */

#include "cobs.h"

size_t cobs_encode(void *_RESTRICT dest, const void *_RESTRICT src, size_t len) {
  const uint8_t *s = (const uint8_t *)src;
  const uint8_t *src_end = (const uint8_t *)(s + len);
  uint8_t *d = (uint8_t *)dest;
  uint8_t *code = d++;
  *code = 1;

  do {
    if (*code == 0xFF) {
      code = d++;
      *code = 1;
    }
    if (*s) {
      *code += 1;
      *d++ = *s;
    } else {
      code = d++;
      *code = 1;
    }
  } while (++s < src_end);

  return (size_t)(d - (uint8_t *)dest);
}

size_t cobs_decode(void *_RESTRICT dest, const void *_RESTRICT src, size_t len) {
  const uint8_t *s = (const uint8_t *)src;
  const uint8_t *src_end = (const uint8_t *)(s + len);
  uint8_t *d = (uint8_t *)dest;
  const uint8_t *code = s++;
  const uint8_t *next_code = (code + *code);

  do {
    if (s == next_code) {
      if (next_code != (code + 0xFF)) {
        *d++ = 0x00;
      }
      next_code = (s + *s);
      code = s;
    } else {
      *d++ = *s;
    }
  } while (++s < src_end);

  return (size_t)(d - (uint8_t *)dest);
}
