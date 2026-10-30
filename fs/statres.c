#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

static off_t flen(const char *fname){
    struct stat statres;
    if (stat(fname,&statres) < 0)
    {
        /* code */
        perror("stat()");
        exit(1);
    }
    return statres.st_size;
}

int main(int argc,char *argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage..\n");
        exit(1);
    }
    
    off_t n = flen(argv[1]);
    printf("%lld\n",n);
    exit(0);
}