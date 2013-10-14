#include <cstdio>

#define REAL long double
#define INT long
#define EPSILON 0.000000000001

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)
#define ABS(a) (a > 0 ? a : -a)


int main(){
    INT s, w , k;
    scanf("%ld %ld %ld", &s, &w, &k);
    
    REAL position = 0;
    REAL water = w;
    while(position < s){
        INT moves = water / k;
        if(moves == 0){
            water -= s - position;
            water = MAX(0, water);
            break;
        }
        REAL x = (water - moves * k) / (2 * moves + 1) + EPSILON;
        if(position + x >= s){
            x = s - position + EPSILON;
        }
        
        position += x;
        water = moves * (k - 2 * x) + x + MAX(0, water - moves * k - 2 * x);
       //  printf("(%f, %f) ", position, water);
    }
    
    printf("%.3Lf\n", water);
    return 0;
}
