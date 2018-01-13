#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char *argv[]){
  if (argc != 2){
    errx(0, "Usage: ./main <command-name>");
  }

  // Fork the process:
  pid_t child_pid = fork();
  if(child_pid == -1){
    err(1, "Could not fork");
  }
  if(child_pid == 0){
    // In child process execute command:
    if(execlp(argv[1],argv[1],0) == -1){
      err(2, "Problem executing command");
    }
  } else {
    int status;
    // Wait for some child to change state:
    wait(&status);
    // WIFEXITED   - child terminated normally
    // WEXITSTATUS - return status of the child
    if(WIFEXITED(status) && WEXITSTATUS(status) == 0){
      printf("%s\n", argv[1]); 
    }
  }
  
  exit(0);
}
