#include <cstdio>
#include <list>

#define NUM int
#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)
#define ABS(a) (a > 0 ? a : -a)

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

int main(){
    
    int t;
    scanf("%d", &t);
    for(int z = 0; z < t; ++z){
        
        int s, k, n;
        scanf("%d %d %d", &s, &k, &n);
        
        std :: list<Interval> ls;
        ls.push_back(Interval(-k + s, 0));
        
        NUM position = 0;
        for(int i = 0; i < n; ++i){
            int p;
            scanf("%d", &p);
            
            if(i % 2){
                while((!ls.empty()) && ls.front().b + k < position){
                    ls.pop_front();
                }
                
                Interval current(position - k, position + p - k);
                
                for(std :: list<Interval> :: iterator it = ls.begin(); it != ls.end(); ++it){
                    Interval t = intersect(*it, current);
                    if(!t.is_valid()){
                        break;
                    }
                    if(t.length() >= s){
                        ls.push_back(t.translate(k));
                    }
                }
            }
            
            position += p;
            
        }
        while((!ls.empty()) && ls.front().b + k < position){
            ls.pop_front();
        }
        
        Interval current(position - k, position - s);
        
        bool is_ok = false;
        for(std :: list<Interval> :: iterator it = ls.begin(); it != ls.end(); ++it){
            Interval t = intersect(*it, current);
            if(!t.is_valid()){
                break;
            }
            if(t.length() >= s){
                is_ok = true;
                break;
            }
        }
        if(is_ok) { printf("TAK\n"); }
        else { printf("NIE\n"); }
    }
    
    
    
    return 0;
}
