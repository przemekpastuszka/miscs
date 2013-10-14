#include <cstdio>
//#include <stack>

char* data[1002];//[1003];
bool* visited[1002];//[1002];

struct Pair{
    short int x, y;
    
    Pair(short int a, short int b) : x(a), y(b) {}
    Pair() {}
};

Pair* elements;
int height;

void Visit(short int i, short int j){
    
    //std :: stack<Pair> elements;
    //elements.push(Pair(i, j));
    elements[height++] = Pair(i, j);
    
    while(height > 0){
    
        Pair p = elements[--height];
        i = p.x; j = p.y;
        
        if(visited[i][j]){
            continue;
        }
        
        visited[i][j] = true;
        
        short int q = 2;
        char moves[] = {'L', 'U', 'R', 'D'};
        switch(data[i][j]){
            case 'B':
                moves[0] = 'L'; moves[1] = 'D';
                break;
            case 'C':
                moves[0] = 'L'; moves[1] = 'U';
                break;
            case 'D':
                moves[0] = 'U'; moves[1] = 'R';
                break;
            case 'E':
                moves[0] = 'R'; moves[1] = 'D';
                break;
            case 'F':
                q = 4;
                break;
        }
        for(short int z = 0; z < q; ++z){
            switch(moves[z]){
                case 'U':
                    if(data[i - 1][j] == 'B' || data[i - 1][j] == 'E' || data[i - 1][j] == 'F')
                        if(!visited[i - 1][j]){
                            //elements.push(Pair(i - 1, j));
                            elements[height++] = Pair(i - 1, j);
                        }
                    break;
                case 'D':
                    if(data[i + 1][j] == 'D' || data[i + 1][j] == 'C' || data[i + 1][j] == 'F')
                        if(!visited[i + 1][j]){
                            //elements.push(Pair(i + 1, j));
                            elements[height++] = Pair(i + 1, j);
                        }
                    break;
                case 'L':
                    if(data[i][j - 1] == 'D' || data[i][j - 1] == 'E' || data[i][j - 1] == 'F')
                        if(!visited[i][j - 1]){
                            //elements.push(Pair(i, j - 1));
                            elements[height++] = Pair(i, j - 1);
                        }
                    break;
                case 'R':
                    if(data[i][j + 1] == 'B' || data[i][j + 1] == 'C' || data[i][j + 1] == 'F')
                        if(!visited[i][j + 1]){
                            //elements.push(Pair(i, j + 1));
                            elements[height++] = Pair(i, j + 1);
                        }
                    break;
            }
        }
    }
}

int main(){
    
    int n, m;
    scanf("%d %d\n", &n, &m);
    
    for(short int i = 0; i < 1002; ++i){
        data[i] = new char[1003];
        visited[i] = new bool[1002];
    }
    
    for(short int i = 1; i <= n; ++i){
        fgets(&data[i][1], m + 2, stdin);
    }
    
    for(short int i = 0; i <= n + 1; ++i){
        data[i][0] = data[0][i] = data[i][m + 1] = data[n + 1][i] = 'A';
        for(short int j = 0; j <= m + 1; ++j){
            visited[i][j] = false;
        }
    }
    
    int result = 0;
    
    elements =  new Pair[2500 * 1000];
    height = 0;
    
    for(short int i = 1; i <= n; ++i){
        for(short int j = 1; j <= m; ++j){
            if(data[i][j] != 'A' && visited[i][j] == false){
                Visit(i, j);
                ++result;
            }
        }
    }
    
    delete [] elements;
    
    printf("%d\n", result);
    
    for(short int i = 0; i < 1002; ++i){
        delete [] data[i];
        delete [] visited[i];
    }
    
    return 0;
}
