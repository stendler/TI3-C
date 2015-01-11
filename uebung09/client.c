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
  if(argc != 2){
    fputs("usage: ./client PORTNUMBER",stderr);
    return 1;
  }else{

    int status // used for getaddrinfo to check if an error occured or not if value is other than 0 an error occcured
    int port; //obsolete #FIX
    int socket_descriptor;
    struct addrinfo addr, addr_res;
    char client_message[2000];

    //read Parameter //obsolete #FIX
    port = atoi(argv[1]); //string to int?!?!
    if(port <= 0){
      perror("Parameter input invalid.");
      exit(-5);
    }
    printf("Port is %i \n",port);
    fflush(stdout);

    //setup sockaddr addrinfo structure
    memset(&addr, 0 sizeof addr);

    addr.ai_family = AF_UNSPEC;
    addr.ai_socktype = SOCK_STREAM;     //Type - SOCK_DGRAM - UDP
    addr.ai_flags = AI_PASSIVE;
  //  addr.sin_port = htonl(port);

    //getaddressinfo
    if(status = getaddrinfo(NULL,argv[1],&addr,&addr_res) != 0){
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(1);
    }

    //create socket
    socket_descriptor = socket(
      addr_res->ai_family,     // Address Family - AF_INET (this is IP version 4) - AF-INET6 IPv6 - AF_UNSPEC
      addr_res->ai_socktype, // Type           - SOCK_STREAM (this means connection oriented TCP protocol)
      addr_res->ai_protocol            // Protocol       - 0 [ or IPPROTO_IP This is IP protocol]
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

    //connect!
    if(status = connect(socket_descriptor,addr_res->ai_addr,addr_res->ai_addrlen) < 0){
      fprintf(stderr, "conntect error: %s\n", gai_strerror(status));
      exit(3);
    }

  }
}
