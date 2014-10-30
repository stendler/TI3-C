#include <stdio.h>

double c2f(int tc)
{
	double tf = (double)9/5;
	tf *= (double)(tc+32);
	return tf;
}
	
int main()
{
	//oberer Tabellenteil
	printf("------------\n");
	
	int i = -30;
	while (i < 101){
		printf("|"); //linker Tabellenrand
		
		//Formatierung
		if(i>=0 && i<100){
			printf(" ");
		}
		if(i==0){
			printf(" ");
		}
		
		printf("%i",i); //Temperatur in Celsius
		printf("|"); //Tabellenmitte
		double out = c2f(i);
		printf("%6.2f", out); //Temperatur in Fahrenheit
		printf("|"); //rechter Tabellenrand
		printf("\n");
		
		i += 10;
	}
	
	//unterer Tabellenteil
	printf("------------\n");
	
	return 0;
}