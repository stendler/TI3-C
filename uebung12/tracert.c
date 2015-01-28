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
//#include <pthread.h>
//#include <netdb.h>	//hostend
//#include <arpa/inet.h>

//holt die lokale ip adresse aus einem socket paket, das nach draußen geschickt wird
int get_local_ip ( char * buffer)
{
	int sock = socket ( AF_INET, SOCK_DGRAM, 0);

	const char* kGoogleDnsIp = "8.8.8.8";
	int dns_port = 53;

	struct sockaddr_in serv;

	memset( &serv, 0, sizeof(serv) );
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
	serv.sin_port = htons( dns_port );

	int err = connect( sock , (const struct sockaddr*) &serv , sizeof(serv) );

	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);
	err = getsockname(sock, (struct sockaddr*) &name, &namelen);

	const char *p = inet_ntop(AF_INET, &name.sin_addr, buffer, 100);

	close(sock);
}

struct pseudo_header    //needed for checksum calculation
{
	unsigned int source_address; //u_int32_t
	unsigned int dest_address;   //u_int32_t
	unsigned char placeholder;   //u_int8_t
	unsigned char protocol;      //u_int8_t
	unsigned short tcp_length;   //u_int16_t

	struct tcphdr tcp;           // not in 1st source
};

//main
int main(int argc, char *argv[])
{
  //check parameter -> hostname / ip-address - TODO: - maybe an additional optional argument for a custom port?
  if(argc == 2){

  	//create send socket
  	int sd = socket (PF_INET, SOCK_RAW, IPPROTO_ICMP); //IPv4,Raw-socket, IPPROTO_TCP/RAW TODO IPPROTO_RAW and we save goto1 later
  	if(sd == -1)
  	{
    	//socket creation failed, may be because of non-root privileges
    	perror("Failed to create socket - try being root next time\n");
    	exit(1);
  	}
		//IP_HDRINCL to tell the kernel that headers are included in the packet
		int one = 1;
		const int *val = &one;

		if (setsockopt (sd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) //TODO is IPPROTO_IP correct?
		{
			printf ("Error setting IP_HDRINCL. Error number : %d . Error message : %s \n" , errno , strerror(errno));
			exit(0);
		}

		/* TODO: this is a container for code used in sources but I couldn't find use for yet

	struct pseudo_header psh;

	int source_port = 43591;

		END unplaced source code container*/

  	//build custom headers
    	//Datagram to represent the packet
			char datagram[4096];
			memset (datagram, 0, 4096);	// zero out the buffer
			//WIP ip

				//get local ip
				char source_ip[20]; //TODO memset?
				get_local_ip( source_ip );
				printf("Local source IP is %s \n" , source_ip);

				//get host ip
				struct in_addr dest_ip;
				struct sockaddr_in  dest,recv;
				char *target = argv[1];

				if( inet_addr( target ) != -1)
				{
					dest_ip.s_addr = inet_addr( target );
				}
				else
				{
					char *ip = hostname_to_ip(target); //FIXME function currently not existent
					if(ip != NULL)
					{
						printf("%s resolved to %s \n" , target , ip);
						//Convert domain name to IP
						dest_ip.s_addr = inet_addr( hostname_to_ip(target) );
					}
					else
						{
						printf("Unable to resolve hostname : %s" , target);
						exit(1);
					}
				}

			//Fill in the IP Header

			//IP header
			struct iphdr *iph = (struct iphdr *) datagram;

			//TCP header
			struct icmphdr *icmph = (struct icmphdr *) (datagram + sizeof (struct ip));

			iph->ihl = 5; //XXX unknown use
			iph->version = 4;
			iph->tos = 0; //XXX unknown use
			iph->tot_len = sizeof (struct ip) + sizeof (struct icmphdr);
			iph->id = htons (54321);	//Id of this packet //XXX: what does ID mean? seq number?
			iph->frag_off = htons(16384); //XXX unknown use
			iph->ttl = 0; 			//we need this one later
			iph->protocol = IPPROTO_ICMP;
			iph->check = 0;		//Set to 0 before calculating checksum
			iph->saddr = inet_addr ( source_ip );	//Spoof the source ip address
			iph->daddr = dest_ip.s_addr;

    	//icmp
			icmphd->type = ICMP_ECHO; // ping request
      icmphd->code = 0;					//
      icmphd->checksum = 0;
      icmphd->un.echo.id = 0;

			//setting up sending socket
			dest.sin_family = AF_INET;
			dest.sin_addr.s_addr = dest_ip.s_addr;
			dest.sin_port = htons(7); //TODO ping port?

			//create listener socket_addr
			struct sockaddr_in listener;
			socklen_t saddr_len = sizeof (struct sockaddr_in);

  		//our hop counter which gets incremented;
			unsigned char ttl = 1;
			icmphd->un.echo.sequence = ttl;

			//while loop
			while(ttl <= 255){
				//set ttl in ip header
				iph->ttl = ttl;
				//TODO package / checksum? &more
				//iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1); //TODO put this never - maybe the kernel does this for us?
				//icmp->checksum = ...
				//TODO multiple simultanously ? --> for loop which does'nt incr ttl but sends 3 packages
					//send custom ip header with icmp header in body and ttl
					sendto (sd, buf, sizeof(struct ip) + sizeof(struct icmphdr), 0, (struct sockaddr *)&dest, sizeof dest);
    			//receive ICMP packets
					char rcvbuffer[4096] = { 0 };
					recvfrom (sd, rcvbuffer, sizeof(struct ip) + sizeof(struct icmphdr), 0, (struct sockaddr *) &listener, &saddr_len);
					struct icmphdr *icmphd2 = (struct icmphdr *) (rcvbuffer + 20);
					//print [ttl] hop_ip (TODO resolve) [timestamp/delay]
					printf("[%d] %s\n",ttl,inet_ntoa(listener.sin_addr));
					if (icmphd2->type == 0){
						printf("\n Destination reached!\n");
						return 0;
					}
				ttl++;
  		}
	}else{
    //nicht genuegend argumente
    //print usage
    printf("usage: %s hostname [port]\n\nPort is OPTIONAL\n\n",argv[0]);
  }
}
