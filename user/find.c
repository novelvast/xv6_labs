#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }


    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("ls: path too long\n");
        exit(1);
    }


    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){     // read a dirent
        if(de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }

        switch(st.type){
        case T_FILE:
            if (strcmp(de.name, filename) == 0)
                printf("%s\n", buf);
            break;
        case T_DIR:
            if (strcmp(".", p) && strcmp("..", p))
                find(buf, filename); // recursion
            break;
        }
    
    }

    
    close(fd);
}


int
main(int argc, char *argv[])
{

    if (argc != 3){
        fprintf(2, "Usage: find [path] [file name]\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
