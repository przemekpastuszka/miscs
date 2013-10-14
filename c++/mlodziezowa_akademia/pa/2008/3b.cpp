#include <cstdio>
#include <vector>
#include <list>
using namespace std;

#define BEGIN 0
#define END 1

struct Pair{
    long a, b;
    
    Pair() {}
    Pair(long x, long y): a(x), b(y) {}
};

int main(){
    
    long n, m;
    
    scanf("%ld %ld", &n, &m);
    Pair* diagonals = new Pair[m];
    list<long>* sorted = new list<long>[n + 1];
    
    for(long i = 0; i < m; ++i){
        scanf("%d %d", &diagonals[i].a, &diagonals[i].b);
        if(diagonals[i].b < diagonals[i].a){
            long tmp = diagonals[i].a;
            diagonals[i].a = diagonals[i].b;
            diagonals[i].b = tmp;
        }
        sorted[diagonals[i].a].push_back(i);
        //sorted[diagonals[i].b].push_front(i);
    }
    
    for(int i = 1; i <= n; ++i){
        for(list<long> :: iterator k = sorted[i].begin(); k != sorted[i].end(); k++){
            long p = *k;
            if(diagonals[p].a == i){
                sorted[diagonals[p].b].push_front(p);
            }
        }
    }
    
    vector<Pair> q;
    for(int i = 1; i <= n; ++i){
        q.push_back(Pair(i, 0));
        for(list<long> :: iterator k = sorted[i].begin(); k != sorted[i].end(); k++){
            long p = *k;
            if(diagonals[p].a == i){
                q[q.size() - 1].b++;
            }
            else{ /*** COŚ TU NIE DZIAŁAAAAAAAAAAAAAAAAAAAAA!!!!*/
                long begin = diagonals[p].a;
                if(q.back().b == 0){
                    q.pop_back();
                }
                if(q.back().a > begin){
                    for(list<long> :: iterator it = sorted[q.back().a].begin(); it != sorted[q.back().a].end(); it++){
                        if(diagonals[*it].b > diagonals[p].b){
                            printf("%ld %ld\n", p + 1, (*it) + 1);
                            break;
                        }
                    }
                    //printf("%ld %ld\n", q[t] + 1, p + 1);
                    delete [] diagonals;
                    delete [] sorted;
                    return 0;
                }
                q[q.size() - 1].b--;
            }
        }
        if(q.back().b == 0){
            q.pop_back();
        }
    }
    printf("NIE\n");
    
    delete [] diagonals;
    delete [] sorted;
    
    return 0;
}
