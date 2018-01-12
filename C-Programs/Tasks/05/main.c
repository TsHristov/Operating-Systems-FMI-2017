#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

// Description:
//   Finds the max/min uint16_t number in a binary file.
// Input:
//     -FILE **file   - binary file with uint16_t numbers
//     -uint16_t *max - max number in file
//     -uint16_t *min - min number in file
//     -int print     - boolean flag - whether to print file`s contents or not
// Return value/s:
//     -Max/Min value via max and min parameters
void find_min_max(FILE **file, uint16_t *max, uint16_t *min, int print){
  uint16_t number;
  if(fread(&number,sizeof(number),1,*file)) *min = *max = number;
  while(fread(&number, sizeof(number), 1, *file)){
    if(print) printf("%d\n", number);
    if(number > *max) *max = number;
    if(number < *min) *min = number;
  }
}

void help(){
  fprintf(stdout, "%s", "Usage: ./main [OPTION]... FILE\n \
  FILE is binary file consisting of uint16_t numbers\n \
  --max, Find max number in FILE\n \
  --min, Find min number in FILE\n \
  --print, Print FILE`s contents\n");
}

int main(int argc, char *argv[]){
  if(argc != 3){
    help();
    errx(0,"main.c");
  }
  
  FILE *file = fopen(argv[2], "rb");
  
  if(!file){
    err(0,"%s\n", argv[2]);
  }
  
  uint16_t min;
  uint16_t max;

  if((strcmp(argv[1], "--min")) == 0){
    find_min_max(&file,&max,&min,0);
    printf("%d\n", min);    
  }
  if((strcmp(argv[1], "--max")) == 0){
    find_min_max(&file,&max,&min,0);
    printf("%d\n", max);    
  }
  if((strcmp(argv[1], "--print")) == 0){
    find_min_max(&file,&max,&min,1);
  }

  fclose(file);
  exit(0);
}
