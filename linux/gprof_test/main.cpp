#include <stdio.h>
#include <stdlib.h>

void One()
{
	for (unsigned int i=0, size=500000000; i<size; ++i);
}

void Two()
{
	for (unsigned int i=0, size=300000000; i<size; ++i);
}
void Three()
{
	for (unsigned int i=0, size=200000000; i<size; ++i);
}

void SingleCall()
{
	One();
}

void DoubleCall()
{
	One();
	printf("Hello word.\n");
	One();
	Two();
	printf("Hello word.\n");
	Two();
	Three();
	Three();
}

int main(int argc, char * args[])
{

	unsigned int num = getchar();
	SingleCall();
	DoubleCall();
	return 0;
}
