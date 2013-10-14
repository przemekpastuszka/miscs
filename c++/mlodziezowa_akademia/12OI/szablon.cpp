/*
Shortest word template
What's template? It's a string which can be used to cover whole input text by repeating it over and over
Based on prefix function from KMP
Coded by Przemysław Pastuszka
1.05.2010
*/ 

#include <cstring>
#include <stack>
#include <iostream>

//again we copy prefix function
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

//checks whether the given ps is a template in a very straightforward way
//we're just looking for all occurences of ps in a text and then check if they can cover the word
bool Check(long long ps, char word[], long long n)
{
	char *temp = new char[n + ps + 2];
	long long* prefix = new long long[n + ps + 2];
	
	strncpy(temp, word, ps + 1);
	temp[ps + 1] = '#';
	strcpy(temp + ps + 2, word + 1);
	
	PrefixFunc(prefix, temp, ps + n + 1);
	
	//ok, we've found all occurences. Time to check whether they can cover all the word
	long long left = 0;
	bool result = true;
	for(long long i = n + ps + 1; i > ps + 1; --i, --left)
	{
		if(prefix[i] == ps) 
			left = ps;
		if(left <= 0)
		{
			result = false;
			break;
		}
	}
	delete [] temp;
	delete [] prefix;
	return result;
}

//ok, here comes the algorithm
//we observe that any template must be a prefix-sufix of input text - simple
//so doing it in naive way we should examine all prefix-sufixes of input text, but
//that would be O(n^2), which is not nice, because we can do it better
//so we do next observation - if p is not a template then all ps shorter than 2 * |p|
//aren't templates too. It's a little bit harder, but can be done
//first: prove that if q is a template, p is ps and ceil(q/2) <= p <= q then p is a template
//second: make a transopsiton and you'll have what you've ever wanted to have
long long ShortestTemplate(char text[], long long n)
{
	long long result = n;
	
	long long *prefix = new long long[n + 1];
	PrefixFunc(prefix, text, n);
	
	std::stack<long long> candidates;
	long long t = prefix[n];
	while(t > 0)
	{
		candidates.push(t);
		t = prefix[t];
	}
	
	while(!candidates.empty())
	{
		t = candidates.top();
		candidates.pop();
		
		if(Check(t, text, n))
		{
			result = t;
			break;
		}
		while(!candidates.empty() && candidates.top() <= 2 * t)
			candidates.pop(); 
	}	
	
	delete [] prefix;
	return result;
}

int main()
{
	char text[500002];
	text[0] = 'p';
	std::cin >> text + 1;
	
	long long length = strlen(text + 1);
	
	std::cout << ShortestTemplate(text, length);
	return 0;
}
