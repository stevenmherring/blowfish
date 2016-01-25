//Steven Herring 376 - 1/25/16
#include <stdio.h>
#include <stdlib.h>
char* firstTopic() {
  char* notes = "Programmer decided that being in a while loop that only ran once was unnecessary and switched \
  his loop an if statement without modifying other code properly. This programmer lost his job.\n \
  The problem lied in a break statement inside the function, without the while loop it could \
  detriment the program and break out of a loop that is outside of the programmers scope.";
  return notes;
}

char* secondTopic() {
  char* notes = "Never post a major change on a Friday afternoon when you won't have engineers available to fix it.";
  return notes;
}

char* thirdTopic() {
  char* notes = "Why C? \n \
  C is still very popular and competitive in use versus the other popular C++ and Java.\n \
  There are so many existing code bases in C already, it's financially consuming to switch large systems to a new language.\n \
  Embedded systems, memory management all strongly require the use of a low level language like C\n \
  Because of this, C is very mistake prone. In the hands of the right programmer you can have amazing control and power.\
  Entire industries work almost entirely and exclusively in C due to their lack of memory and available power.\
  Performance is everything, so the lower level and performance gains you get from using C makes for big gains that win sales.\
  ";
  return notes;
}

char* fourthTopic() {
  char* notes = "http://www3.cs.stonybrook.edu/~ezk/cse376-s16/info.html#homework";
  return notes;
}

int main() {
  int i;
  char* greeting = "January 25th 2016 Notes for CSE 376\n \
  Please select the appropriate numeric option or -1 to exit\n \
  1. While Loop Situation / Class Introduction\n \
  2. Life tip\n \
  3. Why C?\n \
  4. Homework\n \
  9. Repost options.\n";
  printf("%s", greeting);
  while(scanf("%d",&i) > 0) {
    switch(i){
      case 1:
        printf("%s\n", firstTopic());
        break;
      case 2:
        printf("%s\n", secondTopic());
        break;
      case 3:
        printf("%s\n", thirdTopic());
        break;
      case 4:
        printf("%s\n", fourthTopic());
        break;
      case 9:
        printf("%s", greeting);
        break;
      default:
        printf("No available option, terminating\n");
        exit(-1);
    }
  }
  exit(0);
}//main
