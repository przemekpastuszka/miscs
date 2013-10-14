#include <cstdio>
#include <cstring>

#define MAX(a, b) (a > b ? a : b)

void count_pref(char* elements, int n, int* pref){
    pref[1] = 0;
    int t = pref[1];
    for(int i = 2; i <= n; ++i){
        while(t > 0 && elements[t + 1] != elements[i]){
            t = pref[t];
        }
        if(elements[t + 1] == elements[i]){
            ++t;
        }
        pref[i] = t;
    }
    
}

int main(){
    int t;
    scanf("%d", &t);
    
    for(int z = 0; z < t; ++z){
        int n;
        scanf("%d\n", &n);
        
        char *elements = new char[n + 2];
        int *pref = new int[n + 2];
        char* pff = new char[n + 2];
        
        for(int i = 1; i <= n; ++i){
            scanf("%c", &elements[i]);
        }
        
        int max = 0;
        
        for(int i = 1; i <= n; ++i){
            strncpy(pff + i, elements + i + 1, n - i);
            /* for(int k = 1; k <= n; ++k){
                printf("%c", pff[k]);
            }
            printf("\n"); */
            count_pref(pff, n - 1, pref);
            max = MAX(max, pref[n - 1]);
            pff[i] = elements[i];
        }
        
        printf("%d\n", n - 1 - max);
        
        delete [] pff;
        delete [] pref;
        delete [] elements;
        
    }
    
    return 0;
}
