#include <cstdio>
#include <cstdlib>

char* buffer;
char* last;



long get_long(){
    long result = 0;
    while(*last == ' ' || *last == '\n'){
        ++last;
    }
    while(*last != ' ' && *last != '\n'){
        result *= 10;
        result += ((char)*last) - 48;
        ++last;
    }
    return result;
}

int main(){
    buffer = new char[30000000];
    last = buffer;
    
    fread(buffer, 1, 30000000, stdin);
    
    printf("%ld", get_long());
     printf("%ld", get_long()); 
    
    delete [] buffer;
}
