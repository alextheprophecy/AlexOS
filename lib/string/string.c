#include "../integer/integer.h"

int int_to_str(int value, char *sp) {
    if (value == MIN_INT) {
        // Handle the special case where value is MIN_INT
        const char min_int_str[] = "-2147483648";
        for (int i = 0; min_int_str[i] != '\0'; i++) {
            *sp++ = min_int_str[i];
        }
        *sp = '\0';
        return 11; // Length of the string "-2147483648"
    }

    unsigned int v;
    int sign = value < 0;

    if (sign) {
        v = -value; // Convert to positive if negative
    } else {
        v = (unsigned int)value;
    }

    char tmp[32];
    char *tp = tmp;

    // Convert number to string
    do {
        int i = v % 10;
        v /= 10;
        *tp++ = i + '0';
    } while (v);

    int len = tp - tmp;

    if (sign) {
        *sp++ = '-';
        len++;
    }

    // Reverse the string
    while (tp > tmp) {
        *sp++ = *--tp;
    }

    *sp = '\0';
    return len;
}

int is_char_alphabetical(char c){
    return c>='a' && c<='z';
}