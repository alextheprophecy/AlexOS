void some_function () {
}

void main () {
    char* video_memory = (char*)0xb8000;  
    int string_size = 5;
    for (int i = 0; i < string_size; i++) {
        (video_memory[i*2]) = 'a';
    }
 
    some_function ();
}