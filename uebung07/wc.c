//TI3 - Uebung 07
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

/*
(b) Implementieren Sie ein Programm "wc", das einen Dateinamen als Parameter erhaelt und
die Anzahl der Zeilen, Worten und Bytes dieser Datei ausgibt.
*/

#include <stdio.h>

int main(int argc, char *argv[]){
  //Anzahl der Parameter ueberpruefen
  //char* inputfile = "";
  void* filestream;
  if(argc > 1){ //Parameter wurde direkt uebergeben
    filestream = fopen(argv[1],"r");
  }else{ //ueberpruefen, ob dateiname evtl in stdin?  //TODO: Soll denn ueberhaupt der Dateiname hierdrin sein? Oder bereits die ganze Datei?
    //char inputfile[128] = "";
    //fgets(inputfile,sizeof(inputfile),stdin);
    //printf("%s\n",inputfile);
    //filestream = fopen(inputfile,"r");

    filestream = stdin;
  }
  //datei oeffnen

  //ueberpruefen, ob valide
  if(filestream){
    //datei blockweise einlesen
    char buffer[1024];
    //char chr = ' ';
    int c = 0;
    int w = 0;
    int l = 0;
    while(fgets(buffer,1024,filestream)){
      l++; //fuer jedes (umm enter oder \0 ??) zeilen zaehler incr fgets
    /*  while(chr=fgetc(buffer)){
        c++;  //fuer jedes zeichen byte zaehler incr fgetc
        if(chr==' '){
          w++;  //fuer jedes leerzeichen wort zaehler incr
        }
      }*/
      for(int i=0; i<sizeof(buffer);i++){
        if(buffer[i]==0 || buffer[i]==127){
          break;
        }
        c++;
        if(buffer[i]==32){
          w++;
        }
      }
    }
    if(l>0){
      w++;
    }
    /*fseek(filestream,0,SEEK_SET);
    while( (chr=getc(filestream)) != EOF){
      c++;  //fuer jedes zeichen byte zaehler incr fgetc
      printf("%c",chr);
      //fflush(stdin);
      if(chr==32){
        printf(" _ %i\n",w);
        fflush(stdin);
        w++;  //fuer jedes leerzeichen wort zaehler incr
      }
    }*/
    //fseek(filestream,0,SEEK_SET);
    //while(ftell(filestream)!=)
    printf("Zeilen: %i, Worte: %i, Bytes: %i\n",l,w,c);
    //fputs(stdout,"Zeilen: %i, Worte: %i, Bytes: %i\n",l,w,c);
    //datei zeichenweise einlesen
        //am ende der datei -> Zaehler ausgeben
  }else{
    //error- stderr
    fputs("File not found.",stderr);
  }
  fclose(filestream);
}
