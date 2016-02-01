#include <stdio.h>
int main() {
    unsigned int i, j;
    scanf("%d", &i);
    scanf("%d", &j);
    while (i != j) {
        if (i > j) {
            unsigned int t = i / j;  //store the result of divided the two
            i = i % j;
            if(i == 0){ //if i % j is zero, then the GCD is t
                i = t;
                break;
            }
            i = t; //set i back to t and start over
        } else {
            j = j % i;
        }
    }
    printf("Result: %d\n", i);
}