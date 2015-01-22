//TI3 - Uebung 10
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>//open + close
#include <unistd.h> //read+write

#define DECODE 0
#define ENCODE 1

int main(int argc, char *argv[])
{
  if(argc == 2){ //argumente ueberpruefen
    char mode;
    unsigned char divident[2]; //CRC16 = x 16 + x 15 + x 2 + 1
    // 1100 0000  0000 0101
    divident[0] = 192;
    divident[1] = 5;

    FILE *fp = fopen(argv[1],"r");
    if(fp != NULL){
      //.crc endung? --> letzte 4 Zeichen auf .crc ueberpruefen
      /*for(int i = 0; i<10;i++){printf("%c %d\n",argv[1][i],argv[1][i]);   fflush(stdout);    }*/ //string terminiert bei 0 - wer haette es gedacht..
      int len = 0;
      while(argv[1][len] != 0){
        len++;
      }
      if(
        argv[1][len-3] == '.' &&
        argv[1][len-2] == 'c' &&
        argv[1][len-1] == 'r' &&
        argv[1][len] == 'c'
      ){
        //check checksum
        mode = DECODE;
      }else{
        //create checksum
        //dateiinhalt um 2 leere (16x0) bytes erweitern
        //immer 2 bytes einlesen und durch divident teilen (mod 2) / bzw xor
        mode = ENCODE;
      }
      //neuer dateiname
      char filename[128];
      char input[2];
      memset(filename,0,sizeof filename);
      filename = argv[1];
      if(mode){ //ENCODE
        filename[len-3] = '.';
        filename[len-2] = 'c';
        filename[len-1] = 'r';
        filename[len] = 'c';
      }else{  //DECODE
        filename[len-3] = 0;
      }
      //create file
      FILE *outputf = fopen(filename,"w");

      
    }else{
      printf("File not found\n");
    }
  }else{
    printf("usage: %s <filename>\n",argv[0]);
  }
}
