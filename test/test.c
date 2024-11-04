#include "test.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../cobs.h"

static const uint8_t *test_list[] = {test_1, test_2, test_3, test_4,  test_5, test_6,
                                     test_7, test_8, test_9, test_10, test_11};

static const uint8_t *encoded_list[] = {
    test_1_encoded, test_2_encoded, test_3_encoded, test_4_encoded,  test_5_encoded, test_6_encoded,
    test_7_encoded, test_8_encoded, test_9_encoded, test_10_encoded, test_11_encoded};

static const uint32_t test_len_list[] = {
    sizeof(test_1), sizeof(test_2),  sizeof(test_3),  sizeof(test_4),
    sizeof(test_5), sizeof(test_6),  sizeof(test_7),  sizeof(test_8),
    sizeof(test_9), sizeof(test_10), sizeof(test_11),
};

static const uint32_t encoded_len_list[] = {
    sizeof(test_1_encoded), sizeof(test_2_encoded),  sizeof(test_3_encoded), sizeof(test_4_encoded),
    sizeof(test_5_encoded), sizeof(test_6_encoded),  sizeof(test_7_encoded), sizeof(test_8_encoded),
    sizeof(test_9_encoded), sizeof(test_10_encoded), sizeof(test_11_encoded)};

bool diff(const uint8_t *result, size_t res_len, const uint8_t *expected, size_t exp_len,
          bool print_out) {
  if (print_out) {
    printf("Expected Length: %lu, Result Length: %lu\nExp:", exp_len, res_len);
  }

  bool is_same = (res_len == exp_len) ? 1 : 0;

  for (int i = 0; i < exp_len; i++) {
    if (print_out) {
#if DIFF_DATA_PRINT_ALL
      printf(" %02X", expected[i]);
#else
      if (exp_len < 20 || i < 7 || i > exp_len - 8) {
        printf(" %02X", expected[i]);
      } else if (i == 7) {
        printf(" ...");
      }
#endif
    }
    if (res_len == exp_len) {
      is_same &= result[i] == expected[i];
    }
  }

  if (print_out) {
    printf("\nRes:");
    for (int i = 0; i < res_len; i++) {
#if DIFF_DATA_PRINT_ALL
      printf(" %02X", result[i]);
#else
      if (res_len < 20 || i < 7 || i > res_len - 8) {
        printf(" %02X", result[i]);
      } else if (i == 7) {
        printf(" ...");
      }
#endif
    }
  }

  if (print_out) {
    printf("\nRESULT: %s\n", is_same ? "PASSED" : "FAILED");
  }
  return is_same;
}

int32_t main() {
  uint8_t buffer[1030] = {0};
  int32_t test_num = sizeof(test_list) / sizeof(test_list[0]);

#if ENC_DEC_TEST

  int32_t enc_passed = 0, enc_failed = 0;
  int32_t dec_passed = 0, dec_failed = 0;

  printf("###############################################################\n");
  printf("# ENCODING TEST\n");
  printf("###############################################################\n\n");

  for (int32_t i = 0; i < test_num; i++) {
    printf("TEST %u\n", i + 1);
    size_t len = cobs_encode(buffer, test_list[i], test_len_list[i]);
    bool is_same = diff(buffer, len, encoded_list[i], encoded_len_list[i], true);
    if (is_same) {
      enc_passed++;
    } else {
      enc_failed++;
    }
    if (i < test_num - 1) {
      printf("---------------------------------------------------------------\n");
    }
  }

  printf("\n###############################################################\n");
  printf("# DECODING TEST\n");
  printf("###############################################################\n\n");

  for (int32_t i = 0; i < test_num; i++) {
    printf("TEST %u\n", i + 1);
    size_t len = cobs_decode(buffer, encoded_list[i], encoded_len_list[i]);
    bool is_same = diff(buffer, len, test_list[i], test_len_list[i], true);
    if (is_same) {
      dec_passed++;
    } else {
      dec_failed++;
    }
    if (i < test_num - 1) {
      printf("---------------------------------------------------------------\n");
    }
  }

  printf("\n###############################################################\n");
  printf("# ENCODING AND DECODING RESULTS\n");
  printf("###############################################################\n\n");
  printf("ENCODING - TESTED: %u, PASSED: %u, FAILED: %u\n", enc_passed + enc_failed, enc_passed,
         enc_failed);
  printf("DECODING - TESTED: %u, PASSED: %u, FAILED: %u\n", dec_passed + dec_failed, dec_passed,
         dec_failed);
  printf("---------------------------------------------\n");
  printf("TOTAL    - TESTED: %u, PASSED: %u, FAILED: %u\n\n",
         enc_passed + enc_failed + dec_passed + dec_failed, enc_passed + dec_passed,
         enc_failed + dec_failed);

#endif

#if RANDOM_ENC_DEC_TEST
  printf("\n###############################################################\n");
  printf("# RANDOM ENCODING AND DECODING TEST\n");
  printf("###############################################################\n\n");

  uint8_t rnd_src[1024] = {0};
  uint8_t rnd_dest[1030] = {0};
  int32_t rnd_passed = 0;
  int32_t rnd_failed = 0;

  srand(time(NULL));

  for (int32_t i = 0; i < 100; i++) {
    size_t rnd_enc_len = cobs_encode(rnd_dest, rnd_src, 1024);
    bool is_same = true;
    for (int32_t j = 0; j < rnd_enc_len; j++) {
      if (rnd_dest[j] == 0x00) {
        is_same = false;
        break;
      }
    }

    if (is_same) {
      size_t rnd_dec_len = cobs_decode(buffer, rnd_dest, rnd_enc_len);
      is_same = diff(rnd_src, 1024, buffer, rnd_dec_len, false);
    }

    if (is_same) {
      rnd_passed++;
    } else {
      rnd_failed++;
    }
  }
  printf("RANDOM   - TESTED: %u, PASSED: %u, FAILED: %u\n", rnd_passed + rnd_failed, rnd_passed,
         rnd_failed);
#endif

#if TIME_BENCH

  printf("\n###############################################################\n");
  printf("# TIME BENCHMARKING\n");
  printf("###############################################################\n\n");

  uint8_t tm_bench_src[5][1024] = {0};
  uint8_t tm_bench_dest[5][1030] = {0};
  size_t tm_dest_len_list[5] = {0};
  uint8_t temp_buff[1024] = {0};

  for (int32_t i = 0; i < 5; i++) {
    for (int32_t j = 0; j < 1024; j++) {
      tm_bench_src[i][j] = j % (i + 1);
    }
  }

  // diff
  // for (int32_t i = 0; i < 5; i++) {
  //   size_t enc_len = cobs_encode(tm_bench_dest[i], tm_bench_src[i], 1024);
  //   size_t dec_len = cobs_decode(temp_buff, tm_bench_dest[i], enc_len);
  //   diff(temp_buff, dec_len,  tm_bench_src[i], 1024, true);
  // }

  double enc_time_sum = 0;
  for (volatile int32_t t = 0; t < 10; t++) {
    clock_t enc_start = clock();
    for (volatile int32_t i = 0; i < 10000; i++) {
      for (volatile int32_t j = 0; j < 5; j++) {
        tm_dest_len_list[j] = cobs_encode(tm_bench_dest[j], tm_bench_src[j], 1024);
      }
    }
    clock_t enc_end = clock();
    enc_time_sum += (double)(enc_end - enc_start) / CLOCKS_PER_SEC;
  }

  printf("Encoding Time Avg.: %f sec\n", enc_time_sum / 10);

  double dec_time_sum = 0;
  for (volatile int32_t t = 0; t < 10; t++) {
    clock_t dec_start = clock();
    for (volatile int32_t i = 0; i < 10000; i++) {
      for (volatile int32_t j = 0; j < 5; j++) {
        cobs_decode(temp_buff, tm_bench_dest[j], tm_dest_len_list[j]);
      }
    }
    clock_t dec_end = clock();
    dec_time_sum += (double)(dec_end - dec_start) / CLOCKS_PER_SEC;
  }

  printf("Decoding Time Avg.: %f sec\n\n", dec_time_sum / 10);

#endif

  return 0;
}
