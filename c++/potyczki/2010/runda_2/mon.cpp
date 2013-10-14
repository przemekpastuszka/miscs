#include <cstdio>

long iloscReszek(long ileReszek[], long i, long j)
{
	return ileReszek[j] - ileReszek[i - 1];
}

long iloscOrlow(long ileReszek[], long i, long j)
{
	return (j - ileReszek[j]) - (i - 1 - ileReszek[i - 1]);
}

long Compute(long n, long k, long ileReszek[])
{
	long max = 0;

    for(long i = 1; i <= n; ++i)
    {
    	if(i + max + k > n)
    		break;
		for(long j = i + max + k; j <= n; )
		{
			long r = iloscReszek(ileReszek, i, j);
			long o = iloscOrlow(ileReszek, i, j);
			if(k * r == o)
			{
				max = j - i + 1;
				j = i + (k + 1) * (r + 1) - 1;
			}
			if(k * r > o)
				j = i + (k + 1) * r - 1;
			if(k * r < o)
			{
				long x = o / k;
				if(o % k)
					++x;
				j = i + (k + 1) * x - 1;
			}
		}
	}
	return max;
}

int main()
{

	long n, k;
	scanf("%ld", &n);
	scanf("%ld", &k);
	//bool* reszki = new bool[n + 1];
	long* ileReszek = new long[n + 1];

	ileReszek[0] = 0;

	for(long i = 1; i <= n ;)
	{
		char c;
		scanf("%c", &c);

		ileReszek[i] = ileReszek[i - 1];

		if(c == 'R')
		{
		//	reszki[i] = true;
			++ileReszek[i];
                        ++i;
		}
                if(c == 'O')
                    ++i;
		//else
			///reszki[i] = false;
	}

	long result = Compute(n, k, ileReszek);
	printf("%ld", result);

	//delete [] reszki;
	delete [] ileReszek;
	return 0;
}
