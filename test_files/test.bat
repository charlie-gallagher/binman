cl src\binman.c src\bin_arg_parse.c src\bin_print.c src\bin_ops.c src\bin_flow.c src\bin_error.c

binman /o test_files\p1.txt test_files\p2.txt /D /d test_files\num_file.txt
binman /d /b 10 /e 5 test_files\num_file.txt
