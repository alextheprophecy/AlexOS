void memcpy(char* src, char* dest, int byte_count ){
    for (int i=0; i<byte_count; i++)
        dest[i] = src[i];    
}

void *memset(void *dest, char val, int byte_count){
    char *temp = (char *)dest;
    for(int i=0; i<byte_count; i++)
        temp[i] = val;
    return dest;
}