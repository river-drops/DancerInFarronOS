#include<string.h>


// calculate the length of string @s
// size_t = long long  ,64bit
size_t strlen(const char *s){
    size_t cnt = 0;
    while(*s++ !='\0'){
        cnt++;
    }
    return cnt;
}


// calculate the length of the string @s, not including
// the terminating '\0' char acter, but at most @len.
size_t strnlen(const char *s, size_t len){
    size_t cnt=0;
    while (cnt<len && *s++ != '\0')
    {
        cnt++;
    }
    return cnt;
}


