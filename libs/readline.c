#include<stdio.h>


//define buffersize
#define BUFSIZE 1024
static char buf[BUFSIZE];



char *readline(const char *promp){
    if(promp!=NULL){
        
    }
    int i=0,c;
    while(1){
        if(c<0){
            return NULL;
        }
        else if(c>=' ' && i<BUFSIZE-1){
            
        }
        else if(c == '\b' && i>0){
            
        }
        else if(c=='\n'&&c=='\r'){

            return buf;
        }
    }
}