
#include <cstdio>

using namespace std;

long* results;
long* dist;

class MotF{
    long* data;
    long left, right;
    
    public:
    MotF(long n){
        data = new long[n + 1];
        left = right = 0;
    }
    ~MotF(){
        delete [] data;
    }
    
    bool empty(){
        return left >= right;
    }
    long top(){
        return data[left];
    }
    void pop(){
        ++left;
    }
    void push(long i){
        while(right > left && results[data[right - 1]] >= results[i]){
            --right;
        }
        data[right++] = i;
        
    }
    
};


int main(){
    long l, n, b;
    //cin >> l >> n >> b;
    scanf("%ld %ld %ld", &l, &n, &b);
    results = new long[n + 1];
    dist = new long[n + 1];
    
    results[0] = dist[0] = 0;
    MotF q(n);
    q.push(0);
    
    for(long i = 1; i <= n; ++i){
        long cost;
        scanf("%ld %ld", &dist[i], &cost);
        
        while((!q.empty()) && dist[i] - dist[q.top()] > b){
                q.pop();
        }
        
        if(q.empty()){
            printf("-1\n");
            return 0;
        }
        results[i] = cost + results[q.top()];
        q.push(i);
    }
    
    if(l - dist[n] > b){
        printf("-1\n");
    }
    long min = results[n];
    long k = n - 1;
    while(k >= 0 && l - dist[k] <= b){
        min = results[k] < min ? results[k] : min;
        --k;
    }
    printf("%ld\n", min);
    
    delete [] results;
    delete [] dist;
    
    return 0;
}
