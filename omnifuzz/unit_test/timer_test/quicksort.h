#include <cstdlib>
#include <iostream>


void PrintIntArray(int* array, size_t size) {
  std::cout << "[";
  for (size_t i = 0; i < size; ++ i) {
    if (i != size - 1) {
      std::cout << array[i] << ", ";
    } else {
      std::cout << array[i] << "]" << std::endl;
    }
  }
}

void swap(int &a, int &b) {
  int tmp = a;
  a = b;
  b = tmp;
}


void __quicksort(int*, int, int);

void quicksort(int* array, size_t size) {
  srand(time(NULL));
  __quicksort(array, 0, size-1); 
}


void __quicksort(int* array, int left, int right) {

  if (left >= right) {
    return;
  }
  
  int l = left, r = right;
  int pivot_idx = l + rand() % (r - l + 1);
  int pivot = array[l];
  
  int i = l;
  while (i <= r) {
    if (array[i] < pivot) {
      swap(array[l++], array[i++]);
    } else if (array[i] > pivot) {
      swap(array[r--], array[i]);
    } else {
      i ++;
    }
  }

  __quicksort(array, left, l-1);
  __quicksort(array, r+1, right);
  
}
