#include <iostream>
using namespace std;

struct Node
{
	int value;
	Node *left, *right, *parent;
	Node(int v) : value(v), parent(0), left(0), right(0) {}
	~Node()
	{
		if(left)
			delete left;
		if(right)
			delete right;
	}
	
};

void InsertToBST(Node** t, int value)
{
	Node* n = new Node(value);
	if(!(*t))
	{
		*t = n;
		return;
	}
	Node* parent = 0, *current = *t;
	while(current)
	{
		parent = current;
		if(value <= current -> value)
			current = current -> left;
		else
			current = current -> right;
	}
	n -> parent = parent;
	if(value <= parent -> value)
		parent -> left = n;
	else
		parent -> right = n;
}

void PrintNaive(Node* t)
{
	if(!t)
		return;
	PrintNaive(t -> left);
	cout << t -> value << " ";
	PrintNaive(t -> right);
}

//zabawa polega na tym, by wypisać wszystkie węzły drzewa w czasie O(n), ale wykorzystując tylko stałą ilość dodatkowej pamięci,
//nierekurencyjnie i nie modyfikując (nawet tymczasowo) struktury drzewa
void PrintFuckinAwesome(Node *t)
{
	if(!t)
		return;
		
	int last = 0;
	int num = 1;
	Node* current = t;
	while(num > 0)
	{
		if(last < num && current -> left)
		{
			last = num;
			num *= 2;
			current = current -> left;
			continue;
		}
		if(last != num * 2 + 1)
			cout << current -> value << endl;
		if(last != num * 2 + 1 && current -> right)
		{
			last = num;
			num = num * 2 + 1;
			current = current -> right;
			continue;
		}
		
		last = num;
		num /= 2;
		current = current -> parent;
	}
}

int main()
{
	Node* tree = 0;
	InsertToBST(&tree, 21);
	InsertToBST(&tree, 100);
	InsertToBST(&tree, 254);
	InsertToBST(&tree, 99);
	InsertToBST(&tree, 19);
	InsertToBST(&tree, 20);
	
	PrintFuckinAwesome(tree);
	
	delete tree;
	return 0;
}
