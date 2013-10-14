#include <cstdio>
#include <cstdlib>
#include <ctime>
int main(){
    srand(time(NULL));
    printf("%d\n", 2 * 100000);
    for(int i = 0; i < 200000; ++i){
        printf("%d ", rand() % 5000000);
    }
    
    return 0;
}
