#ifndef RTSHADOW_BRTREE
#define RTSHADOW_BRTREE

template <class T>
class BRTree
{
    protected:
    enum Color {BLACK, RED};
    struct Node
    {       
        Color color;
        T value;
        Node *left, *right, *parent;
        
        Node() : left(0), right(0), parent(0), color(RED) {}
        ~Node()
        {
            if(left)
                delete left;
            if(right)
                delete right;
        }
    };
    Node* root;
    
    void RotateLeft(Node* x);
    void RotateRight(Node* x);
    void InsertFixup(Node* x);
    
    public:
    class Iterator
    {
        private:
        Node* current;
        Iterator(Node* t) : current(t) {}
        
        public:
        Iterator(const Iterator& it)
        {
            *this = it;
        }
        
        T operator*() const { return current -> value; }
        Iterator Left() { return Iterator(current -> left); }
        Iterator Right() { return Iterator(current -> right); }
        Iterator Up() { return Iterator(current -> parent); }
        bool IsEmpty() const { return !current; }
        
        friend class BRTree;
    };
    
    BRTree() : root(0) {}
    ~BRTree()
    {
        if(root)
            delete root;
    }
    Iterator GetRoot() const { return Iterator(root); }
    virtual Iterator Insert(const T value);
    virtual Iterator Find(const T value) const;
};


template <class T>
typename BRTree<T> :: Iterator BRTree<T> :: Insert(const T value)
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
    
    Node* tmp = new Node();
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
    InsertFixup(tmp);
    return Iterator(tmp);
}

template <class T>
typename BRTree<T> :: Iterator BRTree<T> :: Find(const T value) const
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

template <class T>
void BRTree<T> :: RotateLeft(Node* x)
{
    Node* y = x -> right;
    x -> right = y -> left;
    y -> left = x;
    y -> parent = x -> parent;
    x -> parent = y;
    
    if(y -> parent)
    {
        if(y -> parent -> left == x)
            y -> parent -> left = y;
        if(y -> parent -> right == x)
            y -> parent -> right = y;
    }
    else
        root = y;
}

template <class T>
void BRTree<T> :: RotateRight(Node* y)
{
    Node* x = y -> left;
    y -> left = x -> right;
    x -> right = y;
    x -> parent = y -> parent;
    y -> parent = x;
    if(x -> parent)
    {
        if(x -> parent -> left == y)
            x -> parent -> left = x;
        if(x -> parent -> right == y)
            x -> parent -> right = x;
    }
    else
        root = x;
}

template <class T>
void BRTree<T> :: InsertFixup(Node* x)
{
    while(x -> parent && x -> parent -> color == RED)
    {
        if(x -> parent == x -> parent -> parent -> left)
        {
            Node* y = x -> parent -> parent -> right;
            if(y && y -> color == RED)
            {
                x -> parent -> color = BLACK;
                y -> color = BLACK;
                x -> parent -> parent -> color = RED;
                x = x -> parent -> parent;
            }
            else
            {
                if(x == x -> parent -> right)
                {
                    x = x -> parent;
                    RotateLeft(x);
                }
                x -> parent -> color = BLACK;
                x -> parent -> parent -> color = RED;
                RotateRight(x -> parent -> parent);
            }
        }
        else
        {
            Node* y = x -> parent -> parent -> left;
            if(y && y -> color == RED)
            {
                x -> parent -> color = BLACK;
                y -> color = BLACK;
                x -> parent -> parent -> color = RED;
                x = x -> parent -> parent;
            }
            else
            {
                if(x == x -> parent -> left)
                {
                    x = x -> parent;
                    RotateRight(x);
                }
                x -> parent -> color = BLACK;
                x -> parent -> parent -> color = RED;
                RotateLeft(x -> parent -> parent);
            }
        }
    }
    
    root -> color = BLACK;
}
#endif
