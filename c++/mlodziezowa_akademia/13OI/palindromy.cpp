#include <cstring>
#include <cstdio>
#include <algorithm>

struct Palindrom
{
	char* text;
	long long length;
	long long root;
	bool isBad;
};

void PrefixFunc(long long prefix[], char word[], long long n)
{
	prefix[1] = 0; //one character word has no proper prefix-sufix
	long long t = 0;
	for(long long i = 2; i <= n; ++i)
	{
		while(t > 0 && word[t + 1] != word[i])
			t = prefix[t];
		if(word[t + 1] == word[i])
			++t;
		prefix[i] = t;
	}
}

long long Root(long long n, char text[])
{
	long long* prefix = new long long[n + 1];
	PrefixFunc(prefix, text, n);
	long long result = n;
	if(n % (n - prefix[n]) == 0)
		result = n - prefix[n];
	delete [] prefix;
	return result;
}

bool Cmp(Palindrom a, Palindrom b)
{
	if(a.root < b.root)
		return true;
	if(a.root > b.root)
		return false;
	if(strcmp(a.text, b.text) < 0)
		return true;
	return false;
}

int main()
{
	long long n;
	scanf("%lld", &n);
	Palindrom* palindroms = new Palindrom[n];
	for(long long i = 0; i < n; ++i)
	{
		long long a;
		scanf("%lld", &a);
		palindroms[i].text = new char[a + 2];
		palindroms[i].text[0] = 'p';
		scanf("%s", palindroms[i].text + 1);
		palindroms[i].length = strlen(palindroms[i].text + 1);
		palindroms[i].root = Root(palindroms[i].length, palindroms[i].text);
		palindroms[i].text[palindroms[i].root + 1] = '\0';
		palindroms[i].isBad = false;
	}
	std::sort(palindroms, palindroms + n, Cmp);
	
	long long result = 0;
	for(long long i = 0; i < n; ++i)
	{
		if(palindroms[i].isBad)
			continue;
		++result;
		for(long long j = i + 1; 
			j < n && palindroms[i].root == palindroms[j].root && 
				strcmp(palindroms[i].text, palindroms[j].text) == 0; ++j)
			{
				if(palindroms[i].length == palindroms[j].length)
					palindroms[j].isBad = true;
				else
					result += 2;
			}
	}
	
	printf("%lld", result);
	
	for(long long i = 0; i < n; ++i)
		delete [] palindroms[i].text;
	delete [] palindroms;
	return 0;
}
