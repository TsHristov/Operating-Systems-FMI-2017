// swap FILE1 FILE2 - swaps the contents of FILE1 and FILE2
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

ssize_t size;
char buffer[4096];

void close_file_descriptors(int *fd, int count){
  for(int i=0; i < count; ++i){
    close(fd[i]);
  }
}

void write_file(int source_fd, int dest_fd, int *fd, int fd_count){
  ssize_t bytes_read;
  // Adjust read positions to beginning of files:
  if (lseek(source_fd, (off_t) 0, SEEK_SET) == -1){
    exit(1);
  }
  if (lseek(dest_fd, (off_t) 0, SEEK_SET) == -1){
    exit(1);
  }
  if((bytes_read = read(source_fd, buffer, sizeof(buffer))) == -1){
    close_file_descriptors(fd, fd_count);
    err(3, "%d", source_fd);
  }
  do {
    if(write(dest_fd, buffer, bytes_read) == -1){
      close_file_descriptors(fd, fd_count);
      exit(1);
    }
  } while((bytes_read = read(source_fd, buffer, sizeof(buffer))));
}

int main(int argc, char *argv[]){
  if (argc != 3){
    errx(0, "Usage: swap FILE1 FILE2");
  }

  int fd[3];
  char *file1 = argv[1];
  char *file2 = argv[2];

  if((fd[0] = open(file1, O_RDWR)) == -1){
    err(1, "%s", file1);
  }
  if((fd[1] = open(file2, O_RDWR)) == -1){
    close(fd[1]);
    err(2, "%s", file2);
  }
  if((fd[2] = open("temp_file", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU)) == -1){
    close(fd[0]);
    close(fd[1]);
    exit(1);
  }

  write_file(fd[0], fd[2], fd, 3);
  write_file(fd[1], fd[0], fd, 3);
  write_file(fd[2], fd[1], fd, 3);

  close_file_descriptors(fd,3);
  exit(0);
}
