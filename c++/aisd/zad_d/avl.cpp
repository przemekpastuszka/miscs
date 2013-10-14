#include <cstdlib>
#include <cstdio>

#define KEY int
#define MAX(a, b) (a > b ? a : b)

class AVL{
    
    private:
    
    class Node{
        //private:
        public:
        Node *left, *right, *parent;
        int height;
        
        Node* balance();
        int calc_balance();
        void update_data();
        Node* rotate_left();
        Node* rotate_right();
        Node* finish_rotation(Node* new_root);
        
        public:
        KEY key;
        int value;
        int max;
        
        Node(KEY k, int v, Node* p) : left(0), right(0), parent(p), height(1), key(k), value(v), max(v) {}
        ~Node(){
            if(left) {delete left;}
            if(right) {delete right;}
        }
        
        //Node* insert(KEY k, int v);
        //int get_max(KEY k);
        
    };
    
    Node* root;
    
    public:
    
    AVL() : root(0) {}
    ~AVL(){
        if(root) {delete root;}
    }
    
    /*void insert(KEY k, int v){
        if(!root){ 
            root = new Node(k, v, 0);
        }
        else{
            root = root -> insert(k, v);
        }
    }*/
    /*int get_max(KEY k){
        if(root){
            return root -> get_max(k);
        }
        return 0;
    }*/
    void insert(KEY k, int v);
    int get_max(KEY k);
};

int main(){
    int T;
    scanf("%d", &T);
    
    const int size = 5 * 100000 + 2;
    
    int input[size];
    int preprocess[size];
    
    for(int t = 0; t < T; ++t){
        AVL tree;
        
        int n;
        scanf("%d", &n);
        //fgets(input, n + 2, stdin);
        for(int i = 0; i < n; ++i){
            scanf("%d", &input[i]);
        }
        
        preprocess[n - 1] = 1; // preprocessing z prawa na lewo
        int last = input[n - 1];
        int len = 1;
        for(int i = n - 2; i >= 0; --i){
            int current = input[i];
            if(current < last){
                preprocess[i] = ++len;
            }
            else{
                preprocess[i] = 1;
                len = 1;
            }
            last = current;
        }
        
        int max = preprocess[0];
        
        last = input[0];
        len = 1;
        tree.insert(last, 1);
        for(int i = 1; i < n; ++i){
            int current = input[i];
            if(current > last){
                ++len;
            }
            else{
                len = 1;
            }
            last = current;
            
            int m = tree.get_max(current - 1);
            if(m + preprocess[i] > max){
                max = m + preprocess[i];
            }
            if(m < len){
                tree.insert(current, len);
            }
        }
        
        printf("%d\n", max);
    }
    
    return 0;
}

/*int AVL :: Node :: get_max(KEY k){
    int max = left && left -> max > value ? left -> max : value;
    if(key == k){
        return max;
    }
    if(k < key && left){
        return left -> get_max(k);
    }
    if(k > key){
        int r = 0;
        if(right){
            r = right -> get_max(k);
        }
        return r > max ? r : max;
    }
    return 0;
}*/
int AVL :: get_max(KEY k){
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
}

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

/*AVL :: Node* AVL :: Node :: insert(KEY k, int v){
    if(k <= key){
        if(left){
            left -> insert(k, v);
        }
        else{
            left = new Node(k, v, this);
        }
    }
    else{
        if(right){
            right -> insert(k, v);
        }
        else{
            right = new Node(k, v, this);
        }
    }
    return balance();
}*/

void AVL :: insert(KEY k, int v){
    if(!root){
        root = new Node(k, v, 0);
        return;
    }
    Node *parent = 0, *current = root;
    while(current){
        parent = current;
        if(k <= current -> key){
            current = current -> left;
        }
        else{
            current = current -> right;
        }
    }
    
    Node* tmp = new Node(k, v, parent);
    if(k <= parent -> key){
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
    int left_height = 0, left_max = 0, right_height = 0, right_max = 0;
    if(left){
        left_height = left -> height;
        left_max = left -> max;
    }
    if(right){
        right_height = right -> height;
        right_max = right -> max;
    }
    height = 1 + (left_height > right_height ? left_height : right_height);
    max = value;
    if(left_max > max){
        max = left_max;
    }
    if(right_max > max){
        max = right_max;
    }
}
