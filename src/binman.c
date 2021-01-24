#include <stdio.h>
#include "header\\bin_arg_parse.h"
#include "header\\bin_flow.h"

//#define DEBUG



int main(int argc, char *argv[])
{
    #ifdef DEBUG
    printf("\n*************\nDEBUG MODE\n*************\n\n");
    #endif

    /* If help asked for, don't run program, just help */
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

    /* Process arguments */
    if (process_flags_and_params(bm) != 0) {
        fprintf(stderr, "Error processing flags and parameters.\n");
        exit(-1);
    }

    /* Run program */
    if (run_binman(bm) != 0) {
        return -1;
    }

    return 0;
}
