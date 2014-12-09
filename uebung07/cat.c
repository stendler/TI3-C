//TI3 - Uebung 07
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

/*
(a) Implementieren Sie ein Programm "cat", das einen Dateinamen als Parameter erhaelt und
den Inhalt dieser Datei ausgibt.
*/

#include <stdio.h>

int main(int argc, char *argv[]){
  //Anzahl der Parameter ueberpruefen
  char* inputfile = "";
  if(argc > 1){ //Parameter wurde direkt uebergeben
    inputfile = argv[1];
  }else{ //ueberpruefen, ob dateiname evtl in stdin?
    fread(inputfile,128,1,stdin);
  }
  //datei oeffnen
  void* filestream = fopen(inputfile,"r");
  //ueberpruefen, ob valide
  if(filestream){
    //datei blockweise einlesen
    //char* buffer[1024];
    while(fread(stdout,1024,1,filestream)){
    //datei blockweise in stdout schreiben
      //fwrite(stdout,1024,1,buffer);
    }
  }else{
    //or error - stderr
    fwrite("File not found.",128,1,stderr);
  }
  fclose(filestream);
}
