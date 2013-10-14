#include <cstdio>
#include <ctime>
#include <cstdlib>

int main(){
    srand(time(NULL));
    
    printf("10\n");
    for(int i = 0; i < 10; ++i){
        printf("500000\n");
        
        for(int j = 0; j < 500000;){
            int start = rand() % 100000;
            int length = rand() % 100000;
            for(int k = 0; k < length && j < 500000; ++k, ++j){
                printf("%d ", start);
                start += rand() % 100000;
            }
            
        }
        printf("\n");
    }
    
    return 0;
}
