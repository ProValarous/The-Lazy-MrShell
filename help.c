#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    fprintf(stdout,"input format:\n -cmd [ | cmd2 | cmd3...] [<inFile] [>outFile] [&]\n \n");
    fprintf(stdout,"shell built-in commands:\n \n");
   
    fprintf(stdout,"-jobs : provides a list of all background processes and their local pIDs [work in pogress]\n");
    fprintf(stdout,"-cd <PATHNAME> : sets the PATHNAME as working directory.\n");
    fprintf(stdout,"-history : prints a list of previously executed commands. Assume 10 as the maximum history\n");
    fprintf(stdout,"-kill PID : terminates the background process identified locally with PID in the jobs list [work in pogress]\n");
    fprintf(stdout,"-!CMD : runs the command numbered CMD in the command history [work in pogress]\n");
    fprintf(stdout,"-exit : terminates the shell only if there are no background jobs.\n");
    fprintf(stdout,"-help : prints the list of builtin commands along with their description.\n");
    return 0;
}