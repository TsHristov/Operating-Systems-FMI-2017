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

struct DIB_HEADER {
  uint32_t BYTES_NUMBER;                // 4B
  uint32_t PIXELS_WIDTH;                // 4B
  uint32_t PIXELS_HEIGHT;               // 4B
  uint16_t COLOR_PLANES;                // 2B
  uint16_t BITS_PER_PIXEL;              // 2B
  uint32_t COMPRESSION;                 // 4B
  uint32_t RAW_BITMAP_DATA_SIZE;        // 4B
  uint32_t PRINT_RESOLUTION_HORIZONTAL; // 4B
  uint32_t PRINT_RESOLUTION_VERTICAL;   // 4B
  uint32_t NUMBER_OF_COLORS;            // 4B
  uint32_t IMPORTANT_COLORS;            // 4B
}__attribute__((packed));               // -> Total: 40B

struct BMP_IMAGE_HEADER {
  struct BMP_HEADER bmp_header;
  struct DIB_HEADER dib_header;
};

struct PIXEL {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
}__attribute__((packed)); // -> Total: 3B -> 24 bits

void change_borders(struct PIXEL **pixels, uint32_t height, uint32_t width){
  uint32_t pixel_size = sizeof(struct PIXEL);
  struct PIXEL black_pixel = {0, 0, 0};
  struct PIXEL red_pixel   = {255, 0, 0};

  uint8_t offset          = 5;
  uint32_t bottom_x       = height - (height - offset * pixel_size);
  uint32_t bottom_left_y  = offset * pixel_size;
  uint32_t bottom_right_y = width - offset * pixel_size;

  uint32_t upper_x        = height - (offset * pixel_size);
  uint32_t upper_left_y   = offset * pixel_size;
  uint32_t upper_right_y  = width - offset * pixel_size;
  
  // Color all corners black:
  pixels[bottom_x][bottom_left_y]  = black_pixel;
  pixels[bottom_x][bottom_right_y] = black_pixel;
  pixels[upper_x][upper_left_y]    = black_pixel;
  pixels[upper_x][upper_right_y]   = black_pixel;

  // Color the bottom horizontal line RED:
  for(uint32_t i=bottom_left_y; i < bottom_right_y; i++){
    pixels[bottom_x][i] = red_pixel;
  }
}

int main(int argc, char *argv[]){

  if (argc != 3){
    errx(1, "Usage: <INPUT_IMAGE.bmp> <OUTPUT_IMAGE.bmp>");
  }

  struct BMP_IMAGE_HEADER BMP_IMAGE;

  int input_image;
  if((input_image = open(argv[1], O_RDONLY)) == -1){
    err(1, "Could not open: %s", argv[1]);
  }
  
  if(read(input_image, &BMP_IMAGE, sizeof(BMP_IMAGE)) == -1){
    close(input_image);
    err(2, "Could not read: %s", argv[1]);
  }

  uint32_t pixels_height = BMP_IMAGE.dib_header.PIXELS_HEIGHT;
  uint32_t pixels_width  = BMP_IMAGE.dib_header.PIXELS_WIDTH;
  
  // Create an matrix of PIXEL arrays:
  struct PIXEL **pixels = (struct PIXEL**)(malloc(sizeof(struct PIXEL*) * pixels_height));

  for(uint32_t row=0; row < pixels_height; row++){
    // Each pixels row has WIDTH pixels:
    pixels[row] = (struct PIXEL*)(malloc(sizeof(struct PIXEL) * pixels_width));
    for(uint32_t column=0; column < pixels_width; column++){
      if(read(input_image, &pixels[row][column], sizeof(struct PIXEL)) == -1){
	close(input_image);
	err(5, "Could not read: %s", argv[1]);    
      }
    }
  }

  change_borders(pixels, pixels_height, pixels_width);
  
  int output_image;
  if((output_image = creat(argv[2], S_IRWXU)) == -1){
    close(output_image);
    err(3, "Could not create/open: %s", argv[2]);
  }

  // Write file header:
  if(write(output_image, &BMP_IMAGE, sizeof(BMP_IMAGE)) == -1){
    close(input_image);
    close(output_image);
    err(4, "Could not write to: %s", argv[2]);
  }

  // Write pixels to output image file:
  for(uint32_t row=0; row < pixels_height; row++){
    for(uint32_t column=0; column < pixels_width; column++){
      if(write(output_image, &pixels[row][column], sizeof(struct PIXEL)) == -1){
	close(input_image);
	err(5, "Could not write to: %s", argv[2]);    
      }
    }
  }
    
  close(input_image);
  close(output_image);

  // Free the contents of the pixels matrix:
  for(uint32_t row=0; row < pixels_height; row++){
    free(pixels[row]);
  }
  
  free(pixels);
  
  exit(0);
}
