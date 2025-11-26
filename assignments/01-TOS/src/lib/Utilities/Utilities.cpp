#include "lib/Utilities/Utilities.h"
#include "Arduino.h"
#include "stdlib.h"
#include "string.h"

void swap(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

void shuffle(int *vector, int vectorSize) 
{
    srand(millis());
    for (int i = vectorSize - 1; i >= 0; i--) 
    {
        swap(&vector[i], &vector[rand() % (i + 1)]);
    }
}

char *intArrayToString(int *vector, int vectorSize, const char *separator) 
{
    // Worst case: "-2147483648," is 12 chars per element
    int maxLen = vectorSize * 12 + 1;

    char* res = (char*)malloc(maxLen);
    res[0] = '\0';  // start empty

    char buffer[16]; // temporary buffer for each number

    for (int i = 0; i < vectorSize; i++) {
        itoa(vector[i], buffer, 10);
        strcat(res, buffer);
        if (i < vectorSize - 1) 
        {
            strcat(res, separator);
        }
    }

    return res;
}