#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char filename[20];
	scanf("Dateiname %s",&filename);
	char call[80] = "gcc -std=c99 -Wall -pedantic -o";
	char filenamec[22] = strcat(filename, ".c");
	printf("%s %s %s",call,filenamec,filename);
	return 0;
}
