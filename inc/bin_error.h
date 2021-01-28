#ifdef BIN_ERROR

#else

#define BIN_ERROR
#define PRINT_ERRORS

/* Error macros */
#define error_msg(msg) fprintf(stderr, "%s (%d): %s\n", __FILE__, __LINE__, (msg))
#define error_msg_arg(msg, p) fprintf(stderr, "%s (%d): %s [%c]\n", __FILE__, __LINE__, (msg), (p))


int bin_errno;
void print_errors(void);

/* Error codes */
// files
#define CANNOT_OPEN_TMP 996
#define CANNOT_OPEN 997
#define CANNOT_OVERWRITE 998
#define FAILED_TO_COPY 999
#define FILENOTEXIST 1000

// CLI
#define ILLEGAL_ARGUMENT 1001
#define INVALID_FLAG 1002
#define INVALID_PARAM 1003

// executability
#define INVALID_WORD 1010
#define INTERLEAVE_AND_DEINTERLEAVE 1011
#define TWOIN_TWOOUT 1012
#define TOO_FEW_INPUT 1013
#define TOO_MANY_INPUT 1014
#define TOO_MANY_OUTPUT 1015
#define TOO_FEW_OUTPUT 1016

#define START_BYTE_AFTER_END 1017


// printing
#define INVALID_OUT_TYPE 1020
#define WRITE_ERROR 1021
#define HELP_FILE_ERROR 1022

// operations -- no new types of errors

#endif
