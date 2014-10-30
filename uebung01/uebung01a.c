#include <stdio.h>

int main()
{
	float f;
	int i;
	long l;
	i = 1024;
	l = (long) i;
	printf("%d = %ld \n",i,l);
	l = 23;
	printf("l = %ld\n",l);
	i = (int) l;
	printf("%ld = %d\n",l,i);
	l = 17179869184;
	printf("l = %ld\n",l);
	i = (int) l;
	printf("%ld = %d\n",l,i);
	f = 5/2;
	printf("5/2 = %6.2f\n",f);
	f = (float) 5/2;
	printf("5/2 = %6.2f\n",f);
	f = 1.333;
	i = (int) f;
	printf("1.333 = %d\n",i);
	f = 5.0/2;
	i = (int) f;
	printf("2.5 = %d\n",i);
	return 0;
}
