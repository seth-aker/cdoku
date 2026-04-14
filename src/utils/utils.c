#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

void shuffle_uint_8_array(uint8_t arr[], int arrLen) {
  uint8_t temp;
  for(int i = arrLen - 1; i > 0; --i) {
    int j = rand() % (i + 1);
    temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

bool includes_int(const int array[], int arrLen, int num) {
  for(int i = 0; i < arrLen; ++i) {
    if(array[i] == num) {
      return true;
    }
  }
  return false;
}

bool includes_uint8(const uint8_t array[], int arrlen, uint8_t num) {
  for(int i = 0; i < arrlen; ++i) {
    if(array[i] == num) {
      return true;
    }
  }
  return false;
}
// TODO: optimize this function to be faster; look into _mm_cmpeq_epi32 or 'shotgun intersections'
int get_uint8_intersection(const uint8_t arr1[], const uint8_t arr2[], int arr1Len, int arr2Len, uint8_t out[]) {
  int i = 0, j = 0, out_len = 0;
  while(i < arr1Len && j < arr2Len) {
    if(arr1[i] < arr2[j]) {
      i++;
    } else if(arr1[i] > arr2[j]) {
      j++;
    } else {
      out[out_len++] = arr1[i];
      i++; j++;
    }
  }
  return out_len;
}
