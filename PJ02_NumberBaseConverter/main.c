/*
*#####################################################################################################################
 *Ian Gunnis
 *CS-2160 Assembly
 *PJ02 Part B
 *Random Number Base Converter
 *
 *DEVELOPMENT ENVIRONMENT INFORMATION
 *Created by usr_skwolf on 10/29/2024
 *Operating System Version: Fedora 40 (Kernel 6.11.5-200)
 *Target architecture: AARCH64 / x86_64
 *Target operating system: Linux / UNIX / macOS
 *
 *COMPILATION INSTRUCTIONS:
 *This program was written with Linux and macOS combined. Or an operating system that has GCC installed.
 *Compile using gcc -o progname cfile.c
 *
 *USAGE INSTRUCTIONS:
 * When calling from the command line, supply two arguments. Input and output files.
 *#####################################################################################################################
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_BASE 64

// Characters used for bases up to 64
const char base_chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";

// Convert character values into raw integers
int char_to_value(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if (c >= 'a' && c <= 'z') return c - 'a' + 36;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1; // Invalid character
}//end char_to_value

//Base 10 convertion
unsigned int base_to_decimal(const char *number, int base)
{
    unsigned int result = 0;
    while (*number)
    {
        int value = char_to_value(*number++);
        if (value < 0 || value >= base)
        {
            fprintf(stderr, "FAILURE: Invalid character in number for base %d\n", base);
            exit(1);
        }//end if
        result = result * base + value;
    }//end while
    return result;
}

// Convert a base 10 integer to a string in a given base
void decimal_to_base(unsigned int decimal, int base, char *output)
{
    char buffer[33]; // 32 bits max + null terminator
    int i = 0;

    do {
        buffer[i++] = base_chars[decimal % base];
        decimal /= base;
    } while (decimal > 0);

    //Reverse computed value
    for (int j = 0; j < i; j++) {
        output[j] = buffer[i - j - 1];
    }
    output[i] = '\0';
}//end decimal_to_base

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Please specify (2) files when calling this program: %s <input_file.dat> <output_file.out>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("FAILURE: COULD NOT OPEN INPUT FILE. Please check to make sure that it actually exists.");
        return 1;
    }

    FILE *output_file = fopen(argv[2], "w");
    if (!output_file) {
        perror("FAILURE: COULD NOT OPEN THE OUTPUT FILE. Please check to make sure that it actually exists.");
        fclose(input_file);
        return 1;
    }

    char line[100];
    unsigned int total_base10_sum = 0;

    // Write table headers
    fprintf(output_file, "%-15s %-15s %-15s %-20s %-25s\n",
            "INPUT", "ORIGINAL BASE", "TARGET BASE", "CONVERTED VALUE", "CONVERTED VALUE IN BASE 10");
    fprintf(output_file, "-------------------------------------------------------------------------------\n");

    // Read each line in the input file
    while (fgets(line, sizeof(line), input_file))
    {
        char number[33] = {0};
        int original_base, target_base;

        // Parse the line
        if (sscanf(line, "%32[^,],%d,%d", number, &original_base, &target_base) != 3)
        {
            fprintf(stdout, "FAILURE: Unknown line input format for input file. Check design specification: %s", line);
            continue;
        }//end if
        unsigned int base10_value = base_to_decimal(number, original_base); //base 10 conversion
        char converted_number[33];
        decimal_to_base(base10_value, target_base, converted_number); //convert from b10 to target base

        fprintf(output_file, "%-15s %-15d %-15d %-20s %-25u\n",
                number, original_base, target_base, converted_number, base10_value);
        total_base10_sum += base10_value; //total sum
    }//end while

    // Write the total base 10 sum to the output file
    fprintf(output_file, "-------------------------------------------------------------------------------\n");
    fprintf(output_file, "%-66s %-25u\n", "TOTAL IN BASE 10:", total_base10_sum);

    //Close files
    fclose(input_file);
    fclose(output_file);

    return 0;
}//end main

