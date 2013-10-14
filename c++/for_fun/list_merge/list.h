template <class T>
class List
{
	private:
	struct Node
	{
		T value;
		Node* next;
	};
	
	Node *head, *tail;
	
	public:
	List() : head(0), tail(0) {}
	~List();
	
	T GetFront() const;
	T GetBack() const;
	//void ExtractBack();
	void ExtractFront();
	void PushBack(const T value);
	void PushFront(const T value);
	bool IsEmpty() const;
};

template <class T>
T List<T> :: GetFront() const
{
	return head -> value;
}

template <class T>
T List<T> :: GetBack() const
{
	return tail -> value;
}

template <class T>
bool List<T> :: IsEmpty() const
{
	return !head;
}

template <class T>
void List<T> :: PushFront(const T value)
{
	Node* tmp = new Node();
	tmp -> value = value;
	tmp -> next = head;
	head = tmp;
	if(!tail)
		tail = head;
}

template <class T>
void List<T> :: PushBack(const T value)
{
	Node* tmp = new Node();
	tmp -> value = value;
	tmp -> next = 0;
	if(!tail)
		head = tail = tmp;
	else
	{
		tail -> next = tmp;
		tail = tmp;
	}
}

template <class T>
void List<T> :: ExtractFront()
{
	Node* tmp = head;
	head = head -> next;
	if(!head)
		tail = 0;
	delete tmp;
}

template <class T>
List<T> :: ~List()
{
	while(head)
	{
		Node* tmp = head;
		head = head -> next;
		delete tmp;
	}
}
