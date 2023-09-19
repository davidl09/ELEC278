#include <stdio.h>
#include <stdint.h>

int array_sum(const int *array, size_t size) {
    int sum = 0;
    for(int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum;
}

int array_average(const int *array, size_t size) {
    return array_sum(array, size) / size;
}

int array_largest(int *array, size_t size) {
    int largest = INT32_MIN;
    for(int i = 0; i < size; i++) {
        largest = (array[i] > largest ? array[i] : largest);
    }
    return largest;
}

int main() {
    int array[] = {3, 1, 7, 5, 9};
    printf("Sum: %d\n", array_sum(array, 5));
    printf("Largest: %d\n", array_largest(array, 5));
    printf("Average: %d\n", array_average(array, 5));

    int array2[] = {4, 2, 19, 7, -5, 8};
    printf("Sum: %d\n", array_sum(array2, 5));
    printf("Largest: %d\n", array_largest(array2, 5));
    printf("Average: %d\n", array_average(array2, 5));
}
