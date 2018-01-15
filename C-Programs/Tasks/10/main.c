#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>

int main(int argc, char *argv[]){
  if (argc != 3){
    errx(0, "Usage: ./main COMMAND COMMAND");
  }

  pid_t pid = fork();
  if(pid == -1){
    err(1, "Error on fork");
  }
  if(pid == 0){
    if(execlp(argv[1],argv[1],0) == -1){
      err(2, "Error on exec()");
    }
  }

  int status;
  wait(&status);

  if(!(WIFEXITED(status) && WEXITSTATUS(status) == 0)){
    exit(-1);
  }
  
  pid = fork();
  if(pid == -1){
    err(4, "Error on fork");
  }
  if(pid == 0){
    if(execlp(argv[2],argv[2],0) == -1){
      err(5, "Error on exec()");
    }
  }

  wait(&status);

  if(!(WIFEXITED(status) && WEXITSTATUS(status) == 0)){
    exit(-1);
  }
  
  exit(0);
}
