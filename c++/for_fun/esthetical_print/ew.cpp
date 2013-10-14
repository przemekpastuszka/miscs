#include <iostream>
#include <cstring>
//#include <stack>
using namespace std;

int Cube(int n)
{
	return n * n * n;
}

const int MAX_NUM = 10000000;

void PrintTheFucker(string text[], int nlines[], int c)
{
	if(c < 0)
		return;
	PrintTheFucker(text, nlines, nlines[c]);
	for(int i = nlines[c] + 1; i <= c; ++i)
		cout << text[i] << " ";
	cout << endl;
}

//zadanie: wypisz tekst w taki sposób, aby zminimalizowac sumę sześcianów ilości wolnych spacji na koncu kazdego wiersza (nie licząc ostatniego)
//n to ilość słów, M długość wiersza
void EsteticPrinting(string text[], int n, int M)
{
	int* lengths = new int[n];
	int* bests = new int[n];
	int* nlines = new int[n];
	for(int i = 0; i < n; ++i)
	{
		lengths[i] = strlen(text[i].c_str());
		bests[i] = MAX_NUM;
		nlines[i] = -1;
	}
	
	for(int i = 0; i < n; ++i)
	{
		int sum = 0;
		for(int j = i; j >= 0 && sum + lengths[j] + i - j <= M; --j)
		{
			sum += lengths[j];
			int tmp = Cube(M - (sum + i - j)) + (j > 0 ? bests[j - 1] : 0);
			if(tmp < bests[i])
			{
				bests[i] = tmp;
				nlines[i] = j - 1;
			}
		}
	}
	
	
	int sum = 0;
	int min = bests[n - 1];
	int q = n - 1;
	for(int j = n - 1; j > 0 && sum + lengths[j] + n - 1 - j <= M; --j)
	{
		sum += lengths[j];
		if(bests[j - 1] < min)
		{
			min = bests[j - 1];
			q = j - 1; 
		}
	}
	PrintTheFucker(text, nlines, q);
	for(int i = q + 1; i < n; ++i)
		cout << text[i] << " ";
	cout << endl;
	delete [] lengths;
	delete [] bests;
	delete [] nlines;
}

int main()
{
	string tab[] = {"Szedl", "pan", "sasza", "bardzo", "sucha", "niegdys", "szosa,", "ale", "ktora", "niestety", "zdolala", "zamoknac", "w", "czasie", "dosc", "niedlugim.", "Ale", "wracajac", "do", "glownej", "opowiesci.", "Szedl", "tak", "sobie", "i", "szedl", "i", "szedl", "az", "w", "koncu", "ujrzal", "stojace", "nad", "przepascia", "bawoly.", "I", "zawolal.", "Hej,", "hej", "bawoleta.", "Chodzcie", "do", "mnie", "napijem", "sie", "wodki", "prosto", "z", "gwinta.", "Na", "co", "owe", "bawoly", "nie", "mogly", "odpowiedziec", "inaczej", "anizeli", "aprobujacym", "skinieciem", "swych", "wielkich", "rogatych", "lbow", "porosnietym", "futrem", "wschodniobizantyjskim", "ktore", "to", "sprowadzil", "w", "te", "rejony", "pewien", "cesarz", "indochinski", "pochodzacy", "z", "konstantynopola", "choc", "slyszalem", "tez", "inne", "wersje", "tej", "niebywalej", "i", "intrygujacej", "historii", "o", "ktorej", "opowiadac", "mozna", "co", "prawda", "wiele", "ale", "moj", "stary", "mozg", "i", "moje", "jeszcze", "starsze,", "pamietajace", "triumf", "wielkiej", "dziwki-matki,", "cialo", "nie", "pozwala", "mi", "na", "takie", "psycho-wybryki", "czy", "wynaturzenia", "kompletnie", "nieprzystajace", "do", "obecnej", "sytuacji", "gospodarczo-ekonomicznej", "ktora", "z", "tak", "wielkim", "przytupem", "zapanowala", "w", "naszym", "jakze", "niecudownym", "kraju,", "ktory", "przeciez", "szczycic", "sie", "moze", "tak", "wspanialymi", "artystami", "i", "wynalazkami", "ze", "nie", "chce", "mi", "sie", "nawet", "o", "nich", "wspominac"};
	EsteticPrinting(tab, 154, 133);
	return 0;
}
