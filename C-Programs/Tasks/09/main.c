#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>

int main(){
  int fd;
  if((fd = creat("tmp_file",S_IRWXU)) == -1){
    err(0, "%s\n", "tmp_file");
  }

  pid_t pid = fork();
  if(pid == -1){
    close(fd);
    err(1, "Could not fork");
  }
  if(pid == 0){
    const char *text = "foo";
    if(write(fd, text, strlen(text)) == -1){
      close(fd);
      err(2, "Write failed");
    }
    exit(0);
  }

  // Wait for first child to write "foo":
  int status;
  wait(&status);
  if(WIFEXITED(status) && WEXITSTATUS(status) != 0){
    close(fd);
    err(3, "Child did not terminate normally");
  }

  // Proceed with second child:
  pid = fork();
  if(pid == -1){
    close(fd);
    err(3, "Could not fork");
  }
  if(pid == 0){
    const char *text = "bar";
    if(write(fd, text, strlen(text)) == -1){
      close(fd);
      err(4, "Write failed");
    }
    exit(0);
  }
  
  close(fd);
  exit(0);
}
