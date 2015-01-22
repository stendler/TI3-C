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
    //.crc endung? --> letztes vorkommen eines . (wenn ueberhaupt) und die naechsten 3 Zeichen ueberpruefen
    if((strstr(argv[1],".crc")) != NULL){
      //check checksum
    }else{
      //create checksum
    }
  }else{
    printf("usage: crc <filename>");
  }
}
