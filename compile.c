#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char filename[20];
	printf("Dateiname: ");
	scanf("%s",filename);
	char call[80] = "gcc -std=c99 -Wall -pedantic -o";
	char filenamec[22];
	//filenamec = strcat(filename, ".c");
	int i = 0;
	for(int i = 0; i <= 20; i++){
		if(filename[i] != 127 && filename[i] != 0){
			filenamec[i] = filename[i];
			//DEBUG
			printf("%d %d %c \n",i,filenamec[i],filenamec[i]);
		}else{
			filenamec[i] = (char)".";
			printf("%d %d %c \n",i,filenamec[i],filenamec[i]);
			filenamec[i+1] = (char)'c';
			printf("%d %d %c \n",i,filenamec[i],filenamec[i]);
			filenamec[i+2] = filename[i];
			printf("%d %d %c \n",i,filenamec[i],filenamec[i]);
			i+=2;
		}
	}
	printf("%s %s %s",*call,*filename,*filenamec);
	return 0;
}
