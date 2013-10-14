#include <iostream>
#include <cmath>

using namespace std;

//specjalny binary search (znajduje ostatnie wystąpienie zadanej wartości lub największej z mniejszych od niej)
int BSearch(int* tab, int p, int k, int value)
{
	++value;
	while(p < k)
	{
		int q = (p + k) / 2;
		if(value <= tab[q])
			k = q - 1;
		else
			p = q + 1;
	}
	return tab[p] < value ? p : p - 1;
}

const int MAX_NUM = 32000;

//znajduje najdłuższy niemalejący podciąg w czasie n log n (wypisuje w odwrotnej kolejności ze względu na lenistwo)
void NWP(int* tab, int n)
{
	int* tmp = new int[n];
	int *tmp2 = new int[n];
	int* pred = new int[n];
	for(int i = 0; i < n; ++i)
	{
		tmp[i] = MAX_NUM;
		pred[i] = -1;
	}
	int k = 0;
	tmp[0] = tab[0];
	tmp2[0] = 0;
	for(int i = 1; i < n; ++i)
	{
		int q = BSearch(tmp, 0, k, tab[i]);
		if(q >= 0)
			pred[i] = tmp2[q];
		if(tab[i] < tmp[q + 1])
		{
			tmp[q + 1] = tab[i];
			tmp2[q + 1] = i;
		}
		k = max(k, q + 1);
	}
	
	int last = tmp2[k];
	while(last >= 0)
	{
		cout << tab[last] << " ";
		last = pred[last];
		
	}
	cout << endl;
	
	delete [] tmp;
	delete [] pred;
}

int main()
{
	int tab[] = {7, 4, 13, 14, 29, 18, 4, 15, 61, 2, 77, 28, 10, 5, 9};
	NWP(tab, 13);
	return 0;
}
