#include<stdio.h>
#include<stdlib.h>
#include <string.h>
int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr,"Usage: %s <source file>\n",argv[0]);
        exit(1);
    }
    char *linebuf;
    size_t linesize;
    FILE *fp;
    
    linebuf = NULL;
    linesize = 0;
    while(1){
        if(getline(&linebuf,&linesize,fp) < 0)
            break;
        printf("%d\n",strlen(linebuf));
        printf("%d\n",linesize);
    }
    exit(0);
}