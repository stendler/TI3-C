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
//https://stackoverflow.com/questions/15458438/implementing-traceroute-using-icmp-in-c
//http://unix.superglobalmegacorp.com/Net2/newsrc/netinet/ip_icmp.h.html

//includes
#include <stdio.h>	//for printf
#include <stdlib.h> //for exit(0);
#include <sys/types.h>
#include <sys/socket.h>	//for socket ofcourse
#include <netdb.h>	//hostend
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>	//Provides declarations for ip header
#include <netinet/ip_icmp.h>
#include <string.h> //memset
#include <arpa/inet.h>
#include <errno.h> //For errno - the error number

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

	inet_ntop(AF_INET, &name.sin_addr, buffer, 100); //removed unused pointer

	return err; //err was unused
}

/*
	Get ip from domain name
 */
char* hostname_to_ip(char * hostname)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;

	if ( (he = gethostbyname( hostname ) ) == NULL)
	{
		// get the host info
		perror("gethostbyname");
		return NULL;
	}

	addr_list = (struct in_addr **) he->h_addr_list;

	for(i = 0; addr_list[i] != NULL; i++)
	{
		//Return the first one;
		return inet_ntoa(*addr_list[i]) ;
	}

	return NULL;
}

/*
 Checksums - IP and TCP
 */
unsigned short csum(unsigned short* ptr,int nbytes)
{
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;
	while(nbytes>1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((unsigned char*)&oddbyte) = *(unsigned char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;

	return(answer);
}

//main
int main(int argc, char *argv[])
{
  //check parameter -> hostname / ip-address - TODO: - maybe an additional optional argument for a custom port?
  if(argc == 2){

  	//create send socket
  	int sd = socket (PF_INET, SOCK_RAW, IPPROTO_ICMP); //IPv4,Raw-socket, IPPROTO_TCP/RAW
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

  	//build custom headers
    	//Datagram to represent the packet
			char datagram[4096];
			memset (datagram, 0, 4096);	// zero out the buffer
			//ip

				//get local ip
				char source_ip[20]; //TODO memset?
				get_local_ip( source_ip );
				printf("Local source IP is %s \n" , source_ip);

				//get host ip
				struct in_addr dest_ip;
				struct sockaddr_in  dest;
				char *target = argv[1];

				if( inet_addr( target ) != -1)
				{
					dest_ip.s_addr = inet_addr( target );
				}
				else
				{
					char *ip = hostname_to_ip(target);
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
			struct icmphdr *icmphd = (struct icmphdr *) (datagram + sizeof (struct iphdr));

			iph->ihl = 5; //XXX unknown use
			iph->version = 4;
			iph->tos = 0; //XXX unknown use
			iph->tot_len = sizeof (struct iphdr) + sizeof (struct icmphdr);
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
      //icmphd->un.echo.id = 0;

			//setting up sending socket
			dest.sin_family = AF_INET;
			dest.sin_addr.s_addr = dest_ip.s_addr;
			dest.sin_port = htons(80); //TODO ping port?

			//create listener socket_addr
			struct sockaddr_in listener;
			socklen_t saddr_len = sizeof (struct sockaddr_in);

  		//our hop counter which gets incremented;
			unsigned char ttl = 0;

			//while loop
			while(ttl <= 255){
        ttl++;
        //DEBUG
        //printf("--(1)ttl++\n");
				//set ttl in ip header
				iph->ttl = ttl;
				//icmphd->un.echo.sequence = ttl;
				//package / checksum? &more
        iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
				icmphd->checksum = csum((unsigned short *) (datagram + 20), 4);
				//DEBUG
				//printf("--(2) checksum\n");
				//TODO multiple simultanously ? --> for-loop which doesn't incr ttl but sends 3 packages
					//send custom ip header with icmp header in body and ttl
					sendto (sd, datagram, sizeof(struct iphdr) + sizeof(struct icmphdr), 0, (struct sockaddr *)&dest, sizeof dest);
    			//receive ICMP packets
					char rcvbuffer[4096] = { 0 };
					//DEBUG
					//printf("--(3)package send\n");
					recvfrom (sd, rcvbuffer, sizeof(struct iphdr) + sizeof(struct icmphdr), 0, (struct sockaddr *) &listener, &saddr_len);
					struct icmphdr *icmphd2 = (struct icmphdr *) (rcvbuffer + 20);
					//print [ttl] hop_ip (TODO resolve) [timestamp/delay]
					printf("[%d] %s\n",ttl,inet_ntoa(listener.sin_addr)); //TODO  sind das ueberhaupt meine icmp's --> check that plox
					if (icmphd2->type == 0){ //TODO weitere Bedingungen, e.g.ist der gewuenschte host im header?
						printf("\n Destination reached!\n");
						return 0;
					}
  		}
	}else{
    //nicht genuegend argumente
    //print usage
    printf("usage: %s hostname [port]\n\nPort is OPTIONAL\n\n",argv[0]);
  }
}
