
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <limits.h>

int f = 0;

int frobcmp(char const *a, char const *b)
{
  if (f) {
    if ((*a ^ 42) > UCHAR_MAX || (*a ^ 42) < 0 || (*b ^ 42) > UCHAR_MAX || (*b ^ 42) < 0) {
      fprintf(stderr, "Characters can't be used with '-f' options\n");
      exit(1);
    }
    // compares words character by character disregarding cases
    for(;; a++, b++) {
      if(*a == ' ' && *b == ' ') {
        return 0;
      }
      else if(*a == ' ' || (toupper(*a ^ 42) < toupper(*b ^ 42))) {
        return -1;
      }
      else if(*b == ' ' || (toupper(*a ^ 42) > toupper(*b ^ 42))) {
        return 1;
      }
    }
  }
  else {
    // compares words character by character
    for(;; a++, b++) {
      if(*a == ' ' && *b == ' ') {
    return 0;
      }
      else if(*a == ' ' || ((*a ^ 42) < (*b ^ 42))) {
    return -1;
      }
      else if(*b == ' ' || ((*a ^ 42) > (*b ^ 42))) {
    return 1;
      }
    }
  }
}


int compareFunc(const void* a, const void* b){
    return frobcmp(*(char const**) a, *(char const**) b);
}


void inputErr(){
    if(ferror(stdin)){
        fprintf(stderr, "Error: Unable to read input\n");
        exit(1);
    }
}

void outputErr(){
    if(ferror(stdout)){
        fprintf(stderr, "Error: Unable to read output\n");
        exit(1);
    }
}

void memErr(void *ptr){
    if(ptr == NULL){
        fprintf(stderr, "Error: Unable to allocate memory\n");
        exit(1);
    }
}


int main(int argc, char ** argv){

    if (argc > 2){
        fprintf(stderr, "Error: Too many operands");
        exit(0);
    }
    if (argc == 2){
        if (strcmp(argv[1], "-f") == 0){
            f = 1;
        }
        else{
            fprintf(stderr, "Error: wrong options");
            exit(1);
        }
    }


    char *buf;
    int bufsize;
    int bufindex; //buf control, 0-strlen(readbuf)-1;

    int startbuf = 8 * 1024;
    int charsread;

    struct stat fileData;
    if (fstat(STDIN_FILENO, &fileData) < 0){
        fprintf(stderr, "fstat error" );
        exit(1);
    }

    if(S_ISREG(fileData.st_mode)) {
    // for regular files, read entire file
      bufsize = fileData.st_size;
      bufindex = bufsize;
      // allocate memory
      buf = malloc(sizeof(char) * (bufsize+1));
      if (read(STDIN_FILENO, buf, bufsize) < 0) {
        fprintf(stderr, "Error reading file\n");
        exit(1);
      }
    }  
    else{

        buf = malloc(startbuf * sizeof(char));
        memErr(buf);
        bufsize = startbuf * sizeof(char);
        bufindex = 0; 
        char c;

        while ( read(STDIN_FILENO, &c, sizeof(char)) > 0 ){
            inputErr();
            buf[bufindex] = c;
            bufindex++;
            if (bufindex > bufsize / 2){
                bufsize = bufsize * 2;
                buf = realloc(buf, sizeof(char) * bufsize);
                memErr(buf);
            }
            
        }
    }
    buf[bufindex] = ' ';




    int row = 1; // row size
    int col = 1; // col size
    char** twoDarr = malloc(sizeof(char *));
    twoDarr[0] = malloc(sizeof(char));
    int current;
    int i = 0; // row index
    int j = 0; // col index
    int check = 1;
    int index = 0;

    while(check){

        current = buf[index];
        inputErr();
        if (index >= bufindex ){ // make need change to stelen(buf)
            check = 0;
            current = ' ';
        }
        // work on one word
        if (col <= j){
            col = col * 2;
            twoDarr[i] = realloc(twoDarr[i], col * sizeof(char));
            memErr(twoDarr[i]);
        }
        twoDarr[i][j] = current;
        j++;

        // go to next line 
        if ( current == ' '){
            i++;
            if (row <= i){
                row = row * 2;
                twoDarr = realloc(twoDarr, row * sizeof(char *));
                memErr(twoDarr);
            }
            twoDarr[i] = malloc(sizeof(char));
            col = 1;
            j = 0;
        }
        index++;
    }


    qsort(twoDarr, i, sizeof(char *), compareFunc);

    int cc = 0;
    for (int rr = 0; rr < i; rr++){
        cc = 0;
        while (1){
            if (twoDarr[rr][cc] != ' '){
                write(STDOUT_FILENO, &twoDarr[rr][cc], sizeof(char));
                cc++;
                if(ferror(stdout)){
                    outputErr();
                }
            }
            else{
                write(STDOUT_FILENO, &twoDarr[rr][cc], sizeof(char));
                if(ferror(stdout)){
                    outputErr();
                }
                break;
            }
            
        }
    }
    free(twoDarr);
    free(buf);

    return 0;
}
