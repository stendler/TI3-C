#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h> //write
//TI3 - Uebung 09
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler
// Ver 1.5
//main() - creates a tcp/ip4 socket
int main(int argc, char *argv[])
{
// 1 Parameter (Portnr.)
  if(argc < 2){
    fputs("No Parameter. At least Portnumber needed.",stderr);
    fflush(stdout);
    return 1;
  }else{
    int port;
    int socket_descriptor, client_socket;
    struct sockaddr_in server, client;
    char client_message[2000];

    //leeren des variablenspeichers
    memset(&server,0,sizeof server);
    memset(&client,0,sizeof client);


    //read Parameter
    port = atoi(argv[1]); //string to int?!?!
    if(port <= 0){
      perror("Parameter input invalid.");
      exit(-5);
    }
    printf("Port is %i \n",port);
    fflush(stdout);
    //create socket
    socket_descriptor = socket(
      AF_INET, // Address Family - AF_INET (this is IP version 4) - AF-INET6 IPv6 - AF_UNSPEC
      SOCK_STREAM, // Type - SOCK_STREAM (this means connection oriented TCP protocol)
      0 // Protocol - 0 [ or IPPROTO_IP This is IP protocol]
    );
    //Type - SOCK_DGRAM - UDP
    // check if socket got successfully created
    if(socket_descriptor == -1){
      //failed to create socket
      fputs("Failed to create socket.",stderr);
      return 2;
    }else{
      fputs("Socket created.\n",stdout);
      fflush(stdout);
    }
    //sockaddr_in
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    //Bind
    if(bind(socket_descriptor,(struct sockaddr *)&server,sizeof(server)) < 0){
      fputs("Failed to bind.",stderr);
      //alternative: perror("Failed to bind."); ?!
      return 3;
    }else{
      fputs("Bind - check.\n",stdout);
      fflush(stdout);
    }
    //while True ?!
    //Listen
    listen(socket_descriptor,3); // what does the 3?!
    fputs("Listening...\n",stdout);
    fflush(stdout);
    //Accept connection
    int c = sizeof(client);
    client_socket = accept(socket_descriptor, (struct sockaddr *)&client,(socklen_t*)&c);
    if(client_socket < 0){
      fputs("Failed to accept",stderr);
      return 4;
    }
    fputs("Connection established.\n",stdout);
    fflush(stdout);
    //while connected?
    //Receive message
    int rsize = recv(client_socket,client_message,sizeof(client_message),0);
    while(rsize > 0){
      //client_message Vergleichen mit weihnachtlichen etc
      if(strstr(client_message,"jingle bells") != NULL){
        write(client_socket , "jingle all the way" , strlen("jingle all the way"));
      }else if(strstr(client_message,"rudolph") != NULL){
        write(client_socket , "The red nosed reindeer" , strlen("The red nosed reindeer"));
      }else if(strstr(client_message,"we wish you") != NULL){
        write(client_socket , "we wish you a merry cristmas\n and a happy new year" , strlen("we wish you a merry cristmas\n and a happy new year"));
      }else if(strstr(client_message,"santa claus is comin") != NULL){
        write(client_socket , "to toooooown" , strlen("to toooooown"));
      }else if(strstr(client_message,"last christmas") != NULL){
        write(client_socket , "i gave you my heart" , strlen("i gave you my heart"));
      }
      rsize = recv(client_socket,client_message,sizeof(client_message),0);
    }
      if(rsize == 0){
        fputs("Client quit connection.\n",stdout);
        fflush(stdout);
      }else if(rsize < 0){
        fputs("Receiving message failed.",stderr);
      }
      close(client_socket);
      close(socket_descriptor);
      return 0; // end of program
    }
}

//sources:
//http://www.binarytides.com/server-client-example-c-sockets-linux/
//http://en.cppreference.com/w/c/string/byte/strtol
