# Binary Manipulation <img src="doc\logo.png" align="right" height=120 />



&nbsp;


## Overview
The `binman` program inputs a file and performs various manipulations at the byte and word level. Word sizes may range from 8 to 64 bits. In addition to single-file operations, multi-file interleave and de-interleave operations are also supported. These allow one to input two files and combine them, turning one file into the odd words and the other into the even words of the final file.

I'm writing a manual for development. See `doc\man.md`.

### Compilation
The program was written for Windows, but efforts will soon be aiming at making it compatible with Linux. See "Upcoming Changes."

There are two makefiles, `win_makefile` and `unix_makefile`. The Windows makefile uses `NMAKE` and `CL` from the VS C/C++ build tools.

```
nmake /f win_makefile
```

For the `gcc` compiler, use `make` and the Unix makefile:

```
make -f unix_makefile
```

## Upcoming Changes
The following changes are immediately in progress:

- Expand "Debug Mode" using conditional preprocessor directives.
- Improve build tools


### To-do
These changes are planned, but not started.

- Add POSIX-style command-line interface

### Bug and hotfix notes
Nothing to note.

### Recent changes
- Replaced command file with makefile for `NMAKE`
- Added command file for `cl.exe`
- Reogranized includes, added makefile, and made Linux-safe (sjgallagher2)
- Remade the error system to give descriptive, clean errors.
- Fixed a bug with de-interleaving, in which two temporary files shared the same name.
- Added support for all word sizes from 1 to 8 bytes.
- Added warnings for when file size is not a multiple of word size.


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

Usage:  binman [/i] [/f] [/F] [/l] [/W] [/d] [/b [offset]] [/e [ending]]
    [/L [length]] [/w [word]] [/o [file1] [file2]] file1 [file2]

    /b <offset>    Set offset (beginning)
    /d    Dump file bytes as ASCII hexadecimal characters. If /o or /O is
          given, output to file; otherwise, print to stdout. Both write as
          ASCII hex characters.
    /D    De-interleave words; requires two output files.
    /e <ending>    Set final byte included (ending); see also /L
    /F    Flip bytes in each word left-to-right
    /f    Flip bits in each word left-to-right
    /h    Display this help and exit
    /i    Invert all bits
    /l    List nearest 20 lines (may be done anywhere in operation sequence)
    /L    Set max length from offset; see also /e
    /o <filename> [<filename2>]    Save result to filename, overwriting if file
          exists. Second filename only valid when de-interleaving.
    /O <filename> [<filename2>]    Save result to filename, do not overwrite if
          file exists. Second filename only valid when de-interleaving.
    /w <bytes>    Set number of bytes in a word (from 1 to 8).
    /W    Interleave words from file1 and file2; requires two input files.


Further Information
-------------------
Flags and parameters are case-sensitive. Operations may be combined, but each
operation may be executed only once. Order of operations is the order in which
the arguments are given. You cannot interleave and de-interleave in a single
operation.

The final argument is assumed to be a filename unless `/?` or `/h` is given as
only argument. By default, the output is not sent anywhere unless `/d` (dump)
is given, in which case the ASCII hex values of the file are sent to stdout. If
`/d` and an output file are given, the ASCII hex is written to the output as
text.

`/o` and `/O` are greedy, so if you specify, for example, `binman /o f1 f2 f3`,
you will get two output files and one input file, rather than one output file
and two input files. Because of this, it's good practice to specify the output
file(s) first. However, `binman /o f1 f2` works as expected. See above paragraph.

If both `/e` and `/L` are given, `/L` wins.


Report any bugs to charlesjgallagher15@gmail.com or submit as an issue on
GitHub. Contributions welcome.
```


---
Charlie Gallagher, 2021
