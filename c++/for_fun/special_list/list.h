/*

Stwarzamy listę, która ma wszystkie zalety listy dwukierunkowej, ale potrzebuje tylko jednego
dodatkowego wskaźnika na element, który zawsze zawiera wynik operacji:
XOR(poprzednik, nastepnik)

*/

template <class T>
class List
{
	private: //PRIVATE
	struct Node
	{
		T value;
		Node* np;
	};
	
	//ZMIENNE
	Node *head, *tail;
	bool usable; //gdy lista zostanie dołączona do innej nie może już zostać użyta
	unsigned long listNumber;
	
	static unsigned long listsInTotal;
	
	//FUNKCJE
	static Node* XOR(Node* a, Node* b) { return (Node*)((long)a ^ (long)b); }
	static void Move(Node** prev, Node** head); //mając element poprzedni i aktualny przesuń wskaźniki odpowiednio na aktualny i następny
	void Delete(Node* pprev, Node* prev, Node* tmp);
	
	/*********************************************************************************/
	public: //PUBLIC
	class Iterator
	{
		private:
		List<T> :: Node *pprev, *prev, *current;
		unsigned long listNumber;
		
		Iterator(Node* a, Node* b, Node* c, unsigned long n) : pprev(a), prev(b), current(c), listNumber(n) {}
		
		public:
		bool Move();
		T Get() const;
		void Set(const T value) const;
		bool IsEnd() const;
		
		Iterator& operator++()
		{
			Move();
			return *this;
		}
		T& operator*() const
		{
			return current -> value;
		}
		
		friend class List;
	};
	
	List() : head(0), tail(0), usable(true) { listNumber = listsInTotal++; }
	List(const List<T>& x) : usable(false) { *this = x; }
	~List();
	
	void InsertFront(const T value);
	void InsertBack(const T value);
	
	bool IsEmpty() const { return !head; }
	bool Contains(const T value) const; //sprawdza czy taki element znajduje się na liście
	Iterator Find(const T value) const;
	
	void Reverse();
	void Join(List<T> &ls); //dołącza na koniec listę ls. Po wykonaniu tej operacji lista ls nie nadaje się do użytku
	
	void Delete(const T value); //usuwa pierwsze wystąpienie danego el. na liście
	void Delete(const Iterator &it);
	
	Iterator GetIterator() const { return Iterator(0, 0, head, listNumber); }
	Iterator GetReversedIterator() const { return Iterator(0, 0, tail, listNumber); }
	
	List<T>& operator=(const List<T>& x);
};

template <class T> 
List<T>& List<T> :: operator=(const List<T>& x)
{
	this -> ~List();
	if(!x.usable)
	{
		usable = false;
		return *this;
	}
	usable = true;
	for(List<T> :: Iterator it = x.GetIterator(); !it.IsEnd(); ++it)
		InsertBack(*it);
	listNumber = listsInTotal++;
	
	return *this;
}

template <class T>
unsigned long List<T> :: listsInTotal = 0;

template <class T>
void List<T> :: Move(Node** prev, Node** head)
{
	Node* tmp = *head;
	*head = XOR(*prev, tmp -> np);
	*prev = tmp;
} 

template <class T>
bool List<T> :: Iterator :: IsEnd() const
{
	return !current;
}

template <class T>
bool List<T> :: Iterator :: Move()
{
	pprev = prev;
	List<T> :: Move(&prev, &current);
	return current;
}

template <class T>
T List<T> :: Iterator :: Get() const
{
	return current -> value;
}

template <class T>
void List<T> :: Iterator :: Set(const T value) const
{
	current -> value = value;
}

template <class T>
List<T> :: ~List()
{
	head = tail = 0;
	
	if(!usable)
		return;
	Node* prev = 0;
	while(head)
	{
		Move(&prev, &head);
		delete prev;
	}	
}

template <class T>
void List<T> :: InsertFront(const T value)
{
	if(!usable)
		return;
		
	Node* node = new Node();
	node -> value = value;
	node -> np = XOR(0, head);
	if(head)
		head -> np = XOR(node, XOR(0, head -> np));
	head = node;
	if(!tail)
		tail = node;
}

template <class T>
void List<T> :: InsertBack(const T value)
{
	if(!usable)
		return;
		
	Node* node = new Node();
	node -> value = value;
	node -> np = XOR(0, tail);
	if(tail)
		tail -> np = XOR(node, XOR(0, tail -> np));
	tail = node;
	if(!head)
		head = node;
}

template <class T>
bool List<T> :: Contains(const T value) const
{
	if(!usable)
		return false;
		
	Node* prev = 0, *tmp = head;
	while(tmp)
	{
		if(tmp -> value == value)
			return true;
		Move(&prev, &tmp);
	}
	return false;
}

template <class T>
void List<T> :: Reverse()
{
	Node* tmp = head;
	head = tail;
	tail = tmp;
}

template <class T>
typename List<T> :: Iterator List<T> :: Find(const T value) const
{
	if(!usable)
		return Iterator(0, 0, 0, listNumber);
		
	Node *prev = 0, *pprev = 0, *tmp = head;
	while(tmp)
	{
		if(tmp -> value == value)
			break;
		pprev = prev;
		Move(&prev, &tmp);
	}
	
	return Iterator(pprev, prev, tmp, listNumber);
}

template <class T>
void List<T> :: Delete(Node* pprev, Node* prev, Node* tmp)
{
	if(!tmp)
		return;
		
	Node* next = XOR(prev, tmp -> np);
	if(prev)
		prev -> np = XOR(pprev, next);
	if(next)
		next -> np = XOR(prev, XOR(tmp, next -> np));
	
	head = prev ? head : next;
	tail = head ? tail : 0;
		
	delete tmp;
	listNumber = listsInTotal++;
}

template <class T>
void List<T> :: Delete(const T value)
{
	Delete(Find(value));
}

template <class T>
void List<T> :: Delete(const Iterator &it)
{
	if(!usable || it.listNumber != listNumber)
		return;
		
	Delete(it.pprev, it.prev, it.current);
}

template <class T>
void List<T> :: Join(List<T> &ls)
{
	if(!usable)
		return;
		
	if(!head)
		head = ls.head;
	else
	{
		tail -> np = XOR(XOR(0, tail -> np), ls.head);
		if(ls.head)
			ls.head -> np = XOR(tail, XOR(0, ls.head -> np));
	}
	if(ls.tail)
		tail = ls.tail;
	ls.usable = false;
	listNumber = listsInTotal++;
}
