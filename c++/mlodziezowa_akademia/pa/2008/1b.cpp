#include <cstdio>

int main(){
    int powers[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    int k;
    int peppers[11];
    scanf("%d", &k);
    for(int i = 0; i <= k; ++i){
        scanf("%d", &peppers[i]);
    }
    
    if(peppers[0] == 0){
        printf("1\n");
        return 0;
    }
    long long sum = peppers[0];
    for(int i = 1; i <= k; ++i){
        if(sum + 1 < powers[i]){
            printf("%lld\n", sum + 1);
        }
        sum += powers[i] * peppers[i];
    }
    
    printf("%lld\n", sum + 1);
    
    return 0;
}
