// Accepts two commands as arguments. Runs them in parallel and
// prints the PID of the first finished command. If none has finished
// successfully then return -1.
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

  const char *command1 = argv[1];
  const char *command2 = argv[2];

  pid_t pid = fork();
  if(pid == -1){
    err(1, "%s\n", command1);
  }
  if(pid == 0){
    if(execlp(command1,command1,0) == -1){
      err(2, "%s\n", command1);
    }
  }

  pid = fork();
  if(pid == -1){
    err(3, "%s\n", command2);
  }
  if(pid == 0){
    if(execlp(command2,command2,0) == -1){
      err(4, "%s\n", command1);
    }
  }

  int first_status;
  pid_t first_pid  = wait(&first_status);
  int second_status;
  pid_t second_pid = wait(&second_status);

  if(WIFEXITED(first_status) && WEXITSTATUS(first_status) == 0){
    fprintf(stdout, "PID of first finished process: %d\n", first_pid);
  }
  else if(WIFEXITED(second_status) && WEXITSTATUS(second_status) == 0){
    fprintf(stdout, "PID of first finished process: %d\n", second_pid);
  }
  else {
    fprintf(stdout, "%d\n", -1);
  }
  
  exit(0);
}
