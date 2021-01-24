# Binary Manipulation <img src="doc\logo.png" align="right" height=120 />



&nbsp;


## Overview
The `binman` program inputs a file and performs various manipulations at the byte and word level. Currently, only 16- and 32-bit word-sizes are valid. See 'Upcoming Changes'. In addition to single-file operations, multi-file interleave and de-interleave operations are also supported. These allow one to input two files and combine them, turning one file into the odd words and the other into the even words of the final file.

This is a sort of capstone project for me finishing a book on programming in C. I am not a computer scientist; my background is in economics and statistical computing. `binman` should not be considered a model of (or held to the standards of) good programming.

I'm writing a manual for development. See `doc\man.md`.

### Compilation
The program was written for Windows, but efforts will soon be aiming at making it compatible with Linux. See "Upcoming Changes." I compile using the Microsoft VS C/C++ Compiler with the following command.

```raw
cl src\binman.c src\bin_flow.c src\bin_arg_parse.c src\bin_ops.c src\bin_print.c
```


## Upcoming Changes
The following changes are immediately in progress:

- Expand "Debug Mode" using conditional preprocessor directives.
- Improve behavior when number of bytes in file is not a mutliple of the word size.
- Increase potential word sizes to include 8-bit and 64-bit, in addition to the current 16-bit and 32-bit versions. I will need to rewrite the bit and byte flipping functions, at a minimum. Interleaving already has the infrastructure to work, I just have to write conditions for the other two word sizes.
- Port to Linux. This will involve only a little work on the program proper, but a lot of work on the command-line argument parser.

### Bug and hotfix notes
TODO:

- Print a message when file size is not a multiple of the word size.
- Print a message when (de)interleaving files of two different sizes.

Both of these require a function to tell me the size of a file. 

&nbsp;

---

## Usage

```raw
C:\> binman /h                                                      [1]

Usage:  binman [\i] [\f] [\F] [\l] [\W] [\d] [\b [offset]] [\e [ending]]
    [\L [length]] [\w [word]] [\o [file1] [file2]] file1 [file2]
...
--abbreviated--


C:\> binman /l test_files\num_file.txt                              [2]

01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10
11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e


C:\> binman /F /l test_files\num_file.txt                           [3]

04 03 02 01 08 07 06 05 0c 0b 0a 09 10 0f 0e 0d
14 13 12 11 18 17 16 15 1c 1b 1a 19 1d 1e

C:\> binman /d /W test_files\num_file.txt test_files\num_file.txt   [4]

01 02 03 04 01 02 03 04 05 06 07 08 05 06 07 08
09 0a 0b 0c 09 0a 0b 0c 0d 0e 0f 10 0d 0e 0f 10
11 12 13 14 11 12 13 14 15 16 17 18 15 16 17 18
19 1a 1b 1c 19 1a 1b 1c
```

1. Print help
2. List first 320 bytes (16 per row, 20 rows) of file
3. Flip bytes in each word (defaults to 32-bit word)
4. Interleave words from the numeric test file with itself, dumping output to terminal

## Help file
```raw
Utilities for manipulating bits, bytes, and words in a file.

Usage:  binman [\i] [\f] [\F] [\l] [\W] [\d] [\b [offset]] [\e [ending]]
    [\L [length]] [\w [word]] [\o [file1] [file2]] file1 [file2]

    \b <offset>    Set offset (beginning)
    \d    Dump file bytes. If \o or \O is given, output to file; otherwise,
          print to stdout. Both write as ASCII hex characters.
    \D    De-interleave words; requires two output files.
    \e <ending>    Set final byte included (ending); see also \L
    \F    Flip bytes in each word left-to-right
    \f    Flip bits in each word left-to-right
    \h    Display this help and exit
    \i    Invert all bits
    \l    List nearest 20 lines (may be done anywhere in operation sequence)
    \L    Set max length from offset; see also \e
    \o <filename> [<filename2>]    Save result to filename, overwriting if file
          exists. Second filename only valid when de-interleaving.
    \O <filename> [<filename2>]    Save result to filename, do not overwrite if
          file exists. Second filename only valid when de-interleaving.
    \w <bytes>    Set number of bytes in a word
    \W    Interlace (weave) words from file1 and file2


Further Information
-------------------
Flags and parameters are case-sensitive. Operations may be combined, but each
operation may be executed only once. Order of operations is the order in which
the arguments are given. You cannot interleave and de-interleave in a single
operation.

The final argument is assumed to be a filename unless `/?` or `/h` is given as
only argument. By default, the output is not sent anywhere unless `/d` (dump)
is given, in which case the ASII hex values of the file are sent to stdout. If
`/d` and an output file are given, the ASCII hex is written to the output as
text.

`/o` and `/O` are greedy, so if you specify, for example, `binman \o f1 f2 f3`,
you will get two output files and one input file, rather than one output file
and two input files. Because of this, it's good practice to specify the output
file(s) first.

If both `/e` and `/L` are given, `/L` wins.


Report any bugs to charlesjgallagher15@gmail.com or submit as an issue on
GitHub. Contributions welcome.
```
