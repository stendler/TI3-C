//TI3 - Uebung 10
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>//open + close
#include <unistd.h> //read+write
#include <string.h>
#include <math.h>

#define DECODE 0
#define ENCODE 1

int MODE;

void shiftLeft(unsigned char bits[3], int nr){
  for(int i = 1; i <= nr; i++){
    //shift the first byte nr times
    bits[0] <<= 1;
    //get bits which will get shifted out of the second byte
    if(bits[1] >= 128){
      //add those bits shifted out to first byte
      bits[0] += 1;
    }
    //shift 2nd byte
    bits[1] <<= 1;
    //get bits which will get shifted out of the third byte
    if(bits[2] >= 128){
      //add those bits shifted out to second byte
      bits[1] += 1;
    }
    //!---- do that in main plz --- care if 3rd byte will be empty --> refresh from file
    //shift 3rd byte
    bits[2] <<= 1;
  }
}

int sqr(int basis, int exponent){
  for(int i = 0; i<exponent;i++){
    basis *= basis;
  }
  return basis;
}

int main(int argc, char *argv[])
{
  if(argc == 2){ //argumente ueberpruefen
    unsigned char divisor[2]; //CRC16 = x 16 + x 15 + x 2 + 1
    // 1100 0000  0000 0101
    divisor[0] = 192;
    divisor[1] = 5;

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
    filename[i-4] = 0;
  }
  //DEBUG
  printf("filename %s\n\n",filename);

  char decodeChecksum[2];
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
        decodeChecksum[0] = fgetc(fp);
        decodeChecksum[1] = fgetc(fp);
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

  unsigned char bits[3]; // 0&1 sind divident 2 ist nachfueller
  bits[0] = (unsigned char)fgetc(outputf);
  bits[1] = (unsigned char)fgetc(outputf);
  bits[2] = (unsigned char)fgetc(outputf);

  unsigned char remain[2];
  unsigned char shiftCount, shift = 0; //zaehlt von 0 bis 8 -> bei 8 reset und char[2] bekommt neue werte
  while(bits[2] != 255){
      //shift bis eine 1 vorne steht ---- evtl auslagern ?!
        //zaehlen wie viele 0en vorne stehen
        for(int p=7;p>=0;p--){
          if(bits[0] > sqr(2,p)){ //sqr != ^
            //shiftCount++;
            shift++;
          }
        }
        if(8-shiftCount-shift <= 0){
          //bits <<= (unsigned char)(8-shiftCount);
          shiftLeft(bits,8-shiftCount);
          bits[2] = (unsigned char)fgetc(outputf);
          if(bits[2] == 255){
            //TODO no more shifts -- bits[0&1] are the remainder
            shiftCount = 0;
            //but one more xor and then break (so nothing more happens here, right?)
          }else{
            //shift the rest
            //bits <<= (shift-shiftCount);
            shiftLeft(bits,shift-shiftCount);
            shiftCount = shift - shiftCount;
          }
        }else{
          //bits <<= shift;
          shiftLeft(bits,shift);
          shiftCount += shift;
        }
      //xor von bits[0/1] und divisor[0/1]
      remain[0] = bits[0] ^ divisor[0];
      remain[1] = bits[1] ^ divisor[1];
      bits[0] = remain[0];
      bits[1] = remain[1];
  }

  //Mode:
  if(MODE){
  //encode --> remiander ans ende der datei haengen
    fputc((char)remain[0],outputf);
    fputc((char)remain[1],outputf);
  }else{
  //decode --> remainder mit decodeChecksum ueberpruefen und Ausgabe
    if(remain[0] == decodeChecksum[0] && remain[1] == decodeChecksum[1]){
      //erfolgreich
    }else{
      //datei wieder loeschen & warnung ausgeben
      printf("CRC-Checksum stimmt nicht mit dem Inhalt ueberein!\n");
      fclose(outputf);
      remove(filename);
      return 0;
    }
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
