#include <stdio.h>
#include "header\\bin_print.h"

/* OUTPUT_FILE

    Writes a file to a stream. If type is DUMP, convert to hex characters
    rather than raw bytes.
*/
int bin_out(FILE *tmp, FILE *output, char type)
{
    switch (type) {
        case DUMP: {
            #ifdef DEBUG
            printf("Dumping bytes...\n");
            #endif

            byte_to_hex(tmp);  // Convert tmp to hex chars
            break;
        }
        case WRITE: break;
        default: {
            fprintf(stderr, "Error, type was not one of DUMP or WRITE.\n");
            return -1;
        }
    }

    // Write to output (either file or stdout)
    if (bin_write(tmp, output) != 0) {
        fprintf(stderr, "Error writing to output file.\n");
        return -2;
    }

    return 0;
}


/* BIN_WRITE
    Copy bytes from one file to another.

    fp  :  File for reading from
    out :  File for writing to
*/
int bin_write(FILE *fp, FILE *out)
{
    unsigned char c;

    rewind(fp);  // Ensure input and output are set to zero
    rewind(out);

    while(1) {
        c = fgetc(fp);
        if (feof(fp))
            break;
        fputc(c, out);
    }
    return 0;
}


int byte_to_hex(FILE *fp)
{
    char *hex_tmp_name;
    FILE *hex_tmp;
    int c, i;       // i = number of bytes written

    // Make temporary file
    hex_tmp_name = tmpnam(NULL);

    if ((hex_tmp = fopen(hex_tmp_name, "wb+")) == NULL) {
        fprintf(stderr, "Error opening temporary file.\n");
        return -1;
    }

    // Store hex output in temporary file
    rewind(fp);  // Ensure whole file is read
    i = 1;
    while (1) {
        c = fgetc(fp);
        if (feof(fp)) break;

        fprintf(hex_tmp, "%02x ", c);

        // Add newline every 16 bytes read
        if (i % 16 == 0) {
            fputc('\n', hex_tmp);
        }
        i++;
    }

    fputc('\n', hex_tmp);

    // Copy temporary file original file
    bin_write(hex_tmp, fp);

    fclose(hex_tmp);
    remove(hex_tmp_name);

    return 0;
}


int bin_dump(FILE *fp)
{
    unsigned char c[16];
    int len, i;

    // Ensure whole file is read
    rewind(fp);

    // Print 16 (or fewer) bytes until end of file has been read
    while (1) {
        len = fread(c, 1, 16, fp);

        for (i = 0; i < len; i++)
            printf("%02x ", (int)c[i]);

        putchar('\n');

        if (feof(fp)) break;
    }

    return 0;
}

int bin_list(FILE *fp)
{
    unsigned char c[16];
    int len, i, j;      // j is line counter

    // Ensure whole file is read
    rewind(fp);

    // Print 16 (or fewer) bytes until end of file has been read
    j = 1;
    while (j <= 20) {
        len = fread(c, 1, 16, fp);

        for (i = 0; i < len; i++)
            printf("%02x ", (int)c[i]);

        putchar('\n');
        j++;

        if (feof(fp)) break;
    }

    return 0;
}

/* PRINT_HELP
    Prints help file.
*/
int print_help(void)
{
    int c;
    FILE *help;

    // Help file location defined in bin_print.h
    if ((help = fopen(HELP_FILENAME, "r")) == NULL)
    {
        fprintf(stderr, "Error opening help file.\n");
        return -1;
    }

    while ((c = fgetc(help)) != EOF)
        putchar(c);

    return 0;
}


/* Interleave and print or write
    fp1 : Odd byte file
    fp2 : Even byte file
    fp3 : Output file
    type: Output type (DUMP or WRITE)
*/
int interleave_out(FILE *fp1, FILE *fp2, FILE *fp3, int word, char type) {
    // Open temporary file
    FILE *tmp_out;
    char *tmp_name = tmpnam(NULL);

    if ((tmp_out = fopen(tmp_name, "wb+")) == NULL) {
        fprintf(stderr, "Interleave out: Error opening temporary file.\n");
        return -1;
    }

    // Interleave and print
    interleave_words(fp1, fp2, tmp_out, word);
    bin_out(tmp_out, fp3, type);

    fclose(tmp_out);
    remove(tmp_name);

    return 0;
}

int deinterleave_out(FILE *fp1, FILE *fp2, FILE *fp3, int word, char type) {
    // Open two temporary files
    FILE *tmp_out_a, *tmp_out_b;
    char *tmp_name_a = tmpnam(NULL);
    char *tmp_name_b = tmpnam(NULL);

    if ((tmp_out_a = fopen(tmp_name_a, "wb+")) == NULL) {
        fprintf(stderr, "Interleave out: Error opening temporary file.\n");
        return -1;
    }
    if ((tmp_out_b = fopen(tmp_name_b, "wb+")) == NULL) {
        fprintf(stderr, "Interleave out: Error opening temporary file.\n");
        return -1;
    }

    // Deinterleave and print
    deinterleave_words(fp1, tmp_out_a, tmp_out_b, word);
    #ifdef DEBUG
    printf("Odd bytes: \n-----------\n");
    #endif
    bin_out(tmp_out_a, fp2, type);

    #ifdef DEBUG
    printf("\nEven bytes: \n-----------\n");
    #endif
    bin_out(tmp_out_b, fp3, type);

    fclose(tmp_out_a);
    fclose(tmp_out_b);
    remove(tmp_name_a);
    remove(tmp_name_b);

    return 0;
}
