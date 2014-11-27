//TI3 - Uebung 05
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

#include "mm.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	status();
/*
	//DEBUG
	char* bug = (char*)my_malloc(10216);
	int k = 0;
	for(int j = 0; j < 8; j++){
		for(int i = 0; i < 1277; i++){
			bug[k] = k;
			printf("+%d - %d\n",k,bug[k]);
			k++;
		}
	//	status();
	}
//	status();
	my_free((void*) bug);
	//ENDDEBUG
*/
	int *a = (int*)my_malloc(sizeof(int));
	*a = 5;
	printf("%d\n",*a);
	status();

	char* b = (char*)my_malloc(100);
	status();
	b[0] = 'c';
	b[1] = 'o';
	b[2] = 'o';
	b[3] = 'l';
	b[4] = '\0';
	printf("%s\n",b);


	char* c = (char*)my_malloc(80);
	status();
	//DEBUG
	/*for(int i = 0; i < 80; i++){
		c[i] = i;
		printf("+%d - %s \n",i,c);
	}*/

	char* d = (char*)my_malloc(9960);
	//DEBUG
/*	for(int i = 0; i < 9960; i++){
		d[i] = i;
		printf("+%d - %d \n",i,c[i]);
	}*/
	//d[9958] = '\0';
	d[9959] = '\0';

	status();
	my_free((void*)b);
	status();
	my_free((void*) d);
	status();
	char *e = (char*)my_malloc(80);
	status();
	my_free((void*)c);
	status();
	my_free((void*)a);
	status();
	my_free((void*)e);
	status();
	char buffer[3];
	printf("Press any key to continue...");
	fgets(buffer, 2, stdin);

}
