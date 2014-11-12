/**
 * File: trashcan.c
 * TI 3 - Betriebs- und Kommunikationssysteme
 * Freie Universitaet Berlin
 //Tutor: Thomas
 //Bearbeiter: Jasmine Cavael, Maximilian Stendler
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE			1024
#define PATHSIZE		256
#define TRASHFOLDER	".ti3_trash"


char copy_buffer[BUFSIZE];


/* 'copy' kopiert den Dateiinhalt einer Datei namens
 * "source". Eine Kopie target wird nur erzeugt, wenn
 * eine Datei "target" noch nicht existiert.
 * Die Zugriffsrechte werden nicht kopiert, sondern
 * auf "rwx --- ---" gesetzt. --------------------------------------????????????
 */
int copy(char *sourcename, char *targetname)
{
  //open() target //ueberpruefen, ob targetname im Verzeichnis vorhanden
  int target = open(targetname,O_RDWR);
  int source = open(sourcename,O_RDONLY);

  if(source < 0){
    close(source);
    close(target);
    return -1;
  }else if(target >= 0){
    close(source);
    close(target);
    return -2;
  }else{
    close(target);
    //neue Datei erstellen : targetname
    target = open(targetname,O_RDWR|O_CREAT);

    //dateiinhalt von source kopieren
    int buff;
    //LOOP
    while((buff = read(source,copy_buffer,BUFSIZE))){ //read from source -> puffer
      write(target,copy_buffer,buff); //write from puffer -> target
    }
    close(target);
    close(source);
  }
  return 0;
}


char parse_command(char *command)
{
  return command[1];
}


/* erzeugt einen Ordner foldername */
int setup_trashcan(char *foldername)
{
  mkdir(foldername);
  return 0;
}

/* führt trashcan -p[ut] filename aus */
int put_file(char *foldername, char *filename)
{
  char* pfad = foldername;
  strcat(pfad,filename);
  int ret = copy(filename,pfad);
  if(ret){
    //delete old file
    if(!remove(filename)){
      ret = -3;
    }
    return ret;
  }else{
    return ret;
  }
}

/* führt trashcan -g[et] filename aus */
int get_file(char *foldername, char *filename)
{
  //TODO: ist Datei bereits im verzeichnis?
  //falls ja: ABBRUCH!
  char* pfad = foldername;
  strcat(pfad,filename);
  int ret = copy(pfad,filename);
  if(ret){
    //delete old file
    if(!remove(pfad)){
        ret = -3;
    }
    return ret;
  }else{
    return ret;

}

/* führt trashcan -r[emove] filename aus */
int remove_file(char *foldername, char *filename)
{
  char* pfad = foldername;
  strcat(pfad,filename);
  return remove(pfad);
}


int main(int argc, char *argv[])
{
  if (argc == 1) {
    printf("...not enough arguments!\n");
    return EXIT_FAILURE;
  } else {
    setup_trashcan(TRASHFOLDER);
    char command = parse_command(argv[1]);
    switch (command) {
      case 'p':
        if (argc != 3) {
          printf("...not enough arguments!\n");
          return EXIT_FAILURE;
        }
        int p_error = put_file(TRASHFOLDER, argv[2]);
        switch (p_error) {
          case -1:
            printf("...source file not found!\n");
            break;
          case -2:
            printf("...trash file was not created!\n");
            break;
          case -3:
            printf("...source file could not be removed!\n");
          default:
            break;
        }
        break;
      case 'g':
        if (argc != 3) {
          printf("...not enough arguments!\n");
          return EXIT_FAILURE;
        }
        int g_error = get_file(TRASHFOLDER, argv[2]);
        switch (g_error) {
          case -1:
            printf("...trash file not found!\n");
            break;
          case -2:
            printf("...restore file was not created!\n");
            break;
          case -3:
            printf("...trash file could not be removed!\n");
          default:
            break;
        }
        break;
      case 'r':
        if (argc != 3) {
          printf("...not enough arguments!\n");
          return EXIT_FAILURE;
        }
        int r_error = remove_file(TRASHFOLDER, argv[2]);
        if (r_error)
          printf("...trash file could not be removed!\n");
        break;
      default :
        printf("...unknown command!\n");
        return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
