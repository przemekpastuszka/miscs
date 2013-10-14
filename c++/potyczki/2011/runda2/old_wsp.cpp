#include <cstdio>

#define NUM int
#define MIN(a, b) a < b ? a : b
#define MAX(a, b) a > b ? a : b
#define ABS(a) a > 0 ? a : -a

struct Interval{
    NUM a, b;
    
    Interval(NUM x, NUM y) : a(x), b(y) {}
    Interval() {}
    
    bool is_valid() { return a <= b; }
    void flip(NUM k){
        NUM tmp = a;
        a = k - b;
        b = k - tmp;
    }
};

Interval intersect(const Interval& x, const Interval& y){
    return Interval(MAX(x.a, y.a), MIN(x.b, y.b));
}

/*struct MoTF{
    Interval* tab;
    int left, right;
    
    MoTF(int n){
        left = 0; right = 0;
        tab = new Interval[n];
    }
    ~MoTF(){
        delete [] tab;
    }
    bool empty(){
        return left == right;
    }
    void insert(Interval t){
        while(right > left && tab[right - 1].a <= t.a){
            --right;
        }
        tab[right++] = t;
    }
    Interval top(){
        return tab[left];
    }
    Interval pop(){
        ++left;
    }
};*/

int main(){
    int n;
    scanf("%d", &n);
    
    NUM* rope_length = new NUM[n];
    NUM* longest_seq = new NUM[n];
    for(int i = 0; i < n; ++i){
        NUM a, b;
        scanf("%d %d", &a, &b);
        rope_length[i] = a + b;
    } 
    
    /********************************************************/
    // calculate longest sequence for each rope
    Interval* intervals = new Interval[n];
    longest_seq[0] = 0;
    intervals[0] = Interval(0, rope_length[0]);
    for(int i = 1; i < n; ++i){
        intervals[i] = Interval(1, 0);
    }
    
    for(int i = 1; i < n; ++i){
        longest_seq[i] = 0;
        Interval tmp = Interval(0, rope_length[i]);
        for(int j = n - 1; j > 0; --j){
            if(intervals[j - 1].is_valid()){
                intervals[j] = intersect(tmp, intervals[j - 1]);
                intervals[j].flip(rope_length[i]);
                if(intervals[j].is_valid()){
                    longest_seq[i] = MAX(longest_seq[i], j);
                }
            }
            else{
                intervals[j] = Interval(1, 0);
            }
        }
        intervals[0] = tmp;
    }
    
    delete [] intervals;
    /********************************************************/
    
    /*MoTF q(n);
    q.insert(Interval(1, -1));
    for(int i = 0; i < n; ++i){
        Interval t = q.top();
        while(t.b < i - (longest_seq[i] + 1)){
            q.pop();
            t = q.top();
        }
        q.insert(Interval(t.a - 1, i));
    }*/
    
    
    
    delete [] longest_seq;
    delete [] rope_length;
    
    return 0;
}
