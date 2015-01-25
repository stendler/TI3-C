//TI3 - Uebung 12
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

/*
Begriffserklaerungen:

Traceroute: Traceroute ist ein Programm, welches eine Route bis zu einem gewuenschten Host anzeigt.
            Dies wird dadurch realisisert, dass ein Internet-Paket mit einem TTL von 1 versendet wird, wodurch es
            vom ersten Router, der dieses Paket bekommt verworfen wird. Der Router schickt eine Fehlermeldung zurueck.
            Dies wird jeweils mit einem inkrementierten TTL wiederholt, bis der gewuenschte Host erreicht wird.
            Alle Router, die dabei einen Fehler gemeldet haben, bilden dann eine Route.
            https://en.wikipedia.org/wiki/Traceroute

Time-To-Live(TTL): TTL in IP Paketen bezeichnet die uebrige Lebensdauer eines Paketes bis es verworfen wird.
                  Jeder Hop, der dieses Paket erhaelt dekrementiert den TTL um 1 und verwirft das Paket, wenn der TTL 0 erreicht.
                  Zusaetzlich wird ein Fehler dem urspruenglichen Sender des Pakets uebermittelt.
                  Diese Methode wird verwende, damit Pakete, die ihr Ziel nicht finden, nicht das Netzwerk fluten können.
                  https://en.wikipedia.org/wiki/Time_to_live

Internet Control Message Protocol (ICMP): Das ICMP wird von Netzwerk Geraeten dazu verwendet, um Fehlerberichte zu uebermitteln.
            Sollte z.B. der TTL eines IP Pakets 0 erreichen, dann wird der Hop einen Fehler mittels ICMP versenden.
            In dem 8-Byte Header vom ICMP wird unter anderem der Typ des Fehlers angegeben.
            https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol

Raw-Socket: Raw-Sockets sind Internet-Sockets, die nicht wie Standart-Sockets auf dem Transport Layer aufsetzen, sondern
            einen Layer tiefer Raw-Packets inklusive header senden und empfangen koennen.

*/

//CODE - SOURCES:
//http://www.binarytides.com/raw-sockets-c-code-linux/
////http://www.binarytides.com/tcp-syn-portscan-in-c-with-linux-sockets/

//includes
#include <stdio.h>	//for printf
#include <string.h> //memset
#include <sys/socket.h>	//for socket ofcourse
#include <stdlib.h> //for exit(0);
#include <errno.h> //For errno - the error number
#include <netinet/tcp.h>	//Provides declarations for tcp header
#include <netinet/ip.h>	//Provides declarations for ip header
//
//
//


//main
int main(int argc, char *argv[])
{
  //check parameter -> hostname / ip-address - TODO: - maybe an additional optional argument for a custom port?
  if(argc == 2){

  //create send socket
  int sd = socket (PF_INET, SOCK_RAW, IPPROTO_TCP); //IPv4,Raw-socket, IPPROTO_TCP/RAW
  if(sd == -1)
  {
    //socket creation failed, may be because of non-root privileges
    perror("Failed to create socket - do you tried it with root?");
    exit(1);
  }
  //create listener socket

  //unsigned char ttl = 1;
  //while loop

//send custom ip header with udp header in body and ttl
//multiple simultanously ?

//receive ICMP packets -> print hopname/ip & timestamp(?)

//else (port unreachable) -> end we got our host
  }else{
    //nicht genuegend argumente
    //print usage
  }
}
