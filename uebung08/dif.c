#include<stdio.h>
#include<unistd.h>
#include <string.h>
#include<stdint.h>

#define EXIT_FAILURE -1

/*
	Gebe die restliche Zeile aus.
*/
char* printLine(FILE *fp, char *buffer, int buffersize)
{
	char *getF = fgets(buffer,buffersize,fp) ;

	while(!strchr(buffer,'\n') )
	{
		printf(buffer);
		getF = fgets(buffer,buffersize,fp) ;
		if(getF == NULL)
		{
			return NULL;
		}
	}
	printf(buffer);
	return fgets(buffer,buffersize,fp);
}

void printName(char* output, char* fname)
{
	strcpy(output,fname);
	strcat(output,":\n");
	printf(output);
}

FILE* openfile(char *fname)
{
	/*
		�berrp�fe die Dateien
		Existieren die Dateien?
		Haben wir Leserechte?
	*/
	FILE *fp;
	if (access(fname, F_OK) == -1) {
		fprintf(stderr,"File %s does not exist!\n", fname);
		return NULL;
	}
	fp = fopen(fname, "r");
	if (fp == NULL) {
   		fprintf(stderr,"Cannot open %s for reading!\n", fname);
		return NULL;
	}
	return fp;
}
/*
	Berechnet naiv die unterschiedlichen Zeilen von zwei Dateien und gibt diese aus.
*/
int main()
{
	FILE *fp1, *fp2,*lastfp;
	char fname1[32], fname2[32], output[64];
	int8_t wrong = 0;
	char *getF1, *getF2, *getLastFile;
	char buffer1[8], buffer2[8];
	int size = 8;

	/*Lese Dateiname ein.*/
	printf("Enter name of first file :");
	gets(fname1);
	printf("Enter name of second file:");
	gets(fname2);
	fp1 = openfile(fname1);
	fp2 = openfile(fname2);
	if (fp1 == NULL) {
		return EXIT_FAILURE;
	}
	else if (fp2 == NULL)
	{
   		return EXIT_FAILURE;
	}

	/*
		Wir lesen die Dateien aus und vergleichen die Strings.
		Wir zaehlen die Anzahl der ungleichen Zeilen.
	*/
	getF1 = fgets(buffer1,size,fp1) ;
	getF2 = fgets(buffer2,size,fp2) ;
	getLastFile = NULL;
	while((getF1 != NULL) && (getF2!= NULL))
	{
		if(strcmp(buffer1,buffer2) != 0)
		{
			/*
				Gebe die Zeile aus der zweiten Datei aus.
			*/

			wrong++;
			printf("Ungleiche Zeile:\n");			
			printName(output,fname1);
			printf(buffer1);
			if(!strchr(buffer1,'\n'))
			{
				getF1 = printLine(fp1,buffer1,size);
			}
			else
			{
				getF1 = fgets(buffer1,size,fp1) ;
			}
			/*
				Gebe die Zeile aus der zweiten Datei aus.
			*/
			printName(output,fname2);
			printf(buffer2);
			/*
				�berpr�fe, ob die letzte Zeile ausgegeben werden muss.
			*/
			if(!strchr(buffer2,'\n'))
			{
				getF2 = printLine(fp2,buffer2,size);
			}
			else
			{
				getF2 = fgets(buffer2,size,fp2) ;
			}
		}
		else
		{
			getF1 = fgets(buffer1,size,fp1) ;
			getF2 = fgets(buffer2,size,fp2) ;
		}
	}
	/*
		Falls eine Datei fr�her komplett gelesen wurde.
		Wir geben den Dateinamen und den Rest aus dem Buffer aus.
	*/
	if(!(getF1 == NULL))
	{
		getLastFile = getF1;
		lastfp = fp1;
		printName(output,fname1);
		printf(buffer1);
	}
	else if(!(getF2 == NULL))
	{
		getLastFile = getF2;
		lastfp = fp2;
		printName(output,fname2);
		printf(buffer2);
	}
	/*
		Wir lesen die restliche Datei aus.
	*/
	if(getLastFile != NULL)
	{
		getLastFile= fgets(buffer1,size,lastfp) ;
		while(getLastFile !=NULL)
		{
			wrong++;
			printf(buffer1);
			getLastFile = printLine(lastfp,buffer1,size);
		}
	}
	/*
		Sind die Datein verschieden?
	*/
	if(wrong > 0)
	{
		printf("\nDateien sind ungleich. Anzahl ungleicher Zeilen: %d\n",wrong);
	}
	else
	{
		printf("\nDateien sind gleich.\n");
	}
	fclose(fp1);
	fclose(fp2);
	return (0);
}
