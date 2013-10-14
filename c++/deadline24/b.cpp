#include <iostream>
#include <list>

using namespace std;

char kupa[100][101];

int next[2][100][100];
int visited[100][100];

int n;
int k;
int fuck;

int Visit(int i, int j){
    if(i < 0 || j < 0 || i >= n || j >= n || kupa[i][j] == '#' || visited[i][j] >= fuck){
        return 0;
    }
    
    visited[i][j] = fuck;
    int max = 0;
    next[k][i][j] = 0;
    
    int tmp = Visit(i - 1, j);
    if(tmp > max){
        max = tmp;
        next[k][i][j] = 1;
    }
    
    tmp = Visit(i + 1, j);
    if(tmp > max){
        max = tmp;
        next[k][i][j] = 3;
    }
    
    tmp = Visit(i, j - 1);
    if(tmp > max){
        max = tmp;
        next[k][i][j] = 2;
    }
    
    tmp = Visit(i, j + 1);
    if(tmp > max){
        max = tmp;
        next[k][i][j] = 4;
    }
    
    return max + 1;
}

int main(){

    cin >> n;
    for(int i = 0; i < n; ++i){
        cin >> kupa[i];
    }
    
    k = 0;
    fuck = 1;
    
    int max = 0;
    int a = 0, b = 0;
    
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(kupa[i][j] == '.'){
                int c = Visit(i, j);
                if(c > max){
                    max = c;
                    a = i;
                    b = j;
                    k = (k + 1) % 2;
                }
            }
            ++fuck;
        }
    }
    
    cout << max << endl;
    
    k = (k + 1) % 2;
    while(next[k][a][b] > 0){
        cout << a + 1 << " "<< b + 1 << endl;
        switch(next[k][a][b]){
            case 1:
                --a;
                break;
            case 3:
                ++a;
                break;
            case 2:
                --b;
                break;
            case 4:
                ++b;
                break;
        }
    }
    cout << a + 1 << " " << b + 1 << endl;
    
    return 0;
}
