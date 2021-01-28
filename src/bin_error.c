#include <stdio.h>
#include "header\\bin_error.h"


void print_errors(void) {
    extern int bin_errno;
    switch (bin_errno) {
        case CANNOT_OPEN_TMP: {
            fprintf(stderr, "\nCannot open temporary file.\n");
            break;
        }
        case CANNOT_OPEN: {
            fprintf(stderr, "\nCannot open file.\n");
            break;
        }
        case CANNOT_OVERWRITE: {
            fprintf(stderr, "\nCannot overwrite file.\n");
            break;
        }
        case FAILED_TO_COPY: {
            fprintf(stderr, "\nFailed to copy file to internal structure.\n");
            break;
        }
        case FILENOTEXIST: {
            fprintf(stderr, "\nFile does not exist.\n");
            break;
        }
        case ILLEGAL_ARGUMENT: {
            fprintf(stderr, "\nIllegal argument.\n");
            break;
        }
        case INVALID_FLAG: {
            fprintf(stderr, "\nInvalid flag.\n");
            break;
        }
        case INVALID_PARAM: {
            fprintf(stderr, "\nInvalid parameter.\n");
            break;
        }
        case INVALID_WORD: {
            fprintf(stderr, "\nInvalid word size.\n");
            break;
        }
        case INTERLEAVE_AND_DEINTERLEAVE: {
            fprintf(stderr, "\nCannot interleave and de-interleave in one operation.\n");
            break;
        }
        case TWOIN_TWOOUT: {
            fprintf(stderr, "\nCannot have two inputs and two outputs.\n");
            break;
        }
        case TOO_MANY_INPUT: {
            fprintf(stderr, "\nToo many input files.\n");
            break;
        }
        case TOO_FEW_INPUT: {
            fprintf(stderr, "\nToo few input files.\n");
            break;
        }
        case TOO_MANY_OUTPUT: {
            fprintf(stderr, "\nToo many output files.\n");
            break;
        }
        case TOO_FEW_OUTPUT: {
            fprintf(stderr, "\nToo few output files.\n");
            break;
        }
        case START_BYTE_AFTER_END: {
            fprintf(stderr, "\nStart byte after end byte.\n");
            break;
        }
        case INVALID_OUT_TYPE: {
            fprintf(stderr, "\nInvalid output type.\n");
            break;
        }
        case WRITE_ERROR: {
            fprintf(stderr, "\nFailed to write to file.\n");
            break;
        }
        case HELP_FILE_ERROR: {
            fprintf(stderr, "\nError opening help file.\n");
            break;
        }
        case 0: {
            fprintf(stderr, "No errors.\n");
            break;
        }
        default: {
            fprintf(stderr, "Unknown error code.\n");
            break;
        }
    }
}
