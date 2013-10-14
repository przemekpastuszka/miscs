/*
Coded by Przemysław Pastuszka
Wersja tzw. syfna
*/
#include <cstdio>

 long FindMin( long long tab[],  long n)
{
	 long long currentMin = tab[0];
	 long currentMinIndex = 0;
	for( long i = 1; i < n; ++i)
		if(tab[i] < currentMin)
		{
			currentMin = tab[i];
			currentMinIndex = i;
		}
	return currentMinIndex;
}

long Modulo(long q, long n)
{
	long r = q % n;
	if(r >= 0)
		return r;
	else
		return n + r;
}

bool Check( long long tab[],  long n,  long start, int direction)
{ 
	for( long i = 0; i < n - 1; ++i)
	{
		if(tab[Modulo(start + i * direction, n)] > tab[Modulo(start + (i + 1) * direction, n)])
			return false;
	}
	return true;
}

int main()
{
	 long n;
	scanf("%ld", &n);
	 long long* tab = new  long long[n];
	for( long i = 0; i < n ; ++i)
		scanf("%lld", &tab[i]);
	 long min = FindMin(tab, n);
	if(Check(tab, n, min, 1) || Check(tab, n, min, -1))
		printf("TAK");
	else
		printf("NIE");
	delete [] tab;
	return 0;
}
