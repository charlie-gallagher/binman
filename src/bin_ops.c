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
*/

void reverse_bits_in_word(char c[], int word) {
    int i;

    for (i = 0; i < word; i++) {
        c[i] = BitReverseTable256[c[i]];
    }

    reverse_bytes_in_word(c, word);
}

/* Reverse bytes
    Flips bytes in a word left to right
*/
void reverse_bytes_in_word(char c[], int word) {
    static char out[8];
    int i, j;

    // Copy to buffer
    for (i = 0, j = word - 1; i < word; i++, j--)
        out[i] = c[j];

    // Copy back to array
    for (i = 0; i < word; i++) {
        c[i] = out[i];
    }
}


/* flip_bits
    Reverse bits for all words in a file
*/
int flip_bits(FILE *fp, int word)
{
    unsigned char c[8];
    FILE *tmp;
    char *tmp_name;

    if (word < 0 || word > 8) {
        fprintf(stderr, "Invert bits: Invalid word size\n");
        return -1;
    }

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
        int i;
        if ((i = fread(c, 1, word, fp)) != word) {
            #ifdef DEBUG
            printf("Incomplete word!\n");
            #endif
            break;
        }

        if (feof(fp)) break;

        reverse_bits_in_word(c, word);
        fwrite(c, 1, word, tmp);
    }
    #ifdef DEBUG
    printf("Done.\n");
    #endif

    bin_write(tmp, fp);

    fclose(tmp);
    remove(tmp_name);
    return 0;
}


/* Flip bytes
    Reverses order of bytes in each word in a file
*/
int flip_bytes(FILE *fp, int word)
{
    unsigned char c[8];
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
        fread(c, 1, word, fp);

        if (feof(fp)) break;

        reverse_bytes_in_word(c, word);
        fwrite(c, 1, word, tmp);
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
