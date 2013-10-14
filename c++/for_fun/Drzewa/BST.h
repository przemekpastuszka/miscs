#ifndef RTSHADOW_BST
#define RTSHADOW_BST

template <class T>
class BST
{
	private:
	static Node* CreateBSTNode() { return new Node(); }
	
	protected:
	struct Node
	{
		T value;
		Node *left, *right, *parent;
		
		Node() : left(0), right(0), parent(0) {}
		~Node()
		{
			if(left)
				delete left;
			if(right)
				delete right;
		}
	};
	Node* root;
	Node* (*NodeCreator)();
	
	public:
	class Iterator
	{
		private:
		Node* current;
		Iterator(Node* t) : current(t) {}
		
		public:
		T operator*() const { return current -> value; }
		bool Left() { return current = current -> left; }
		bool Right() { return current = current -> right; }
		bool Up() { return current = current -> parent; }
		bool IsEmpty() const { return !current; }
		
		friend class BST;
	};
	
	BST() : root(0), NodeCreator(BST<T> :: CreateBSTNode) {}
	~BST()
	{
		if(root)
			delete root;
	}
	Iterator GetIterator() const { return Iterator(root); }
	virtual Iterator Insert(const T value);
	virtual Iterator Find(const T value) const;
};


template <class T>
typename BST<T> :: Iterator BST<T> :: Insert(const T value)
{
	Node *parent = 0, *current = root;
	while(current)
	{
		parent = current;
		if(value <= current -> value)
			current = current -> left;
		else
			current = current -> right;
	}
	
	Node* tmp = NodeCreator();
	tmp -> value = value;
	tmp -> parent = parent;
	if(!parent)
		root = tmp;
	else
	{
		if(value <= parent -> value)
			parent -> left = tmp;
		else
			parent -> right = tmp;
	}
	return Iterator(tmp);
}

template <class T>
typename BST<T> :: Iterator BST<T> :: Find(const T value) const
{
	Node* tmp = root;
	while(tmp)
	{
		if(tmp -> value == value)
			return Iterator(tmp);
		if(value < tmp -> value)
			tmp = tmp -> left;
		else
			tmp = tmp -> right;
	}
	return Iterator(0);
}
#endif
