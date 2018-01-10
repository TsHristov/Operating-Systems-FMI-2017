// Implementation of cat command:
//     cat <file1> <file2> ... <filen>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

int main(int argc, char* argv[]){
  char buffer[4096];
  int fd;
  int size;

  for(int i=1; i < argc; i++){
    // Try to open the file supplied as an argument:
    if((fd = open(argv[i],O_RDONLY)) == -1){
      err(0, "%s", argv[i]);
    }
    // Try to read file`s contents:
    if((size = read(fd, buffer, sizeof(buffer))) == -1){
      // If read operation failed, close the opened file descriptor:
      close(fd);
      err(1, "%s", argv[i]);
    }
    do{
      // Write the bytes read into stdout:
      write(1, buffer, size);
    }while((size = read(fd, buffer, sizeof(buffer))));
  }
  exit(0);
}
