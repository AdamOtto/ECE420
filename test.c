#include "omp.h"
#include <stdio.h>

int main()
{
	#pragma omp parallel
	{
		int id = 0;
		printf("hello(%d)\n", id);
		printf("world(%d)\n", id);
	}
	return 0;
}
