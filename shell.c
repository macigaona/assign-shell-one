#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define BSIZE 20

/***********************************************************************
 * This starter shell implements a simple shell that implements:
 * 
 * (1) An internal EXIT command that exits the shell.
 * (2) Executes a external program without any command line arguments
 * 
 ***********************************************************************/
int main(int argc, char *argv[]){
    char buffer[BSIZE];
    pid_t child;
    int status;
    char *args[BSIZE];

    int i;
    char *token;
    signal(SIGINT, SIG_IGN);
    while(1){
        printf("shell> ");
        fgets(buffer,BSIZE-1,stdin);
        buffer[(strlen(buffer)-1)]='\0';  //Remove NewLine

        token = strtok(buffer, " ");
        i = 0;
        while(token != NULL && i < (BSIZE - 1)){
            args[i] = token;
            i++;
            token = strtok(NULL, " ");
        }    
        args[i] = NULL;
        
        if(args[0] == NULL){
            continue;
        }

        //** Internal Command **
        if(strcmp(args[0],"exit")==0){
            break;
        }
        
        if(strcmp(args[0], "cd")==0){
            if((args[1] == NULL)){
                chdir(getenv("HOME"));
            }else if(chdir(args[1])==-1){
                perror("cd faileld");
            }
            continue;
        }

        //** External Command **
        child = fork();
        if (child==-1){
            perror("fork failed");
            continue;
        }
        if(child==0){
            //*** Child Process ***
            signal(SIGINT, SIG_DFL);
            execvp(args[0],args);
            perror("execvp failed");
            exit(1);
        }else{
            //*** Parent Process **
            waitpid(child,&status,0);
        }
    }
    return 0;
}