#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

void swap(int *a, int *b) {
    if (!(a && b)) return;
    if (a == b) return;
    int temp = *a;
    *a = *b;
    *b = temp;
}

void quickSort(int array[], const size_t size) {
    if (size < 2 || !array) return;
    if (size == 2) {
        if (array[0] > array[1]) {
            swap(array, array + 1);
        }
        return;
    }
    int *const pivot = array + size - 1;
    int *second = array;
    for (int *i = array; i < pivot; ++i) {
        if (*i > *pivot) {
            second = i;
        }
        else if (*i < *pivot) {
            swap(i, second);
            second = i;
        }
    }
    swap(pivot, second);

    quickSort(array, second - array);
    quickSort(second, size - (second - array));
}

__attribute__((const))
size_t parentOf(size_t index) {
    return (index - 1) / 2;
}


__attribute__((const))
size_t leftChildOf(size_t index) {
    return 2 * index + 1;
}

__attribute__((const))
size_t rightChildOf(size_t index) {
    return 2 * index + 2;
}

void heapDown(int array[], const size_t size, int index) {
    if (leftChildOf(index) >= size) return;

    if (rightChildOf(index) >= size) {
        if (array[leftChildOf(index)] < array[index]) {
            swap(array + index, array + leftChildOf(index));
            heapDown(array, size, (int)leftChildOf(index));
        }
        return;
    }

    size_t maxElemIndex = (array[leftChildOf(index)] > array[rightChildOf(index)] ? leftChildOf(index) : rightChildOf(index));
    if (array[maxElemIndex] > array[index]) {
        swap(array + index, array + maxElemIndex);
        heapDown(array, size, (int)maxElemIndex);
    }
}

void heapify(int array[], size_t size) {
    for (int i = (int)size - 1; i >= 0; --i) {
        heapDown(array, size, i);
    }
}

void heapSort(int array[], const size_t size) {
    heapify(array, size);
    size_t last = size - 1;
    while (last > 1) {
        swap(array, array + last);
        heapDown(array, last--, 0);
    }
}

void mergeSort(int array[], const size_t size) {
    if (!array || size < 2) return;
    if (size == 2) {
        if (array[0] > array[1]) {
            swap(array, array + 1);
        }
        return;
    }

    const size_t halfSize = size / 2;
    mergeSort(array, halfSize);
    mergeSort(array + halfSize, size - halfSize);

    int *result = malloc(size * sizeof (int));

    int *first = array, *second = array + halfSize;

    for (int *res = result; res < result + size; ++res) {
        if (first < array + halfSize && second < array + size) {
            if (*first < *second) {
                *res = *first++;
            }
            else *res = *second++;
        }
        else if (first < array + halfSize) {
            *res = *first++;
        }
        else if (second < array + size) {
            *res = *second++;
        }
        else exit(1);
    }
    memcpy(array, result, size * sizeof (int));
    free(result);
}

int main() {
    srand(time(NULL));
    const size_t size = 10;
    int *array = malloc(size * sizeof (int));
    for (int i = 0; i < size; ++i) {
       //array[i] = size - i;
        array[i] = rand() % 100;
        printf("%d, ", array[i]);
    }
    puts("\n");

    clock_t start = clock();

    quickSort(array, size);

    start = clock() - start;
    double timeTaken = (double)start / (double)CLOCKS_PER_SEC;

    for (int i = 0; i < size; ++i) {
        printf("%d, ", array[i]);
    }

    bool sorted = true;
    for (int i = 0; i < size - 1; ++i) {
        sorted = array[i] <= array[i + 1];
        if (!sorted) break;
    }

    putc('\n', stdout);
    assert(sorted);

    printf("Time taken: %lg ms\n", 1000 * timeTaken);
    free(array);
}
