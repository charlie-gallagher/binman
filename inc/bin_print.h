#ifdef BIN_PRINT


#else

#define BIN_PRINT

#ifndef DEBUG
//#define DEBUG
#endif

#define DUMP 0
#define WRITE 1

// TODO: Improve ability to find help file, esp. when PATH is set
#ifdef _MSC_VER
	#define HELP_FILENAME "doc\\windows_help.txt"
#else
	#define HELP_FILENAME "doc/windows_help.txt"
#endif

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
