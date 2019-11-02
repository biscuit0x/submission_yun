#include <stdio.h>

int main(void)
{
	int q = 15;
	int r; 
	int sum0300 = 0;
	int sum0500 = 0;
	
	for (int i = 0; i < 125; i++)
	{
		r = q % 5;
		if (r == 0)
			sum0300 += q * 2;
		else
			sum0500 += q * 2;
		q++;
	}
	
	printf("R17:R16 = %i \n", sum0300);
	printf("R19:R18 = %i \n", sum0500);
	
	return 0; 
}