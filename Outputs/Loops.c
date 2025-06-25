#include <stdio.h>
int main()
{
	printf("Number race\n");
	float x = 50;
	float y = 200000;
	while(x > 5 && y > 5)
	{
		x = x - 1;
		y = y / 2;
	}
	if(y < 5)
	{
		printf("y won! y is %f\n", y);
	}
	else
	{
		printf("x won! x is %f\n", x);
	}
	return 0;
}