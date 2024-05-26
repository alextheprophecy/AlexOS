#include "integer.h"
char hex_chars[16] = "0123456789abcdef";

int digit_count(int n) {
    int r = 1;
    if (n < 0) n = -n;//(n == INT_MIN) ? INT_MAX: -n;
    while (n > 9) {
        n /= 10;
        r++;
    }
    return r;
}

char* int_to_hex_string(int n){
    char mask = 0xF; 
    char* out = "0x0000";
    for(int i=0;i<4;i++){
        out[2+(3-i)] = hex_chars[mask & (n>>(4*i))];
    }
    return out;
}

int abs(n){
    return n<0 ? -n:n;
}

int sign(n){
    return n>0 ? 1:(n==0 ? 0:-1);
}