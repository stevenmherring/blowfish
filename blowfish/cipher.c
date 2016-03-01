#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "blowfish.h"
#include <errno.h>
#include "builds.h"
#include <termios.h>
#define VERS 1.0
/* Pre-Defined strings - Usage, error messages, etc. */
static char usage[] = "usage: %s [-devh] [-p PASSWD] infile outfile\n";
static char PROMPT_PASS[] = "Please enter an encryption password (you will need this for decryption, save it!): \n";
static char PROMPT_PASS_SECURE[] = "Please re-enter your password.\n";

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
  if(strcmp(file1, "-") == 0) {
    return 0;
  } else if(strcmp(file2, "-") == 0) {
    if(stat(file1, &f1) != 0 || (access(file1, F_OK ) == -1)) {
        fprintf(stderr, "Error: Input file does not exist");
        return 1;
      } else {
        return 0;
      }
  } else {
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

  //error check, confirm that files are unique and not symlinks of another file
  //confirm file1 (input file is infact a file)
    if((f1.st_dev == f2.st_dev && f1.st_ino == f2.st_ino) || (S_ISREG(f1.st_mode) == 0)) {
      fprintf(stderr, "Error: Input and Output files must not be the same.");
      return 1;
    } else {
      return 0;
    }
  }//else
}//fileDiff

/*
performCipher - performs the cipher function
param1 - char*: buffer to use for translation
param2 - int: flag for encryption/decryption - 1 for encryption, 0 for decryption
param3 - int: pagesize
return - int: 0 for success, ~0 for failure
*/
int performCipher(int flag, int pageSize, unsigned char* password) {
  unsigned char iv[8];		/* Initialization Vector */
  int n = 0;			/* internal blowfish variables */
  int r = 0, err = 0;
  void* buffer = NULL;
  void* cipherBuffer = NULL;
  /* define a structure to hold the key */
  BF_KEY key;
  /* fill the IV with zeros (or any other fixed data) */
  memset(iv, 0, 8);
  if((buffer = malloc(pageSize))== NULL) {
    fprintf(stderr, "Error: Malloc Failure.");
    err = errno;
    goto clean;
  }
  /* call this function once to setup the cipher key */
  if(password == NULL) {
    err = 1;
    goto clean;
  }
  BF_set_key(&key, sizeof(password), password);
  while((r = read(STDIN_FILENO, buffer, pageSize)) > 0) {
    if((cipherBuffer = malloc(r)) == NULL) {
      fprintf(stderr, "Error: Malloc Failure.");
      free(buffer);
      if(cipherBuffer) {
        free(cipherBuffer);
      }
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
    if(write(STDOUT_FILENO, cipherBuffer, r) == -1) {
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
  int dflag = 0, eflag = 0, vflag = 0, hflag = 0, pflag = 0, sflag = 0;
  char infile[64], outfile[64];
  char tempFileName[] = "CipherTemporary.txt";
  //char outFileRename[] = "OLD(Cipher)-";
  //char* outRenameBuffer;
  char std_def[] = "-";
  int fin = 0, fout = 0;
  int err_code = 0;
  char* temp_pass = NULL;
  char* temp_pass2 = NULL;

  /* a temp buffer to read user input (the user's password) */
  unsigned char* temp_buf = NULL;
  //getopt, standard stuff flags devhp:
  while((c = getopt(argc, argv, "dsevhp:")) != -1) {
    switch(c) {
      case 'd':
        dflag = 1;
        break;
      case 's':
        sflag = 1;
        break;
      case 'e':
        eflag = 1;
        break;
      case 'v':
        vflag = 1;
        printf("Cipher Version %f \nBuild number %d\n", VERS, BUILDS);
        break;
      case 'h':
        hflag = 1;
        break;
      case 'p':
        //strcpy(temp_buf, optarg); //TODO: is strcpy the best choice????
        //we need to cast to char * since blowfish using unsigned char[]
        pflag = 1;
        temp_buf = calloc(1, sizeof(optarg));
        if(temp_buf == NULL) {
          fprintf(stderr, "Error: Calloc Faileld\n");
          err_code = errno;
          goto cleanup;
        }
        strcpy((char *)temp_buf, optarg);
        break;
      case '?':
        err = 1;
        break;
    }
  }//while
  if(hflag == 1) {
    print_help(argv[0]);
    err_code = 0;
    goto cleanup;
  }
  //argument error checking
  if((dflag == 1 && eflag == 1) || (dflag == 0 && eflag == 0) || ((optind + 2) != argc) || err) {
    print_help(argv[0]);
    err_code = 1;
    goto cleanup;
  }
  /* Copy the argument file names into designated locations.*/
  if(!pflag) {
    temp_pass = getpass(PROMPT_PASS);
    temp_buf = calloc(1, sizeof(temp_pass));
    if(temp_buf == NULL) {
      fprintf(stderr, "Error: Calloc Faileld\n");
      err_code = errno;
      goto cleanup;
    }
    strcpy((char *)temp_buf, temp_pass);
    if(sflag) {
      temp_pass = NULL;
      temp_pass = getpass(PROMPT_PASS_SECURE);
      if(strcmp(temp_pass, (char *) temp_buf) != 0 ) {
        //strings dont match, error and quit
        fprintf(stderr, "Error: Passwords do no match\n");
        goto cleanup;
      }
  }
    memset(temp_pass, 0, strlen(temp_pass)); //remove clear-text password from program space
  } //!pflag

  if(pflag && sflag) {
    temp_pass = getpass(PROMPT_PASS_SECURE);
    if(strcmp(temp_pass, (char *) temp_buf) != 0 ) {
      //strings dont match, error and quit
      fprintf(stderr, "Error: Passwords do no match\n");
      goto cleanup;
    }
      memset(temp_pass, 0, strlen(temp_pass)); //remove clear-text password from program space
  }//if p and s flag are set
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
      if( dup2(fin, STDIN_FILENO) == -1) {
        err_code = errno;
        fprintf(stderr, "Error: Bad file descriptor, or too many files open.\n");
        goto cleanup;
      }
      close(fin);
    } else {
      fprintf(stderr, "Error: Bad input file name.\n");
      err_code = errno;
      goto cleanup;
    }
  }
  if(strcmp(outfile, std_def) != 0) {
    //stdout will not be used for the output
    //perhaps we dup to save the desc. then reassign.
    if((fout = open(tempFileName, O_RDWR | O_CREAT, 0666)) >= 0) {
      if (dup2(fout, STDOUT_FILENO) == -1) {
          err_code = errno;
          fprintf(stderr, "Error: Bad file descriptor, or too many files open.\n");
          goto cleanup;
      }
      close(fout);
    } else {
        fprintf(stderr, "Error: Bad output file name.\n");
        err_code = errno;
        goto cleanup;
    }
  }
  err_code = performCipher((eflag == 1) ? 1: 0, getpagesize(), temp_buf);
  /*
  Check return codes, handle errors
  */
  if(err_code != 0) {
      //if not zero, then an error. handle them
      //TODO - handle all the error codes appropriately
      switch(err_code) {
        case ENOSPC: //no space
          fprintf(stderr, "Error: Not enough space available. %s", (eflag == 1) ? "Encrypt failed\n" : "Decrypt failed\n");
          break;
        case EIO:
          fprintf(stderr, "Error: An error occured during input/output operations. %s", (eflag == 1) ? "Encrypt failed\n" : "Decrypt failed\n");
          break;
        case ENOMEM:
          fprintf(stderr, "Error: Insufficient memory. %s", (eflag == 1) ? "Encrypt failed\n" : "Decrypt failed\n");
          break;
        case EDQUOT:
          fprintf(stderr, "Error: Quota fail %s", (eflag == 1) ? "Encrypt failed\n" : "Decrypt failed\n");
          break;
        default:
          fprintf(stderr, "Error: Undefined, refer to stack trace\n");
          break;
      }//switch
      strerror(errno);
      if(access(tempFileName, F_OK) == 0) {
        err_code = unlink(tempFileName);
      }
      goto cleanup;
  }//if error
  //rename temp to outfile TODO: Error check this
  if(rename(tempFileName, outfile) != 0) {
    //error on rename
    strerror(errno);
  }
  if(unlink(tempFileName) != 0) {
    //error on unlink
    strerror(errno);
  }

  /* successfully run */
  cleanup:
    if(temp_buf) {
      free(temp_buf);
    }
    if(temp_pass) {
      free(temp_pass);
    }
    if(temp_pass2) {
      free(temp_pass2);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    exit(err_code);

}
