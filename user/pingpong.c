#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[]){
    int pid, p[2];
    char oneByte;

    if (argc != 1){
        fprintf(2, "Error: Too many arguments!\n");
        exit(1);
    }

    
    pipe(p);
    pid = fork();

    if (pid == 0){      // is child
        close(p[0]);    // close write

        oneByte = ' ';
        if (write(p[1],&oneByte,1))
            fprintf(2,"%d: received ping\n",getpid());

        close(p[1]);
    }
    else if(pid > 0){   // is parent
        wait((int *)0);
        close(p[1]);    // close read

        if (read(p[0],&oneByte,1))
            fprintf(2,"%d: received pong\n",getpid());

        close(p[0]);    
    }
    else{
        fprintf(2,"read error\n");
    }

    exit(0);
}