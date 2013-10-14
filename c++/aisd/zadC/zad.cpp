
#include <cstdio>
#include <iostream>
using namespace std;

//long long* results;
//long* dist;
//long* data;
long long results[1000001];
long dist[1000001];
long data[1000001];
//long long* cost;

/*class MotF{
    long long* data;
    long long left, right;
    
    public:
    MotF(long long n){
        data = new long long[n + 1];
        left = right = 0;
    }
    ~MotF(){
        delete [] data;
    }
    
    bool empty(){
        return left >= right;
    }
    long long top(){
        return data[left];
    }
    void pop(){
        ++left;
    }
    void push(long long i){
        while(right > left && results[data[right - 1]] >= results[i]){
            --right;
        }
        data[right++] = i;
        
    }
    
};*/

char* buffer;
char* last;

long get_long(){
    long result = 0;
    while(*last == ' ' || *last == '\n'){
        ++last;
    }
    while(*last != ' ' && *last != '\n'){
        result *= 10;
        result += ((char)*last) - 48;
        ++last;
    }
    return result;
}

int main(){
    //std :: ios ::sync_with_stdio(false);
    
    long l, n, b;
    buffer = new char[30000000];
    last = buffer;
     fread(buffer, 1, 30000000, stdin);
    
    l = get_long();
    n = get_long();
    b = get_long();
    //cin >> l >> n >> b;
    //scanf("%ld %ld %ld", &l, &n, &b);
    //results = new long long[n + 1];
    //dist = new long[n + 1];
    //data = new long[n + 2];
    //cost = new long long[n + 1];
    long left = 0, right = 1;
    results[0] = dist[0] = 0;
    //MotF q(n);
    data[0] = 0;
    
    //for(long i = 1; i <= n; ++i){
        
    //}
    
    for(long i = 1; i <= n; ++i){
        //scanf("%ld %ld", &dist[i], &results[i]);
        //cin >> dist[i] >> results[i];
        dist[i] = get_long();
        results[i] = get_long();
        while(right > left && dist[i] - dist[data[left]] > b){
                ++left;
        }
        
        if(left >= right){
            printf("-1\n");
            //cout << "-1\n";
            return 0;
        }
        results[i] += results[data[left]];
        while(right > left && results[data[right - 1]] >= results[i]){
            --right;
        }
        data[right++] = i;
    }
    
    if(l - dist[n] > b){
        printf("-1\n");
        //cout << "-1\n";
    }
    long long min = results[n];
    long k = n - 1;
    while(k >= 0 && l - dist[k] <= b){
        min = results[k] < min ? results[k] : min;
        --k;
    }
    printf("%lld\n", min);
    //cout << min << endl;
    
    delete [] buffer;
    //delete [] results;
    //delete [] dist;
    //delete [] data;
    //delete [] cost;
    return 0;
}
