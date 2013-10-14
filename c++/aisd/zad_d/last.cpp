#include <cstdlib>
#include <cstdio>

#define MAX(a, b) (a > b ? a : b)
#define INFINITY 1000000001

int binary_search(int *tab, int p, int k, int v){
    
    while(p <= k){
        int q = (k + p) / 2;

        if(v <= tab[q]){
            k = q - 1;
        }
        else{
            p = q + 1;
        }
    }
    return k;
}

int main(){
    int T;
    scanf("%d", &T);
    
    const int size = 5 * 100000 + 2;
    
    int input[size];
    int preprocess[size];
    int lengths[size];
    
    for(int t = 0; t < T; ++t){
        
        int n;
        scanf("%d", &n);

        for(int i = 0; i < n; ++i){
            scanf("%d", &input[i]);
            lengths[i + 1] = INFINITY;
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
        
        int max_len = 1;
        
        last = input[0];
        lengths[1] = input[0];
        len = 1;

        for(int i = 1; i < n; ++i){
            int current = input[i];
            if(current > last){
                ++len;
                max_len = MAX(max_len, len);
            }
            else{
                len = 1;
            }
            last = current;
            
            int p = binary_search(lengths, 1, max_len, current);
            
            max = MAX(p + preprocess[i], max);
            
            if(lengths[len] > current){
                lengths[len] = current;
            }
        }
        
        printf("%d\n", max);
    }
    
    return 0;
}
