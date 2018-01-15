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
  if (argc != 4){
    errx(0, "Usage: ./main COMMAND COMMAND");
  }

  const char *command1 = argv[1];
  const char *command2 = argv[2];
  const char *filename = argv[3];

  int fd;
  if((fd = creat(filename, S_IRWXU)) == -1){
    err(1, "%s\n", filename);
  }

  pid_t pid = fork();
  if(pid == -1){
    close(fd);
    err(2, "%s\n", filename);
  }
  if(pid == 0){
    if(execlp(command1,command1,0) == -1){
      close(fd);
      err(3, "%s\n", command1);
    }
  }

  int status;
  wait(&status);
  if(!(WIFEXITED(status) && WEXITSTATUS(status) == 0)){
    close(fd);
    err(4, "%s failed\n", command1);
  }

  // If the first command succeeded, write it`s name
  // to the file:
  if(write(fd, command1, strlen(command1)) == -1){
    close(fd);
    err(5, "%s\n", command1);
  }

  pid = fork();
  if(pid == -1){
    close(fd);
    err(2, "%s\n", filename);
  }
  if(pid == 0){
    if(execlp(command2,command2,0) == -1){
      close(fd);
      err(3, "%s\n", command1);
    }
  }

  wait(&status);
  close(fd);
  
  exit(0);
}
