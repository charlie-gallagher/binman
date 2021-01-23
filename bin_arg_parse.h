#ifdef CMDLINE
// Do nothing
#else
#define CMDLINE

/* Symbolic constants for the ordered_flags element */
#define I_BIT 1
#define F_BIT 2
#define F_BYTE 3
#define LIST 4

#define DUMP 0
#define WRITE 1


/* Argument storage structure */
// TODO: Convert indicators to bits

struct binman_struct {
    char ordered_flags[4];   // invert, flip bits/bytes
    long access_locs[2];     // offset, ending
    long file_length;       // Number of bytes in (sub)file
    char overwrite;         // Overwrite indicator
    int word_length;
    char output_type;       // DUMP or WRITE
    char interleave;
    char deinterleave;
    char display_help;
    char *input_file_name;
    char *second_input_file_name;
    FILE *input_copy;       // Must not be NULL
    FILE *second_input_copy;     // Second input if -W, else NULL
    char *output_file_name;
    char *second_output_file_name;
    FILE *output_file;      // NULL if none
    FILE *second_output_file;     // Second output for -o, else NULL
};


/* Structure access for bit operation function arguments */
/*
    Using these macros, I can change the names of the structure
    parts without having to change any of the code. For now, all
    code uses BM as the name of the struct instance.
*/

// Name of struct
#define BM bm

// Parts of struct (Assumes BM is a pointer to the struct)
#define OFLAGS(x) BM->ordered_flags[x]
#define FILE_START BM->access_locs[0]
#define FILE_END BM->access_locs[1]
#define FILE_LENGTH BM->file_length
#define FILE_OVERWRITE BM->overwrite
#define WORD_SIZE BM->word_length
#define OUTPUT_TYPE BM->output_type
#define INTER_FILES BM->interleave
#define DEINTER_FILES BM->deinterleave
#define DISPLAY_HELP BM->display_help
#define INPUT_FILE_NAME BM->input_file_name
#define SECOND_INPUT_FILE_NAME BM->second_input_file_name
#define INPUT_FILE BM->input_copy
#define INPUT_FILE2 BM->second_input_copy
#define OUTPUT_FILE_NAME BM->output_file_name
#define SECOND_OUTPUT_FILE_NAME BM->second_output_file_name
#define OUTPUT_FILE BM->output_file
#define OUTPUT_FILE2 BM->second_output_file


/* Functions */
struct binman_struct *make_test_struct(char *in_name, char *other_in_name, char *tmp_name, char *other_tmp_name, char *out_name);

/* Main Parser functions */
struct binman_struct *bin_arg_parse(int argc, char *argv[]);
void bin_init_struct(struct binman_struct *BM);
int process_flags_and_params(struct binman_struct *BM);
int parse_flags(struct binman_struct *BM, char *flags[]);
int parse_params(struct binman_struct *BM, char *params[10][3]);
int is_bin_flagform(char *item);
int is_bin_flag(int argc, char *argv[], int index);
int is_bin_param(int argc, char *argv[], int index);
int is_bin_outputs(int argc, char *argv[], int index);
int is_bin_terminal_input(int argc, char *argv[], int index);
int is_duplicate_flag(char already_appeared[], char c, char index);
int bin_open_input(struct binman_struct *BM);
int bin_open_output(struct binman_struct *BM);
FILE *copy_file(FILE *fp, long n_bytes);
int is_executable(struct binman_struct *BM);
int file_exists(char *filename);




#endif
