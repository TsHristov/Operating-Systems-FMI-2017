// Implementation of cp command:
//   cp SOURCE DESTINATION
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[]){
  char* source = NULL;
  char* destination = NULL;

  if (argc == 1){
    errx(0, "missing file operand");
  } else if (argc == 3){
    source = argv[1];
    destination = argv[2];    
  } else {
    errx(1, "Usage: cp SOURCE DEST");
  }
    
  int fd1;
  int fd2;
  char buffer[4096];
  ssize_t size;

  // Try to open SOURCE file:
  if((fd1 = open(source, O_RDONLY)) == -1){
    err(2, "%s", source);
  }
  // Try to open DESTINATION file:
  if((fd2 = creat(destination, S_IRWXU)) == -1){
    err(3, "%s", destination);
  }
  // Try to read from SOURCE:
  if((size = read(fd1, buffer, sizeof(buffer))) == -1){
    close(fd1);
    close(fd2);
    err(4, "%s", source);
  }
  do {
    // Try to write to DESTINATION:
    if(write(fd2, buffer, size) == -1){
      close(fd1);
      close(fd2);
      err(5, "%s", destination);
    }
  }while((size = read(fd1, buffer, sizeof(buffer))));

  close(fd1);
  close(fd2);

  exit(0);
}
