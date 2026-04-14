#ifndef SRC_UTILS_UTILS_H
#define SRC_UTILS_UTILS_H
#include <stdint.h>
#include <stdbool.h>

void shuffle_uint_8_array(uint8_t arr[], int arrLen);
bool includes_int(const int array[], int arrLen, int num);
bool includes_uint8(const uint8_t array[], int arrlen, uint8_t num);
int get_uint8_intersection(const uint8_t arr1[], const uint8_t arr2[], int arr1Len, int arr2Len, uint8_t out[]);
#endif // SRC_UTILS_UTILS_H
