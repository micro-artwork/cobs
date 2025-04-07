# COBS (Consistent Overhead Byte Stuffing) Library for Zephyr Project


This library provides functionality for encoding and decoding byte streams using the COBS (Consistent Overhead Byte Stuffing) algorithm.
Fundamentally, the COBS algorithm encodes data by removing 0x00 bytes, making it easier to distinguish between frames.
It is commonly used in communication protocols where 0x00 is used as a packet delimiter.

At the time of developing this library, there was no available library for using COBS in Zephyr RTOS, so the COBS algorithm was implemented manually for use.
However, in the latest version of Zephyr RTOS, a library supporting the COBS algorithm has been added.
The newly added library also provides the ability to set a delimiter other than 0x00, which means this library has become less necessary in the latest Zephyr RTOS.

Nonetheless, this implementation can still be useful in environments using older versions of Zephyr RTOS or as a simple and concise reference to help understand and use the COBS algorithm.

# How to Apply for Zephyr Project
In west.yml, under projects, add the following
```yml
  projects:
    ... 
    - name: cobs
      path: modules/lib/cobs
      revision: main
      url: https://github.com/micro-artwork/cobs
    ...
```
And run the following command
##  Existing Projects
```shell
west update
```
## New Proejcts
```shell
west init -l your-project/west.yml
west update
```
# How to Use for Zephyr Project

## Project Settings
Set up Kconfig in prj.conf
```
CONFIG_LIB_COBS=y
```
## Usage Examples
```c
#include <stdint.h>
#include <stdio.h>
#include <cobs.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

void enc_dec_test() {
  // Data to be encoded
  uint8_t data[] = {0x01, 0x02, 0x00, 0x03, 0x04};
  size_t data_len = sizeof(data);
  
  // Buffer for encoded data
  // COBS requires a minimum of 1 byte overhead, and a maximum of ⌈n/254⌉ bytes
  uint8_t encoded[sizeof(data) + MAX(1, sizeof(data) / 254)];
  size_t encoded_len = cobs_encode(encoded, data, data_len);

  printf("Encoded Data: ");
  for (size_t i = 0; i < encoded_len; i++) {
    printf("%02X ", encoded[i]);
  }
  printf("\n");

  // Buffer for decoded data
  uint8_t decoded[sizeof(data)];
  size_t decoded_len = cobs_decode(decoded, encoded, encoded_len);

  printf("Decoded Data: ");
  for (size_t i = 0; i < decoded_len; i++) {
    printf("%02X ", decoded[i]);
  }
  printf("\n");
}
```
### Result
```shell
Encoded Data: 03 01 02 03 03 04 
Decoded Data: 01 02 00 03 04 
```
