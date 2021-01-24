#include <stdio.h>
#include "header\\bin_ops.h"
#include "header\\bin_print.h"


/* Invert all bits
    Inverts all bits in file.
    Doesn't actually use 'word' argument. Included for consistency.
*/
int invert_bits(FILE *fp)
{
    FILE *tmp;
    char *tmp_name;
    char c;


    // Open temporary file
    tmp_name = tmpnam(NULL);
    if ((tmp = fopen(tmp_name, "wb+")) == NULL) {
        fprintf(stderr, "Invert bits: Error opening temporary file.\n");
        return -1;
    }

    // Invert all bits of original file and write to temporary file
    #ifdef DEBUG
    printf("Flipping bits...\n");
    #endif

    rewind(fp);

    while (1) {
        c = fgetc(fp);
        if (feof(fp)) break;

        fputc(~c, tmp);
    }
    #ifdef DEBUG
    printf("Done.\n");
    #endif

    // Copy temporary file back to original file
    // NOTE: bin_write takes care of rewinding files
    bin_write(tmp, fp);

    fclose(tmp);
    remove(tmp_name);

    return 0;
}


/* Reverse bits
    Fips bits in a word left to right
    Uses BitReverseTable256[] from bin_ops.h
    16-bit handles 32 bits at a time (2 words at once) (may change)
*/
unsigned int reverse_bits_in_word(unsigned int input, int word)
{
    unsigned int output;

    switch(word) {
        case 2: {
            output = (BitReverseTable256[input & 0xff] << 8) |
                (BitReverseTable256[(input >> 8) & 0xff]) |
                (BitReverseTable256[(input >> 16) & 0xff] << 24) |
                (BitReverseTable256[(input >> 24) & 0xff] << 16);
            break;
        }
        case 4: {
            output = (BitReverseTable256[input & 0xff] << 24) |
                (BitReverseTable256[(input >> 8) & 0xff] << 16) |
                (BitReverseTable256[(input >> 16) & 0xff] << 8) |
                (BitReverseTable256[(input >> 24) & 0xff]);
            break;
        }
        default: {
            fprintf(stderr, "Error: word size not recognized. Setting output to zero.\n");
            output = 0;
            break;
        }
    }

    return output;
}

/* flip_bits
    Reverse bits for all words in a file
*/
int flip_bits(FILE *fp, int word)
{
    unsigned int c[1];
    FILE *tmp;
    char *tmp_name;

    // Open temporary file
    tmp_name = tmpnam(NULL);
    if ((tmp = fopen(tmp_name, "wb+")) == NULL) {
        fprintf(stderr, "Invert bits: Error opening temporary file.\n");
        return -1;
    }

    rewind(fp);

    #ifdef DEBUG
    printf("Reversing bits in words...\n");
    #endif
    while (1)
    {
        fread(c, sizeof(unsigned int), 1, fp);

        if (feof(fp)) break;

        c[0] = reverse_bits_in_word(c[0], word);
        fwrite(c, sizeof(unsigned int), 1, tmp);
    }
    #ifdef DEBUG
    printf("Done.\n");
    #endif

    bin_write(tmp, fp);

    fclose(tmp);
    remove(tmp_name);
    return 0;
}

unsigned int reverse_bytes_in_word(unsigned int input, int word)
{
    unsigned int output;

    switch(word) {
        case 2: {
            output = ((input & 0xff) << 8) |
                ((input >> 8) & 0xff) |
                (((input >> 16) & 0xff) << 24) |
                (((input >> 24) & 0xff) << 16);
            break;
        }
        case 4: {
            output = ((input & 0xff) << 24) |
                (((input >> 8) & 0xff) << 16) |
                (((input >> 16) & 0xff) << 8) |
                ((input >> 24) & 0xff);
            break;
        }
        default: {
            fprintf(stderr, "Error: word size not recognized. Setting output to zero.\n");
            output = 0;
            break;
        }
    }

    return output;
}

int flip_bytes(FILE *fp, int word)
{
    unsigned int c[1];
    FILE *tmp;
    char *tmp_name;

    // Open temporary file
    tmp_name = tmpnam(NULL);
    if ((tmp = fopen(tmp_name, "wb+")) == NULL) {
        fprintf(stderr, "Invert bits: Error opening temporary file.\n");
        return -1;
    }

    rewind(fp);

    #ifdef DEBUG
    printf("Reversing bytes in words...\n");
    #endif
    while (1)
    {
        fread(c, sizeof(unsigned int), 1, fp);

        if (feof(fp)) break;

        c[0] = reverse_bytes_in_word(c[0], word);
        fwrite(c, sizeof(unsigned int), 1, tmp);
    }
    #ifdef DEBUG
    printf("Done.\n");
    #endif

    bin_write(tmp, fp);

    fclose(tmp);
    remove(tmp_name);
    return 0;
}



/* Deinterleave words
    Takes one file and places every other byte in one output file, the
    remaining bytes in another file.
*/
int deinterleave_words(FILE *fp1, FILE *fp2, FILE *fp3, int word) {
    #ifdef DEBUG
    printf("Starting de-interleave process...\n");
    #endif
    int i;
    char word_array[4];

    rewind(fp1);
    rewind(fp2);
    rewind(fp3);

    i = 1;
    while(1) {
        fread(word_array, 1, word, fp1);

        if (feof(fp1)) break;

        if ((i % 2) == 0)
            fwrite(word_array, 1, word, fp2);
        else if ((i % 2) == 1)
            fwrite(word_array, 1, word, fp3);

        i++;
        #ifdef DEBUG
        printf("%d\n", i);
        #endif
    }

    return 0;
}

/* Interleave words
    Takes two files and outputs a third, which contains the interleaved
    words.
*/
int interleave_words(FILE *fp1, FILE *fp2, FILE *fp3, int word) {
    int i;
    char word_array_a[4];
    char word_array_b[4];

    rewind(fp1);
    rewind(fp2);
    rewind(fp3);


    i = 1;
    while(1) {
        if ((i % 2) == 0)
        {
            fread(word_array_a, 1, word, fp1);
            if (feof(fp1)) break;  // What does this break do?
            fwrite(word_array_a, 1, word, fp3);
        }
        else if ((i % 2) == 1)
        {
            fread(word_array_b, 1, word, fp2);
            if (feof(fp2)) break;
            fwrite(word_array_b, 1, word, fp3);
        }

        i++;

        #ifdef DEBUG
        printf("%d\n", i);
        #endif
    }

    return 0;
}
