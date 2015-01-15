//TI3 - Uebung 10
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h> //write
#include <fcntl.h>

//#define FOLDER "web" falls wir nen unterordner machen wollen oder so

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
    char client_message[2000];
    memset(&client_message,0,sizeof client_message);
    int rsize = recv(client_socket,client_message,sizeof(client_message),0);
    //while(rsize > 0){ // fuer ne einfache anfrage vom browser nicht mehr notwendig

// parse input header
      //DEBUG: output der browser anfrage
      fputs(client_message,stdout);
      fflush(stdout);
      //testing parse input
      /*printf("%c %i\n",client_message[0],client_message[0]);
      printf("%c %i\n",client_message[1],client_message[1]);
      printf("%c %i\n",client_message[2],client_message[2]);
      printf("%c %i\n",client_message[3],client_message[3]);
      printf("%c %i\n",client_message[4],client_message[4]);
      printf("%c %i\n",client_message[5],client_message[5]);
      fflush(stdout);*/

      char dateiname[128]; // hier wird der geparste dateiname abgelegt
      memset(&dateiname,0,sizeof dateiname);
      if((client_message[0] == 71) &&
         (client_message[1] == 69) &&
         (client_message[2] == 84) &&
         (client_message[3] == 32) &&
         (client_message[4] == 47)
         ){
           int index = 5;
           while(client_message[index] != 32 && index < 133){
             dateiname[index-5] = client_message[index];
             index++;
           }
           //DEBUG
           //fputs(dateiname,stdout);

//header geruesst erstellen
          char header[128];
          memset(&header,0,sizeof header);
          strcat (header,"HTTP/1.0 ");

// datei suchen
           int source = open(dateiname,O_RDONLY);
           if (source <= 0){
             //printf("\nFile not found.\n\n");
             strcat(header,"404 Not Found\n");
           }else{
            // printf("\nFile found!\n\n");
             strcat(header,"200 OK\n");
           }
           //write test
          // int std = open(stdout,O_WRONLY);
           write(1,header,strlen(header));
           write(client_socket,header,strlen(header));
           fflush(stdout);
           memset(&header,0,sizeof header);

  //header content type
          int type = 0;
          strcat(header,"Content-Type: ");
          if(strstr(client_message,"text/html") != NULL){
            strcat(header,"text/html");
            //write test
            write(1,header,strlen(header));
            write(client_socket,header,strlen(header));
            fflush(stdout);
            memset(&header,0,sizeof header);
            type = 0;
          }else if (strstr(client_message,"image/jpeg")){
            strcat(header,"image/jpeg");
            //write test
            write(1,header,strlen(header));
            write(client_socket,header,strlen(header));
            fflush(stdout);
            memset(&header,0,sizeof header);

            type = 1;
          }else if (strstr(client_message,"image/gif")){
            strcat(header,"image/gif");
            //write test
            write(1,header,strlen(header));
            write(client_socket,header,strlen(header));
            fflush(stdout);
            memset(&header,0,sizeof header);

            type = 2;
          }
          strcat(header,"\nConnection: close\n");

  //header: content length
          strcat(header,"Content-Length: ");
          int count,buff = 0;
          char chrbuffer[1];
          while((buff = read(source,chrbuffer,1))){
            count++;
          }
          //convert int count to char chrCount
          char chrCount[100];
          memset(&chrCount,0,sizeof chrCount);
          sprintf(chrCount,"%d",count);
          strcat(header,chrCount);
          //write test
          write(1,header,strlen(header));
          write(client_socket,header,strlen(header));
          fflush(stdout);
          memset(&header,0,sizeof header);
          //strcat(header,"\n\n");
          write(1,"\n",strlen("\n"));
          write(client_socket,"\n",strlen("\n"));

          //DEBUG
          printf("%s",header);
          fflush(stdout);

          //send header?
          //write(client_socket,header,strlen(header));

// return header + document (if there) ummm what about jpeg pictures?
          if(source <= 0){
            //content = errormessage 404 not found
            write(client_socket,"404 File not found",strlen("404 File not found"));
            write(1,"404 File not found",strlen("404 File not found"));
          }else{
            write(client_socket,"200 File found",strlen("200 File found"));
            write(1,"200 File found\n",strlen("200 File found"));
            char strType[10];
            sprintf(strType, "%d", type);
            write(1,strType,strlen(strType));
            //wenn type = 0 = text/html:
            if(type == 0){
              //hehe test without loop and BIIIG buffer
              /*char filebuffer[count];
              read(source,filebuffer,count);
              write(client_socket,filebuffer,count);
              printf("%s",filebuffer);
              fflush(stdout);*///k maybe a loop is better xD

              //filecontent read/write loop
              write(1,"\n yo type = 0 lets start read'n'write\n",strlen("\n yo type = 0 lets start read'n'write\n"));
              char buffer[512];
              buff = 0;
              while((buff = read(source,buffer,512))){
                  write(client_socket,buffer,strlen(buffer));
                  write(1,buffer,strlen(buffer));
              }

            }else if(type == 1){
              //jpeg bytestream
              write(client_socket,"404 File not found",strlen("404 File not found"));
              write(1,"404 File not found",strlen("404 File not found"));
            }else if(type == 2){
              //gif bytestream
              write(client_socket,"404 File not found",strlen("404 File not found"));
              write(1,"404 File not found",strlen("404 File not found"));
            }
          }

         }

//close connection
    close(client_socket);
    fputs("Connection closed.\n",stdout);
    fflush(stdout);

  }
}
