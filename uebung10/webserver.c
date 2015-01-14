//TI3 - Uebung 10
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h> //write

int main(int argc, char *argv[])
{
// geruest fuer den webserver

// socket
  int socket_descriptor = socket(
    AF_UNSPEC, // Address Family - AF_INET (this is IP version 4) - AF-INET6 IPv6 - AF_UNSPEC
    SOCK_STREAM, // Type - SOCK_STREAM (this means connection oriented TCP protocol)
    0 // Protocol - 0 [ or IPPROTO_IP This is IP protocol]
  );
  //Fehlerbehandlung
  if(socket_descriptor == -1){
    //failed to create socket
    fputs("Failed to create socket.",stderr);
    return 2;
  }else{
    fputs("Socket created.\n",stdout);
    fflush(stdout);
  }

// port - bind
  struct sockaddr_in saddr_server;
  saddr_server.sin_family = AF_INET;
  saddr_server.sin_addr.s_addr = INADDR_ANY;
  saddr_server.sin_port = htons(8080);

  if(bind(socket_descriptor,(struct sockaddr *)&saddr_server,sizeof(saddr_server)) < 0){
    fputs("Failed to bind.",stderr);
    //alternative: perror("Failed to bind."); ?!
    return 3;
  }else{
    fputs("Bind - check.\n",stdout);
    fflush(stdout);
  }

// listen
  listen(socket_descriptor,3);

//loop
  
//accept

//fork?

// parse input header

// return header + document (if there)


}
