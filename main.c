#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse.c"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include<signal.h>


#define MAXLINE 81

void help();
void cd(char *path); //not used from here
void enqueue(char* item); //not used from here
void dequeue();
void showHistory();

typedef struct backgroundProcess
{
    parseInfo *cmdname;
    int id;

}backgroundProcess;

char* history[10]; 
int rear = -1;
int front = -1;
int max_no_history = 10;
int hist_counter = 0;

int bk_counter = 0;
backgroundProcess bk_process[10];

void handler(int sig)
{
    //printf("child been completed, signal : %d\n",sig);
    //bk_counter--;
    //printf("remaining bk processes = %d \n",bk_counter);
    printf("MrShell>");
    fflush(stdout);    
   
}

int main() 
{
    printf("MrShell> "); 
    while (1) 
    {
        
        char* cmdline = readline("");  
        
        
        if (strcmp(cmdline,"exit") == 0)  
        {
            if (bk_counter == 0)
            {
                fprintf(stderr, "---exited MrShell---\n");
                exit(1);
            }
            else
            {
                fprintf(stderr, "cannot exit MrShell, %d remaining\n",bk_counter);
            }
            
        }

        if (hist_counter == 10)
        {
            dequeue();
            enqueue(cmdline);
        } 
        enqueue(cmdline);
        hist_counter++;

        //<Parse the command args>
        parseInfo *p = parse(cmdline);
        //print_info(p);
    
        //////////////////////////////////////////////////////////////////////////////
     
    
        int fd[2];
        int read = STDIN_FILENO;
        int write = STDOUT_FILENO;

        if (p->boolBackground)
            {
               
                bk_process[bk_counter].cmdname = p;
                bk_process[bk_counter].id = 1;
                //bk_counter++;
                
            }
        
        
        signal(SIGCHLD,handler);
        int mega_fork = fork();
        if (mega_fork < 0) 
            {
                fprintf(stderr, "fork failed\n");
                exit(1);
            }
        else if (mega_fork == 0) 
        {
            //sleep(10)
            if (p->boolInfile)
                {
                    int infile_desc = open(p->inFile, O_RDONLY, 0);
                    if (infile_desc < 0)
                    {
                        printf("Infile Error");
                    }
                    read = dup2(infile_desc,STDIN_FILENO); // duplicated the file desciptor of outfile to the place of stdout
                    close(infile_desc);
                }
            else
            {
                read = STDIN_FILENO;
            }
            int i = 0;
            for(i = 0; i<p->pipeNum;++i)
            {
                int pipe_flag = pipe(fd);
                if (pipe_flag < 0)
                {
                    fprintf(stderr, "pipe failed at %d\n",i);   
                }

                write = fd[1];

                int pid = fork();       
                /* From this point on, there are two instances of the program */
                /* pid takes different values in the parent and child processes */
                if (pid < 0) 
                {
                    fprintf(stderr, "fork failed\n");
                    exit(1);
                }
                else if (pid == 0) 
                {
                    /* Execute the command in child process */
                    //printf("hello, I am child %d (pid:%d)\n",i, (int) getpid());
                    
                    
                                    
                    if (read != 0)
                    {
                        dup2(read,STDIN_FILENO);
                        close(read);
                    }
                    if (write != 1)
                    {
                        dup2(write,STDOUT_FILENO);
                        close(write);
                    }
                    
                    char *args[p->CommArray[i].VarNum + 1];
                    for(int l=0;l<p->CommArray[i].VarNum; l++)
                    {
                        args[l] = p->CommArray[i].VarList[l];
                    }
                    args[p->CommArray[i].VarNum] = NULL;
                    
                    
                    if ( (strcmp(args[0],"help") == 0) || (strcmp(args[0],"cd") == 0) )
                    {
                        execv(args[0], args); 
                    }
                    else if ((strcmp(args[0],"history") == 0 ))
                    { 
                        showHistory();
                    }
                    else
                    {
                        execvp(args[0], args); 
                    }
                    

                    //exec(...args...);
                    printf("this shouldnt print out");
                    
            
                }
                close (fd [1]);
                read = fd[0];
            }

            if (p->boolOutfile)
            {
                int outfile_desc = open(p->outFile, O_CREAT|O_WRONLY|O_TRUNC,0644);
                if (outfile_desc < 0)
                {
                    printf("outfile Error");
                }
                write = dup2(outfile_desc,STDOUT_FILENO); // duplicated the file desciptor of outfile to the place of stdout
                close(outfile_desc);
            }
            else
            {
                write = STDOUT_FILENO;
            }

            if (read != 0)
            {
                dup2(read, STDIN_FILENO);
            }

            char *args[p->CommArray[i].VarNum + 1];
            for(int l=0;l<p->CommArray[i].VarNum; l++)
            {
                args[l] = p->CommArray[i].VarList[l];
            }
            args[p->CommArray[i].VarNum] = NULL;
            
            if ( (strcmp(args[0],"help") == 0) || (strcmp(args[0],"cd") == 0) )
            {
                execv(args[0], args); 
            }
            else if ((strcmp(args[0],"history") == 0 ))
                { 
                    showHistory();
                }
            else
            {
                execvp(args[0], args); 
            } 
        //sleep(10);
        }
        else
        {
            int pid_val;
            if (p->boolBackground)
            {
                pid_val = waitpid(-1,NULL,WNOHANG);
                //printf("\n no bk \n ");
               
            }
            else
            {
                pid_val = waitpid(mega_fork,NULL,0);
                //printf("bk");
            }
            
            //printf("hello, I am parent of (pid:%d)\n", pid_val, (int) getpid());
        }    
        
    }

    return 0;
}

////////////////////// built-in commands ////////////////////////////////
void help() //not used from here
{
    printf("input format : cmd [ | cmd2 | cmd3...] [<inFile] [>outFile] [&]\n \n");
    printf("shell commands:\n");
    printf("help \n");
    printf("exit \n");
}

void cd(char *path) //not used from here
{
    int a = chdir(path);
    if (a < 0)
    {
        fprintf(stderr, "operation failed\n");
    }
}

void enqueue(char* item)
{
     
    if (rear == max_no_history - 1)
       fprintf(stderr,"queue overflow\n");
    else
    {
        if (front == - 1)
      
        front = 0;
        
        rear = rear + 1;
        history[rear] = item;
    }
} 

void dequeue()
{
    if (front == - 1 || front > rear)
    {
        fprintf(stderr,"cannot dequeue further\n");
    }
    else
    {
        front = front + 1;
    }
} 

void showHistory()
{
    
    if (front == - 1)
        printf("...no history... \n");
    else
    {
        printf("history: \n");
        for (int i = front; i <= rear; i++)
            printf("-%s \n", history[i]);
        printf("\n");
    }
} 
