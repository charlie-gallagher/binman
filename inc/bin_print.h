#ifndef BIN_PRINT

/* Contains a macro to convert macro text to string */
#include <stringize.h>

#define BIN_PRINT

#ifndef DEBUG
//#define DEBUG
#endif

#define DUMP 0
#define WRITE 1




#if defined INSTALL_ROOT
#if defined _MSC_VER
	 #define HELP_FILENAME STRINGIZE_MACRO(INSTALL_ROOT) "\\share\\binman\\help.txt"
#else
	 #define HELP_FILENAME STRINGIZE_MACRO(INSTALL_ROOT) "/share/binman/help.txt"
#endif /* defined _MSC_VER */
#elif defined _MSC_VER
	#define HELP_FILENAME "doc\\windows_help.txt"
#else
	#define HELP_FILENAME "./doc/unix_help.txt"
#endif /* INSTALL_ROOT */


/* Print functions */
int bin_out(FILE *input, FILE *output, char type); // Chooses program flow
int interleave_out(FILE *fp1, FILE *fp2, FILE *fp3, int word, char type);
int deinterleave_out(FILE *fp1, FILE *fp2, FILE *fp3, int word, char type);
int bin_write(FILE *fp, FILE *out);  // Copies fp to out
int bin_dump(FILE *fp); // Dumps whole file
int bin_list(FILE *fp); // Dumps first 20 lines at 16 bytes per line

int print_help(void);

/* Conversion functions */
int byte_to_hex(FILE *fp);


#endif
