/*
*#####################################################################################################################
 *Ian Gunnis
 *CS-2160 Assembly
 *PJ02 Part A
 *CLANG MEMORY DUMPER
 *
 *DEVELOPMENT ENVIRONMENT INFORMATION
 *Created by usr_skwolf on 10/29/2024
 *Operating System: Fedora 40 (Kernel 6.11.5-200)
 *Target architecture: AARCH64 / x86_64
 *Target operating system: Linux / UNIX / macOS
 *#####################################################################################################################
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void fmemdump(FILE *fp, void *base, uintptr_t size, int ascii, char placeholder)
{
    uintptr_t offset = (uintptr_t)base;  // starting address or offset
    uintptr_t end = offset + size;       // ending address
    uintptr_t start_line, i;
    unsigned char buffer[16];

    for (start_line = offset & ~0xF; start_line < end; start_line += 16)
    {
        // Print the address (16-byte aligned)
        printf("%08lX  ", (unsigned long)start_line);

        // Read 16 bytes from either memory or file
        for (i = 0; i < 16; i++)
        {
            uintptr_t addr = start_line + i;
            if (addr >= offset && addr < end)
                {
                if (fp == NULL)
                {
                    // Reading from memory
                    printf("%02X ", ((unsigned char *)addr)[0]);
                }//end if
                else
                {
                    // Reading from file
                    fseek(fp, addr - offset, SEEK_SET);
                    fread(&buffer[i], 1, 1, fp);
                    printf("%02X ", buffer[i]);
                }//end else
            }//end if
            else
            {
                // Placeholder for out-of-bounds bytes
                printf("%c%c ", placeholder, placeholder);
            }//end else
        }//end for
        //ASCII conversions
        if (ascii)
            {
            printf(" |");
            for (i = 0; i < 16; i++)
            {
                uintptr_t addr = start_line + i;
                if (addr >= offset && addr < end)
                {
                    unsigned char ch = (fp == NULL) ? ((unsigned char *)addr)[0] : buffer[i];
                    printf("%c", isprint(ch) ? ch : placeholder); //Isprint is checking if ch value can be printed / valid ascii
                }//end if
                else
                {
                    printf("%c", placeholder);
                }//end else
            }//end for
            printf("|");
        }//end if

        printf("\n");
    }//end for
}//end fmemdump

int main(int argc, char *argv[])
{
    // Default memory buffer
    char data[] = "This is a temporary string for data.";

    if (argc > 1)
    {
        // File specified on command line
        FILE *fp = fopen(argv[1], "rb");
        if (fp == NULL)
        {
            perror("FAILURE: COULD NOT OPEN THE GIVEN INPUT FILE. Please check that it exists and is not corrupted.");
            return EXIT_FAILURE;
        }//end if

        //print out file dump
        printf("FILE DUMP (%s):\n", argv[1]);
        fmemdump(fp, 0, 64, 1, '.');  // Adjust size if you need more bytes
        fclose(fp);
    }//end if
    else
    {
        // No file specified, dump memory content
        printf("MEMORY DUMP:\n");
        fmemdump(NULL, data, sizeof(data), 1, '.');
    }//end else

    return EXIT_SUCCESS;
}//end main
