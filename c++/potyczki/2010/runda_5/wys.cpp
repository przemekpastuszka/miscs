#include <cstdio>

/*long long DoTheJob(long long numbers[], long long from, long long to, long long n)
{
	long long min = numbers[from] < n ? numbers[from] : n;
	n -= min;
	numbers[from] -= min;
	numbers[to] += min;

	return n;
}*/

/*void CalculateStep(long long numbers[], long long step)
{
	long long n;
	switch(step)
	{
		case 1:
		{
			n = DoTheJob(numbers, 3, 9, 1);
			n = DoTheJob(numbers, 5, 9, n);
			n = DoTheJob(numbers, 8, 9, n);
			n = DoTheJob(numbers, 0, 8, n);
			n = DoTheJob(numbers, 6, 8, n);
			n = DoTheJob(numbers, 1, 7, n);
			break;
		}
		case 2:
		{
			n = DoTheJob(numbers, 0, 9, 1);
			n = DoTheJob(numbers, 4, 9, n);
			n = DoTheJob(numbers, 6, 9, n);
			n = DoTheJob(numbers, 2, 8, n);
			break;
		}
		case 3:
		{
			n = DoTheJob(numbers, 2, 9, 1);
			n = DoTheJob(numbers, 7, 9, n);
			break;
		}
		case 4:
		{
			DoTheJob(numbers, 1, 9, 1);
			break;
		}
	}
}*/

struct Pair
{
	long long x, y;

	Pair(long long wX, long long wY) : x(wX), y(wY) {}
};

Pair CalculateStep(long long numbers[], long long step)
{
	switch(step)
	{
		case 1:
		{
			if(numbers[3] > 0)
				return Pair(3, 9);
			if(numbers[5] > 0)
				return Pair(5, 9);
			if(numbers[8] > 0)
				return Pair(8, 9);
			if(numbers[0] > 0)
				return Pair(0, 8);
			if(numbers[6] > 0)
				return Pair(6, 8);
			if(numbers[1] > 0)
				return Pair(1, 7);
			break;
		}
		case 2:
		{
			if(numbers[0] > 0)
				return Pair(0, 9);
			if(numbers[4] > 0)
				return Pair(4, 9);
			if(numbers[6] > 0)
				return Pair(6, 9);
			if(numbers[2] > 0)
				return Pair(2, 8);
			break;
		}
		case 3:
		{
			if(numbers[2] > 0)
				return Pair(2, 9);
			if(numbers[7] > 0)
				return Pair(7, 9);
			break;
		}
		case 4:
		{
			if(numbers[1] > 0)
				return Pair(1, 9);
			break;
		}
	}
	return Pair(-1, -1);
}

void Copy(long long a[], long long b[])
{
	for(long long i = 0; i < 10; ++i)
		b[i] = a[i];
}

bool Cmp(long long a[], long long b[])
{
	for(long long i = 9; i > 0; --i)
	{
		if(a[i] < b[i])
			return true;
		if(a[i] > b[i])
			return false;
	}
	return false;
}

void PerformStep(long long from[], long long to[], long long step)
{
	Pair t = CalculateStep(from, step);
	if(t.x >= 0)
	{
		--from[t.x];
		++from[t.y];
		if(Cmp(to, from))
			Copy(from, to);
		--from[t.y];
		++from[t.x];
	}
        else
            if(Cmp(to, from))
		Copy(from, to);

}

long long Shift(long long x)
{
	return x < 0 ? 5 + x : x;
}

void Compute(long long numbers[], long long n)
{
	long long steps[5][10];
	for(long long i = 0; i < 5; ++i)
		for(long long j = 0; j < 10; ++j)
			steps[i][j] = 0;
	Copy(numbers, steps[0]);
	for(long long i = 1; i <= n; ++i)
	{
		long long current = i % 5;
		PerformStep(steps[Shift(current - 1)], steps[current], 1);
		if(i > 1)
			PerformStep(steps[Shift(current - 2)], steps[current], 2);
		if(i > 2)
			PerformStep(steps[Shift(current - 3)], steps[current], 3);
		if(i > 3)
			PerformStep(steps[Shift(current - 4)], steps[current], 4);
	}
	Copy(steps[n % 5], numbers);
}

int main()
{
	long long k;
	scanf("%lld", &k);

	long long numbers[10];

	for(long long i = 0; i < k ; ++i)
	{
		for(long long j = 0; j < 10; ++j)
			numbers[j] = 0;

		long long n, l;
		scanf("%lld", &n);

		scanf("%lld", &l);
		for(long long j = 0; j < l;)
		{
			char c;
			scanf("%c", &c);
			if(c >= '0' && c <= '9')
			{
				++numbers[c - '0'];
				++j;
			}
		}

		Compute(numbers, n);

		for(long long j = 9; j >= 0; --j)
			for(long long z = 0; z < numbers[j]; ++z)
				printf("%lld", j);
		printf("\n");
	}

	return 0;
}
