#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>  

int main(int argc,char *argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage..\n");
        exit(1);
    }

    int fd;
    fd = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0600);

    if(fd < 0){
        perror("fopen()");
        exit(1);
    }
    lseek(fd,5LL*1024LL*1024LL*1024LL-1LL,SEEK_SET);
    write(fd,"",1);
    close(fd);
    exit(0);
}