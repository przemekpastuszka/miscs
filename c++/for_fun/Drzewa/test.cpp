#include <iostream>
#include "list.h"

#include "BRTree.h"

using namespace std;

struct Pair
{
	BRTree<int> :: Iterator it;
	int num;
	
	Pair(BRTree<int> :: Iterator q, int w) : it(q), num(w) {}
	//Pair() {}
};

void Wypisz2(BRTree<int> :: Iterator it)
{
	int last = 0;
	List<Pair> list;
	list.InsertBack(Pair(it, 1));
	while(!list.IsEmpty())
	{
		Pair q = *(list.GetIterator());
		list.Delete(list.GetIterator());
		if(q.num != last)
			cout << endl;
		last = q.num;
		if(q.it.IsEmpty())
			continue;
		cout << *q.it << " ";
		list.InsertBack(Pair(q.it.Left(), last + 1));
		list.InsertBack(Pair(q.it.Right(), last + 1));
		
	}
}

void Wypisz(BRTree<int> :: Iterator it)
{
	/*List<BTRee<int> :: Iterator> lista;
	lista.push_back(it);
	while(!lista.Empty())
	{
		
	}*/
	if(it.IsEmpty())
		return;
	cout << *it << " ";	
		
	Wypisz(it.Left());
	
	Wypisz(it.Right());
	
}

int main()
{
	BRTree<int> t;
	t.Insert(5);
	t.Insert(10);
	t.Insert(12);
	t.Insert(3);
	t.Insert(4);
	t.Insert(11);
	t.Insert(27);
	t.Insert(19);
	t.Insert(0);
	t.Insert(31);
	t.Insert(42);
	t.Insert(72);
	t.Insert(84);
	t.Insert(95);
	
	Wypisz2(t.GetRoot());
	return 0;
}
