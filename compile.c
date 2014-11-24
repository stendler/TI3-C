#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc == 2){
		char *filenamec = argv[1];	//Eingabe des Quelldateinamen
		printf("Quell-Dateiname: %s \n",filenamec);
		char call[80] = "gcc -std=c99 -Wall -pedantic -o"; //Systemaufruf ohne Dateinamen

		char syscall[118];
		int j = 0;
		int k = 0;
		while(call[k] != 111){ //solange wiederholen, bis man beim o (111) ankommt
			//DEBUG
			//printf("%d %c %d %c \n",k,call[k],j,syscall[j]);
			syscall[j] = call[k]; //Systemaufruf
			j++;
			k++;
		}
		syscall[j++] = 111; //das o hinzufuegen
		syscall[j++] = 32;	//Leerzeichen
		k = 0;
		//TODO: Outputfilename als optionalen Parameter zulassen
		printf("Zieldateiname: ");
		while(filenamec[k] != 46){ //dateinamen bis zum '.' (46) einfuegen
			//DEBUG
			//printf("%d %c %d %c \n",k,filenamec[k],j,syscall[j]);
			printf("%c",filenamec[k]);
			syscall[j++] = filenamec[k++]; //Outputdateiname wird hinzuegfuegt
		}
		printf(".out");
		syscall[j++] = '.';
		syscall[j++] = 'o';
		syscall[j++] = 'u';
		syscall[j++] = 't';
		printf("\n");
		syscall[j++] = 32; //Leerzeichen
		k = 0;
		while(filenamec[k] != 46){ //same as above

					//DEBUG
					//printf("%d %c %d %c \n",k,filenamec[k],j,syscall[j]);
				syscall[j++] = filenamec[k++]; //Quelldateiname
			}
			syscall[j] = 46; 		//Punkt '.' hinzufuegen
			syscall[j+1] = 99;	//ein c (99) hinzuguegen
			syscall[j+2] = 0;		//Ende des Strings

			printf("%s \n",syscall);
			int syserr = system(syscall);
			if(syserr){
				return EXIT_FAILURE;
			}else{
				return EXIT_SUCCESS;
			}
	}else{
		return EXIT_FAILURE;
	}
}
