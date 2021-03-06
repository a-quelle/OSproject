int strlen(char* s){

    int len = 0;
    while(s[len] != '\0')
        ++len;
    return len;
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

void append(char* s, char n){
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void int_to_ascii(int n, char* str){
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

void hex_to_ascii(int n, char* str){
    append(str, '0');
    append(str, 'x');
    int zeroes = 0;
    int i;
    for(i = 28; i>0; i-=4){
        int tmp = (n >> i) & 0xF;
        if(!tmp && !zeroes) continue;
        zeroes = 1;
        if(tmp >= 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }
    int tmp = n & 0xF;
    if(tmp >= 0xA) append(str, tmp - 0xA + 'a');
    else append(str, tmp + '0');
}

void backspace(char* s){
    int len = strlen(s);
    if(len)
        s[len-1] = '\0';
}
//returns -1 if s1<s2, 1 if s2>s1, 0 otherwise
int strcmp(char* s1, char* s2){
    int i;
    for(i = 0; s1[i] == s2[i]; ++i)
        if(s1[i] == '\0') return 0;
    return s1[i]-s2[i];
}