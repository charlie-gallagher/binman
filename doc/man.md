# Overview
`binman` inputs a file and performs various manipulations at the byte and word level. This file will explain the design of the operations and the design of the parser.

## Flow of Information
As a typical use-case, consider the following program call:

```raw
binman /o output_file.exe /i /b 10 /e 20 input_file.exe
```

This inverts bits 10 through 20 (inclusive) and stores the result in `output_file.exe`. How does this go through the program? First, the command-line arguments are parsed, and the resulting values are stored in a structure. The structure has fields for all possible inputs. Second, the arguments are processed (i.e. checked for inconsistencies), and the output file `output_file.exe` and a copy of the input file are stored in the structure. The copy of the input file only contains 11 bytes (byte 10 through byte 20).

Third, the various parts of the structure get passed around from operation to operation until there are no more operations to do. Here, that means the bytes in the temporary input file are inverted. Finally, the temporary input file is copied to `output_file.exe` and the program terminates.

By distributing file information this way, the operations become simpler. They do not have to know how many bytes to change, nor do they have to handle the different ways to specify which bytes to act on in the file. They input a file and they manipulate the whole file.


## Order of Operations
Operations are generally performed in the order they are written. The strictly ordered operations are:

- List
- Invert bits
- Reverse bits
- Reverse bytes

The (de)interleaving operations are always performed at the end. This is because they have special output routines. If interleaving, operations are done to both input files before they are combined. If de-interleaving, operations are done to original file before splitting. This order is a little arbitrary, as the outcome is the same regardless of when you (de)interleave. 


## Error System
I recently implemented a basic error system that gives a traceback and an error
message. Error messages are set by the lowest level functions and printed at
the end of program execution using `atexit()`. The traceback is done using a 
macro `error_msg()` defined in `bin_error.h`. Any time an error is thrown at
any level, there is a call to `error_msg()`, where the argument is the function
name in which the error occurred. As the errors cascade up through the program
flow, the programmer is shown filenames, line numbers, and function names in
which the errors occurred. 

In the place where the error is thrown at the lowest level, the global variable
`bin_errno` is set with a constant defined in `bin_error.h`. These constants
are error codes corresponding to the various error messages needed right now. 
There are no warning codes. 

And while I'm on the subject of warnings, there is no warning system. Warnings
are thrown at the moment they are needed. Reflecting on this, I should consider
adding warnings to the final message list. This is tough because I like desc-
riptive warnings like "Final [2] bytes dropped." I would need a different macro
for each possible case, and that's not convenient. Maybe better for now to do 
nothing. 
