struct Element{
};

class Heap{
    private:
    long* elements;
    long last;
    
    void swap(long a, long b){
        long tmp = elements[a];
        elements[a] = elements[b];
        elements[b] = tmp;
    }
    
    void push_up(long nr){
        while(nr > 1){
            long parent = nr >> 1;
            if(cmp(elements[nr], elements[parent])){
                swap(parent, nr);
                nr = parent;
            }
            else{
                break;
            }
        }
    }
    
    void push_down(long nr){
        long left = nr << 1;
        while(left < last){
            long min = cmp(elements[nr], elements[left]) ? elements[nr] : elements[left];
            long right = left + 1;
            if(right <= last){
                min = cmp(elements[right], min) ? elements[right] : min;
            }
            if(min == elements[nr]){
                break;
            }
            if(right <= last && min == elements[right]){
                swap(nr, right);
                nr = right;
            }
            else{
                swap(nr, left);
                nr = left;
            }
            left = nr << 1;
        }
    }
    
    public:
    long top(){
        return elements[1];
    }
    void push(long n){
        elements[++last] = n;
        push_up(last);
    }
    void pop(){
        elements[1] = elements[last--];
        push_down(1);
    }
    bool empty(){
        return last == 0;
    }
    Heap(long n){
        elements = new long[n + 1];
        last = 0;
    }
    ~Heap(){
        delete [] elements;
    }
};
