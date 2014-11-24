//TI3 - Uebung 04
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>//open + close
#include <unistd.h> //read+write


/*Implementierung einer Prozessverwaltung:

Implementieren Sie die durch das Framework vorgegebene Prozessverwaltung,
die eine Menge von Prozessen aus der Datei prcs.dat auslesen,und diese
in eine doppelt verkettete Liste einfuegen soll.

Nun soll Prozess um Prozess der Reihenfolge ihrer ids entsprechend
aus der Liste entfernt werden,wobei die uebrig gebliebenen Prozesse in jeder
Iteration ausgegeben werden.

Die Prozesse sind in der Datei prcs.dat Zeile fuer Zeile in der Form:
    ProzessId,Ankunftszeit,Ausfuehrungszeit
gespeichert.

*/

struct pData //Daten der doppelt verketteten Liste
{
 int pId;//ProzessID
 int aTime;//Ankunftszeit
 int sTime;//Ausfuehrungszeit

  /*Struktur vervollstaendigen */
  struct pData* prev;
  struct pData* next;
};

//Um Tipparbeit zu sparen
typedef struct pData PROCESS;
typedef PROCESS *LINK;

LINK findNextProcess(LINK head);

void addProcess(LINK head,LINK neu);
void deleteProcess(LINK current);

void printPr(LINK current);//Methode zur Ausgabe eines Prozesses

void printList(LINK head);//Alle Prozesse der Liste ausgeben

void readProcesses(LINK head);//Einlesen der Prozesse

int main(void)
{
 LINK next;
 /*head initialisieren*/
 LINK head = (LINK)malloc(sizeof(PROCESS));
 //DEBUG //printf("head malloc");
  //prev und next
  head->prev = head;
  head->next = head;  //printf("head prev/next");
 readProcesses(head);
 //DEBUG //printf("Prozessliste gelesen\n");
 while(head->next!=head)
 {
   printList(head);
   //DEBUG   //printf("LIST printed\n");
   next=findNextProcess(head);
   //DEBUG  // printf("found next process\n");
   deleteProcess(next);// printf("next deleted\n");
 }
 return 0;
}

/*==========================================================================
Implementierung der Methoden*/


//Liest die Prozesse ein und erstellt die Liste
void readProcesses(LINK head)
{
 /*TODO:WIP - clean up needed*/
 int source = open("prcs.dat.txt",O_RDONLY);
 char buffer[1];
 int breakbool = 0;
  LINK curr = head;
   //DEBUG
   //printf("curr=head\n");
 while(read(source,buffer,1)){
   while(*buffer<49 || *buffer>57){ //solange der eingelesene speicher KEINE Zahl
     //DEBUG
     //printf("%c , %d\n",*buffer,*buffer);
     if(!read(source,buffer,1)){
       breakbool = 1;
       break;
     }
   }
   if(breakbool>1){
     break;
   }
   int pId = *buffer;
   //DEBUG
   //printf("%c , %d\n",*buffer,*buffer);
   read(source,buffer,1);
   while(*buffer<49 || *buffer>57){ //solange der eingelesene speicher KEINE Zahl
     //DEBUG
     //printf("%c , %d\n",*buffer,*buffer);
     if(!read(source,buffer,1)){
       breakbool = 1;
       break;
     }
   }
   if(breakbool>1){
     break;
   }
   //DEBUG
   //printf("%c , %d\n",*buffer,*buffer);
   int ankunftszeit = *buffer;
   read(source,buffer,1);
   while(*buffer<49 || *buffer>57){ //solange der eingelesene speicher KEINE Zahl
     //DEBUG
     //printf("%c , %d\n",*buffer,*buffer);
     if(!read(source,buffer,1)){
       breakbool = 1;
       break;
     }
   }
   if(breakbool>1){
     break;
   }
   //DEBUG
   //printf("%c , %d\n",*buffer,*buffer);
   int laufzeit = *buffer;
    //DEBUG //printf("nex eingelesen\n");
   LINK nex = (LINK)malloc(sizeof(PROCESS));
    //Werte dem Prozess zuweisen
    nex->pId = pId;
    nex->aTime = ankunftszeit;
    nex->sTime = laufzeit;
    nex->next = head;
    nex->prev = curr;
    //DEBUG//printf("nex malloc\n");
    addProcess(head,nex);
    //DEBUG//printf("process added\n");//printPr(nex);
    curr = nex;
    if(curr == head){
      break;
    }
 }
}

//Einen Prozess an das Ende der Liste (also vor head) einfuegen
void addProcess(LINK head,LINK neu)
{
 //printf("nachfolger von neu = head");
 neu->next = head;
 //printf("vorganger von neu = vorgaenger von head");
 neu->prev = head->prev;
 //printf("vorgaenger von head = neu");
 head->prev = neu;
 //printf("nachfolger des vorgaengers von neu = neu");
 neu->prev->next = neu;
}

//Loeschen des angegebenen Knotens
void deleteProcess(LINK current)
{
  //DEBUG
  //printf("Loesch jetzt: ");
  //printPr(current);
  //DEBUG
  //printf("Nachfolger von current:");
  //printPr(current->next);
 current->next->prev = current->prev;
 //DEBUG
 //printf("Bekommt neuen vorgaenger:");
 //printPr(current->prev);
 //DEBUG
 //printf("Anstatt von");
 //printPr(current->next->prev);
 //DEBUG
 //printf("vorgaenger von current:");
 //printPr(current->prev);
 current->prev->next = current->next;
 //DEBUG
 //printf("bekommt neuen nachfolger:");
 //printPr(current->next);
 //DEBUG
 //printf("anstatt von:");
 //printPr(current->prev->next);
 free(current);
}


/*gibt den Prozess mit der kleinsten Id aus der Liste zurueck*/

LINK findNextProcess(LINK head)
{
 LINK ret = head->next;
 LINK iterator = head->next->next;
 while(iterator!=head){ //gehe die Liste durch
   if(iterator->pId < ret->pId){ // wenn das derzeitige element kleiner ist als ret
     ret = iterator;            //ist ret nun das kleinere element
   }
   iterator = iterator->next;
 }
 //DEBUG//printf("Ist der Kleinste: ");//printPr(ret);
 return ret;
}


/*Ausgabemethoden fuer die Prozesse und die gesamte Liste*/
void printPr(LINK current)
{
 printf("\n pId:%c arrival time:%c service time:%c\n ",current->pId,current->aTime,current->sTime);
}

void printList(LINK head)
{
     printf("\n\n Current QUEUE of PROCESSES ---------------------------------------\n\n");
     LINK curr=head->next;
     while(curr!=head)
     {
      printPr(curr);
      curr=curr->next;
     }
}
/*


         ---<>---
 /    --<        >--    \
/| --<      «»      >-- |\
\\<        «##»        >//
 \\--<      «»      >--//
  \\  --<        >--  //
   \\    ---<>---    //
    \\______________//
    |----------------|
    |\/--\/----\/--\/|
    |/\--/\----/\--/\|
    |
    |
    |
    |
    |
    |
    |
    |
    |
    |
    |
    |




    */
