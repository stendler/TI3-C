//Tutor: Thomas
//Bearbeiter: Jasmine Cavael, Maximilian Stendler
/*Uebung 03
****Aufgabe 2b)
*/

#include <stdlib.h>
#include <fcntl.h>//open + close
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> //read+write

int BUFFERSIZE = 10;

int copy(char *sourcename, char *targetname)
{
    //neue Datei erstellen : targetname
    //open() target //ueberpruefen, ob targetname im Verzeichnis vorhanden
    int target = open(targetname,O_RDWR);
    int source = open(sourcename,O_RDONLY);

    if(target >= 0 || source < 0){
      return -1;
    }else{
      //open() source
      target = open(targetname,O_RDWR|O_CREAT);

      //dateiinhalt von source kopieren
      //puffer?
      char buffer[BUFFERSIZE];
      int buff;
      //LOOP
      while((buff = read(source,buffer,BUFFERSIZE))){ //read from source -> puffer
        write(target,buffer,buff); //write from puffer -> target
      }

      close(target);
      close(source);
    }

    return 0;
}

int main(int argc, char *argv[])
{
  if(argc == 3){
    copy(argv[1],argv[2]);
    return EXIT_SUCCESS;
  }else{
    return EXIT_FAILURE;
  }

}
