#ifndef MEMDUMP_DOT_H
#define MEMDUMP_DOT_H

#include <stdio.h>
#include <stdint.h>

void fmemdump(FILE *fp, void *base, uintptr_t size, int ascii, char placeholder);
void *memgen(uintptr_t n);

#endif
