#include <cstdio>
#include <set>

#define NUM int
#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)
#define ABS(a) (a > 0 ? a : -a)

#define SET std :: set<Interval, bool(*)(Interval, Interval)>

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

bool cmp(Interval x, Interval y){
    return x.b < y.b;
}

void cut(SET* window, int position, int p, int s){
    SET :: iterator left = window -> upper_bound(Interval(0, position));
    SET :: iterator right = window -> upper_bound(Interval(0, position + p));
    
    if(left == window -> end() || (*left).a >= position + p){
        return;
    }
    
    Interval first(1, 0), second(1, 0);
    
    if((*left).a < position){
        first = Interval ((*left).a, position);
    }
    if(right != window -> end() && (*right).a < position + p){
        second = Interval(position + p, (*right).b);
        right++;
    }
    window -> erase(left, right);
    if(first.length() >= s){
        window -> insert(first);
    }
    if(second.length() >= s){
        window -> insert(second);
    }
}

int main(){
    
    int t;
    scanf("%d", &t);
    for(int z = 0; z < t; ++z){
        
        int s, k, n;
        scanf("%d %d %d", &s, &k, &n);
        
        SET window(cmp);
        window.insert(Interval(0, k));
        
        NUM position = 0;
        for(int i = 0; i < n; ++i){
            int p;
            scanf("%d", &p);
            
            if(i % 2 == 0){
                cut(&window, position, p, s);
            }
            
            position += p;
            if(position >= k){
                if(i % 2 == 0 && position >= 2 * k){
                    window.clear();
                    break;
                }
                position = position % k;
                if(i % 2 == 0 && position != 0){
                    cut(&window, 0, position, s);
                }
            }
        }
        
        if(window.empty()) { printf("NIE\n"); }
        else { printf("TAK\n"); }
        
    }
    
    
    return 0;
}
