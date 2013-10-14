/*
 * Przemysław Pastuszka
 * Systemy operacyjne 2011 / 2012
 *
 */

#include <stdlib.h>
#include <poll.h>
#include <stdio.h>
#include "workers.h"

void idle_thread() {
    while(1) {
        poll(NULL,0,100);
    };
}

// naive (recurrency applied) fibonacci
int naive_fibonacci(int n) {
    if(n == FIB) { // we are going to leave function, so print the result
        printf("FIB: %d\n", (naive_fibonacci(n - 1) + naive_fibonacci(n - 2)) % MAGIC);
        return 0;
    }
    if(n < 2) {
        return 1;
    }
    return (naive_fibonacci(n - 1) + naive_fibonacci(n - 2)) % MAGIC;
}

// bongo sort works like:
// while data not sorted:
//      shuffle data
//
// expected running time: O(n!)
void bongo_sort() {
    int i;
    int *data = malloc(sizeof(int) * BONGO_SIZE); // some random data
    for(i = 0; i < BONGO_SIZE; ++i) {
        data[i] = rand();
    }
    print_data(data);

    while(is_not_sorted(data)) { // bongo sort
        shuffle(data);
    }

    print_data(data);
    free(data);
}

void print_data(int *data) {
    int i;
    for(i = 0; i < BONGO_SIZE; ++i) {
        printf("%d ", data[i]);
    }
    printf("\n");
}

int is_not_sorted(int *data) {
    int i;
    for(i = 1; i < BONGO_SIZE; ++i) {
        if(data[i - 1] > data[i]) {
            return 1;
        }
    }
    return 0;
}

void shuffle(int data[]) {
    int k = rand() % BONGO_SIZE;
    int i;
    for(i = 0; i < k; ++i) {
        int i = rand() % BONGO_SIZE;
        int j = rand() % BONGO_SIZE;
        swap(data, i, j);
    }
}

void swap(int data[], int i, int j) {
    int tmp = data[i];
    data[i] = data[j];
    data[j] = tmp;
}
