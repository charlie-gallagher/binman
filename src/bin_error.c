#include <stdio.h>
#include <bin_error.h>


void print_errors(void) {
    extern int bin_errno;
    switch (bin_errno) {
        case CANNOT_OPEN_TMP: {
            fprintf(stderr, "\nError: Cannot open temporary file\n");
            break;
        }
        case CANNOT_OPEN: {
            fprintf(stderr, "\nError: Cannot open file\n");
            break;
        }
        case CANNOT_OVERWRITE: {
            fprintf(stderr, "\nError: Cannot overwrite file\n");
            break;
        }
        case FAILED_TO_COPY: {
            fprintf(stderr, "\nError: Failed to copy file to internal structure\n");
            break;
        }
        case FILENOTEXIST: {
            fprintf(stderr, "\nError: File does not exist\n");
            break;
        }
        case ILLEGAL_ARGUMENT: {
            fprintf(stderr, "\nError: Illegal argument\n");
            break;
        }
        case INVALID_FLAG: {
            fprintf(stderr, "\nError: Invalid flag\n");
            break;
        }
        case INVALID_PARAM: {
            fprintf(stderr, "\nError: Invalid parameter\n");
            break;
        }
        case INVALID_WORD: {
            fprintf(stderr, "\nError: Invalid word size\n");
            break;
        }
        case INTERLEAVE_AND_DEINTERLEAVE: {
            fprintf(stderr, "\nError: Cannot interleave and de-interleave in one operation\n");
            break;
        }
        case TWOIN_TWOOUT: {
            fprintf(stderr, "\nError: Cannot have two inputs and two outputs\n");
            break;
        }
        case TOO_MANY_INPUT: {
            fprintf(stderr, "\nError: Too many input files\n");
            break;
        }
        case TOO_FEW_INPUT: {
            fprintf(stderr, "\nError: Too few input files\n");
            break;
        }
        case TOO_MANY_OUTPUT: {
            fprintf(stderr, "\nError: Too many output files\n");
            break;
        }
        case TOO_FEW_OUTPUT: {
            fprintf(stderr, "\nError: Too few output files\n");
            break;
        }
        case START_BYTE_AFTER_END: {
            fprintf(stderr, "\nError: Start byte after end byte\n");
            break;
        }
        case INVALID_OUT_TYPE: {
            fprintf(stderr, "\nError: Invalid output type\n");
            break;
        }
        case WRITE_ERROR: {
            fprintf(stderr, "\nError: Failed to write to file\n");
            break;
        }
        case HELP_FILE_ERROR: {
            fprintf(stderr, "\nError: Error opening help file\n");
            break;
        }
        case 0: {
            break;
        }
        default: {
            fprintf(stderr, "\nError: unrecognized error code\n");
            break;
        }
    }
}
