#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char filenamec[20];
	printf("Dateiname: (beispiel.c) ");
	scanf("%s",filenamec);
	char call[80] = "gcc -std=c99 -Wall -pedantic -o";
	//char filename[18];
	//filenamec = strcat(filename, ".c");
	int b = 1;
	/*for(int i = 0; i < 18; i++){
		if(filenamec[i] != 46){
			filename[i] = filenamec[i];
			//DEBUG
			printf("%d %d %c \n",i,filenamec[i],filenamec[i]);
		}else{
			if(filenamec[i+1] == 99){
				b = 1;
				break;
			}else{
				return 2;
			}

		}
	}*/
	//printf("%s %s %s",call,filename,filenamec);

	char syscall[118];
	int j = 0;
	int k = 0;
	while(call[k] != 111){

		//DEBUG
		printf("%d %c %d %c \n",k,call[k],j,syscall[j]);
		syscall[j] = call[k];
		j++;
		k++;
	}
	syscall[j++] = 111;
	syscall[j++] = 32;
	k = 0;
	while(filenamec[k] != 46){

				//DEBUG
				printf("%d %c %d %c \n",k,filenamec[k],j,syscall[j]);
		syscall[j++] = filenamec[k++];

	}
	syscall[j++] = 32;
	k = 0;
	while(filenamec[k] != 46){

				//DEBUG
				printf("%d %c %d %c \n",k,filenamec[k],j,syscall[j]);
		syscall[j++] = filenamec[k++];

	}
	syscall[j] = 46;
	syscall[j+1] = 99;
	syscall[j+2] = 0;

	if(b){
		printf("%s \n",syscall);
		system(syscall);
		return 0;
	}else{
		return 1;
	}
}
