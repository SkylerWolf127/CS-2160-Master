/*
 *#####################################################################################################################
 *Ian Gunnis
 *CS-2160 Assembly
 *PJ01 PART C
 *Hack Assembler
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
 * Examples: gcc assembler.c -o hack_assembler
 * chmod 755 hack_assembler
 * File names can be any case but CANNOT contain spaces.
 *
 *USAGE INSTRUCTIONS:
 *From command line, run ./<PROGNAME> <INPUT.asm> <OUTPUT.hack> <SYMBOL.sym>
 *Where INPUT.asm is the input assembly file. OUTPUT.hack is where the binary integers will be stored
 *SYMBOL.sym is the symbol table.
 *example: ./hack_assembler fill.asm fill.hack
 *Output and input files MUST be present in the current working directory in order for this to work right.
 *I'm not spending another week working in pathing and dynamic file generation. That's above my pay grade.
 *
 *#####################################################################################################################
 */
//DIRECTIVES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//SYMBOLIC CONSTANTS
#define MAX_LINE 300
#define MAX_SYMBOLS 2000
#define MAX_BINARY_LENGTH 16 //hack instructions are only 16-bits long. actual sanity check this time. Was causing ISSUES!!!!
//symbol table structure
typedef struct
{
    char symbol[MAX_LINE];
    int address;
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0; //Count those symbols bro!
int currentAddress = 16;  //allocate vars for mem16
int instructionCount = 0; //Instruction counter counter for counting instructions.


// Trim whitespace function
char* trimWhitespace(char* str)
{
    char* end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    //All spaces? (wtff)
    if(*str == 0) return str;

    //Nuke trailling space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end+1) = 0;

    return str;
}//end trimWhiteSpace

void addSymbol(char* symbol, int address) {
    if(symbolCount < MAX_SYMBOLS)
        {
        strcpy(symbolTable[symbolCount].symbol, symbol);
        //add to table
        symbolTable[symbolCount].address = address;
        symbolCount++;
    }//end if
    else {
        printf("Unable to add symbol. The table must be full or there was an internal error.  \"%s\"\n", symbol);
    }//end else
}//end add symbol

int getSymbolAddress(char* symbol)
{
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].symbol, symbol) == 0) {
            return symbolTable[i].address;
        }
    }
    // If not found, add it
    addSymbol(symbol, currentAddress);
    return currentAddress++;
}//end getSymbolAddress

// Predefined symbols for Hack
void initializeSymbolTable()
{
    //generate pre-defined symbols
    addSymbol("SP", 0);
    addSymbol("LCL", 1);
    addSymbol("ARG", 2);
    addSymbol("THIS", 3);
    addSymbol("THAT", 4);
    //screen and keyboard are special
    addSymbol("SCREEN", 16384);
    addSymbol("KBD", 24576);
    //R0 through R15 are made here
    for(int i = 0; i <= 15; i++)
    {
        char reg[4];
        sprintf(reg, "R%d", i);
        addSymbol(reg, i);
    }//end forloop
}//end initializeSymboltable
void writeSymbolTable(const char* symFile)
{
    FILE* output = fopen(symFile, "w");
    if (!output)
    {
        printf("Error opening symbol file.");
        return;
    }

    for (int i = 0; i < symbolCount; i++)
    {
        fprintf(output, "%s %d", symbolTable[i].symbol, symbolTable[i].address);
    }

    fclose(output);
}

char* decToBin(int n, char* result)
{
    for (int i = MAX_BINARY_LENGTH - 1; i >= 0; i--)
        {
        result[i] = (n & 1) + '0';
        n >>= 1;
    }//end for
    result[MAX_BINARY_LENGTH] = '\0';
    return result;
}//decimal to binary

//Handle A-instructions @symbol something
void handleAInstruction(char* line, FILE* output)
{
    char result[MAX_BINARY_LENGTH + 1];
    if (isdigit(line[1]))
        {
        // Convert constant value to binary
        int value = atoi(&line[1]);
        fprintf(output, "%s\n", decToBin(value, result));
    }//end if
    else
        {
        //Send symbols or handle them
        int address = getSymbolAddress(&line[1]);
        fprintf(output, "%s\n", decToBin(address, result));
    }//end else
}//end aInstructionHandler

//Map dest mnemonic to binary
int destToBin(char* dest)
{
    if (!dest) return 0;
    if (strcmp(dest, "M") == 0) return 1;
    if (strcmp(dest, "D") == 0) return 2;
    if (strcmp(dest, "MD") == 0) return 3;
    if (strcmp(dest, "A") == 0) return 4;
    if (strcmp(dest, "AM") == 0) return 5;
    if (strcmp(dest, "AD") == 0) return 6;
    if (strcmp(dest, "AMD") == 0) return 7;
    return 0;
    //I feel like I could make this a case statement but I'm totally running out of time...
}//end destToBinary

// Map comp mnemonic to binary
int compToBin(char* comp)
{
    if (strcmp(comp, "0") == 0) return 0b0101010;
    if (strcmp(comp, "1") == 0) return 0b0111111;
    if (strcmp(comp, "-1") == 0) return 0b0111010;
    if (strcmp(comp, "D") == 0) return 0b0001100;
    if (strcmp(comp, "A") == 0) return 0b0110000;
    if (strcmp(comp, "!D") == 0) return 0b0001101;
    if (strcmp(comp, "!A") == 0) return 0b0110001;
    if (strcmp(comp, "-D") == 0) return 0b0001111;
    if (strcmp(comp, "-A") == 0) return 0b0110011;
    if (strcmp(comp, "D+1") == 0) return 0b0011111;
    if (strcmp(comp, "A+1") == 0) return 0b0110111;
    if (strcmp(comp, "D-1") == 0) return 0b0001110;
    if (strcmp(comp, "A-1") == 0) return 0b0110010;
    if (strcmp(comp, "D+A") == 0) return 0b0000010;
    if (strcmp(comp, "D-A") == 0) return 0b0010011;
    if (strcmp(comp, "A-D") == 0) return 0b0000111;
    if (strcmp(comp, "D&A") == 0) return 0b0000000;
    if (strcmp(comp, "D|A") == 0) return 0b0010101;
    return 0;
    //I could probably make this a case statement... or a look up table now that I think about it.
    //TODO: make this a case statement if time permits.
}//end compToBin

// Map jump mnemonic to binary
int jumpToBin(char* jump)
{
    if (!jump) return 0;
    if (strcmp(jump, "JGT") == 0) return 1;
    if (strcmp(jump, "JEQ") == 0) return 2;
    if (strcmp(jump, "JGE") == 0) return 3;
    if (strcmp(jump, "JLT") == 0) return 4;
    if (strcmp(jump, "JNE") == 0) return 5;
    if (strcmp(jump, "JLE") == 0) return 6;
    if (strcmp(jump, "JMP") == 0) return 7;
    return 0;
    //I could probably make this a case statement...
    //TODO: make this a case statement if time permits.
}//end jumpToBin

// Handle C-instructions (dest=comp;jump)
void handleCInstruction(char* line, FILE* output) {
    char result[MAX_BINARY_LENGTH + 1];
    char* dest = strtok(line, "=");
    char* comp = strtok(NULL, ";");
    char* jump = strtok(NULL, "");

    if (comp == NULL)
    {
        comp = dest;
        dest = NULL;
    }//end if

    int cbin = compToBin(comp);
    int dbin = destToBin(dest);
    int jbin = jumpToBin(jump);

    int instruction = 0b1110000000000000 | (cbin << 6) | (dbin << 3) | jbin;
    fprintf(output, "%s\n", decToBin(instruction, result));
}

// First pass to process labels
void firstPass(FILE* input) {
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), input)) {
        // Trim and remove comments
        char* comment = strchr(line, '/');
        if (comment) *comment = '\0';

        trimWhitespace(line);

        if (line[0] == '(')
            {
            //Label declaration (e.g., (LOOP))
            line[strlen(line) - 1] = '\0'; // Remove closing parenthesis
            addSymbol(&line[1], instructionCount);
        } else if (line[0] != '\0') {
            //Valid instruction (either A or C)
            instructionCount++;
        }
    }
    rewind(input); // Reset the file pointer for the second pass
}

void secondPass(FILE* input, FILE* output) //actually do the conversion of instructions lmao
{
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), input)) //fgets to find input size (and scanf sucks)
        {
        // Trim and remove comments
        char* comment = strchr(line, '/');
        if (comment) *comment = '\0';

        trimWhitespace(line);

        if (line[0] == '@')
            {
            handleAInstruction(line, output);
        } else if (strchr(line, '=') || strchr(line, ';')) {
            handleCInstruction(line, output);
        }//end elif
    }//end if
}//end second pass

void assemble(const char* inputFile, const char* outputFile, const char* symFile) //build into instruction
{
    FILE* input = fopen(inputFile, "r");
    FILE* output = fopen(outputFile, "w");

    if (!input || !output)
        {
        printf("Error opening file.\n"); //throw this if the file reading in couldn't be read.
        return;
    }//end if

    initializeSymbolTable();
    firstPass(input);   //First pass to handle labels
    secondPass(input, output);  //Second pass to handle instructions

    fclose(input);
    fclose(output);
    //close out files (Not a good idea to leave that open :D)
}//end assemble

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Please specify an input, output, and symbol file.\n", argv[0]);
        return 1;
    }//end if
    //take args and call assemble func.
    assemble(argv[1], argv[2], argv[3]);
    //lord this works but IDK how!

    return 0;
}//end main
