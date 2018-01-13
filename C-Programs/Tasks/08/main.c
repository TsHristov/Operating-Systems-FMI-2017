#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <err.h>

int main(int argc, char *argv[]){
  if(argc != 2){
    errx(0, "Usage: ./main FILE");
  }

  int fd;
  if((fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC)) == -1){
    err(1, "%s\n", argv[1]);
  }
  
  pid_t child_pid = fork();
  if(child_pid == -1){
    err(1, "Could not fork");
  }
  if(child_pid == 0){
    const char *text = "foobar";
    if(write(fd, text, sizeof(text)) == -1){
      err(2, "%s\n", argv[1]);
    }
    //!!!!!
    exit(0);
  }

  int status;
  wait(&status);
  if(WIFEXITED(status) && WEXITSTATUS(status) == 0){
    if(lseek(fd, 0, SEEK_SET) == -1){
      err(3, "%s\n", argv[1]);
    }

    char buffer[4096];
    ssize_t read_bytes;
    if((read_bytes = read(fd, buffer, sizeof(buffer))) == -1){
      err(4, "%s\n", argv[1]);
    }
    do {
      for(int i=0; i < read_bytes; i++){
	write(1,&(buffer[i]),1);
	write(1," ",1);
      }
    } while((read_bytes = read(fd, buffer, sizeof(buffer))));
  }
  
  close(fd);
  exit(0);
}
