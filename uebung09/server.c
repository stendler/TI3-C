#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr

//TI3 - Uebung 09
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler
// Ver 1.3
//main() - creates a tcp/ip4 socket
int main(int arc, char *argv[])
{
  // 1 Parameter (Portnr.)
  if(argc < 2){
    fputs("No Parameter. At least Portnumber needed.",stderr);
    return 1;
  }else{

    int port;
    int socket_descriptor, client_socket;
    struct sockaddr_in server, client;
    char client_message[2000];

    //read Parameter
    port = argv[1]; //string to int?!?!

    //create socket
    socket_descriptor = socket(
      AF_INET,     // Address Family - AF_INET (this is IP version 4) - AF-INET6 IPv6 - AF_UNSPEC
      SOCK_STREAM, // Type           - SOCK_STREAM (this means connection oriented TCP protocol)
      0            // Protocol       - 0 [ or IPPROTO_IP This is IP protocol]
    );
    //Type - SOCK_DGRAM - UDP

    // check if socket got successfully created
    if(socket_descriptor == -1){
      //failed to create socket
      fputs("Failed to create socket.",stderr);
      return 2;
    }else{
      fputs("Socket created.",stdout);
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
      fputs("Bind - check.",stdout);
    }

    //Listen
    listen(socket_descriptor,3); // what does the 3?!
    fputs("Listening...",stdout);

    //Accept connection
    int c = sizeof(struct sockaddr_in);
    if(client_socket = accept(socket_descriptor, (struct sockaddr *)&client,(socklen_t*)&c)){
      fputs("Failed to accept",stderr);
      return 4;
    }
    fputs("Connection established.",stdout);




    return 0; // end of program
  }
}

//http://www.binarytides.com/server-client-example-c-sockets-linux/
