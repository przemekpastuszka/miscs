#include <vector>

template <class T>
class Heap
{
	private:
	int heapSize;
	std :: vector<T> heap;
	bool (*comparator)(T a, T b);
	
	void Heapify(int i);
	
	public:
	Heap(bool (*c)(T a, T b)) : comparator(c), heapSize(0) 
	{
		heap.resize(2);
	}
	//~Heap();
	
	T GetMin() const;
	void ExtractMin();
	void Insert(const T value);
	bool IsEmpty() const;
};

template <class T>
bool Heap<T> :: IsEmpty() const
{
	return heapSize <= 0;
}

template <class T>
T Heap<T> :: GetMin() const
{
	return heap[1];
}

template <class T>
void Heap<T> :: ExtractMin()
{
	if(heapSize <= 0)
		return;
	heap[1] = heap[heapSize];
	--heapSize;
	heap.resize(heapSize + 1);
	Heapify(1);
}

template <class T>
void Heap<T> :: Heapify(int i)
{
	int min = i;
	if(2 * i <= heapSize && comparator(heap[2 * i], heap[i]))
		min = 2 * i;
	if(2 * i + 1 <= heapSize && comparator(heap[2 * i + 1], heap[min]))
		min = 2 * i + 1;
	if(min != i)
	{
		T tmp = heap[i];
		heap[i] = heap[min];
		heap[min] = tmp;
		Heapify(min);
	}
}

template <class T>
void Heap<T> :: Insert(const T value)
{
	int i = ++heapSize;
	heap.resize(heapSize + 1);
	heap[heapSize] = value;
	
	while(i > 1 && comparator(heap[i], heap[i / 2]))
	{
		T tmp = heap[i];
		heap[i] = heap[i / 2];
		heap[i / 2] = tmp;
		i /= 2;
	}
}
