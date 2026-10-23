#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#define BUFSIZE 1024
int main(int argc, char *argv[]){
    int fd1,fd2;
    char buf[BUFSIZE];
    ssize_t num = 0;
    if(argc < 3){
        fprintf(stderr,"usage error ");
        exit(1);
    }
    fd1 = open(argv[1],O_RDONLY);
    fd2 = open(argv[2],O_WRONLY|O_CREAT,0600);
    if (fd1 <0|fd2<0){
        perror("fopen()");
        close(fd1);
        close(fd2);
        exit(1);
    }
    while((num = read(fd1,buf,BUFSIZ)) > 0){
        write(fd2,buf,num);
    }
    printf("copy success");
    close(fd1);
    close(fd2);
    exit(0);
}