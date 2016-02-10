/* Fix me */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "blowfish.h"

/* Pre-Defined strings - Usage, error messages, etc. */
static char usage[] = "usage: %s [-devh] [-p PASSWD] infile outfile\n";
static char PROMPT_PASS[] = "Please enter a password: \n";

void print_help(char* name) {
  fprintf(stderr, usage, name);
}//print_help

int main(int argc, char **argv)
{

  extern char *optarg;
  extern int optind;
  int c, err = 0;
  int dflag = 0, eflag = 0, vflag = 0, hflag = 0;
  char infile[64], outfile[64];
  char std_def[] = "-";
  FILE *fin, *fout;

  unsigned char from[128], to[128];
  int len = 128;

  /* define a structure to hold the key */
  BF_KEY key;

  /* a temp buffer to read user input (the user's password) */
  unsigned char temp_buf[16];
  char* temp_pass;

  /* don't worry about these two: just define/use them */
  int n = 0;			/* internal blowfish variables */
  unsigned char iv[8];		/* Initialization Vector */

  /* fill the IV with zeros (or any other fixed data) */
  memset(iv, 0, 8);

  while((c = getopt(argc, argv, "devhp:")) != -1) {
    switch(c) {
      case 'd':
        dflag = 1;
        break;
      case 'e':
        eflag = 1;
        break;
      case 'v':
        vflag = 1;
        break;
      case 'h':
        hflag = 1;
        break;
      case 'p':
        //strcpy(temp_buf, optarg); //TODO: is strcpy the best choice????
        temp_pass = getpass(PROMPT_PASS);
        break;
      case '?':
        err = 1;
        break;
    }
  }//while
  if((optind + 2) > argc) {
    print_help(argv[0]);
    exit(1);
  } else if(err) {
    print_help(argv[0]);
    exit(1);
  }//error
  /* Copy the argument file names into designated locations.*/
  strcpy(infile, argv[optind++]);
  strcpy(outfile, argv[optind]);
  if(strcmp(infile, std_def) != 0) {
    //stdin will not be used
    //perhaps we dup to save the desc. then reassign.
    close(STDIN_FILENO);
    fin = fopen(infile, "r");
    //TODO: confirm fin is valid.
    dup(fileno(fin));

  }
  if(strcmp(outfile, std_def) != 0) {
    //stdout will not be used for the output
    //perhaps we dup to save the desc. then reassign.
    close(STDOUT_FILENO);
    fout = fopen(outfile, "w");
    //TODO: confirm fout is valid.
    dup(fileno(fout));
    fprintf(stdout, "What");
  }


  /* call this function once to setup the cipher key */
  BF_set_key(&key, 16, temp_buf);

  /*
   * This is how you encrypt an input char* buffer "from", of length "len"
   * onto output buffer "to", using key "key".  Jyst pass "iv" and "&n" as
   * shown, and don't forget to actually tell the function to BF_ENCRYPT.
   */
  BF_cfb64_encrypt(from, to, len, &key, iv, &n, BF_ENCRYPT);

  /* Decrypting is the same: just pass BF_DECRYPT instead */
  BF_cfb64_encrypt(from, to, len, &key, iv, &n, BF_DECRYPT);
}