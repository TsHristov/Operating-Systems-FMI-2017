#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>

int main(int argc, char *argv[]){
  if(argc != 4){
    errx(0, "Usage: ./main <command1> <command2> <command3>");
  }
  
  for(int i=1; i < argc; i++){
    pid_t child_pid = fork();
    if(child_pid == -1){
      err(1, "Could not fork");
    }
    if(child_pid == 0){
      if(execlp(argv[i],argv[i],0) == -1){
	err(2, "Could not exec");
      }
    }

    int status;
    wait(&status);
    if(WIFEXITED(status) && WEXITSTATUS(status) == 0){
      printf("Child PID: %d\n", (int)child_pid);
      printf("Status: %d\n", status);
    }
  }
  
  exit(0);
}
