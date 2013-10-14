#include <cstdio>
int main()
{
	long long q = 0;
	for(long i = 0; i < 1000; ++i)
	{
			for(long j = 0; j < 1000; ++j)
			{
				for(long w = 0; w < 1000;++w) 
				++q;
				--q;
			}
	}
	printf("%lld", q);
	return 0;
}
