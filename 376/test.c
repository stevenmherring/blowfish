//Steven Herring 376 - 1/25/16
#include <stdio.h>
#include <stdlib.h>

int main(){
    int i;
    scanf("%d", &i);
    char* str = malloc(sizeof(char) * i);
    scanf("%s", str);
  //str = "Hello World";
  printf("%lu\n", sizeof(str));
    free(str);
}
