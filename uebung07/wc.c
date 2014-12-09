//TI3 - Uebung 05
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

/*
(b) Implementieren Sie ein Programm "wc", das einen Dateinamen als Parameter erhaelt und
die Anzahl der Zeilen, Worten und Bytes dieser Datei ausgibt.
*/

#include <stdio.h>

int main(int argc, , char *argv[]){
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
    //datei zeichenweise einlesen

    //fuer jedes zeichen byte zaehler incr fgetc
    //fuer jedes leerzeichen wort zaehler incr
    //fuer jedes (umm enter oder \0 ??) zeilen zaehler incr fgets
    //am ende der datei -> Zaehler ausgeben
  }else{
    //error- stderr
    fwrite("File not found.",128,1,stderr);
  }
  fclose(filestream);
}
