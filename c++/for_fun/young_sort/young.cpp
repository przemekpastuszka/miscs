#include <iostream>
#include <cmath>

using namespace std;

const int MAX_NUM = 1000000;

void Swap(int* young[], int i, int j, int a, int b)
{
	int tmp = young[i][j];
	young[i][j] = young[a][b];
	young[a][b] = tmp;
}

void InsertToYoung(int* young[], int k, int element)
{
	young[k][k] = element;
	int i = k, j = k;
	while(young[i - 1][j] > young[i][j] || young[i][j - 1] > young[i][j])
	{
		int nI = i - 1, nJ = j;
		if(young[i - 1][j] < young[i][j - 1])
		{
			nI = i;
			nJ = j - 1;
		}
		Swap(young, i, j, nI, nJ);
		i = nI;
		j = nJ;
	}
}

int GetMin(int* young[])
{
	return young[1][1];
}

void ExtractMin(int* young[])
{
	young[1][1] = MAX_NUM;
	int i = 1, j = 1;
	while(young[i][j] > young[i + 1][j] || young[i][j] > young[i][j + 1])
	{
		int nI = i + 1, nJ = j;
		if(young[i + 1][j] > young[i][j + 1])
		{
			nI = i;
			nJ = j + 1;
		}
		Swap(young, i, j, nI, nJ);
		i = nI;
		j = nJ;
	}
}

//sortuje tablicę za pomocą sortowania younga (czas: n ^ 3/2)
void YoungSort(int elements[], int n)
{
	int k = (int)sqrt(n) + 1;
	
	int **tab = new int*[k + 2]; //+ 2 bo dodam wartowników
	for(int i = 0; i < k + 2; ++i)
		tab[i] = new int[k + 2];
		
	for(int i = 1; i < k + 2; ++i) //na dole i z prawej strony wartownicy MAX_NUM
		for(int j = 1; j < k + 2; ++j)
			tab[i][j] = MAX_NUM;
	for(int i = 0; i < k + 2; ++i)
		tab[0][i] = tab[i][0] = -MAX_NUM; //u góry i z lewej wartownicy -MAX_NUM
		
	
	for(int i = 0; i < n; ++i)
		InsertToYoung(tab, k, elements[i]);
	for(int i = 0; i < n; ++i)
	{
		elements[i] = GetMin(tab);
		ExtractMin(tab);
	}
		
	for(int i = 0; i < k + 2; ++i)
		delete [] tab[i];
	delete [] tab;
}

int main()
{
	int t[] = {7, 21, 32, 48, 9, 19, 22, 34, 88, 100};
	YoungSort(t, 10);
	for(int i = 0; i < 10; ++i)
		cout << t[i] << endl;
	return 0;
}
