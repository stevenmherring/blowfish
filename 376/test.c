//Steven Herring 376 - 1/25/16
#include <stdio.h>
#include <stdlib.h>

int main(){
    char *str = "Hello World";
    str = malloc(11);
    str = "Hello World";
    
    
    fprintf(stderr, "%s\n", str);
    
}
