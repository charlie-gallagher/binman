#include <stdio.h>
#include <bin_print.h>
#include <bin_ops.h>
#include <bin_arg_parse.h>
#include <bin_flow.h>


int run_binman(struct binman_struct *BM) {
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
                if (INPUT_FILE2 != NULL)
                    invert_bits(INPUT_FILE2);
                break;
            }
            case F_BIT: {
                #ifdef DEBUG
                printf("Flipping bits...\n");
                #endif
                flip_bits(INPUT_FILE, WORD_SIZE);
                if (INPUT_FILE2 != NULL)
                    flip_bits(INPUT_FILE2, WORD_SIZE);
                break;
            }
            case F_BYTE: {
                #ifdef DEBUG
                printf("Flipping bytes...\n");
                #endif
                flip_bytes(INPUT_FILE, WORD_SIZE);
                if (INPUT_FILE2 != NULL)
                    flip_bytes(INPUT_FILE2, WORD_SIZE);
                break;
            }
            case LIST: {
                #ifdef DEBUG
                printf("LIST\n----\n");
                #endif
                bin_list(INPUT_FILE);
                if (INPUT_FILE2 != NULL) {
                    printf("File 2: \n");
                    bin_list(INPUT_FILE2);
                }
                break;
            }
            case 0:
            default: break;
        }
        i++;
    }

    if (OUTPUT_TYPE == WRITE && OUTPUT_FILE_NAME == NULL) {
        fprintf(stderr, "\nNOTE: Neither dump nor output file specified. No output produced.\n");
    }
    else if (INPUT_FILE2 != NULL) {
        #ifdef DEBUG
        printf("Interleave starting...\n");
        #endif
        interleave_out(INPUT_FILE, INPUT_FILE2, OUTPUT_FILE, WORD_SIZE, OUTPUT_TYPE);
    }
    else if (OUTPUT_FILE2 != NULL) {
        #ifdef DEBUG
        printf("Deinterleave starting...\n");
        printf("Output file 1: %s\n", OUTPUT_FILE_NAME);
        printf("Output file 2: %s\n", SECOND_OUTPUT_FILE_NAME);
        #endif
        deinterleave_out(INPUT_FILE, OUTPUT_FILE, OUTPUT_FILE2, WORD_SIZE, OUTPUT_TYPE);
    }
    else {
        bin_out(INPUT_FILE, OUTPUT_FILE, OUTPUT_TYPE);
    }


    // Cleaning up
    // It turns out `fcloseall()` is a GNU extension, so I have to find
    // a better way to clean up these files
    // Actually, there's no man-page entry but `fcloseall` is documented
    // in `fclose()`. So I'm not sure what's going on there.
    fclose(INPUT_FILE);
    #ifdef DEBUG
    printf("Removing %s\n", INPUT_FILE_TMP_NAME);
    #endif
    remove(INPUT_FILE_TMP_NAME);


    if (INPUT_FILE2_TMP_NAME != NULL) {
        #ifdef DEBUG
        printf("Removing %s\n", INPUT_FILE2_TMP_NAME);
        #endif
	fclose(INPUT_FILE);
        remove(INPUT_FILE2_TMP_NAME);
    }
    return 0;
}
