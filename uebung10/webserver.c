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
    AF_INET, // Address Family - AF_INET (this is IP version 4) - AF-INET6 IPv6 - AF_UNSPEC
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
  memset(&saddr_server,0,sizeof saddr_server);
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
  listen(socket_descriptor,10);
  fputs("Listening...\n",stdout);
  fflush(stdout);

//loop
  char client_message[2000];
  while(1){ //maybe a close server command? but not yet...

//accept
    struct sockaddr_in saddr_client;
    socklen_t saddr_client_size = sizeof saddr_client;
    memset(&saddr_client,0,saddr_client_size);
    int client_socket = accept(socket_descriptor, (struct sockaddr *)&saddr_client,&saddr_client_size);
    if(client_socket < 0){
      fputs("Failed to accept\n",stderr);
      continue;
    }else{
      fputs("Connection established.\n",stdout);
      fflush(stdout);
    }

//fork? ---- kp - arbeiten wir besser nur einen aufruf zur gleichen zeit ab & das in der endlosschleife

//receive
    memset(&client_message,0,sizeof client_message);
    int rsize = recv(client_socket,client_message,sizeof(client_message),0);
    //while(rsize > 0){ // fuer ne einfache anfrage vom browser nicht mehr notwendig

// parse input header
      //DEBUG: output der browser anfrage
      fputs(client_message,stdout);
      fflush(stdout);
      //testing parse input
            

// return header + document (if there)

//close connection
    close(client_socket);
    fputs("Connection closed.\n",stdout);
    fflush(stdout);

  }
}
