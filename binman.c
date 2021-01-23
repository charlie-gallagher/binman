#include <stdio.h>
#include "bin_print.h"
#include "bin_ops.h"
#include "bin_arg_parse.h"

#define DEBUG



int main(int argc, char *argv[])
{
    #ifdef DEBUG
    printf("\n*************\nDEBUG MODE\n*************\n\n");
    #endif

    /* Check for early exit formats */
    if (argc == 1) {
        print_help();
        return 0;
    }
    if (argc == 2 && (argv[1][1] == 'h' || argv[1][1] == '?')) {
        print_help();
        return 0;
    }

    /* Initialize structure */
    struct binman_struct *bm;
    bm = bin_arg_parse(argc, argv);

    if (process_flags_and_params(bm) != 0) {
        fprintf(stderr, "Error processing flags and parameters.\n");
        exit(-1);
    }

    printf("\n***********************************\n");
    printf("Binary Manipulation Program\n");
    printf("***********************************\n");
    int i = 0;

    if (DISPLAY_HELP == 1) {
        print_help();
        return 0;
    }

    while (i < 4)
    {
        switch(OFLAGS(i)) {
            case I_BIT: {
                printf("Inverting bits...\n");
                invert_bits(INPUT_FILE);
                break;
            }
            case F_BIT: {
                printf("Flipping bits...\n");
                flip_bits(INPUT_FILE, WORD_SIZE);
                break;
            }
            case F_BYTE: {
                printf("Flipping bytes...\n");
                flip_bytes(INPUT_FILE, WORD_SIZE);
                break;
            }
            case LIST: {
                printf("LIST\n----\n");
                bin_list(INPUT_FILE);
                break;
            }
            case 0:
            default: break;
        }
        i++;
    }

    if (OUTPUT_TYPE == WRITE && OUTPUT_FILE_NAME == NULL) {
        fprintf(stderr, "\nMessage: Neither dump nor output file specified. No output produced.\n");
        fcloseall();
        return 0;
    }
    else if (INPUT_FILE2 != NULL) {
        printf("Interleave starting...\n");
        interleave_out(INPUT_FILE, INPUT_FILE2, OUTPUT_FILE, WORD_SIZE, OUTPUT_TYPE);
    }
    else if (OUTPUT_FILE2 != NULL) {
        deinterleave_out(INPUT_FILE, OUTPUT_FILE, OUTPUT_FILE2, WORD_SIZE, OUTPUT_TYPE);
    }
    else {
        bin_out(INPUT_FILE, OUTPUT_FILE, OUTPUT_TYPE);
    }



    // Cleaning up
    fcloseall();

    return 0;
}
