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



void *
memset(void *s,char c,size_t n){
#ifdef __HAVE_ARCH_MEMSET
    return __memset(s,c,n);
#else
    char *p=s;
    while (n-- >0)
    {
        /* code */
        *p++=c;
    }
    
#endif //__HAVE_ARCH_MEMSET
}


//string copy or memcopy
void *
memcpy(void* des,const void *src,size_t n){
#ifdef __HAVE_ARCH_MEMCPY
    return __memcpy(des,src,n);
#else
    const char *s = src;
    char *d = des;
    while(n-->0){
        *d++ = *s++;
    }
    return des;
#endif //__HAVE_ARCH_MEMCPY
}


//compare two blocks of memory
int 
memcmp(const void *v1,const void *v2,size_t n){
    const char *s1 = (const char *)v1;
    const char *s2 = (const char *)v2;
    while(n-->0){
        if(*s1!=*s2){
            return (int)((unsigned char)*s1 - (unsigned char)*s2);
        }
        s1++;s2++;
    }
    return 0;
} 

