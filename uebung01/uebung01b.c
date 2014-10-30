#include <stdio.h>

int main()
{
	int tc;
	double tf = (float)9/5;
	printf("Temperatur in °Celsius: ");
	scanf("%i",&tc);
	tf *= (float)(tc+32);
	printf("Temperatur in Fahrenheit: %6.2f\n",tf);
	return 0;
}
