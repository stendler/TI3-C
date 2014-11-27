#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



/*Implementierung eines Schedulers:
Implementieren Sie die Scheduling Algorithmen Round Robin,First Come First Serve und
Shortest Process Next.

Sie muessen hierbei nur die vorgefertigten Rahmenmethoden

LINK roundRobin(LINK head,LINK current,int tStep);
LINK fcfs(LINK head,LINK current,int tStep);
LINK spn(LINK head,LINK current,int tStep);

implementieren.

TIPP:Machen Sie sich zunaechst mit dem Programmablauf vertraut und
implementieren Sie die Methoden anschliessend.
*/

#define PMAX 4 //Anzahl der Prozesse
#define TINT 500000 //Zeitintervall zwischen den Schritten des Algorithmus in Mikrosekunden
#define PDOTS 2//Anzahl der Punkte fuer die Anzeige eines Intervalls

struct pData //Daten der verketteten Liste
{
 int turns;//Wie oft war der Prozess schon dran
 int waited; //Wie lange musste er insgesamt warten
 int sTime;  //Wie viele Zeitschritte braucht er noch
 int nextStep;//wann moechte er als naechstes ausgefuehrt werden
 int pId;//ProzessID

 struct pData  *next;//Doppelt verkettete Liste
 struct pData  *prev;
};

//Um Tipparbeit zu sparen
typedef struct pData PROCESS;
typedef PROCESS *LINK;

double g_stats[2]={0,0};//Globale Variable zum Aufzeichnen der Wartezeiten

//Methode zur Ausgabe der Wartezeiten nach dem Ausfuehren des Algorithmus
void printStats()
{
 printf("\n=============================================\n");

 printf("\nDie durchschnittliche Wartezeit der Prozesse \n fuer den Algorithmus betraegt:%f Zeitschritte\n",g_stats[0]/PMAX);

 printf("\nDie durchschnittliche Gesamtwartezeit \n fuer den Algorithmus betraegt:%f Zeitschritte\n",g_stats[1]/PMAX);
 g_stats[0]=0;
 g_stats[1]=0;
}

//Methode zum updaten der stats fuer ein geloeschtes Element der Liste
void updateStats(LINK deleted)
{
 if(deleted->turns>0)
  g_stats[0]+=deleted->waited/deleted->turns;//durschnittliche Zeit die der Prozess nicht dran war

  g_stats[1]+=deleted->waited;//Wartezeit
}

void updateProcess(LINK current,int tStep);

LINK findShortestPr(LINK head);


void printSchedule(LINK current,int tStep);
void printScheduleHead(void);

LINK roundRobin(LINK head,LINK current,int tStep);
LINK fcfs(LINK head,LINK current,int tStep);
LINK spn(LINK head,LINK current,int tStep);
void schedule(LINK head,int processes[PMAX][2] ,LINK (*sAlgo)(LINK,LINK,int));

void addProcess(LINK head,int serviceTime,int tStep,int pId);
LINK deleteProcess(LINK current);

void printPr(LINK current);

void printList(LINK head);


int main(void)
{
LINK head=(LINK)malloc(sizeof(PROCESS));//das Kopfelement erzeugen
head->next=head;
head->prev=head;

/*Prozesse die gescheduled werden
sollen.Das erste Element gibt die Ankunftszeit an.Das zweite die Ausfuerungsdauer des
Prozesses in Zeitschritten
*/
int processes[PMAX][2]={{0,3},{2,7},{4,1},{6,5}};


//Aufruf der Methoden
printf("\n Ablauf von First Come First Serve \n");
printScheduleHead();
schedule(head,processes,&fcfs);
printStats();


printf("\n Ablauf von RoundRobin \n");
printScheduleHead();
schedule(head,processes,&roundRobin);
printStats();


printf("\n Ablauf von Shortest Process Next\n");
printScheduleHead();
schedule(head,processes,&spn);
printStats();

return 0;
}

/*==========================================================================
Methoden fuer die Realisierung des Schedulers*/


//Einen Prozess an das Ende der Liste (also vor head) einfuegen
void addProcess(LINK head,int serviceTime,int tStep,int pId)
{
 //Neues Element erzeugen und initialisieren
 LINK neu=(LINK)malloc(sizeof(PROCESS));

 neu->turns=0;
 neu->waited=0;
 neu->sTime=serviceTime;
 neu->nextStep=tStep;
 neu->pId=pId;

 //Zeiger so setzen,dass es das letzte Element ist
 head->prev->next=neu;
 neu->prev=head->prev;

 head->prev=neu;
 neu->next=head;
}

//Loeschen des angegebenen Knotens
LINK deleteProcess(LINK current)
{
 //Update der Stats fuer diesen Knoten
 updateStats(current);

 //naechsten Knoten zwischenspeichern
 LINK tmp=current->next;
 //Zeiger um den zu loeschenden Knoten herumsetzen
 current->next->prev=current->prev;
 current->prev->next=current->next;

 //Speicher freigeben
 free(current);
 current=NULL;

 return tmp;
}


/*Diese Methode ist fuer das eigentliche schedulen einr Methode zustaendig die die
angegebene Signatur hat.Als Argumente erhaelt sie einen Zeiger auf die verkettete
Prozessliste,die auszufuehrenden Prozess und einen Zeiger auf die auszufuehrende Funktion
*/
void schedule(LINK head,int processes[PMAX][2] ,LINK (*sAlgo) (LINK,LINK,int))
{

 LINK current=head;
 int nextPrcs=0;
 int tStep=0;//aktueller Zeitschritt,der nach jeder Iteration erhoeht wird

 /*Fuehre das scheduling durch,solange noch Prozesse kommen werden oder
   noch Prozesse laufen
 */
  while(nextPrcs<PMAX||head->next!=head)
  {

   if(current==head)
    current=current->next;

   /*Hole alle Prozesse ab,die im aktuellen Zeitschritt starten*/
    while((nextPrcs<PMAX)&&(processes[nextPrcs][0]==tStep))
    {
     addProcess(head,processes[nextPrcs][1],processes[nextPrcs][0],nextPrcs);
     if(current==head)
    	current=head->prev;
     nextPrcs++;
    }


   if(current!=head)
    current=sAlgo(head,current,tStep);
   tStep++;
   usleep(TINT);
  }
}

/*Diese Methode fuehrt den Prozess current aus indem die serviceTime
dekrementiert wird und die restlichen Elemente aktualisiert werden
*/
void updateProcess(LINK current,int tStep)
{
 printSchedule(current,tStep);
 current->turns++;
 current->waited+=tStep-current->nextStep;
 current->nextStep=tStep+1;
 current->sTime--;
}

/*Implementierung des Round Robin Algorithmus
head:Kopfelement der Liste der Prozesse
current:aktuelles Element
tStep:aktueller Zeitschritt
*/

LINK roundRobin(LINK head,LINK current,int tStep)
{
 updateProcess(current,tStep);

/*TODO: Implementieren Sie den Round Robin Algorithmus
		Unterscheiden Sie die Faelle, falls der Prozess fertig ist
		oder eben noch nicht.
*/

 return current;
}


/*Implementierung des FCFS

head:Kopfelement der Liste der Prozesse
current:aktuelles Element
tStep:aktueller Zeitschritt
*/
LINK fcfs(LINK head,LINK current,int tStep)
{
 updateProcess(current,tStep);

/*TODO: Implementieren Sie den First Come First Serve Algorithmus
		Beachten Sie den Rueckgabewert der deleteProcess Methode
*/

 return current;
}


/*Implementierung des spn
head:Kopfelement der Liste der Prozesse
current:aktuelles Element
tStep:aktueller Zeitschritt
*/
LINK spn(LINK head,LINK current,int tStep)
{
 updateProcess(current,tStep);

/*TODO: Implementieren Sie die Shortest Job Next Algorithmus
		Nutzen Sie die untere Hilfsfunktion!
*/


 return current;
}

/*Hilfsfunktion fuer den spn Algorithmus*/

LINK findShortestPr(LINK head)
{
     LINK curr=head->next;
     LINK minCurr=curr;
     int sMin=curr->sTime;
     while(curr!=head)
     {
      if(sMin>curr->sTime)
      {
         sMin=curr->sTime;
         minCurr=curr;
      }
      curr=curr->next;
     }
     return minCurr;
}

/*Anzeigemethoden fuer die einzelnen Prozesse*/
void printSchedule(LINK current,int tStep)
{
 int i;
 char *out=(char*)malloc(7*sizeof(char)*current->pId+10) ;
 strcpy(out,"\t");
 for(i=0;i<current->pId;i++)
 {
  strcat(out," \t    ");
 }
 strcat(out,".\n ");

 printf("%d%s",tStep,out);
 for(i=0;i<PDOTS;i++)
 {
  printf(" %s",out);
 }
 free(out);
}


void printScheduleHead(void)
{
 int i;
 char * out=(char*)malloc(10+sizeof(char)*10*PMAX);

 strcpy(out," tstep");

 for(i=0;i<PMAX;i++)
 {
  sprintf(out,"%s\t Pid %d ",out,i);
 }

 printf("\n=============================================\n");
 printf("%s \n ",out);
 free(out);
}


/*Ausgabemethoden fuer die Prozesse und die gesamte Liste (DEBUG-Methoden)*/
void printPr(LINK current)
{
 printf("\n pId:%d turns:%d waited:%d sTime:%d nextStep:%d \n ",current->pId,current->turns,current->waited,current->sTime,current->nextStep);
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
