#include <cstdio>

int main()
{
	printf("100000\n");
	for(long long i = 1; i <= 50000; ++i)
		printf("Z %lld %lld \n", i + 1, i);
	for(long long i = 1; i <= 50000; ++i)
		printf("P %d %d \n", 1, 50000);
	return 0;
}
