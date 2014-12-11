//TI3 - Uebung 05
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

/*
(c) Implementieren Sie ein Programm "grep", das einen String und einen Dateinamen als Pa-
rameter erhaelt und alle Zeilen aus der Datei ausgibt, die diesen String enthalten.
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
  void* filestream;
  char* inputstring;
  //Anzahl der Parameter ueberpruefen
  if(argc > 1){
    inputstring = argv[1];
    if(argc > 2){ //Parameter wurde direkt uebergeben
      filestream = fopen(argv[2],"r");
    }else{ //ueberpruefen, ob dateiname evtl in stdin?  //TODO: Soll denn ueberhaupt der Dateiname hierdrin sein? Oder bereits die ganze Datei?
      char inputfile[128] = "";
      fgets(inputfile,sizeof(inputfile),stdin);
      //DEBUG
      printf("%s\n",inputfile);
      filestream = fopen(inputfile,"r");
    }
  }
  //ueberpruefen, ob valide
  if(filestream){
    //liest datei zeilenweise ein
    char buffer[1024];
    while(fgets(buffer,1024,filestream)){
    //ueberprueft, ob eingabe string innerhalb der Zeile
      if(strstr(buffer,inputstring)){ //smthng like elem(buffer,inputstring); | find(buffer,inputstring);
      //falls ja zeile in stdout schreiben
      fputs(buffer,stdout);
      }
    }
  }else{
    //parameter in stdin zu finden?
    //else error
    fwrite("File not found.\n",128,1,stderr);
  }
    return 0;
  }
