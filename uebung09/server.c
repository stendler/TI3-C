#include <stdio.h>
#include <sys/socket.h>

//TI3 - Uebung 09
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler
// Ver 1.1
//main() - creates a tcp/ip4 socket
int main(int arc, char *argv[])
{

  // 1 Parameter (Portnr.)
    int socket_descriptor;
    socket_descriptor = socket(
      AF_INET,     // Address Family - AF_INET (this is IP version 4)
      SOCK_STREAM, // Type           - SOCK_STREAM (this means connection oriented TCP protocol)
      0            // Protocol       - 0 [ or IPPROTO_IP This is IP protocol]
    );
    //Type - SOCK_DGRAM - UDP

    // check if socket got successfully created
    if(socket_descriptor == -1){
      //failed to create socket

    }

    return 0; // end of program
}
