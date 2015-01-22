//TI3 - Uebung 10
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>//open + close
#include <unistd.h> //read+write

#define DECODE 0
#define ENCODE 1

void byteToBits(char *bits[16], char byte, char byte2){
  //bits:   jedes element dieses arrays[16] stellt ein bit dar
  //byte:   byte das in eine bitfolge konvertiert werden soll
  unsigned char helpbyte = (unsigned char)byte;
/*  if(helpbyte >= 128){
    helpbyte -= 128;
    bits[0] = 1;
  }
  if(helpbyte >= 64){
    helpbyte -= 64;
    bits[1] = 1;
  }
  if(helpbyte >= 32){
    helpbyte -= 32;
    bits[2] = 1;
  }
  if(helpbyte >= 16){
    helpbyte -= 16;
    bits[3] = 1;
  }
  if(helpbyte >= 8){
    helpbyte -= 8;
    bits[4] = 1;
  }
  if(helpbyte >= 4){
    helpbyte -= 4;
    bits[5] = 1;
  }
  if(helpbyte >= 2){
    helpbyte -= 2;
    bits[6] = 1;
  }
  if(helpbyte >= 1){
    helpbyte -= 1;
    bits[7] = 1;
  }
  helpbyte = (unsigned char)byte2;
  if(helpbyte >= 128){
    helpbyte -= 128;
    bits[8] = 1;
  }
  if(helpbyte >= 64){
    helpbyte -= 64;
    bits[9] = 1;
  }
  if(helpbyte >= 32){
    helpbyte -= 32;
    bits[10] = 1;
  }
  if(helpbyte >= 16){
    helpbyte -= 16;
    bits[11] = 1;
  }
  if(helpbyte >= 8){
    helpbyte -= 8;
    bits[12] = 1;
  }
  if(helpbyte >= 4){
    helpbyte -= 4;
    bits[13] = 1;
  }
  if(helpbyte >= 2){
    helpbyte -= 2;
    bits[14] = 1;
  }
  if(helpbyte >= 1){
    helpbyte -= 1;
    bits[15] = 1;
  }*/
  char c = 128;
  for(int i = 0;i<16;i++){
    if(c == 1){
      c = 128;
    }
    if(helpbyte >= c){
      helpbyte -= c;
      c /= 2;
      bits[i] = 1;
    }
  }
}



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

      char bits[16];
      unsigned char input[2];
      input[0] = (unsigned char)fgetc(fp);
      if(input[0] == EOF){ //ende der datei erreicht beim ersten eingelesenen byte
        if(mode){
          input[0] = 0;
          input[1] = 0;
        }else{
          //error - datei zu klein
        }
      }else{
          input[1] = (unsigned char)fgetc(fp);
          if(input[1] == EOF){
            input[1]
          }
        }
      }
    }else{
      printf("File not found\n");
    }
  }else{
    printf("usage: %s <filename>\n",argv[0]);
  }
}
