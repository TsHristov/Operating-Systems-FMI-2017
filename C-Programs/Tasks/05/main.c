#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

void find_min_max(FILE **file, uint16_t *number, uint16_t *max, uint16_t *min, int print){
  while(fread(&*number, sizeof(*number), 1, *file)){
    if(print) printf("%d\n", *number);
    if(*number > *max) *max = *number;
    if(*number < *min) *min = *number;
  }
}

int main(int argc, char *argv[]){
  if(argc != 3){
    errx(0, "No arguments supplied");
  }

  FILE *file = fopen(argv[2], "rb");
  uint16_t number;
  
  if(!file){
    err(0,"%s\n", argv[2]);
  }
  
  uint16_t min;
  uint16_t max;
  if(fread(&number,sizeof(number),1,file)) min = max = number;

  if((strcmp(argv[1], "--min")) == 0){
    find_min_max(&file,&number,&max,&min,0);
    printf("%d\n", min);    
  }
  if((strcmp(argv[1], "--max")) == 0){
    find_min_max(&file,&number,&max,&min,0);
    printf("%d\n", max);    
  }
  if((strcmp(argv[1], "--print")) == 0){
    find_min_max(&file,&number,&max,&min,1);
  }

  fclose(file);
  exit(0);
}
