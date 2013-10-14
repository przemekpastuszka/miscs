#include <cstdio>
#include <vector>
#include <algorithm>

#define NUM unsigned long long
#define MAGIC 1000000007
using namespace std;

std :: vector<int>* verts;
int* queue;
int* next;
int* codes;
NUM* results;

bool cmp(int a, int b){
    return codes[a] > codes[b];
}

bool cmp2(int a, int b){
    int i = a;
    int j = b;
    for(int k = 0; k < verts[i].size() && k < verts[j].size(); ++k){
        if(codes[verts[i][k]] > codes[verts[j][k]]){
            return true;
        }
        if(codes[verts[i][k]] < codes[verts[j][k]]){
            return false;
        }
    }
    return verts[i].size() < verts[j].size();
}

void sort(int qend){
    for(int i = 0; i < qend; ++i){
        sort(verts[queue[i]].begin(), verts[queue[i]].end(), cmp);
    }
    sort(queue, queue + qend, cmp2);
}

bool is_the_same(int i, int j){
    if(verts[i].size() != verts[j].size()){
        return false;
    }
    for(int k = 0; k < verts[i].size() - 1; ++k){
        if(codes[verts[i][k]] != codes[verts[j][k]]){
            return false;
        }
    }
    return true;
}

void calculate(int k){
    int last = -2;

    NUM c = 0;
    for(int i = 0; i < verts[k].size(); ++i){
        int v = verts[k][i];
        if(codes[v] == -1){
            break;
        } 
        if(codes[v] != last){
            c = 1;
            last = codes[v];
        }
        
        else{
            ++c;
        }
        results[k] = (results[k] * results[v]) % MAGIC;
        results[k] = (results[k] * c) % MAGIC;
    }

}

int main(){
    
    int n;
    scanf("%d", &n);
    
    verts = new std :: vector<int>[n + 1];
    int* degs = new int[n + 1];
    codes = new int[n + 1];
    queue = new int[n + 1];
    next = new int[n + 1];
    results = new NUM[n + 1];
    
    for(int i = 1; i <= n; ++i){
        degs[i] = 0;
        codes[i] = -1;
        results[i] = 1;
    }
    
    for(int i = 0; i < n - 1; ++i){
        int a, b;
        scanf("%d %d", &a, &b);
        verts[a].push_back(b);
        verts[b].push_back(a);
        ++degs[a];
        ++degs[b];
    }
    
    
    // printf("leafs : ");
    int qend = 0;
    for(int i = 1; i <= n; ++i){
       /* if(degs[i] == 0){
            printf("SHIT");
        } */
        if(degs[i] == 1){
            queue[qend++] = i;
             results[i] = 1;
         //    printf("%d, ", i);
        }
    }
    
     // printf("\n");
    int last_code = 0;
    int qnextend = 0;
    for(int t = 0; ;++t){
         // printf("\n\n%d pass: \n", t);
        
        for(int i = 0; i < qend; ++i){
         //~ //   printf("\n%d sorted neighbours: ", queue[i]);
         //~ //   for(int w = 0; w < verts[queue[i]].size(); ++w){
                //~ printf("%d, ", verts[queue[i]][w]);
            //~ }
            calculate(queue[i]);
             //~ printf("\n%d result: %d, ", queue[i], results[queue[i]]);
            --degs[verts[queue[i]].back()];
            if(degs[verts[queue[i]].back()] == 1){
                next[qnextend++] = verts[queue[i]].back();
            }
        }
          //~ printf("\n");
        if(qend){
            codes[queue[0]] = last_code++;
             //~ printf("%d code: %d, ", queue[0], codes[queue[0]]);
        }
        for(int i = 1; i < qend; ++i){
            if(is_the_same(queue[i - 1], queue[i])){
                codes[queue[i]] = codes[queue[i - 1]];
            }
            else{
                codes[queue[i]] = last_code++;
            }
             //~ printf("%d code: %d, ", queue[i], codes[queue[i]]);
        }
        //~ printf("\n");
        if(qnextend){
            qend = qnextend;
            int* tmp = queue;
            queue = next;
            next = tmp;
            qnextend = 0;
        }
        else{
            break;
        }
        sort(qend);
    }
    
    if(qend == 1){
        printf("%llu\n", results[queue[0]]);
    }
    if(qend == 2){
        if(codes[queue[0]] == codes[queue[1]]){
            printf("%llu\n", (2 * (results[queue[0]] * results[queue[0]]) % MAGIC) % MAGIC);
        }
        else{
            printf("%llu\n", (results[queue[0]] * results[queue[1]]) % MAGIC);
        }
    } 
    
    delete [] queue;
    delete [] codes;
    delete [] degs;
    delete [] verts;
    delete [] results;
    delete [] next;
    return 0;
}
