// c overview

//single comment style
/*
 Multiple line comments
 lalalalala
 lalalala
 dadadada
 ldadadalsdasdlasd
 sadlasldkasdlaj
 */
//includes

int i; //integer named i (signed int)
int j, k = 0; //declare mulltiple variables, initialized k as 0

unsigned int x; //only positive numbers
//an int is defined to be 32-bit numeric

short int s; //16 bit integer
long int l; //64 bit int sometimes
long l2; //a 64 bit int sometimes

long long l1; //gaurantee to be 64 bit int
quad l1; //gauranteed to be 64 bit int


unsigned char s; //one byte, or 8 bit quantity
char c; //variable to hold a single ascii character

float f; //floating point number (32 bits)
double float f; //double width/precision floating point number

//arrays
int a[10]; //an array of 10 integers, referred as a[0] - a[9] - indexes at 0

char s[80]; //an array of 80 characters, also called a "string"
//when a char array is used as a string, it's often terminated with a NULL
//which is ASCII 0, also can be noted as '\0'

//returns the sum of 2 ints
//return type: int
//func name sum
//zero or more, comma delimited parameters passed to function
//enclosed in mandatory parenthesis
int sum(int a, int b) {
    int s;
    //declare variables at the beginning
    s = a+b;
    return s;
}//sum

int main() {
    int j, val, k = 0;
    int i;
    j = 10;
    k++; //increment operator k=k+1
    j--; //decreme op j = j-1

    val = sum(j,k);
    printf("The value of %d, and %d added together is %d\n", j, k, val);
    
    //loops
    for(i = 0; i < 10; i++) {
        //do stuff
        printf("%d\n", i);
    }//for i

    i=0;
    while(i != 7) {
        printf("%d\n", i);
        i++;
    }//while
    
    i = 10;
    do {
        printf("%d\n",i);
        i--;
    }while( i > 0);
    
    //string manipulation
    char s[80];
    strcpy(s, "hello world");
    printf("my message is: %s\n", s);
    
    char *2;
    
    s2 = malloc(80);
    strcpy(s2, "hello world");
    printf("my message is: %s\n", s2);
    free(s2);

}//main

//--------------------------------------
//boolean
typedef int bool;
#define true 1
#define false 0