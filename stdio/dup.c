#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#define FILENAME "/tmp/out" 
int main(int argc,char *argv[]){

    int fd = open(FILENAME,O_RDWR|O_CREAT|O_TRUNC,0644);
    if(fd<0){
        perror("open()");
        exit(1);
    }
    // close(1);
    // dup(fd);

    dup2(fd,1);
    if(fd != 1) close(fd);
    /**************************/
    puts("hello!");
    exit(0);

}