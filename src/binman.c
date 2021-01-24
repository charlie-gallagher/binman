#include <stdio.h>
#include "header\\bin_print.h"
#include "header\\bin_ops.h"
#include "header\\bin_arg_parse.h"

//#define DEBUG



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

    #ifdef DEBUG
    printf("\n***********************************\n");
    printf("Binary Manipulation Program\n");
    printf("***********************************\n");
    #endif
    int i = 0;

    if (DISPLAY_HELP == 1) {
        print_help();
        return 0;
    }

    while (i < 4)
    {
        switch(OFLAGS(i)) {
            case I_BIT: {
                #ifdef DEBUG
                printf("Inverting bits...\n");
                #endif
                invert_bits(INPUT_FILE);
                break;
            }
            case F_BIT: {
                #ifdef DEBUG
                printf("Flipping bits...\n");
                #endif
                flip_bits(INPUT_FILE, WORD_SIZE);
                break;
            }
            case F_BYTE: {
                #ifdef DEBUG
                printf("Flipping bytes...\n");
                #endif
                flip_bytes(INPUT_FILE, WORD_SIZE);
                break;
            }
            case LIST: {
                #ifdef DEBUG
                printf("LIST\n----\n");
                #endif
                bin_list(INPUT_FILE);
                break;
            }
            case 0:
            default: break;
        }
        i++;
    }

    if (OUTPUT_TYPE == WRITE && OUTPUT_FILE_NAME == NULL) {
        fprintf(stderr, "\nNOTE: Neither dump nor output file specified. No output produced.\n");
        fcloseall();
        return 0;
    }
    else if (INPUT_FILE2 != NULL) {
        #ifdef DEBUG
        printf("Interleave starting...\n");
        #endif
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
