#ifdef BIN_ERROR

#else

#define BIN_ERROR

/* Error macros */
#define error_msg(msg) fprintf(stderr, "%s (%s): %s\n", __FILE__, __LINE__, msg)

int bin_errno;

/* Error codes */
#define NOTEXIST 1000
#define SYNTAX 1001
#define EXECUTABLE 1002
