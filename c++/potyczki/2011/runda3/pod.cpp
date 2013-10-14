#include <cstdio>
#include <list>


#define NUM int
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)
#define LIST(x) std :: list<x>

struct Interval{
    NUM a, b;
    
    Interval(NUM x, NUM y) : a(x), b(y) {}
    Interval() {}
    
    bool is_valid() { return a <= b; }
    NUM length() { return b - a; }
    Interval translate(NUM k){
        return Interval(a + k, b + k);
    }
};

Interval intersect(const Interval& x, const Interval& y){
    return Interval(MAX(x.a, y.a), MIN(x.b, y.b));
}

class AVL{
    
    public:
    
    class Node{
        public:
        Node *left, *right, *parent;
        int height;
        
        Node* balance();
        int calc_balance();
        void update_data();
        Node* rotate_left();
        Node* rotate_right();
        Node* finish_rotation(Node* new_root);
        
        void find_intersecting(Interval i, LIST(Interval)* ints, LIST(AVL :: Node*)* nodes);
        
        public:
        Interval key;
        Interval value;
        Interval spread;
        
        Node(Interval k, Interval v, Node* p) : left(0), right(0), parent(p), height(1), key(k), value(v), spread(k) {}
        ~Node(){
            if(left) {delete left;}
            if(right) {delete right;}
        }
        
        void print(){
            printf("[(%d, %d) : (%d, %d), <%d, %d>] ", key.a, key.b, value.a, value.b, spread.a, spread.b);
        }
    };
    
    Node* root;
    Node* prev(Node* x);
    Node* next(Node* x);
    void remove(Node* x);
    
    void print(){
        LIST(Node*) ls[2];
        int current = 0;
        ls[current].push_back(root);
        while(!ls[current].empty()){
            while(!ls[current].empty()){
                int next = (current + 1) % 2;
                Node* tmp = ls[current].front();
                if(tmp){
                    tmp -> print();
                    ls[next].push_back(tmp -> left);
                    ls[next].push_back(tmp -> right);
                }
                ls[current].pop_front();
            }
            current = (current + 1) % 2;
            printf("\n");
        }
    }
    
    public:
    
    AVL() : root(0) {}
    ~AVL(){
        if(root) {delete root;}
    }

    void insert(Interval k, Interval v);
    void find_intersecting(Interval i, LIST(Interval)* ints, LIST(AVL :: Node*)* nodes){
        if(root){
            root -> find_intersecting(i, ints, nodes);
        }
    }
};

bool cmp(Interval x, Interval y){
    return x.a < y.a || (x.a == y.a && x.b < y.b);
}

bool cmp2(Interval x, Interval y){
    return x.a == y.a && x.b == y.b;
}

int main(){
    int n, m, p;
    scanf("%d %d %d", &n, &m, &p);
    
    AVL tree;
    
    LIST(Interval)* levels = new LIST(Interval)[n];
    LIST(AVL :: Node*) to_be_deleted;
    
    for(int i = 0; i < m; ++i){
        Interval x, y;
        scanf("%d %d %d %d", &x.a, &x.b, &y.a, &y.b);
        tree.insert(x, y);
        tree.insert(y, x);
    }
    
    // tree.print();
    
    levels[0].push_back(Interval(p, p));
    for(int i = 1; i < n; ++i){
        for(LIST(Interval) :: iterator it = levels[i - 1].begin(); it != levels[i - 1].end(); it++){
            tree.find_intersecting(*it, &levels[i], &to_be_deleted);
        }
        to_be_deleted.sort();
        to_be_deleted.unique();
        levels[i].sort(cmp);
        levels[i].unique(cmp2);
        
        while(!to_be_deleted.empty()){
            tree.remove(to_be_deleted.front());
            to_be_deleted.pop_front();
        }
    }
    
    delete [] levels;
    return 0;
}

void AVL :: Node :: find_intersecting(Interval i, LIST(Interval)* ints, LIST(AVL :: Node*)* nodes){
    if(intersect(i, key).is_valid()){
        ints -> push_back(value);
        nodes -> push_back(this);
    }
    if(left && intersect(i, left -> spread).is_valid()){
        left -> find_intersecting(i, ints, nodes);
    }

    if(right && intersect(i, right -> spread).is_valid()){
        right -> find_intersecting(i, ints, nodes);
    }
}

AVL :: Node* AVL :: prev(AVL :: Node* x){
    if(x -> left){
        Node* tmp = x -> left;
        while(tmp -> right){
            tmp = tmp -> right;
        }
        return tmp;
    }

    while(x -> parent && x -> parent -> left == x){
        x = x -> parent;
    }
    return x -> parent;
}

AVL :: Node* AVL :: next(AVL :: Node* x){
    if(x -> right){
        Node* tmp = x -> right;
        while(tmp -> left){
            tmp = tmp -> left;
        }
        return tmp;
    }

    while(x -> parent && x -> parent -> right == x){
        x = x -> parent;
    }
    return x -> parent;
}

/*int AVL :: get_max(KEY k){
    int max = 0;
    Node* current = root;
    while(current){
        int left_max = current -> left && current -> left -> max > current -> value ? 
            current -> left -> max : current -> value;
        if(current -> key == k){
            return MAX(max, left_max);
        }
        if(k < current -> key){
            current = current -> left;
        }
        else{
            max = MAX(max, left_max);
            current = current -> right;
        }
    }
    return max;
}*/

AVL :: Node* AVL :: Node :: balance(){
    int b = calc_balance();
    if(b == -2){
        if(right && right -> calc_balance() > 0){
            right -> rotate_right();
        }
        return rotate_left();
    }
    if(b == 2){
        if(left && left -> calc_balance() < 0){
            left -> rotate_left();
        }
        return rotate_right();
    }
    update_data();
    return this;
}

void AVL :: insert(Interval k, Interval v){
    if(!root){
        root = new Node(k, v, 0);
        return;
    }
    Node *parent = 0, *current = root;
    while(current){
        parent = current;
        if(k.a <= current -> key.a){
            current = current -> left;
        }
        else{
            current = current -> right;
        }
    }
    
    Node* tmp = new Node(k, v, parent);
    if(k.a <= parent -> key.a){
        parent -> left = tmp;
    }
    else{
        parent -> right = tmp;
    }
    while(parent){
        root = parent -> balance();
        parent = parent -> parent;
    }
}

int AVL :: Node :: calc_balance(){
    int l = left ? left -> height : 0;
    int r = right ? right -> height : 0;
    return l - r;
}

AVL :: Node* AVL :: Node :: rotate_left(){
    Node* new_root = right;
    right = new_root -> left;
    new_root -> left = this;
    if(right){
        right -> parent = this;
    }
    return finish_rotation(new_root);
}

AVL :: Node* AVL :: Node :: rotate_right(){
    Node* new_root = left;
    left = new_root -> right;
    new_root -> right = this;
    if(left){
        left -> parent = this;
    }
    return finish_rotation(new_root);
}

AVL :: Node* AVL :: Node :: finish_rotation(AVL :: Node* new_root){
    new_root -> parent = parent;
    parent = new_root;
    update_data();
    new_root -> update_data();
    if(new_root -> parent && new_root -> parent -> left == this){
        new_root -> parent -> left = new_root;
    }
    if(new_root -> parent && new_root -> parent -> right == this){
        new_root -> parent -> right = new_root;
    }
    return new_root;
}

void AVL :: Node :: update_data(){
    int left_height = 0, right_height = 0;
    
    spread = key;
    if(left){
        left_height = left -> height;
        spread = Interval(MIN(spread.a, left -> spread.a), MAX(spread.b, left -> spread.b));
    }
    if(right){
        right_height = right -> height;
        spread = Interval(MIN(spread.a, right -> spread.a), MAX(spread.b, right -> spread.b));
    }
    height = 1 + (left_height > right_height ? left_height : right_height);
}

#define SWAP(a, b) AVL :: Node** t = a; a = b; b = t;

void AVL :: remove(AVL :: Node* x){
    if(!x){
        return;
    }
    if(x -> left && x -> right){
        Node* tmp = next(x);
        /*SWAP(&x -> left, &tmp -> left);
        SWAP(&x -> right, &tmp -> right);
        SWAP(&x -> parent, &tmp -> parent);*/
        
        x -> key = tmp -> key;
        x -> value = tmp -> value;
        remove(tmp);
    }
    else{
        Node* child = x -> left ? x -> left : 0;
        child = x -> right ? x -> right : child;
        
        Node* parent = x -> parent;
        if(child){
            child -> parent = parent;
        }
        
        if(parent && parent -> left == x){
            parent -> left = child;
        }
        if(parent && parent -> right == x){
            parent -> right = child;
        }
        
        root = 0;
        while(parent){
            root = parent -> balance();
            parent = parent -> parent;
        }
        x -> left = x -> right = 0;
        delete x;
    }
}
