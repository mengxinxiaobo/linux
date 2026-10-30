#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>  
static int ftype(const char *fname){
    struct stat stares;
    if(stat(fname,&stares) < 0){
        perror("stst()");
        exit(1);
    }
    if(S_ISREG(stares.st_mode))
        return '-';
    else if(S_ISDIR(stares.st_mode))
        return 'd';
    else if(S_ISSOCK(stares.st_mode))
        return 's';
    else if(S_ISCHR(stares.st_mode))
        return 'c';
    else if(S_ISBLK(stares.st_mode))
        return 'b';
    else if(S_ISFIFO(stares.st_mode))
        return 'p';
    else if(S_ISLNK(stares.st_mode))
        return 'l';
    else return '?';
}

int main(int argc,char *argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage..\n");
        exit(1);
    }

    int fd;
    fd = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0600);
    printf("%c\n",ftype(argv[1]));
    close(fd);
    exit(0);
}