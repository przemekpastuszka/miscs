#include <cstdio>

int main(){
    
    bool towers[1000][1000];
    bool rows[1000];
    bool cols[1000];
    
    int n;
    scanf("%d\n", &n);
    
    for(int i = 0; i < n; ++i){
        rows[i] = cols[i] = false;
    }
    
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            char c;
            scanf("%c", &c);
            towers[i][j] = false;
            if(c == 'W'){
                towers[i][j] = true;
                rows[i] = cols[j] = true;
            }
        }
        scanf("\n");
    }
    
    int i = 0, j = 0;
    while(i < n && j < n){
        while(rows[i]) { ++i; }
        while(cols[j]) { ++j; }
        towers[i][j] = true;
        ++i; ++j;
    }
    
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(towers[i][j]){
                printf("W");
            }
            else{
                printf(".");
            }
        }
        printf("\n");
    }
    
    return 0;
}
