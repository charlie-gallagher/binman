/*
 * Utility to convert raw text to a string with the preprocessor. Used
 * currently to convert the install file path to a string.
 */

#ifndef BIN_STRINGIZE
#define BIN_STRINGIZE

#define STRINGIZE( x ) #x
#define STRINGIZE_MACRO( x ) STRINGIZE(x)


#endif
