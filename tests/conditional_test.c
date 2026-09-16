#define DEBUG

#ifdef DEBUG
int debug = 1;
#else
int debug = 0;
#endif

#ifndef RELEASE
int test = 100;
#else
int test = 200;
#endif