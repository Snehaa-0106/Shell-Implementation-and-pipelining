Name - Snehaa Sivakumar

________________________________________________________

To Run :
________

$ make shell
$ make clean

___________________________________________________________

I have also written command to clean the file inside the make file.

My program will perform the following:

-Executing a single command without arguments
-Executing a single command with arguments
- Input redirection from file: Executing a single command that takes standard input (stdin) from a file
-Output redirection to file: Executing a single command that sends standard output (stdout) to a file
-Filters: Execute a filter chain with two commands
-Filters: Execute a filter chain with more than two commands
-Combination: Executing a filter chain while redirecting the input of first command from a file and/or output of last command to a file
__________________________________________________________

The following commands I tested:
ls
Ls -l
ls -a
ls > out
Ls < out
Cat out
Pwd
ls -a | sort -r
Ls > out | sort -r