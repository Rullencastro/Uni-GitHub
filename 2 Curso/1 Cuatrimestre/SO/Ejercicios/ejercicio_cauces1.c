#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h> //isdigit()
#include <limits.h> //limites como INT_MAX

int main(int argc, char *argv[])
{
    int fd[2];
    pid_t PID;
    int N = atoi(argv[1]);

    char m[10];
    sprintf(m,"-%d",N);
    
    pipe(fd);

    if((PID = fork()) <0){
        perror("fork");
        exit(-1);
    }
    
    if(PID == 0){
        close(fd[0]);
        if (dup2(fd[1],STDOUT_FILENO) <0){
            perror("Error en el write \n");
            exit(EXIT_FAILURE);
        }
        execlp("ls","ls", "-l",NULL);       
    }    
    else{
        close(fd[1]);
        int fd0[2];
        pipe(fd0);

        if((PID = fork())  == 0){
        close(fd0[0]);
        dup2(fd[0],STDIN_FILENO);
        dup2(fd0[1],STDOUT_FILENO);
        execlp("tail","tail", m ,NULL);

        perror("Error en fork");
        exit(-1);
        }
        
        else
        {
        
            close(fd0[1]);
            for(int i = 0;i < N;i++){
                char buff[15]="";
                char aux[2] = "";
                read(fd0[0],&buff,10);
                while(aux[0] != '\n'){
                    read(fd0[0],&aux,1);
                }
                write(STDOUT_FILENO,&buff,10);
                printf("\n");
            }
        }
        
    }
  
    return(0);
}

