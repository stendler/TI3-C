//TI3 - Uebung 10
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>//open + close
#include <unistd.h> //read+write
#include <string.h>

#define DECODE 0
#define ENCODE 1

int MODE;

int main(int argc, char *argv[])
{
  if(argc == 2){ //argumente ueberpruefen
    //char mode;
    /*unsigned char divident[2]; //CRC16 = x 16 + x 15 + x 2 + 1
    // 1100 0000  0000 0101
    divident[0] = 192;
    divident[1] = 5;*/

FILE *fp = fopen(argv[1],"r");
if(fp != NULL){
  //.crc endung? --> letzte 4 Zeichen auf .crc ueberpruefen
  /*for(int i = 0; i<10;i++){printf("%c %d\n",argv[1][i],argv[1][i]);   fflush(stdout);    }*/ //string terminiert bei 0 - wer haette es gedacht..
  int len = 0;
  while(argv[1][len] != 0){
    len++;
  }
  if(
    argv[1][len-4] == '.' &&
    argv[1][len-3] == 'c' &&
    argv[1][len-2] == 'r' &&
    argv[1][len-1] == 'c'
  ){
    //check checksum
    MODE = DECODE;
  }else{
    //create checksum
    //dateiinhalt um 2 leere (16x0) bytes erweitern
    //immer 2 bytes einlesen und durch divident teilen (mod 2) / bzw xor
    MODE = ENCODE;
  }
  printf("MODE: %d\n\n",MODE);
  //neuer dateiname
  char filename[128];
  memset(filename,0,sizeof filename);

  //filename = argv[1];
  int i = 0;
  while(argv[1][i] != 0 && i < 123){
    filename[i] = argv[1][i];
    i++;
  }
  if(MODE == 1){ //ENCODE
    filename[i] = '.';
    filename[i+1] = 'c';
    filename[i+2] = 'r';
    filename[i+3] = 'c';
  }else{  //DECODE
    filename[i] = 0;
  }
  //DEBUG
  printf("filename %s\n\n",filename);

  char remain[2];
  //create file
  FILE *outputf = fopen(filename,"w+");
  if(outputf != NULL){
  //write file
  if(MODE){
      //ENCODE : complete
      char buffer;
      while((buffer= fgetc(fp)) != EOF){
        fputc(buffer,outputf);
      }
  }else{
    //DECODE : all except the last 2 bytes
      char buffer;
      while ((buffer = fgetc(fp)) != EOF){
        fputc(buffer,outputf);
        remain[0] = fgetc(fp);
        remain[1] = fgetc(fp);
        buffer = fgetc(fp);
        if(buffer == EOF){
          break;
        }else{
          fseek(fp,-3,SEEK_CUR);
        }
      }
  }

  //DEBUG
  printf("File copied\n");
  //fseek
  if(fseek(outputf,0,SEEK_SET)!=0){
      printf("error seeking in file\n");
  }
  }else{
    printf("Could not create File %s\n",filename);
  }
  }else{
    printf("File not found\n");
  }
}else{
  printf("usage: %s <filename>\n",argv[0]);
}
}
