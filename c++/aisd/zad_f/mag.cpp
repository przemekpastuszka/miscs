#include <cstdio>
// #include <unistd.h>
int find(int* tab, int p, int q, int k);

void swap(int& a, int& b) {int tmp = a; a = b; b = tmp;}
#define A tab[p]
#define B tab[p + 1]
#define C tab[p + 2]
#define D tab[p + 3]
#define E tab[p + 4]

#define MIN(a, b) (a < b ? a : b)

void sort5(int* tab, int p, int q){
    /* if(A > B){
        swap(A, B);
    }
    if(C > D){
        swap(C, D);
    }
    if(B > D){
        swap(B, D);
        swap(A, C);
    }
    if(E > B){
        
    }
    else{
        
    } */
    for(int i = p + 1; i <= q; ++i){
        int v = tab[i];
        int j;
        for(j = i - 1; j >= p; --j){
            if(tab[j] > v){
                tab[j + 1] = tab[j];
            }
            else{
                break;
            }
        }
        tab[j + 1] = v;
    }
}

int pivot(int* tab, int p, int q){
    if(q - p + 1 < 5){
        sort5(tab, p, q);
        return tab[(p + q) / 2];
    }
    
    int i;
    for(i = 0; p + i + 4 <= q; i += 5){
        sort5(tab, p + i, p + i + 4);
        swap(tab[p + i / 5], tab[p + i + 2]); 
    }
    return find(tab, p, p + i / 5 - 1, (i / 5 - 1) / 2);
    
}

int partition(int* tab, int p, int q, int piv){
    --p; ++q;
    while(p < q){
        while(tab[--q] > piv);
        while(tab[++p] < piv);
        if(p < q){
            swap(tab[p], tab[q]);
        }
    }
    return q;
}

int find(int* tab, int p, int q, int k){
    // printf("p: %d; q: %d\n",  p, q);
    if(p == q){
        return tab[q];
    }
    int piv = pivot(tab, p, q);
    int j = partition(tab, p, q, piv);
   /* printf("pivot: %d; p: %d; q: %d\n", piv, p, q);
    for(int t = p; t <= q; ++t){
        printf("%d ", tab[t]);
    }
    printf("\n\n");
     sleep(1); */
    if(j - p + 1 >= k){
        return find(tab, p, j, k);
    }
    return find(tab, j + 1, q, k - (j - p + 1));
}

int main(){
    
    int n, k;
    scanf("%d %d", &n, &k);
    
    int* tab = new int[n];
    
    for(int i = 0; i < n; ++i){
        scanf("%d", &tab[i]);
    }
    
    printf("%d\n", find(tab, 0, n - 1, k));
    
    delete [] tab;
    
    return 0;
}
