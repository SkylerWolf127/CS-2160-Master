#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "memdump.h"

void *memgen(uintptr_t n)
{
	unsigned char *mem;
	
	srand(time(NULL));
	
	mem = (uint8_t *) malloc(n);
	
	for (int i = 0; i < n; i++)
	{
	    mem[i] = rand() & 0xFF;
	}//end for

	for (int i = 0; i < 16; i++)
	{
	    mem[rand() % n] = 0;
	}//end for
		
	((double *) mem)[3] = M_PI;
	((float *) mem)[25] = M_PI;
	((double *) mem)[6] = M_E;
	((float *) mem)[1] = M_E;
	((int *) mem)[2] = 4242424242;
	((short *) mem)[19] = 12345;
	((short *) mem)[20] = -3333;
	strcpy(mem+59, "PJ02 Mem Gen.");
	strcpy((uint8_t *)(((double *) mem)+13), "CS-2160");
	
	return (void *) mem;
}

void fmemdump(FILE *fp, void *base, uintptr_t size, int ascii, char placeholder)

{
    uintptr_t start = (uintptr_t) base;
    uintptr_t end = start + size;
    uintptr_t aligned_start = start & ~0xF; // Align to 16-byte boundary
    unsigned char buffer[16];
    size_t bytes_read;
    // Print table header
    printf("Address         Hexadecimal Values                               ");
    if (ascii)
        printf("ASCII");
    printf("\\n");
    printf("--------        -----------------------------------------------  ");
    if (ascii)
        printf("----------------");

    printf("\\n");
    for (uintptr_t addr = aligned_start; addr < end; addr += 16)
    {
        printf("%08lx        ", addr);
        // Load 16 bytes either from memory or file
        if (fp == NULL)
        {
            for (int i = 0; i < 16; i++)
            {
                uintptr_t curr_addr = addr + i;
                if (curr_addr >= start && curr_addr < end)
                {
                    buffer[i] = *((unsigned char *) curr_addr);
                }
                else
                {
                    buffer[i] = placeholder;
                }//end else
            }//end for
        }//end if
        else
        {
            fseek(fp, addr - start, SEEK_SET);
            bytes_read = fread(buffer, 1, 16, fp);
            for (int i = bytes_read; i < 16; i++)
            {
                buffer[i] = placeholder;
            }//end for
        }//end else
        for (int i = 0; i < 16; i++)
            {
            if (addr + i >= start && addr + i < end)
            {
                printf("%02x", buffer[i]);
            }//end if
            else
            {
                printf("  ");
            }//end else
            // Add a space after every 4 bytes
            if ((i + 1) % 4 == 0)
            {
                printf(" ");
            }//end if
        }//end for
        if (ascii)
        {
            printf(" ");
            printf("|");
            for (int i = 0; i < 16; i++)
            {
                if (addr + i >= start && addr + i < end && isprint(buffer[i]))
                {
                    printf("%c", buffer[i]);
                }//end if
                else
                {
                    printf(".");
                }//end else
            }//end for
            printf("|");
        }//end if
        printf("\\n");
    }//end for
}//end fmemdump

