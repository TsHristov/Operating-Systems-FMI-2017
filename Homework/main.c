#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdint.h>

struct BMP_HEADER {
  uint8_t  ID_FIELD_1;    // 1B
  uint8_t  ID_FIELD_2;    // 1B
  uint32_t FILE_SIZE;     // 4B
  uint16_t RESERVED_1;    // 2B
  uint16_t RESERVED_2;    // 2B
  uint32_t OFFSET;        // 4B 
}__attribute__((packed)); // -> total 14B

struct BMP_IMAGE_FILE {
  struct BMP_HEADER bmp_header;
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

  fprintf(stdout, "%c\n", BMP_IMAGE.bmp_header.ID_FIELD_1);
  fprintf(stdout, "%c\n", BMP_IMAGE.bmp_header.ID_FIELD_2);
  fprintf(stdout, "%d\n", BMP_IMAGE.bmp_header.FILE_SIZE);
  fprintf(stdout, "%d\n", BMP_IMAGE.bmp_header.OFFSET);

  close(fd);
  exit(0);
}
