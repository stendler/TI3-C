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
  char* inputfile;
  if(argc > 1){ //Parameter wurde direkt uebergeben
    //DEBUG
    printf("Parameter gefunden\n");
    fflush(stdin);


    inputfile = argv[1];
  }else{ //ueberpruefen, ob dateiname evtl in stdin?
    //DEBUG
    //printf("suche in stdin\n");
    //fflush(stdin);

    fgets(inputfile,sizeof(inputfile),stdin);
  }
  //datei oeffnen

  //DEBUG
  printf("Datei oeffnen\n");
  fflush(stdin);

  void* filestream = fopen(inputfile,"r");
  //ueberpruefen, ob valide
  if(filestream){
    //datei blockweise einlesen
    //DEBUG
    printf("einlesen\n");
    fflush(stdin);

    char buffer[1024];
    while(fgets(buffer,sizeof(buffer),filestream)){
    //datei blockweise in stdout schreiben
      //fwrite(stdout,1024,1,&buffer);
      fputs(buffer,stdout);
    }
    //statt blockweise zeilenweise?
  }else{
    //or error - stderr
    fwrite("File not found.\n",128,1,stderr);
  }
  fclose(filestream);
}
