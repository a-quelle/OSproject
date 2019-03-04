void memory_copy(char* source, char* dest, int nbytes){
    int i;
    for(i = 0; i < nbytes; ++i)
        *(dest + i) = *(source + i);
}

void memory_set(char* dest, char val, int len){
    for ( ; len; --len) 
        *(dest++) = val;    
}

char* reverse(char* str){
    int j = strlen(str)-1, i = 0;    
    char tmp;
    for(; i < j; ++i, --j){
        tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
    }

    return str;
}

int strlen(char* s){

    int len = 0;
    while(s[len] != '\0')
        ++len;
    return len;
}

void int_to_ascii(int n, char str[]){
    int i, sign;
    if((sign = n) < 0)
        n = -n;
    i = 0;
    do{
        str[i++] = n % 10 + '0';        
    } while((n /= 10) > 0);
    if(sign < 0)
        str[i++] = '-';
    str[i] = '\0';
    reverse(str);
}