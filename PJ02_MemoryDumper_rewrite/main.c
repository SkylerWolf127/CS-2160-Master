#include <stdio.h>
#include <stdint.h>
#include "memdump.h"

int main(void)
{
    printf("Size of short:...... %i bytes\n", sizeof(short int));
    printf("Size of int:........ %i bytes\n", sizeof(int));
    printf("Size of long:....... %i bytes\n", sizeof(long int));
    printf("Size of long long:.. %i bytes\n", sizeof(long long int));
    printf("Size of float:...... %i bytes\n", sizeof(float));
    printf("Size of double:..... %i bytes\n", sizeof(float));
    printf("Size of pointer:.... %i bytes\n", sizeof(void *));

    FILE *fp;

    fmemdump(NULL, memgen(128), 128, 1, 0);

    if (fp = fopen("getty.txt", "rb"))
    {
        fmemdump(fp, (void *) 1000, 100, 1, ' ');       // fp1 is a FILE pointer for getty.txt
        fclose(fp);
    }

    if (fp = fopen("f15.jpg", "rb"))
    {
        fmemdump(fp, (void *) 0, 200, 1, '*');          // fp2 is a FILE pointer for f15.jpg
        fmemdump(fp, (void *) 20000, 120, 0, 0);        // fp2 is a FILE pointer for f15.jpg
        fclose(fp);
    }

    return 0;
}

