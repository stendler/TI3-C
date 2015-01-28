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
//

//#define _GNU_SOURCE

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
//#include <sys/select.h>
#include <errno.h> //For errno - the error number
//#include <netinet/tcp.h>	//Provides declarations for tcp header
//source stackoverflow TODO test which headers are really needed
//#include <unistd.h>
//#include <sys/ipc.h>
//#include <fcntl.h>
//#include <sys/stat.h>
//#include <sys/sem.h>
//#include <signal.h>
//#include <poll.h>
//#include <pthread.h>
//#include <sys/un.h>

/* IP header structure */
    struct ip {

        unsigned char      ihl;
        unsigned char      version;
        unsigned char      tos;
        unsigned short int len;
        unsigned short int id;
        unsigned char      flags;
        unsigned short int offset;
        unsigned char      ttl;
        unsigned char      protocol;
        unsigned short int chksum;
        unsigned int       sourceip;
        unsigned int       destip;

        };

struct icmp {
	unsigned char	icmp_type;		// type of message, see below
	unsigned char	icmp_code;		// type sub code
	unsigned short	icmp_cksum;		// ones complement cksum of struct
	union {
		unsigned char ih_pptr;			// ICMP_PARAMPROB
		struct in_addr ih_gwaddr;	// ICMP_REDIRECT
		struct ih_idseq {
			n_short	icd_id;
			n_short	icd_seq;
		} ih_idseq;
		int ih_void;
	} icmp_hun;
#define	icmp_pptr	icmp_hun.ih_pptr
#define	icmp_gwaddr	icmp_hun.ih_gwaddr
#define	icmp_id		icmp_hun.ih_idseq.icd_id
#define	icmp_seq	icmp_hun.ih_idseq.icd_seq
#define	icmp_void	icmp_hun.ih_void
	union {
		struct id_ts {
			n_time its_otime;
			n_time its_rtime;
			n_time its_ttime;
		} id_ts;
		struct id_ip  {
			struct ip idi_ip;
			/* options and then 64 bits of data */
		} id_ip;
		unsigned long	id_mask;
		char	id_data[1];
	} icmp_dun;
#define	icmp_otime	icmp_dun.id_ts.its_otime
#define	icmp_rtime	icmp_dun.id_ts.its_rtime
#define	icmp_ttime	icmp_dun.id_ts.its_ttime
#define	icmp_ip		icmp_dun.id_ip.idi_ip
#define	icmp_mask	icmp_dun.id_mask
#define	icmp_data	icmp_dun.id_data
};
/**/



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

	//close(sock);
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
		perror("gethostbyname"); //FIXME impplicit declaration of herror
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
			struct icmphdr *icmphd = (struct icmphdr *) (datagram + sizeof (struct ip));

			iph->ihl = 5; //XXX unknown use
			iph->version = 4;
			iph->tos = 0; //XXX unknown use
			iph->tot_len = sizeof (struct ip) + sizeof (struct icmphdr); //FIXME incomplete type 'struct ip'
			iph->id = htons (54321);	//Id of this packet //XXX: what does ID mean? seq number?
			iph->frag_off = htons(16384); //XXX unknown use
			iph->ttl = 0; 			//we need this one later
			iph->protocol = IPPROTO_ICMP;
			iph->check = 0;		//Set to 0 before calculating checksum
			iph->saddr = inet_addr ( source_ip );	//Spoof the source ip address
			iph->daddr = dest_ip.s_addr;

    	//icmp
			icmphd->type = ICMP_ECHO; // ping request FIXME undeclared
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
					sendto (sd, datagram, sizeof(struct ip) + sizeof(struct icmphdr), 0, (struct sockaddr *)&dest, sizeof dest); //TODO is datagram really complete
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
