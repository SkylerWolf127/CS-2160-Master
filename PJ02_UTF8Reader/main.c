/*
*#####################################################################################################################
 *Ian Gunnis
 *CS-2160 Assembly
 *PJ02 Part C
 *UTF8 Reader
 *
 *DEVELOPMENT ENVIRONMENT INFORMATION
 *Created by usr_skwolf on 10/29/2024
 *Operating System: Fedora 40 (Kernel 6.11.5-200)
 *Target architecture: AARCH64 / x86_64
 *Target operating system: Linux / UNIX / macOS
 *#####################################################################################################################
 */



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Function to calculate the number of leading 1s in a byte
int count_leading_ones(unsigned char byte) {
    int count = 0;
    while (byte & 0x80) { // Check the most significant bit
        count++;
        byte <<= 1;
    }
    return count;
}

// Function to process the HTML (UTF-8 encoded file)
void process_html_file(const char *input_file, const char *output_file) {
    FILE *in = fopen(input_file, "rb");
    FILE *out = fopen(output_file, "w");
    if (!in || !out) {
        perror("FAILURE: Could not open file. Please check that the files exist in your present working directory.");
        exit(EXIT_FAILURE);
    }

    unsigned char buffer[4];
    size_t file_size = 0;
    int ascii_count = 0, total_values = 0;
    uint32_t sum = 0;
    int leading_one_count[8] = {0}; // To count leading ones (0 to 7)

    while (fread(buffer, 1, 1, in)) {
        file_size++;
        int lead_ones = count_leading_ones(buffer[0]);
        leading_one_count[lead_ones]++;

        // Decode the UTF-8 character
        uint32_t value = 0;
        if (lead_ones == 0) {
            value = buffer[0];
            ascii_count++;
        } else {
            value = buffer[0] & (0xFF >> (lead_ones + 1));
            for (int i = 1; i < lead_ones; i++) {
                if (fread(buffer + i, 1, 1, in) != 1) {
                    fprintf(stderr, "FAILURE: Something broke. The UTF-8 sequence is totally busted man.\n");
                    fclose(in);
                    fclose(out);
                    exit(EXIT_FAILURE);
                }
                file_size++;
                value = (value << 6) | (buffer[i] & 0x3F);
            }
        }
        total_values++;
        sum += value;
    }

    fprintf(out, "File size: %zu bytes\n", file_size);
    fprintf(out, "Number of ASCII values: %d\n", ascii_count);
    fprintf(out, "Total values: %d\n", total_values);
    fprintf(out, "Sum of values: %u\n", sum);
    fprintf(out, "Leading ones tally:\n");
    for (int i = 0; i < 8; i++) {
        fprintf(out, "  %d leading ones: %d bytes\n", i, leading_one_count[i]);
    }

    fclose(in);
    fclose(out);
}

// Function to encode a 32-bit value as UTF-8
int encode_utf8(uint32_t value, unsigned char *buffer) {
    if (value <= 0x7F) {
        buffer[0] = value;
        return 1;//check if 1 byte
    } else if (value <= 0x7FF) { //check if value is 2 bytes
        buffer[0] = 0xC0 | (value >> 6);
        buffer[1] = 0x80 | (value & 0x3F);
        return 2;
    } else if (value <= 0xFFFF) { //check if value is 3 bytes
        buffer[0] = 0xE0 | (value >> 12);
        buffer[1] = 0x80 | ((value >> 6) & 0x3F);
        buffer[2] = 0x80 | (value & 0x3F);
        return 3;
    } else if (value <= 0x10FFFF) { //check if value is 4 bytes
        buffer[0] = 0xF0 | (value >> 18);
        buffer[1] = 0x80 | ((value >> 12) & 0x3F);
        buffer[2] = 0x80 | ((value >> 6) & 0x3F);
        buffer[3] = 0x80 | (value & 0x3F);
        return 4;
    }
    return -1; //invalid UTF 8 value
}

// Function to process the binary file
void process_bin_file(const char *input_file, const char *output_file) //Processing for binary files function
{
    FILE *in = fopen(input_file, "rb");
    FILE *out = fopen(output_file, "wb");
    if (!in || !out) {
        perror("FAILURE: Could not open file. Please check that the files exist in your present working directory.");
        exit(EXIT_FAILURE);
    }

    uint32_t value;
    unsigned char utf8_buffer[4];
    size_t total_bytes = 0;
    uint32_t sum = 0;
    int byte_length_count[5] = {0}; // To count 1-byte to 4-byte UTF-8 sequences
    int total_values = 0;

    while (fread(&value, sizeof(uint32_t), 1, in)) {
        // Convert from big-endian to host-endian
        value = (value << 24) | ((value & 0xFF00) << 8) | ((value & 0xFF0000) >> 8) | (value >> 24);
        sum += value;
        total_values++;

        // Encode as UTF-8
        int bytes_written = encode_utf8(value, utf8_buffer);
        if (bytes_written < 0)
        {
            fprintf(stderr, "ENCODER FAILURE ON VALUE %u AS UTF-8\n", value);
            fclose(in);
            fclose(out);
            exit(EXIT_FAILURE);
        }
        fwrite(utf8_buffer, 1, bytes_written, out);
        total_bytes += bytes_written;
        byte_length_count[bytes_written]++;
    }

    // Write summary to the output file
    fprintf(out, "\nTotal bytes: %zu\n", total_bytes);
    fprintf(out, "Sum of values: %u\n", sum);
    fprintf(out, "Summary of UTF-8 encoding:\n");
    for (int i = 1; i <= 4; i++)
    {
        fprintf(out, "  %d-byte sequences: %d\n", i, byte_length_count[i]);
    }//end for

    fclose(in);
    fclose(out);
}//end process_bin_file

// Main function
int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Please specify (4) files when calling this program: %s in.html out.txt in.bin out.utf8\n", argv[0]);
        return EXIT_FAILURE;
    }//end if

    process_html_file(argv[1], argv[2]);
    process_bin_file(argv[3], argv[4]);

    return EXIT_SUCCESS;
}//end main
