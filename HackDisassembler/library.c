/*
*#####################################################################################################################
 *Ian Gunnis
 *CS-2160 Assembly
 *PJ01 PART C
 *
 *DEVELOPMENT ENVIRONMENT INFORMATION
 *Created by usr_skwolf on 9/9/2024
 *Target architecture: AARCH 64 / x86_64
 *Target operating system: Linux / UNIX
 *
 *Known Issues:
 *Windows SUCKS. This program might have problems on Microsoft Windows. Works great on macOS and Linux though!
 *
 *COMPILATION INSTRUCTIONS:
 * Use GCC. This will compile a BASH Script. Make sure it has execution permissions by using something like
 * chmod 755 <FILENAME>
 * Examples: gcc assembler.c -o hack_assembler
 * chmod 755 hack_disassembler
 * File names can be any case but CANNOT contain spaces.
 *
 *USAGE INSTRUCTIONS:
 *From command line, run ./<FILENAME> <INPUT.asm> <OUTPUT.hack> <SYMBOL.sym>
 *example: ./hack_assembler fill.asm fill.hack
 *Output and input files MUST be present in the current working directory in order for this to work right.
 *I'm not spending another week working in pathing and dynamic file generation. That's above my pay grade.
 *
 *#####################################################################################################################
 */

#include <stdio.h>

void hello(void)
{
    printf("Hello, World!\n");
}
