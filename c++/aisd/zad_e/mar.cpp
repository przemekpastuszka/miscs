#include <cstdio>
#include <vector>
#include <ctime>
#include <cstdlib>


#define FRANKY 9999991

#define PATCH 1   
#define ALL 1

int main(){
    srand(time(NULL));
    
    int n;
    scanf("%d", &n);
    
    int X = ALL * n;
    printf("%d\n", X); 
    
    std :: vector<int>* patches = new std :: vector<int>[X];
    int* carrots = new int[n];
    
    for(int i = 0; i < n; ++i){
        scanf("%d", &carrots[i]);
    }
    
    long long A;
    
    while(true){
        for(int i = 0; i < X; ++i){
            patches[i].clear();
        }
        
        A = rand() % FRANKY;
        for(int i = 0; i < n; ++i){
            patches[((A * carrots[i]) % FRANKY) % X].push_back(carrots[i]);
        }
        
        long long sum = 0;
        for(int i = 0; i < X; ++i){
            sum += (int) (PATCH * patches[i].size() * patches[i].size());
        }
        
        if(sum <= 3 * n){
            break;
        }
    }
    
    for(int i = 0; i < X; ++i){
        printf("%d ", (int) (PATCH * patches[i].size() * patches[i].size()));
    }
    printf("\n%lld\n", A);
    
    for(int i = 0; i < X; ++i){
        int size = (int) (PATCH * patches[i].size() * patches[i].size());
        bool* tmp = new bool[size];
        while(true){
            for(int j = 0; j < size; ++j){
                tmp[j] = false;
            }
            
            long long a = rand() % FRANKY;
            bool is_ok = true;
            for(int j = 0; j < patches[i].size(); ++j){
                int t = ((a * patches[i][j]) % FRANKY) % size;
                if(tmp[t]){
                    is_ok = false;
                    break;
                }
                tmp[t] = true;
            }
            if(is_ok) { printf("%lld ", a); break; }
        }
        delete [] tmp;
    }
    printf("\n");
    
    delete [] carrots;
    delete [] patches;
    return 0;
}
