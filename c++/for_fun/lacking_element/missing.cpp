#include <iostream>

using namespace std;

class BitNumber
{
	public:
	virtual int GetBit(int j) const = 0;
};

class BitIntNumber : public BitNumber
{
	private:
	int number;
	
	public:
	BitIntNumber(int n) : number(n) {}
	
	int GetBit(int j) const;
};

int BitIntNumber :: GetBit(int j) const
{
	return (number >> j) & 1;
}


//zadanie: w tablicy o rozmiarze range - 1 znajdują się parami różne elementy z przedziału [1; range]. Znajdź brakujący element w czasie O(n)
//haczyk: można prosić tylko o konkretny bit danej liczby (jest to operacja atomowa wykonywana w czasie stałym)
int FindMissing(BitIntNumber* elements[], int range)
{
	int result = 0;
	int multiplier = 1;
	int p = 0, k = range - 2;
	int currentBit = 0;
	
	while(p <= k && multiplier <= range)
	{
		int howManyEvens = 0; //parzystych
		for(int i = p; i <= k; ++i)
			if(elements[i] -> GetBit(currentBit) == 0)
			{
				BitIntNumber* tmp = elements[p + howManyEvens];
				elements[p + howManyEvens] = elements[i];
				elements[i] = tmp;
				++howManyEvens;
			}
		
		int x = range / (2 * multiplier);
		if(result != 0 && x * 2 * multiplier + result <= range)
			++x;
		if(howManyEvens == x) 
		{
			p += howManyEvens;
			result += multiplier;
		}
		else
		{
			k = p + howManyEvens - 1;
		}
		
		++currentBit;
		multiplier <<= 1;
	}
	
	if(result == 0)
		return multiplier;
	return result;
}

int main()
{
	/*BitIntNumber a(329);
	for(int i = 10; i >= 0; --i)
		cout << a.GetBit(i);
	cout << endl;*/
	
	const int ile = 211;
	
	BitIntNumber** tab = new BitIntNumber*[ile];
	for(int i = 0; i < ile; ++i)
		tab[i] = new BitIntNumber(i + 1);
	
	cout << FindMissing(tab, ile + 1) << endl;
	
	for(int i = 0; i < ile; ++i)
		delete tab[i];
	delete [] tab;
	
	return 0;
}
