/*
*#####################################################################################################################
 *Ian Gunnis
 *CS-2160 Assembly
 *PJ01 PART B
 *Hack Disassembler
 *
 *DEVELOPMENT ENVIRONMENT INFORMATION
 *Created by usr_skwolf on 9/9/2024
 *Target architecture: AARCH64 / x86_64
 *Target operating system: Linux / UNIX / macOS
 *
 *Known Issues:
 *Windows SUCKS. This program might have problems on Microsoft Windows. Works great on macOS and Linux though!
 *Symbol table is wonky... may not work as expected.
 *Does NOT check for endianness nor does it do any conversion.
 *
 *COMPILATION INSTRUCTIONS:
 * Use GCC. This will compile a BASH Script. Make sure it has execution permissions by using something like
 * chmod 755 <PROGNAME>
 * Examples: gcc dehack.c -o hack_disassembler
 * chmod 755 hack_disassembler
 * File names can be any case but CANNOT contain spaces.
 *
 *USAGE INSTRUCTIONS:
 *From command line, run ./<PROGNAME> <INPUT.hack> <OUTPUT.asm>
 *Where INPUT.asm is the input assembly file. OUTPUT.hack is where the binary integers will be stored
 *SYMBOL.sym is the symbol table.
 *example: ./hack_disassembler fill.hack fill.asm
 *Output and input files MUST be present in the current working directory in order for this to work right.
 *I'm not spending another week working in pathing and dynamic file generation. That's above my pay grade.
 *
 *#####################################################################################################################
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//SYMBOLIC CONSTANTS
#define MAX_LENGTH 17 //16 binary bits and a null terminator

const char *cTable[] = {"0101010", "0111111", "0111010", "0001100", "0110000", "0001101", "0110001", "0001111", "0110011", "0011111", "0110111", "0001110", "0110010", "0000010", "0010011", "0000111", "0000000", "0010101", "1110000", "1110001", "1110011", "1110111", "1110010", "1000010", "1010011", "1000111", "1000000", "1010101"};
const char *mnemonicsTable[] = {"0", "1", "-1", "D", "A", "!D", "!A", "-D", "-A", "D+1", "A+1", "D-1", "A-1", "D+A", "D-A", "A-D", "D&A", "D|A", "M", "!M", "-M", "M+1", "D+M", "M-1", "D-M", "M-D", "D&M", "D|M"};
const char *jump_table[] = {"", "JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP"};
const char *destTable[] = {"", "M", "D", "MD", "A", "AM", "AD", "AMD"};

// Function to disassemble A-instruction
void disassemble(const char* input_filename, FILE *output_file)
{
    FILE *file = fopen(input_filename, "r");
    if (!file)
    {
        fprintf(stderr, "YIKES! We couldn't open the input file. Please make sure that it's not corrupted and actually exists. %s\n", input_filename);
        exit(1); //terminate
    }//end if

    char binary[MAX_LENGTH]; // To hold each 16-bit instruction + null terminator
    while (fgets(binary, MAX_LENGTH, file))
    {
        //A inst start with 0 check for that
        if (binary[0] == '0')
        {
            int value = strtol(binary + 1, NULL, 2); // Convert to integer
            fprintf(output_file, "@%d\n", value);  // Write to file
        }//end if
        //Inst start with 1 must be C-type
        else if (binary[0] == '1')
            {
            char comp[8] = {0}, dest[4] = {0}, jump[4] = {0};
            strncpy(comp, binary + 3, 7);
            strncpy(dest, binary + 10, 3);
            strncpy(jump, binary + 13, 3);

            const char *comp_mnemonic = "", *dest_mnemonic = "", *jump_mnemonic = "";
            for (int i = 0; i < 28; i++)
            {
                if (strcmp(cTable[i], comp) == 0)
                {
                    comp_mnemonic = mnemonicsTable[i];
                    break; //TODO: In the next iteration get rid of this break. Only way I could figure out how to do it.
                }//end if
            }//end for loop

            int dest_index = strtol(dest, NULL, 2);
            dest_mnemonic = destTable[dest_index];

            int jump_index = strtol(jump, NULL, 2);
            jump_mnemonic = jump_table[jump_index];

            if (strlen(dest_mnemonic) > 0)
            {
                fprintf(output_file, "%s=%s", dest_mnemonic, comp_mnemonic);
            }//end if
            else
            {
                fprintf(output_file, "%s", comp_mnemonic);
            }//end else

            if (strlen(jump_mnemonic) > 0)
            {
                fprintf(output_file, ";%s", jump_mnemonic);
            }//end if

            fprintf(output_file, "\n");
        }//end elif
    }//end while loop

    fclose(file);
}//end disassemble

int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Please specify an INPUT.HACK file and an OUTPUT.asm file.\n", argv[0]);
        return 1;
    }//end if

    FILE *output_file = stdout;  // Default to standard output
    if (argc == 3)
    {
        output_file = fopen(argv[2], "w");
        if (!output_file)
        {
            fprintf(stderr, "YIKES! We couldn't open the output file. Please check to make sure it's not corrupted and that it actually exists. %s\n", argv[2]);
            return 1;
        }//end if
    }//end if

    // Disassemble the provided binary file and write to the output file
    disassemble(argv[1], output_file);

    if (output_file != stdout) {
        fclose(output_file);
    }

    return 0;
}//end main
