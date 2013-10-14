#include <cstdio>

#define NR_OF_THE_BEAST 15000

int main(){
    int n;
    scanf("%d", &n);
    
    bool tulip_pan[NR_OF_THE_BEAST + 1];
    for(int i = 1; i <= NR_OF_THE_BEAST; ++i){
        tulip_pan[i] = false;
    }
    for(int i = 0; i < n; ++i){
        int tmp;
        scanf("%d", &tmp);
        tulip_pan[tmp] = true;
    }
    
    int result = 0;
    for(int i = 1; i <= NR_OF_THE_BEAST; ++i){
        if(!tulip_pan[i]){
            ++result;
        }
    }
    
    printf("%d\n", result);
    return 0;
}
