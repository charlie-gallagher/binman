#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bin_arg_parse.h"

/* Parse arguments from the command line

*/
struct binman_struct *bin_arg_parse(int argc, char *argv[])
{
    static struct binman_struct BM; // allocate memory for struct
    bin_init_struct(&BM);

    char *flags[10];
    char **pflags;
    pflags = flags;

    char *params[10][3];
    char *(*pparams)[3];  // Pointer to array of three pointers to type char
    pparams = params;

    char *input_file_name = argv[argc - 1];  // Set input filename to last arg
    char *second_input_file_name = NULL;


    int i;

    /* Initialize flags and pflags to NULL */
    for (i = 0; i < 10; i++) {
        flags[i] = NULL;
        params[i][0] = NULL;
        params[i][1] = NULL;
        params[i][2] = NULL;
    }


    /* Decide what's a flag and what's an argument */
    for (i = 1; i < argc - 1; i++) {  // assume last argument is filename
        printf("Input: %s\n", argv[i]);
        if (is_bin_flag(argc, argv, i)) {
            *pflags = argv[i];
            pflags++;
        }
        else if (is_bin_outputs(argc, argv, i)) {
            (*pparams)[0] = argv[i];
            (*pparams)[1] = argv[i + 1];
            (*pparams)[2] = argv[i + 2];
            pparams++;
            i += 2;  // Skip next two command line arguments
        }
        else if (is_bin_param(argc, argv, i)) {
            (*pparams)[0] = argv[i];
            (*pparams)[1] = argv[i + 1];
            pparams++;
            i++;  // Skip next command line argument
        }
        else if (is_bin_terminal_input(argc, argv, i)) {
            printf("Second input detected.\n");
            input_file_name = argv[argc-2];
            second_input_file_name = argv[argc-1];
        }
        else {
            fprintf(stderr, "Illegal argument.\n");
            return NULL;
        }
    }

    /* Set input file(s) */
    BM.input_file_name = input_file_name;

    if (second_input_file_name != NULL) {
        BM.second_input_file_name = second_input_file_name;
    }

    /* Parse flags and parameters, checking for errors */
    if (parse_flags(&BM, flags) != 0) {
        fprintf(stderr, "Error parsing flags: Invalid flag\n");
        return NULL;
    }

    if (parse_params(&BM, params) != 0) {
        fprintf(stderr, "Error parsing parameters: Invalid flag\n");
        return NULL;
    }

    return &BM;
}




/* Process flags and parameters

    This function is responsible for checking that the structure makes
    sense. For example, if there are two output files, there are not
    two input files.

    Returns 1 for success, 0 for failure
*/
int process_flags_and_params(struct binman_struct *BM) {
    FILE *in;
    FILE *second_in;
    FILE *out;
    FILE *second_out;
    char output_type[2];

    // Check executability of struct
    if (is_executable(BM) != 0) {
        fprintf(stderr, "Error interpreting commands.\n");
        return -1;
    }

    // Open output file(s) and copy to struct
    if (bin_open_output(BM) != 0) {
        fprintf(stderr, "Error opening output file(s)\n");
        return -1;
    }

    // Open input file(s) and copy to struct
    if (bin_open_input(BM) != 0) {
        fprintf(stderr, "Error opening input file(s)\n");
        return -1;
    }


    return 0;
}






/* Parse flags and set indicators in structure

    Valid flags: i, f, F, l, W, d, h, ?
        i       invert bits
        f       flip bits in word
        F       flip bytes in word
        l       list
        W       Interleave words
        d       Dump file
        h, ?    Print help screen

        Returns 0 (success) or -1 (failure)
*/
int parse_flags(struct binman_struct *BM, char *flags[]) {
    int i, oflag = 0;
    char f;
    char have_appeared[10];

    for (i = 0; flags[i] != NULL && i < 10; i++) {
        // Flag character
        f = flags[i][1];

        /* Check if duplicate flag */
        if (is_duplicate_flag(have_appeared, f, i) == 1) {
            fprintf(stderr, "\nWarning! Duplicate flag. Skipping.\n");
            continue;
        }

        /* Set flags in struct */
        switch (f) {
            case 'i': {
                OFLAGS(oflag) = I_BIT;
                oflag++;
                break;
            }
            case 'f': {
                OFLAGS(oflag) = F_BIT;
                oflag++;
                break;
            }
            case 'F': {
                OFLAGS(oflag) = F_BYTE;
                oflag++;
                break;
            }
            case 'l': {
                OFLAGS(oflag) = LIST;
                oflag++;
                break;
            }
            case 'd': {
                OUTPUT_TYPE = DUMP;
                break;
            }
            case 'W': {
                INTER_FILES = 1;
                break;
            }
            case 'D': {
                DEINTER_FILES = 1;
                break;
            }
            case 'h':
            case '?': {
                DISPLAY_HELP = 1;
                break;
            }
            default: {
                fprintf(stderr, "Warning: Unknown flag  %c\n", f);
                return -1;
            }
        }

        /* Add flag to list of those that have appeared */
        have_appeared[i] = f;
    }

    return 0;
}


/* Parse parameters and set fields in structure

    Valid parameter flags: b, e, L, o, O, w
        b       offset
        e       Last byte to include
        L       Max length from offset
        o       Output filename(s), overwrite if exists
        O       Output filename(s), do not overwrite
        w       Number of bytes in a word

        Returns 0 if successful, else error occurred
*/
int parse_params(struct binman_struct *BM, char *param[10][3]) {
    int i;
    char p, *p_value;
    char have_appeared[10];

    for (i = 0; param[i][0] != NULL && i < 10; i++) {

        // Parameter name and value
        p = param[i][0][1];
        p_value = param[i][1];

        /* Check if duplicate flag */
        if (is_duplicate_flag(have_appeared, p, i) == 1) {
            fprintf(stderr, "\nWarning! Duplicate flag. Skipping.\n");
            continue;
        }

        /* Set flags in struct */
        switch (p) {
            case 'b': {
                FILE_START = atol(p_value);
                #ifdef DEBUG
                printf("Start location: %ld\n", FILE_START);
                #endif
                break;
            }
            case 'e': {
                FILE_END = atol(p_value);
                break;
            }
            case 'L': {
                FILE_LENGTH = atol(p_value);
                break;
            }
            case 'w': {
                WORD_SIZE = atoi(p_value);
                break;
            }
            case 'o': {
                FILE_OVERWRITE = 1;
                OUTPUT_FILE_NAME = param[i][1];
                if (param[i][2] != NULL)
                    SECOND_OUTPUT_FILE_NAME = param[i][2];
                break;
            }
            case 'O': {
                FILE_OVERWRITE = 0;
                OUTPUT_FILE_NAME = param[i][1];
                if (param[i][2] != NULL)
                    SECOND_OUTPUT_FILE_NAME = param[i][2];
                break;
            }
            default: {
                fprintf(stderr, "Warning: Unknown argument name  [%c\n]", p);
                return -1;
            }
        }

        /* Add flag to list of those that have appeared */
        have_appeared[i] = p;
    }

    return 0;
}

/* Produce an indicator for flag format

    Must be of the form "/[A-Za-z?]"
*/
int is_bin_flagform(char *item) {
    char has_slash, is_character, len_valid;

    len_valid = strlen(item) == 2;
    has_slash = item[0] == '/';
    is_character = (item[1] >= 65 && item[1] <=90) ||
                    (item[1] >= 97 && item[1] <= 122) ||
                    (item[1] == '?');

    if (len_valid && has_slash && is_character)
        return 1;

    return 0;
}

/* Produce an indicator for flag

    Must be followed by another flag or be the penultimate argument
*/
int is_bin_flag(int argc, char *argv[], int index) {
    char flagform = is_bin_flagform(argv[index]);

    if (index == argc - 2 && flagform) {
        return 1;
    }
    if (index == argc - 3 && flagform) {
        switch(argv[index][1]) {
            case 'b':
            case 'e':
            case 'L':
            case 'o':
            case 'O':
            case 'w': return 0;
            default: return 1;
        }
    }
    else if (index < argc - 2 && flagform) {
        if (is_bin_flagform(argv[index + 1])) {
            return 1;
        }
    }

    return 0;
}

/* Produce an indicator for param

    Must not be in penultimate position and must have a flag and a non-flag
*/
int is_bin_param(int argc, char *argv[], int index) {
    if (index == argc - 2)
        return 0;

    if (is_bin_flagform(argv[index]) && !is_bin_flagform(argv[index + 1]))
        return 1;

    return 0;
}

/* Produce an indicator for two output files */
int is_bin_outputs(int argc, char *argv[], int index) {
    if (index >= argc - 3)
        return 0;

    char flag = is_bin_flagform(argv[index]);
    char out1 = !is_bin_flagform(argv[index + 1]);
    char out2 = !is_bin_flagform(argv[index + 2]);

    if ((argv[index][1] == 'o' || argv[index][1] == 'O')
        && flag && out1 && out2) {
            return 1;
        }

    return 0;
}

/* Produce an indicator for two input files */
int is_bin_terminal_input(int argc, char *argv[], int index) {
    if (index == argc - 2 && !is_bin_flagform(argv[index]))
        return 1;

    return 0;
}

/* Initialize the binman struct with deafult values
*/
void bin_init_struct(struct binman_struct *BM) {
    OFLAGS(0) = OFLAGS(1) = OFLAGS(2) = OFLAGS(3) = 0;
    FILE_START = FILE_END = FILE_LENGTH = FILE_OVERWRITE =
        INTER_FILES = DEINTER_FILES = DISPLAY_HELP = 0;
    OUTPUT_TYPE = WRITE;
    WORD_SIZE = 4;
    INPUT_FILE_NAME = NULL;
    SECOND_INPUT_FILE_NAME = NULL;
    INPUT_FILE = NULL;
    INPUT_FILE2 = NULL;
    OUTPUT_FILE_NAME = NULL;
    SECOND_OUTPUT_FILE_NAME = NULL;
    OUTPUT_FILE = NULL;
    OUTPUT_FILE2 = NULL;
}


int is_duplicate_flag(char *already_appeared, char c, char index) {
    int j;

    for (j = 0; j < index; j++) {
        if (c == already_appeared[j])
            return 1;
    }
    return 0;
}

/* Evaluates struct to ensure operations make sense

    At the moment, all I have planned is to make sure proper
    number of files is given.
*/
int is_executable(struct binman_struct *BM) {
    // Conflicts
    if (INTER_FILES == 1 && DEINTER_FILES == 1) {
        fprintf(stderr, "Error: cannot interleave and deinterleave in one program call\n");
        return -1;
    }

    if (SECOND_INPUT_FILE_NAME != NULL && SECOND_OUTPUT_FILE_NAME != NULL) {
        fprintf(stderr, "Error: cannot have two input files and two output files\n");
        return -1;
    }

    // Requirements for input files
    if (INTER_FILES == 1 && SECOND_INPUT_FILE_NAME == NULL) {
        fprintf(stderr, "Error: must have two open files to interleave\n");
        return -1;
    }
    if (INTER_FILES == 0 && SECOND_INPUT_FILE_NAME != NULL) {
        fprintf(stderr, "Error: two input files specified, but no interleave\n");
        return -1;
    }
    if (INTER_FILES == 1 && SECOND_OUTPUT_FILE_NAME != NULL) {
        fprintf(stderr, "Error: Interleave specified but two outputs given\n");
        return -1;
    }

    // Requirements for output files
    if (DEINTER_FILES == 1 && SECOND_OUTPUT_FILE_NAME == NULL) {
        fprintf(stderr, "Error: Must have two output files to de-interleave\n");
        return -1;
    }
    if (DEINTER_FILES == 0 && SECOND_OUTPUT_FILE_NAME != NULL) {
        fprintf(stderr, "Error: Two output files specified, but no de-interleave\n");
        return -1;
    }
    if (DEINTER_FILES == 1 && SECOND_INPUT_FILE_NAME != NULL) {
        fprintf(stderr, "Error: De-interleave specified but two inputs given\n");
        return -1;
    }

    return 0;
}


/* Open output files
    Opens output file(s) for writing
*/
int bin_open_output(struct binman_struct *BM) {
    // Check early for absence of output file
    if (OUTPUT_FILE_NAME == NULL) {
        OUTPUT_FILE = stdout;
        return 0;
    }

    char *output_type = "wb";
    FILE *out, *second_out;

    if (FILE_OVERWRITE == 0) {
        // Check first file
        if (file_exists(OUTPUT_FILE_NAME) == 1) {
            fprintf(stderr, "Error: cannot overwrite %s\n", OUTPUT_FILE_NAME);
            return -1;
        }
        // Check second file (if exists)
        if (SECOND_OUTPUT_FILE_NAME != NULL) {
            if (file_exists(SECOND_OUTPUT_FILE_NAME) == 1) {
                fprintf(stderr, "Error: Cannot overwrite %s\n", SECOND_OUTPUT_FILE_NAME);
                return -1;
            }
        }
    }

    if ((out = fopen(OUTPUT_FILE_NAME, output_type)) == NULL) {
        fprintf(stderr, "Error opening output file for writing.\n");
        return -1;
    }

    OUTPUT_FILE = out;

    if (SECOND_OUTPUT_FILE_NAME != NULL) {
        if ((second_out = fopen(SECOND_OUTPUT_FILE_NAME, output_type)) == NULL) {
            fprintf(stderr, "Error opening output file for writing.\n");
            fclose(out);
            return -1;
        }
        OUTPUT_FILE2 = second_out;
    }
    return 0;
}


/* Open files for the binman struct
*/
int bin_open_input(struct binman_struct *BM) {
    FILE *in, *second_in;
    long start, end, length;
    // Get file location indicators
    start = FILE_START;
    end = FILE_END;
    length = FILE_LENGTH;


    if (end && start > end) {
        fprintf(stderr, "Error: Starting byte after ending byte\n");
        return -1;
    }

    if (length && (length != end - start)) {
        end = start + length;
    }


    if (start && !length && !end)
        length = 0;
    else
        length = end - start;


    // Open first input file
    if ((in = fopen(INPUT_FILE_NAME, "rb")) == NULL) {
        fprintf(stderr, "Error opening input file: file does not exist.\n");
        return -1;
    }

    // Copy to temporary file and store in struct
    fseek(in, start-1, SEEK_SET);
    if ((INPUT_FILE = copy_file(in, length)) == NULL) {
        fprintf(stderr, "Error copying input file to structure.\n");
        fclose(in);
        return -1;
    }
    fclose(in);

    // Optional second input file
    if (SECOND_INPUT_FILE_NAME != NULL) {
        if ((second_in = fopen(SECOND_INPUT_FILE_NAME, "rb")) == NULL) {
            fprintf(stderr, "Error opening input file: file does not exist.\n");
            return -1;
        }

        fseek(second_in, start, SEEK_SET);
        if ((INPUT_FILE2 = copy_file(second_in, length)) == NULL) {
            fprintf(stderr, "Error copying input file to structure.\n");
            fclose(second_in);
            return -1;
        }
        fclose(second_in);
    }
    return 0;
}




/* Copy a file to a temporary file

    Returns a pointer to a temporary copy of the original file.
*/

FILE *copy_file(FILE *fp, long n_bytes) {
    char *tmp_nam = tmpnam(NULL);
    char c;
    long i = 0;
    FILE *tmp;
    if ((tmp = fopen(tmp_nam, "wb+")) == NULL) {
        fprintf(stderr, "Error opening temporary file\n");
        return NULL;
    }

    rewind(tmp);

    if (n_bytes != 0) {
        while (i <= n_bytes) {
            c = fgetc(fp);
            if (feof(fp)) break;
            fputc(c, tmp);
            i++;
        }
    }
    else if (n_bytes == 0) {
        while (1) {
            c = fgetc(fp);
            if (feof(fp)) break;
            fputc(c, tmp);
            i++;
        }
    }

    rewind(tmp);
    return tmp;
}




int file_exists(char *filename) {
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL) {
        fclose(fp);
        return 0;
    }
    return 1;
}
