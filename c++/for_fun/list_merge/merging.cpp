#include <iostream>
#include "list.h"
#include "heap.h"

using namespace std;

template <class T>
bool CompareLists(List<T>* a, List<T>* b)
{ 
	return a -> GetFront() < b -> GetFront();
}

//scal k posortowanych list, zawierających łącznie n elementów, w czasie n log k
template <class T>
List<T>* Merge(List<T>* lists[], int k)
{
	List<T>* result = new List<T>();

	Heap<List<T>*> heap(CompareLists<T>);
	for(int i = 0; i < k; ++i)
		if(!lists[i] -> IsEmpty())
			heap.Insert(lists[i]);

	while(!heap.IsEmpty())
	{
		List<T>* tmp = heap.GetMin();
		heap.ExtractMin();
		result -> PushBack(tmp -> GetFront());
		tmp -> ExtractFront();
		if(!tmp -> IsEmpty())
			heap.Insert(tmp);
	}
	
	return result;
}

bool C(int a, int b) { return a < b; }

int main()
{
	const int nDivK = 10;
	const int k = 10;
	List<int>** lists = new List<int>*[k];

	int counter = 0;
	for(int i = 0; i < k; ++i)
	{
		lists[i] = new List<int>();
		for(int j = 0; j < nDivK; ++j)
			lists[i] -> PushBack(++counter);
	}

	List<int>* ha = Merge<int>(lists, k);

	while(!ha -> IsEmpty())
	{
		cout << ha -> GetFront() << endl;
		ha -> ExtractFront();
	}

	delete ha;
	
	for(int i = 0; i < k; ++i)
		delete lists[i];
		
	delete [] lists;
	
	return 0;
}
