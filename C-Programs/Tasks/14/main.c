#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <err.h>

int main(int argc, char *argv[]){

  if(argc != 2){
    errx(1, "Invalid number of arguments. Usage: %s <filename>\n", argv[0]);
  }
  
  const char *filename = argv[1];

  int fd;
  if((fd = creat(filename,S_IRWXU)) == -1){
    err(1, "Coud not open: %s\n", filename);
  }

  const char *str1 = "foo\n";
  const char *str2 = "bar\n";

  if(write(fd, str1, 2) == -1){
    close(fd);
    err(1, "Could not write first two chars from %s\n", str1);
  }

  const pid_t child = fork();
  if(child == -1){
    close(fd);
    err(1, "Could not fork.");
  }
  if(child == 0){
    if(write(fd, str2, strlen(str2)) == -1){
      close(fd);
      err(1, "Could not write %s\n", str2);
    }
    exit(0);
  }

  /* Wait for child to finish: */
  int child_status;
  wait(&child_status);
  if(WIFEXITED(child_status) && WEXITSTATUS(child_status) == 0){
    if(write(fd, str1 + 2, 2) == -1){
      close(fd);
      err(1, "%s\n", argv[0]);
    }
  }
  close(fd);
  exit(0);
}
