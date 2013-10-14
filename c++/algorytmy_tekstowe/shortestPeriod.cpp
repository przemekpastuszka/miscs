/*
Shortest word period
Algorithm based on prefix-sufix function from KMP
Coded by Przemysław Pastuszka
1.05.2010
*/

#include <iostream>
#include <cstring>
//we just ctrl+c, ctrl+v from KMP file
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

int main()
{
	char text[201];
	std::cin >> text + 1;
	
	long long length = strlen(text + 1);
	
	long long prefix[201];
	PrefixFunc(prefix, text, length);
	
	//shortest period is text - ps[length]! Really simple. Nice
	text[length - prefix[length] + 1] = '\0';
	std::cout << text + 1 << std::endl;
	return 0;
}
