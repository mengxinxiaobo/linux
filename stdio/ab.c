#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
int main(int argc,char *argv[]){

    putchar('a');
    write(1,"b",1);

    putchar('a');
    write(1,"b",1);
    putchar('\n');
    
    putchar('a');
    write(1,"b",1);

    exit(0);
}