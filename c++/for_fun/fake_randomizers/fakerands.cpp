#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

//mając dany randomizer, który zwraca 0 lub 1 z prawdopodobieństwem 1/2 oraz
//liczby a i b, ten randomizer zwraca liczbę z zakresu [a;b] z prawd. 1/(b - a + 1)
//(wydaje mi się, że to prawdopodobieństwo wynosi właśnie tyle, choć nie jestem tego w stanie udowodnić)
//oczekiwany czas działania: b - a + 1
int FakeRandomizerOne(int a, int b, int (*rand)())
{
	int* tab = new int[b - a + 1];
	int toCheck = b - a + 1, chosen = 0;
	
	for(int i = 0; i < toCheck; ++i)
		tab[i] = i;
	
	while(toCheck != 1)
	{
		for(int i = 0; i < toCheck; ++i)
			if(rand() == 1)
				tab[chosen++] = tab[i];
		if(chosen != 0)
			toCheck = chosen;
		chosen = 0;
	}
	int result = tab[0] + a;
	delete [] tab;
	return result;
}

//zad: mając dany randomizer, który zwraca 0 lub 1 z prawdopodobieństwami odpowiednio p i p - 1, gdzie 0 < p < 1
//stwórz randomizer, który zwraca 0 lub 1 z prawd. 1/2
//testy pokazują, że działa w miarę ok, ale nie potrafię tego udowodnić
//poza tym użyłem zmiennej statycznej - brzydkie
/*int FakeRandomizerTwo(int (*rand)())
{
	static int adder = 0;
	int result = rand(), counter = adder;
	
	while(rand() == result)
		++counter;
		
	++adder;
	
	return counter % 2;
}*/


//drugie podejście - bez zmiennej statycznej, ale ciągle beznadziejne
/*int FakeRandomizerTwo(int (*rand)())
{
	int result = rand(), counter = 0;
	
	while(rand() == result)
		++counter;
	
	return counter % 2;
}

int FakeRandomizerThree(int (*rand)())
{
	int counter  = 0;
	for(int i = 0; i < 100; ++i)
		if(FakeRandomizerTwo(rand) == 1)
			++counter;
	return counter % 2;
}*/

int Feed()
{
	if(rand() % 100 < 50)
		return 0;
	return 1;
}

int Tester()
{
	return FakeRandomizerTwo(Feed);
}

int main()
{
	srand(time(NULL));
	
	//czy rzeczywiście daje 0 i 1 z jednakowym prawodpodobieństwem?
	float counter = 0;
	const int howMany = 1000000;
	for(int i = 0; i < howMany; ++i)
		if(FakeRandomizerThree(Feed) == 0)
			++counter;
	cout << counter / howMany << endl;
	
	
	
	//for(int i = 0; i < 10; ++i)
	//	cout << FakeRandomizerOne(1, 500, Tester) << endl;
	
	return 0;
}
