#define DEBUG

#ifdef DEBUG
int debug_mode = 1;
#else
int debug_mode = 0;
#endif

#ifndef RELEASE
int development = 1;
#else
int development = 0;
#endif