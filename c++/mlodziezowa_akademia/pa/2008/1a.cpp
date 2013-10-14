#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>

using namespace std;

#define MAX_VERT 200001
#define INFINITY 1000000000
#define EVEN 0
#define ODD 1

struct Edge{
    int vertex;
    int wage;
    
    Edge(int v, int w): vertex(v), wage(w) {}
};
vector<Edge> *vertices;

long ***distances;

struct Tmp{
    int v;
    long w;
    
    Tmp(int a, long b) : v(a), w(b) {}
    Tmp() {}
};

class Cmp{
    public:
    bool operator()(const Tmp& a, const Tmp& b){
        return a.w > b.w;
    }
};

void dijkstra(int source, int n, int current){
    priority_queue<Tmp, vector<Tmp>, Cmp> q;
    for(int i = 1; i <= n; ++i){
        distances[current][EVEN][i] = distances[current][ODD][i] = INFINITY;
    }
    distances[current][EVEN][source]= 0;//distances[current][ODD][source] = 0;
    q.push(Tmp(source, 0));
    while(!q.empty()){
        Tmp t = q.top();
        q.pop();
        for(int k = 0; k < vertices[t.v].size(); ++k){
            int v = vertices[t.v][k].vertex;
            int w = vertices[t.v][k].wage;
            long even = distances[current][EVEN][t.v] + w;
            long odd = distances[current][ODD][t.v] + w;
            if(even % 2){
                int t = even;
                even = odd;
                odd = t;
            }
            if(even < distances[current][EVEN][v]){
                distances[current][EVEN][v] = even;
                q.push(Tmp(v, even));
            }
            if(odd < distances[current][ODD][v]){
                distances[current][ODD][v] = odd;
                q.push(Tmp(v, odd));
            }
        }
    }
};

int main(){
    vertices = new vector<Edge>[MAX_VERT];
    distances = new long**[2];
    for(int i = 0; i < 2; ++i){
        distances[i] = new long*[2];
        for(int j = 0; j < 2; ++j){
            distances[i][j] = new long[MAX_VERT];
        }
    }
    
    int n, m;
    scanf("%d %d", &n, &m);
    for(int i = 0; i < m; ++i){
        int a, b, wage;
        scanf("%d %d %d", &a, &b, &wage);
        vertices[a].push_back(Edge(b, wage));
        vertices[b].push_back(Edge(a, wage));
    }
    
    dijkstra(1, n, 0);
    dijkstra(n, n, 1);
    long min = distances[0][ODD][n];
    for(int i = 1; i <= n; ++i){
        long k = distances[0][ODD][i] + distances[1][EVEN][i];
        long c = distances[0][EVEN][i] + distances[1][ODD][i];
        if(k < min){
            min = k;
        }
        if(c < min){
            min = c;
        }
    }
    if(min != INFINITY){
        printf("%ld\n", min);
    }
    else{
        printf("0\n");
    }
    
    /*for(int i = 1; i <= n; ++i){
        printf("%d\t", i);
    }
    printf("\n");
    for(int i = 1; i <= n; ++i){
        printf("%d\t", distances[current][EVEN][i]);
    }
    printf("\n");
    for(int i = 1; i <= n; ++i){
        printf("%d\t", distances[current][ODD][i]);
    }
    printf("\n");*/
    
    for(int i = 0; i < 2; ++i){
        for(int j = 0; j < 2; ++j){
            delete [] distances[i][j];
        }
        delete [] distances[i];
    }
    delete [] distances;
    delete [] vertices;
    return 0;
}
