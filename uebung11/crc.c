//TI3 - Uebung 10
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>//open + close
#include <unistd.h> //read+write

int main(int argc, char *argv[])
{
  if(argc == 2){ //argumente ueberpruefen
    char *divident[2];
    //divident[0] =
    //divident[1] =

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

    }else{
        //create checksum
        //dateiinhalt um 2 leere (16x0) bytes erweitern
        //immer 2 bytes einlesen und durch divident teilen (mod 2) / bzw xor

    }

    //if((strstr(argv[1],".crc")) != NULL){

    //}else{
    //}
  }else{
    printf("usage: %s <filename>\n",argv[0]);
  }
}
