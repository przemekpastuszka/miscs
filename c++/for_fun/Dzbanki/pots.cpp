/*

Mamy n dzbanków czerwonych i n niebieskich o różnych pojemnościach przy czym dla każdego dzbanka czerwonego istnieje dokładnie jeden dzbanek niebieski
o tej samej pojemności. Należy pogrupować dzbanki w pary.
Problem leży w tym, że pojemności można porównywać tylko między dzbankami niebieskimi i czerwonymi. Nie można porównać dwóch czerwonych ani
dwóch niebieskich

Zad: zaprojektuj algorytm, który rozwiąze ten problem w oczekiwanym czasie n log n (pesymistyczny: n ^ 2)

*/

#include <iostream>
#include <time.h>
#include <cstdlib>
using namespace std;

struct Node
{
	int value;
	Node *left, *right;
	
	Node(int v) : value(v), left(0), right(0) {}
	~Node()
	{
		if(left)
			delete left;
		if(right)
			delete right;
	}
};

void Swap(int red[], int i, int j)
{
	int tmp = red[i];
	red[i] = red[j];
	red[j] = tmp;
}

int Partition(int red[], int p, int k, int blue)
{
	int i = p;
	for(int j = p; j < k;)
	{
		if(red[j] < blue)
			Swap(red, i++, j++);
		else
		{
			if(red[j] == blue)
				Swap(red, j, k);
			else
				++j;
		}		
	}
	Swap(red, i, k);
	return i;
}

void Match(int blue[], int red[], int n)
{
	Node* tree = 0;
	for(int i = 0; i < n; ++i)
	{
		int p = 0; int k = n - 1;
		Node* tmp = tree, *parent = 0;
		while(tmp)
		{
			parent = tmp;
			if(blue[i] < red[tmp -> value])
			{
				k = tmp -> value - 1;
				tmp = tmp -> left;
			}
			else
			{
				p = tmp -> value + 1;
				tmp = tmp -> right;
			}
		}
		
		if(p < k)
		{
			int q = Partition(red, p, k, blue[i]);
			Node* next = new Node(q);
			if(!parent)
				tree = next;
			else
			{
				if(blue[i] < red[parent -> value])
					parent -> left = next;
				else
					parent -> right = next;
			}
		}
	}
	
	if(tree)
		delete tree;
}

int Randomize(int a, int b)
{
	return a + rand() % (b - a + 1);
}

void Permute(int tab[], int n)
{
	for(int i = 0; i < n - 1; ++i)
		Swap(tab, i, Randomize(i + 1, n - 1));
}

int main()
{
	srand(time(NULL));
	
	const int n = 16;
	int blue[n], red[n];
	for(int i = 0; i < n; ++i)
		blue[i] = red[i] = i;
		
	Permute(red, n);	
	for(int i = 0; i < n; ++i)
		cout << red[i] << " ";
	cout << endl;
	
	Match(blue, red, n);
	
	for(int i = 0; i < n; ++i)
		cout << red[i] << " ";
	cout << endl;
	
	return 0;
}
