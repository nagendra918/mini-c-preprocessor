#include "../include/comment.h"

#define MAX 100
#define PI 3.14

// This comment should disappear

#ifdef MAX
int value = MAX;
#else
int value = 0;
#endif

#ifndef DEBUG
float pi = PI;
#endif

printf("MAX = %d\n", MAX);
