#ifndef __UTILITIES__
#define __UTILITIES__

/*
 * Swap the values of `x` and `y`.
 */
void swap(int *x, int *y);

/* 
 * Generates a permutation of the integer array `vector` of `vectorSize` number of elements.
 */
void shuffle(int *vector, const int vectorSize);

/* 
 * Concatenates all the `vectorSize` elements of the integer array `vector` into a string, where values are separated by the optional separator `separator`. 
 */
char *intArrayToString(int *vector, int vectorSize, const char *separator = "");
#endif