#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[]){
    printf("before fflush");
    fflush(stdout);
    while(1);
    printf("after");
    exit(0);
}