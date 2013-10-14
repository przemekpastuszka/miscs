#include "list.h"
#include <iostream>

using namespace std;

typedef List<int> :: Iterator Iterator;


void WypiszWymaluj(List<int> pies)
{
	Iterator it = pies.GetIterator();
	*it = 17;
	pies.Delete(++it);
	
	for(Iterator it = pies.GetIterator(); !it.IsEnd(); ++it)
		cout << *it << endl;
	cout << endl;
}

int main()
{
	List<int> kupa, lupa;
	kupa.InsertFront(5);
	kupa.InsertBack(3);
	lupa.InsertBack(10);
	lupa.InsertFront(7);
	kupa.Join(lupa);
	
	WypiszWymaluj(kupa);
	
	for(Iterator it = kupa.GetIterator(); !it.IsEnd(); ++it)
		cout << *it << endl;
	cout << endl;

	
	return 0;
}
