#include <iostream>

using namespace std;

struct Range
{
	int a, b;
	
	Range() {}
	Range(int x, int y) : a(x), b(y) {}
	
	bool IsEmpty() { return a > b; }
};

Range Intersect(Range one, Range two)
{
	return Range(one.a > two.a ? one.a : two.a, one.b < two.b ? one.b : two.b);
}

void Swap(Range elements[], int i, int j)
{
	Range tmp = elements[i];
	elements[i] = elements[j];
	elements[j] = tmp;
}

//prawie jak perełka
Range Partition(Range elements[], int p, int k)
{
	int i = p, bound = k;
	Range x = elements[k];
	Range intersected = x;
	
	for(int j = p; j < bound;)
	{
		Range tmp = Intersect(intersected, elements[j]);
		if(!tmp.IsEmpty())
		{
			++j;
			intersected = tmp;
		}
		else
		{
			if(elements[j].a < x.a)
				Swap(elements, i++, j++);
			else
				Swap(elements, j, --bound);
		}
	}
	Swap(elements, bound, k);
	
	return Range(i, bound);
}

//zadanie jest takie: znajdź taką permutację zadanych przedziałów <i1, i2, ..., iN>, że istnieją takie c1, ..., cN, że
//dla każdego j cJ należy do iJ oraz c1 <= c2 <= ... <= cN
//cała magia odbywa się w procedurze partition - im więcej zachodzących na siebie przedziałów, tym szybciej działa algorytm
void QuickSort(Range elements[], int p, int k)
{
	if(p >= k)
		return;
	
	Range x = Partition(elements, p, k);
	QuickSort(elements, p, x.a - 1);
	QuickSort(elements, x.b + 1, k);
}

int main()
{

	Range tab[] = {Range(3, 6), Range(5, 9), Range(2, 11), Range(4, 7), Range(11, 13), Range(1, 2), Range(13, 15), Range(16, 17), Range(10, 12)};
	QuickSort(tab, 0, 8);
	
	for(int i = 0; i < 9; ++i)
		cout << tab[i].a << ", " << tab[i].b << endl;
		
	return 0;
}
