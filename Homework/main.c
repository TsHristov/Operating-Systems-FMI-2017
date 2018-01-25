#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdint.h>
/* #include <inttypes.h> */

struct BITMAP_FILE_HEADER {
  uint8_t  signature[2];
  uint16_t size;
  uint8_t  reserved[2];
  uint16_t offset;
};

struct BMP_IMAGE_FILE {
  struct BITMAP_FILE_HEADER header;
};
  
int main(int argc, char *argv[]){

  if (argc != 3){
    errx(1, "Usage: <INPUT_IMAGE.bmp> <OUTPUT_IMAGE.bmp>");
  }

  struct BMP_IMAGE_FILE BMP_IMAGE;
  const char *input_image  = argv[1];
  
  int fd;


  if((fd = open(input_image, O_RDONLY)) == -1){
    err(1, "Could not open: %s", input_image);
  }
  
  if(read(fd, &BMP_IMAGE, sizeof(BMP_IMAGE)) == -1){
    close(fd);
    err(2, "Could not read: %s", input_image);
  }

  fprintf(stdout, "%c\n", BMP_IMAGE.header.signature[0]);
  fprintf(stdout, "%c\n", BMP_IMAGE.header.signature[1]);
  fprintf(stdout, "%d\n", BMP_IMAGE.header.size);
  fprintf(stdout, "%s\n", BMP_IMAGE.header.reserved);
  fprintf(stdout, "%d\n", BMP_IMAGE.header.offset);

  close(fd);
  exit(0);
}
