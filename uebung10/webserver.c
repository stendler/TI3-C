//TI3 - Uebung 10
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

//Basierend auf dem Server aus Uebung 09
// btw nur unser client war in Uebung 09 etwas fehlerhaft (in 2 Zeilen) - sieh dir zum lachen mal den versuch mit der ip an

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
  }/*else{
    fputs("Socket created.\n",stdout);
    fflush(stdout);
  }*/

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
  }/*else{
    fputs("Bind - check.\n",stdout);
    fflush(stdout);
  }*/

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
    }
      //fputs("Connection established.\n",stdout);
      //fflush(stdout);

//receive
    char client_message[2000];
    memset(&client_message,0,sizeof client_message);
  //  int rsize = recv(client_socket,client_message,sizeof(client_message),0);
    recv(client_socket,client_message,sizeof(client_message),0);

// parse input header
      //DEBUG: output der browser anfrage
      //fputs(client_message,stdout);
      //fflush(stdout);
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

//header geruesst erstellen
          char header[512];
          memset(&header,0,sizeof header);
          strcat (header,"HTTP/1.0 ");

// datei suchen
          FILE *fp = NULL;
          fp = fopen(dateiname,"r");
          char found = 0;
          //printf("file opened\n");
          if(fp == NULL){
            //printf("\nFile not found.\n\n");
            strcat(header,"404 Not Found\n");
            fp = fopen("404.html","r");
          }else{
            //printf("\nFile found!\n\n");
            strcat(header,"200 OK\n");
            found = 1;
          }
           //write(1,header,strlen(header));
           write(client_socket,header,strlen(header));
           //fflush(stdout);
           memset(&header,0,sizeof header);

  //header content type
          strcat(header,"Content-Type: ");
          if (strstr(client_message,"image/jpeg") != NULL || strstr(client_message,".jpg")){
            strcat(header,"image/jpeg\n");
            //write test
            //write(1,header,strlen(header));
            write(client_socket,header,strlen(header));
            //fflush(stdout);
            memset(&header,0,sizeof header);
          }else if (strstr(client_message,"image/gif") != NULL){
            strcat(header,"image/gif\n");
            //write test
            //write(1,header,strlen(header));
            write(client_socket,header,strlen(header));
            //fflush(stdout);
            memset(&header,0,sizeof header);
          }else if(strstr(client_message,"text/html") != NULL || found == 0){
            strcat(header,"text/html\n");
            //write test
            //write(1,header,strlen(header));
            write(client_socket,header,strlen(header));
            //fflush(stdout);
            memset(&header,0,sizeof header);
          }
          memset(&header,0,sizeof header);
          strcat(header,"Connection: close\n");
          write(client_socket,header,strlen(header));
          memset(&header,0,sizeof header);

  //header: content length
          strcat(header,"Content-Length: ");
          char chrbuffer[256];
          size_t count,buff = 0;
          while((buff = fread(chrbuffer,sizeof(char),256,fp))){
            count += buff;
          }
          //convert int count to char chrCount
          char chrCount[100];
          memset(&chrCount,0,sizeof chrCount);
          sprintf(chrCount,"%zu",count);
          strcat(header,chrCount);
          strcat(header,"\n");
          //write test
          //write(1,header,strlen(header));
          write(client_socket,header,strlen(header));
          //fflush(stdout);
          memset(&header,0,sizeof header);
          //strcat(header,"\n\n");
          //write(1,"\n",strlen("\n"));
          write(client_socket,"\n",strlen("\n"));
          //DEBUG
          //printf("%s",header);
          //fflush(stdout);

// return header + document (if there) ummm what about jpeg pictures?
              char buffer[count];
              memset(&buffer,0,sizeof(buffer));
              buff = 0;
              fseek(fp,0,SEEK_SET);
              while((buff = fread(buffer,sizeof(char),count,fp))){
                  write(client_socket,buffer,sizeof(buffer));
                  //write(1,buffer,sizeof(buffer));
              }
         }

//close connection
    close(client_socket);
    //fputs("Connection closed.\n",stdout);
    //fflush(stdout);
  }
  close(socket_descriptor);
}
