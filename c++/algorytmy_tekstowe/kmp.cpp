/*
Knuth-Morris-Pratt algorithm implementation in C++
Code by Przemysław Pastuszka
1.05.2010
*/
#include <cstring>
#include <iostream>

//prefix function calculates the length of the longest proper prefix-sufix for each prefix of word
//algorithm is quite simple - you need only to observe few things:
//-prefix-sufix of prefix-sufix is prefix-sufix (make a sketch, so you'll see that better)
//-you can generate all prefix-sufixes of a word by iterating "the longest prefix-sufix" function
//-if you have word which length is n + 1 and you want to calculate the longest p-s
//	you need only to examine p-ses of word of length n and check whether the next letter is
//	equal to letter on n + 1 position
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

//here comes the dirty part (due to some string operations which are really awful)
long long KMP(char text[], char pattern[])
{
	long long result = -1;
	long long textLen = strlen(text), patternLen = strlen(pattern);
	
	char *temp = new char[textLen + patternLen + 2];
	long long* prefix = new long long[textLen + patternLen + 2];
	
	//we'll calculate prefix function for special string: pattern # text
	//so when any prefix-sufix has length equal to pattern length
	//we do know, that we've found an occurence
	strcpy(temp + 1, pattern);
	temp[patternLen + 1] = '#';
	strcpy(temp + patternLen + 2, text);
	
	PrefixFunc(prefix, temp, textLen + patternLen + 1);
	for(long long i = patternLen + 2; i < textLen + patternLen + 2; ++i)
		if(prefix[i] == patternLen) //here's the check I've promised you
		{
			result = i - 2 * patternLen;
			break;
		}
	
	delete [] prefix;
	delete [] temp;
	return result;
}

int main()
{
 	char text[200], pattern[20];
 	std::cin>>text>>pattern;
 	std::cout<<KMP(text, pattern)<<std::endl;
	return 0;
}
