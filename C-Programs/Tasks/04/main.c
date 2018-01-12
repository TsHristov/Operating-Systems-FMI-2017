// Copies /etc/passwd in the currrent working directory and changes the ':' delimeter to '?'
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <string.h>

char* replace(char *buffer, int size){
  char *replaced = (char*)malloc(size + 1);
  int i;
  for(i=0; i < size; i++){
    if(buffer[i] == ':'){
      replaced[i] = '?';
    } else {
      replaced[i] = buffer[i];
    } 
  }
  replaced[i] = '\0';
  return replaced;
}

int main(){
  const char *source = "/etc/passwd";
  const char *destination = "passwd";
  char *replaced = NULL;
  
  int fd1;
  if((fd1 = open(source, O_RDONLY)) == -1){
    err(0, "%s\n", source);
  }
  int fd2;
  if((fd2 = creat(destination,S_IRWXU)) == -1){
    close(fd1);
    err(1, "%s\n", destination);
  }
  ssize_t bytes_read;
  char buffer[4096];
  if((bytes_read = read(fd1,buffer,sizeof(buffer))) == -1){
    close(fd1);
    close(fd2);
    err(2, "%s\n", source);
  }
  do{
    replaced = replace(buffer, (int)bytes_read);
    if(write(fd2,replaced,bytes_read) == -1){
      close(fd1);
      close(fd2);
      err(3, "%s\n", destination);
    }
  }while((bytes_read = read(fd1, buffer, sizeof(buffer))));
  
  close(fd1);
  close(fd2);
  free(replaced);
  exit(0);
}
