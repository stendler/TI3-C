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

void byteToBits(char bits[16], unsigned char byte, unsigned char byte2)
{
  //bits:   jedes element dieses arrays[16] stellt ein bit dar
  //byte:   byte das in eine bitfolge konvertiert werden soll
  unsigned char helpbyte = byte;
  unsigned char c = 128;
  for(int i = 0;i<16;i++){
    if(c == 1){
      c = 128;
      helpbyte = byte2;
    }
    if(helpbyte >= c){
      helpbyte -= c;
      c /= 2;
      bits[i] = 1;
    }
  }
}

int refillBits(char bits[16], FILE *fp){
    memset(&bits,0,sizeof(bits));
    //TODO DECODE mode?
    unsigned char next1, next2, next3;
    next1 = (unsigned char)fgetc(fp);
    next2 = (unsigned char)fgetc(fp);
    next3 = (unsigned char)fgetc(fp);
    byteToBits(bits,next1,next2);
    if(next3 == EOF){
      return 1;
    }else{
      return 0;
    }
}

int shiftBits(char bits[16],char bits2[16],FILE *fp){
  int i,ret = 0;
  //count how many leading zeros bits has
  while(bits[i] == 0){
    i++;
  }
  int shift = i;
  //shift the first 1 to the most left
  //&& take the rest from bits2
  while(i<16){
    bits[i-shift] = bits[i];
    if(bits2[i-shift] == -1){
      ret = refillBits(bits2,fp);
    }
    bits[i] = bits2[i-shift];
    //shift bits2
    bits2[i-shift] = -1;
    i++;
  }
  return ret;
}

void xor(char divident[16],char divisor[16]){
  for(int i = 0; i < 16; i++){
    if(divident[i] == dividor[i]){
      divident[i] = 1;
    }else{
      divident[i] = 0;
    }
  }
}


int main(int argc, char *argv[])
{
  if(argc == 2){ //argumente ueberpruefen
    //char mode;
    /*unsigned char divident[2]; //CRC16 = x 16 + x 15 + x 2 + 1
    // 1100 0000  0000 0101
    divident[0] = 192;
    divident[1] = 5;*/
    char divisor[16];
    byteToBits(divisor,192,5);

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
        MODE = DECODE;
      }else{
        //create checksum
        //dateiinhalt um 2 leere (16x0) bytes erweitern
        //immer 2 bytes einlesen und durch divident teilen (mod 2) / bzw xor
        MODE = ENCODE;
      }
      //neuer dateiname
      char filename[128];
      memset(filename,0,sizeof filename);

      //filename = argv[1];
      int i = 0;
      while(argv[1][i] != 0 && i < 123){
        filename[i] = argv[1][i];
        i++;
      }
      if(MODE){ //ENCODE
        filename[i+1] = '.';
        filename[i+2] = 'c';
        filename[i+3] = 'r';
        filename[i+4] = 'c';
      }else{  //DECODE
        filename[i+1] = 0;
      }

      char remain[2];
      //create file
      FILE *outputf = fopen(filename,"w");

      //write file
      if(MODE){
          //ENCODE : complete
          char buffer;
          while((buffer= fgetc(fp)) != EOF){
            fputc(buffer,outputf);
          }
      }else{
        //DECODE : all except the last 2 bytes
          char buffer,next,next2;
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

      //fseek
      fseek(outputf,0,SEEK_SET);

      char bits[16], queue[16];
      refillBits(bits,outputf);
      refillBits(queue,outputf);

      //algorithm
      int decMode = 0;
      while(queue[0] != -1){
        xor(bits,divisor);
        decMode = shiftBits(bits,queue,outputf);
  //      if(decMode && !MODE){

    //    }
      }

      //ENCODE --> add remainder to file

      //DECODE check remainder

    }else{
      printf("File not found\n");
    }
  }else{
    printf("usage: %s <filename>\n",argv[0]);
  }
}
