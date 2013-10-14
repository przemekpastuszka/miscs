#include <iostream>
using namespace std;

void Merge(int* arrayOne, int sizeOne, int* arrayTwo, int sizeTwo, int* result)
{
	int i = 0, j = 0;
	while(i < sizeOne && j < sizeTwo)
	{
		if(arrayOne[i] < arrayTwo[j])
			result[i + j] = arrayOne[i++];
		else
			result[i + j] = arrayTwo[j++];
	}
	if(i == sizeOne)
		for(; j < sizeTwo; ++j)
			result[i + j] = arrayTwo[j];
	if(j == sizeTwo)
		for(; i < sizeOne; ++i)
			result[i + j] = arrayOne[i];
}

int* MergeSort(int* array, int p, int q)
{
	if(p > q)
		return 0;
	int* result = new int[q - p + 1];
	
	if(p == q)
	{
		result[0] = array[p];
		return result;
	}
	
	int s = (p + q) / 2;
	
	int* left = MergeSort(array, p, s);
	int* right = MergeSort(array, s + 1, q);
	Merge(left, s - p + 1, right, q - s, result);
	
	delete [] left;
	delete [] right;
	
	return result;
}

int* MergeSort(int* array, int size)
{
	return MergeSort(array, 0, size - 1);
}

int BinarySearch(int* array, int size, int element)
{	
	int p = 0, q = size - 1;
	
	while(p <= q)
	{
		int s = (p + q) / 2;
		if(array[s] == element)
			return s;
		if(p != q)
		{
			if(element < array[s])
				q = s - 1;
			else
				p = s + 1;
		}
		else
			break;
	}
	
	return -1;
}

//sprawdzamy czy w zbiorze liczb istnieją dwie, których suma jest równa x
//działa w czasie n log n
bool CanElementsSumingToGivenNumberBeFound(int* array, int size, int x) // fun :)
{
	int* sorted = MergeSort(array, size);
	
	for(int i = 0; i < size; ++i)
	{
		int tmp = BinarySearch(sorted, size, x - sorted[i]);
		if(tmp >= 0 && tmp != i)
		{
			delete [] sorted;
			return true;
		}
	}
	
	delete [] sorted;
	return false;
}

int main()
{
	int array[] = {20, 7, 18, 42, 51, 2, 9, 19};
	cout << CanElementsSumingToGivenNumberBeFound(array, 8, 31) << endl;
	return 0;
}
