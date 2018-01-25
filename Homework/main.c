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
  uint8_t  id_field_1;   
  uint8_t  id_field_2;   
  uint32_t file_size;    
  uint16_t reserved_1;   
  uint16_t reserved_2;   
  uint32_t offset;       
}__attribute__((packed));

struct DIB_HEADER {
  uint32_t bytes_number;          
  uint32_t pixels_width;          
  uint32_t pixels_height;         
  uint16_t color_planes;          
  uint16_t bits_per_pixel;        
  uint32_t compression;           
  uint32_t raw_data_size;               
  uint32_t horizontal_print_resolution; 
  uint32_t vertical_print_resolution;   
  uint32_t number_of_colors;            
  uint32_t important_colors;            
}__attribute__((packed));               

struct BMP_IMAGE_HEADER {
  struct BMP_HEADER bmp_header;
  struct DIB_HEADER dib_header;
};

struct PIXEL {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
}__attribute__((packed));

void color_borders(struct PIXEL **pixels, uint32_t height, uint32_t width, uint8_t offset){
  uint32_t pixel_size = sizeof(struct PIXEL);

  struct PIXEL BLACK = {0,   0,   0};
  struct PIXEL RED   = {255, 0,   0};
  struct PIXEL GREEN = {0, 255,   0};
  struct PIXEL BLUE  = {0,   0, 255};
  
  uint32_t bottom_x = height - (height - offset * pixel_size);
  uint32_t upper_x  = height - (offset * pixel_size);
  uint32_t left_y   = offset * pixel_size;
  uint32_t right_y  = width  - offset * pixel_size;
  
  // Color all border corners black:
  pixels[bottom_x][left_y]  = BLACK;
  pixels[bottom_x][right_y] = BLACK;
  pixels[upper_x][left_y]   = BLACK;
  pixels[upper_x][right_y]  = BLACK;

  // Color bottom and horizontal lines RED:
  for(uint32_t i=left_y; i < right_y; i++) {
    pixels[bottom_x][i] = RED;
    pixels[upper_x][i]  = GREEN;
  }

  // Color left and right vertical lines BLUE:
  for(uint32_t i=bottom_x; i < upper_x; i++) {
    pixels[i][left_y]  = BLUE;
    pixels[i][right_y] = BLUE;
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

  uint32_t pixels_height = BMP_IMAGE.dib_header.pixels_height;
  uint32_t pixels_width  = BMP_IMAGE.dib_header.pixels_width;
  uint32_t pixels_row_size = sizeof(struct PIXEL) * pixels_width;
  
  // Create matrix of pixels:
  struct PIXEL **pixels = (struct PIXEL**)(malloc(sizeof(struct PIXEL*) * pixels_height));
  
  if(!pixels) {
    close(input_image);
    errx(3, "Bad memory allocation");
  }

  // Read the pixels of the input image file into pixels matrix:
  for(uint32_t row=0; row < pixels_height; row++){

    pixels[row] = (struct PIXEL*)(malloc(pixels_row_size));

    if(!pixels[row]) {
      close(input_image);
      errx(4, "Bad memory allocation");
    }
    
    // Read one pixel row at a time:
    if(read(input_image, pixels[row], pixels_row_size) == -1){
    	close(input_image);
    	err(5, "Could not read: %s", argv[1]);
    }
  }

  color_borders(pixels, pixels_height, pixels_width, 5);

  int output_image;
  if((output_image = creat(argv[2], S_IRWXU)) == -1){
    close(output_image);
    err(6, "Could not create/open: %s", argv[2]);
  }

  // Write file header to output image file:
  if(write(output_image, &BMP_IMAGE, sizeof(BMP_IMAGE)) == -1){
    close(input_image);
    close(output_image);
    err(7, "Could not write to: %s", argv[2]);
  }

  // Write pixels to output image file, one row at a time:
  for(uint32_t row = 0; row < pixels_height; row++) {
    if(write(output_image, pixels[row], pixels_row_size) == -1){
  	close(input_image);
  	err(8, "Could not write to: %s", argv[2]);
    }
  }

  // Close the file descriptors associated with the image files:
  close(input_image);
  close(output_image);

  // Free the contents of the pixels matrix:
  for(uint32_t row=0; row < pixels_height; row++){
    free(pixels[row]);
  }
  free(pixels);
  
  exit(0);
}
