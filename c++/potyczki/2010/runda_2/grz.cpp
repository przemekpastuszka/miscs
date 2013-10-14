#include <cstdio>

unsigned long long Compute(long tab[], long n, long t)
{
	unsigned long long max = 0;
	unsigned long long sum = 0;
	long time = t + 1;
	
	for(long i = 0; i < n - 1 && time > 0; ++i, --time)
	{
		long long localSum = (time - time / 2) * tab[i] + (time / 2) * tab[i + 1] + sum;
		if(localSum > max)
			max = localSum;
		sum += tab[i];
	}
	return max;
}

int main()
{
	long n, t;
	scanf("%ld", &n);
	scanf("%ld", &t);
	long* tab = new  long [n];
	for(long i = 0; i < n ; ++i)
		scanf("%ld", &tab[i]);
	unsigned long long result;
	if(n == 1)
		result = (t/2 + 1) * tab[0];
	else
		result = Compute(tab, n, t);
	printf("%llu", result);
	delete [] tab;
	return 0;
}
