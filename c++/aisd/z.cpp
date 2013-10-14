#include <cstdio>

int main(){
    int a, b;
    scanf("%d %d", &a, &b);
    if(a > b){
        int tmp = a;
        a = b;
        b = tmp;
    }
    for(; a <= b; ++a){
        printf("%d\n", a);
    }
    return 0;
}
