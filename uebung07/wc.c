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
  if(argc > 1){ //Parameter wurde direkt uebergeben
    //datei oeffnen

    //ueberpruefen, ob valide

    //datei zeichenweise einlesen
    //fuer jedes zeichen byte zaehler incr
    //fuer jedes leerzeichen wort zaehler incr
    //fuer jedes (umm enter oder \0 ??) zeilen zaehler incr

    //am ende der datei -> Zaehler ausgeben
  }else{
    //stdin checken

    //same

    //bzw error
  }

}
