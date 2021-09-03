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

    pipe(fd);

    if((PID = fork()) <0){
        perror("Error en fork");
        exit(-1);
    }
    
    if(PID == 0){
        close(fd[0]);
        if (dup2(fd[1],STDOUT_FILENO) <0){
            perror("Error en el write \n");
            exit(EXIT_FAILURE);
        }
        dup2(fd[1],STDOUT_FILENO);
        execlp("ls","ls", "-l",NULL);       
    }    
    else{
        int fd0[2];
        pipe(fd0);

        if((PID = fork()) <0){
        perror("Error en fork");
        exit(-1);
        }

        if(PID)
        {
        close(fd[1]);
        dup2(fd[0],STDIN_FILENO);
        close(fd0[0]);
        dup2(fd0[1],STDOUT_FILENO);
        execlp("wc","wc", "-l",NULL);

        perror("Error en wc");
        exit(-1);
        }
        else
        {
            close(fd[1]);
            close(fd0[1]);
            const int TAM = 100;
            char buffer;

            if (read(fd0[0], &buffer, TAM) < 0){
				perror("Error en read");
				exit(EXIT_FAILURE);
			}

			int wc = strtol(&buffer, 0, 10);
            if(wc%2 == 0)
            printf("Hay %d archivos y es un numero par\n", wc);
            else
            printf("Hay %d archivos y es un numero impar\n", wc);
        }
        
    }


        
   
    return(0);
}

