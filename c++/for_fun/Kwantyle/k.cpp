#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;


void Swap(int tab[], int i, int j)
{
	int tmp = tab[i];
	tab[i] = tab[j];
	tab[j] = tmp;
}

int Randomize(int a, int b)
{
	return a + rand() % (b - a + 1);
}

int RandomizedPartition(int tab[], int p, int k)
{
	int i = p;
	Swap(tab, Randomize(p, k), k);
	int x = tab[k];
	for(int j = p; j < k; ++j)
		if(tab[j] < x)
			Swap(tab, i++, j);
			
	Swap(tab, i, k);
	return i;
}

int FindNthNumber(int elements[], int p, int k, int n)
{
	n += p;
	while(p < k)
	{
		int q = RandomizedPartition(elements, p, k);
		if(q == n)
			return elements[q];
		if(q < n)
			p = q + 1;
		else
			k = q - 1;
	}
	
	return elements[p];
}


//zadaniem jest odnalezienie kwantyli, czyli n takich liczb, które posortowany zbiór podzieliłyby na n + 1 równolicznych (no, prawie) obszarów
//czas działania: (k - p + 1) * log n
void Kwantyle(int elements[], int p, int k, int n)
{
	if(n <= 0)
		return;
	if(n == 1)
	{
		cout << FindNthNumber(elements, p, k, (k - p + 1) / 2) << " ";
		return ;
	}
		
	int area = (k - p + 1) / (n + 1); //wymiar jednego obszaru
	int medium = n / 2; //szukany kwantyl
	
	int kw = FindNthNumber(elements, p, k, medium * area);
	
	Kwantyle(elements, p, p + medium * area - 1, medium - 1);
	cout << kw << " ";
	Kwantyle(elements, p + medium * area + 1, k, n - medium);
	
}

int main()
{
	srand(time(NULL));
	
	const int n = 17;
	int tab[n];
	for(int i = 0; i < n; ++i)
	{
		tab[i] = Randomize(1, 100);
		cout << tab[i] << " ";
	}
	cout << endl;
	
	//cout << FindNthNumber(tab, 0, n - 1, n / 2) << endl;
	Kwantyle(tab, 0, n - 1, 5);
	cout << endl;
	
	for(int i = 0; i < n; ++i)
		cout << tab[i] << " ";
	cout << endl;
	return 0;
}
