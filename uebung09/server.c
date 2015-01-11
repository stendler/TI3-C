#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <arpa/inet.h>	//inet_addr
#include <unistd.h> //write

//TI3 - Uebung 09
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler
// Ver 1.6
//main() - creates a tcp/ip4 socket
int main(int argc, char *argv[])
{
  // 1 Parameter (Portnr.)
  if(argc < 2){
    fputs("No Parameter. At least Portnumber needed.",stderr);
    return 1;
  }else{

    int status // used for getaddrinfo to check if an error occured or not if value is other than 0 an error occcured
    int port;
    int socket_descriptor, client_socket;
    struct addrinfo server, server_res, client;
    char client_message[2000];

    //read Parameter
    port = atoi(argv[1]); //string to int?!?!
    if(port <= 0){
      perror("Parameter input invalid.");
      exit(-5);
    }
    printf("Port is %i \n",port);
    fflush(stdout);

    //setup sockaddr addrinfo structure
    memset(&server, 0 sizeof server);

    server.ai_family = AF_UNSPEC;
    server.ai_socktype = SOCK_STREAM;     //Type - SOCK_DGRAM - UDP
    server.ai_flags = AI_PASSIVE;
  //  server.sin_port = htonl(port);

    //getaddressinfo
    if(status = getaddrinfo(NULL,argv[1],&server,&server_res) != 0){
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(1);
    }

    //create socket
    socket_descriptor = socket(
      server_res->ai_family,     // Address Family - AF_INET (this is IP version 4) - AF-INET6 IPv6 - AF_UNSPEC
      server_res->ai_socktype, // Type           - SOCK_STREAM (this means connection oriented TCP protocol)
      server_res->ai_protocol            // Protocol       - 0 [ or IPPROTO_IP This is IP protocol]
    );

    // check if socket got successfully created
    if(socket_descriptor == -1){
      //failed to create socket
      fputs("Failed to create socket.",stderr);
      exit(2);
    }else{
      fputs("Socket created.\n",stdout);
      fflush(stdout);
    }

    //Bind
    //if(bind(socket_descriptor,(struct sockaddr *)&server,sizeof(server)) < 0){
    if(status = bind(socket_descriptor,server_res->ai_addr, server_res->ai_addrlen) < 0){
      fputs("Failed to bind.",stderr);
      //alternative: perror("Failed to bind."); ?!
      exit(3);
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
    int c = sizeof(struct sockaddr_in);
    if(client_socket = accept(socket_descriptor, (struct sockaddr *)&client,(socklen_t*)&c)){
      fputs("Failed to accept",stderr);
      return 4;
    }
    fputs("Connection established.\n",stdout);
    fflush(stdout);
    //while connected?

    //Receive message
    int rsize;
    while(rsize = recv(client_socket,client_message,sizeof(client_message),0) > 0){
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
    }

    if(rsize == 0){
      fputs("Client quit connection.\n",stdout);
      fflush(stdout);
    }else if(rsize < 0){
      fputs("Receiving message failed.",stderr);
    }

    return 0; // end of program
  }
}

//sources:
//http://www.binarytides.com/server-client-example-c-sockets-linux/
//http://en.cppreference.com/w/c/string/byte/strtol
