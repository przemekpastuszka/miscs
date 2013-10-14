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

int main(){
    int n;
    scanf("%d", &n);
    
    NUM a, b;
    scanf("%d %d", &a, &b);
    
    NUM result = n - 1;
    
    Interval current = Interval(0, a + b);
    for(int i = 1; i < n; ++i){
        scanf("%d %d", &a, &b);
        NUM x = a + b;

        current = intersect(current, Interval(0, x));
        current.flip(x);
        if(!current.is_valid()){
            --result;
            current = Interval(0, x);
        }
    
    }
    
    printf("%d\n", result);
    
    return 0;
}
