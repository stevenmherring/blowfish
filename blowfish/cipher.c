#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "blowfish.h"

/* Pre-Defined strings - Usage, error messages, etc. */
static char usage[] = "usage: %s [-devh] [-p PASSWD] infile outfile\n";
static char PROMPT_PASS[] = "Please enter a password: \n";

/*
print_help - helper method to print the usage string
param1 - char* name
return - void
*/
void print_help(char* name) {
  fprintf(stderr, usage, name);
}//print_help

/*
  fileCheck - check whether two file names are linked to the same file
  param1 - file name
  param2 - file name
  return int - return 0 for success, 1 for failure
*/
int fileCheck(char* file1, char* file2) {
  struct stat f1, f2;


  if(stat(file1, &f1) != 0 || (access(file1, F_OK ) == -1)) {
      fprintf(stderr, "Error: Input file does not exist");
      return 1;
  }
  //case where file2 doesn't exist yet, we just need to know that file1 is a regular file
  if(stat(file2, &f2) != 0) {
    if((S_ISREG(f1.st_mode) == 0)) {
      fprintf(stderr, "Error: I111231nput file does not exist");
      return 1;
    } else {
      return 0;
    }
  }

  //if((access(file2,F_OK) == -1) && (f1.st_mode & S_IFMT) == S_IFREG) {
  //  fprintf(stdout, "Error: 11111");
    //return 1;
  //}

  //error check, confirm that files are unique and not symlinks of another file
  //confirm file1 (input file is infact a file)
  if((f1.st_dev == f2.st_dev && f1.st_ino == f2.st_ino) || (S_ISREG(f1.st_mode) == 0)) {
    fprintf(stderr, "Error: Input file does not exist22");
    return 1;
  } else {
    return 0;
  }
}//fileDiff

/*
performCipher - performs the cipher function
param1 - char*: buffer to use for translation
param2 - int: flag for encryption/decryption - 1 for encryption, 0 for decryption
param3 - int: pagesize
param4 - BF_KEY: key
param5 - char*: password
return - int: 0 for success, ~0 for failure
*/
int performCipher(unsigned char* buffer, int flag, int pageSize, BF_KEY key, unsigned char* password) {
  unsigned char iv[8];		/* Initialization Vector */
  int n = 0;			/* internal blowfish variables */
  int r;

  /* fill the IV with zeros (or any other fixed data) */
  memset(iv, 0, 8);

  if((buffer = calloc(1, pageSize)) == NULL) {
    fprintf(stderr, "Error: Calloc Failure.");
    return 1;
  }

  /* call this function once to setup the cipher key */
  BF_set_key(&key, sizeof(password), password);
  while((r = read(STDIN_FILENO, buffer, pageSize)) > 0) {
    unsigned char* cipherBuffer;
    if((cipherBuffer = calloc(1, r)) == NULL) {
      fprintf(stderr, "Error: Calloc Failure.");
      free(buffer);
      return 1;
    }//bad malloc

    if(flag == 1) {
        BF_cfb64_encrypt(buffer, cipherBuffer, r, &key, iv, &n, BF_ENCRYPT);
    } else {
        BF_cfb64_encrypt(buffer, cipherBuffer, r, &key, iv, &n, BF_DECRYPT);
    }
    //finally
    write(STDOUT_FILENO, cipherBuffer, r); //error check this hard
    free(cipherBuffer);
  }//white read success
  free(buffer);
  return 0;
}//performCipher

int main(int argc, char **argv)
{

  extern char *optarg;
  extern int optind;
  int c, err = 0;
  int dflag = 0, eflag = 0, vflag = 0, hflag = 0, pflag = 1;
  char infile[64], outfile[64];
  char tempFileName[] = "CipherTemporary.txt";
  char std_def[] = "-";
  int fin, fout;
  void* buff;

  /* define a structure to hold the key */
  BF_KEY key;
  printf("%d\n", getpagesize());

  /* a temp buffer to read user input (the user's password) */
  unsigned char temp_buf[16];

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
        //we need to cast to char * since blowfish using unsigned char[]
        pflag = 0;
        strcpy((char *)temp_buf, optarg);
        break;
      case '?':
        err = 1;
        break;
    }
  }//while
  //argument error checking
  if((dflag == 1 && eflag == 1) || (dflag == 0 && eflag == 0) || ((optind +2) > argc) || err) {
    print_help(argv[0]);
    exit(1);
  }
  /* Copy the argument file names into designated locations.*/
  if(pflag) {
    char* temp_pass;
    temp_pass = getpass(PROMPT_PASS);
    strcpy((char *)temp_buf, temp_pass);
    free(temp_pass);
  }
  strcpy(infile, argv[optind++]);
  strcpy(outfile, argv[optind]);

  if(fileCheck(infile, outfile) == 1) {
    fprintf(stderr, "Error: file check failed.\n");
    exit(1);
  }

  if(strcmp(infile, std_def) != 0) {
    //stdin will not be used
    //perhaps we dup to save the desc. then reassign.
    if((fin = open(infile, O_RDONLY)) >= 0) {
      //fin returned correctly, close current stdin, reassign to fin
      close(STDIN_FILENO);
      dup(fin);
    } else {
      fprintf(stderr, "Error: Bad input file name.\n");
      exit(1);
    }
  }
  if(strcmp(outfile, std_def) != 0) {
    //stdout will not be used for the output
    //perhaps we dup to save the desc. then reassign.
    if((fout = open(outfile, O_RDWR | O_CREAT, 0666)) >= 0) {
      close(STDOUT_FILENO);
      dup(fout);
    } else {
        fprintf(stderr, "Error: Bad output file name.\n");
        exit(1);
    }
  }

  /*
  COPY file
  copy from input file to a temporary file, confirm the file successfully finished, move the file to the appropriate name
  */
  if(performCipher(buff, (eflag == 1) ? 1: 0, getpagesize(), key, temp_buf) != 0) {
      //if not zero, then an error. handle em
  }
  /* successfully run */
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  exit(0);

  //error checking, ENOMEM, EIO,
  //ENOSPC, ENOQUOTA
  //ENOMEM most programs just exit()
  //pre reserve RAM
  //check for EOF and partial reads
  //enomem wait a bit and try again or exit or design program to avoid needing more RAM

  //if outfile already exists?
  //policy A: if outfile exists, print an error and refuse to overwrite.
  //policy B: like /bin/cp, can overwrite it.



  /*
   * This is how you encrypt an input char* buffer "from", of length "len"
   * onto output buffer "to", using key "key".  Jyst pass "iv" and "&n" as
   * shown, and don't forget to actually tell the function to BF_ENCRYPT.
   */


  /* Decrypting is the same: just pass BF_DECRYPT instead */

}
