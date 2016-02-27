#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "blowfish.h"
#include <errno.h>
/* Pre-Defined strings - Usage, error messages, etc. */
static char usage[] = "usage: %s [-devh] [-p PASSWD] infile outfile\n";
static char PROMPT_PASS[] = "Please enter an encryption password (you will need this for decryption, save it!): \n";

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
      fprintf(stderr, "Error: Input file name is not regular");
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
    fprintf(stderr, "Error: Input file does not exist");
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
int performCipher(int flag, int pageSize, unsigned char* password, int inFile, int outFile) {
  unsigned char iv[8];		/* Initialization Vector */
  int n = 0;			/* internal blowfish variables */
  int r = 0, err = 0;
  void* buffer = NULL;
  void* cipherBuffer = NULL;
  /* define a structure to hold the key */
  BF_KEY key;
  /* fill the IV with zeros (or any other fixed data) */
  memset(iv, 0, 8);
  if((buffer = calloc(1, pageSize))== NULL) {
    fprintf(stderr, "Error: Calloc Failure.");
    err = errno;
    goto clean;
  }

  /* call this function once to setup the cipher key */
  if(password == NULL) {
    err = 1;
    goto clean;
  }
  BF_set_key(&key, sizeof(password), password);
  while((r = read(inFile, buffer, pageSize)) > 0) {
    if((cipherBuffer = calloc(1, r)) == NULL) {
      fprintf(stderr, "Error: Calloc Failure.");
      free(buffer);
      free(cipherBuffer);
      buffer = NULL;
      err = errno;
      goto clean;
    }
    else {//bad malloc
      if(flag == 1) {
          BF_cfb64_encrypt(buffer, cipherBuffer, r, &key, iv, &n, BF_ENCRYPT);
      } else {
          BF_cfb64_encrypt(buffer, cipherBuffer, r, &key, iv, &n, BF_DECRYPT);
      }
    }
    //finally
    if(write(outFile, cipherBuffer, r) == -1) {
      //error, set errno break out of read write
      err = errno;
      goto clean;
    } //error check this hard
    free(cipherBuffer);
    cipherBuffer = NULL;
  }//white read success
  clean:
    if(buffer) {
      free(buffer);
    }
    if(cipherBuffer) {
      free(cipherBuffer);
    }
    return err;
}//performCipher

int main(int argc, char **argv)
{
  extern char *optarg;
  extern int optind;
  int c, err = 0;
  int dflag = 0, eflag = 0, vflag = 0, hflag = 0, pflag = 1;
  char infile[64], outfile[64];
  char tempFileName[] = "CipherTemporary.txt";
  char outFileRename[] = "OLD(Cipher)-";
  char* outRenameBuffer;
  char std_def[] = "-";
  int fin = 0, fout = 0;
  int err_code = 0;
  char* temp_pass = NULL;

  /* a temp buffer to read user input (the user's password) */
  unsigned char* temp_buf;
  //getopt, standard stuff flags devhp:
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
        temp_buf = calloc(1, sizeof(optarg));
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
    temp_pass = getpass(PROMPT_PASS);
    strcpy((char *)temp_buf, temp_pass);
  }
  strcpy(infile, argv[optind++]);
  strcpy(outfile, argv[optind]);

  if(fileCheck(infile, outfile) == 1) {
    err_code = errno;
    goto cleanup;
  }

  if(strcmp(infile, std_def) != 0) {
    //stdin will not be used
    //perhaps we dup to save the desc. then reassign.a
    if((fin = open(infile, O_RDONLY)) >= 0) {
      //fin returned correctly, close current stdin, reassign to fin
    } else {
      fprintf(stderr, "Error: Bad input file name.\n");
      err_code = errno;
      goto cleanup;
    }
  } else {
    fin = fileno(stdin);
  }
  if(strcmp(outfile, std_def) != 0) {
    //stdout will not be used for the output
    //perhaps we dup to save the desc. then reassign.
    if((fout = open(tempFileName, O_RDWR | O_CREAT, 0666)) >= 0) {

    } else {
        fprintf(stderr, "Error: Bad output file name.\n");
        err_code = errno;
        goto cleanup;
    }
  } else {
    fout = fileno(stdout);
  }
  err_code = performCipher((eflag == 1) ? 1: 0, getpagesize(), temp_buf, fin, fout);
  if(err_code != 0) {
      //if not zero, then an error. handle them
      //TODO - handle all the error codes appropriately
      switch(err_code) {
        case ENOSPC: //no space
          err_code = unlink(tempFileName);
          break;
        case EIO:
          break;
        case ENOMEM:
          break;
        default:
          break;
      }
      goto cleanup;
  }
  /*
  COPY file
  copy from input file to a temporary file, confirm the file successfully finished, move the file to the appropriate name
  if the outfile already exists, I rename it to a file
  */
  if(access(outfile, F_OK) == 0) {
    outRenameBuffer = malloc(sizeof(char) * (strlen(outfile) + strlen(outFileRename)));
    strcat(outRenameBuffer, outFileRename);
    strcat(outRenameBuffer, outfile);
    unlink(outRenameBuffer);
    rename(outfile, outRenameBuffer);
    unlink(outfile);
    free(outRenameBuffer);
  }
  rename(tempFileName, outfile);
  unlink(tempFileName);

  /* successfully run */
  cleanup:
    if(temp_buf) {
      free(temp_buf);
    }
    close(fin);
    close(fout);
    exit(err_code);

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
