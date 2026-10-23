#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[]){
    char buf[100];
    int year = 2014, month = 1, day = 1;
    snprintf(buf,"%d-%02d-%02d", year, month, day);
    puts(buf);
    exit(0);
}