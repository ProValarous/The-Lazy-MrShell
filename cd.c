#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char *argv[])
{
    //char *path = readline("");
    //printf("%s",*path);
    //scanf(&path);
    int flag = chdir(argv[1]);

    if (flag < 0)
    {
        fprintf(stderr, "operation failed\n");
    }
    return 0;
}