//Steven Herring 376 - 1/25/16
#include <stdio.h>
#include <stdlib.h>

int main(){
  char *str;
  str = malloc(sizeof(char)*11);
  //str = "Hello World";
  printf("%s", str);
}
