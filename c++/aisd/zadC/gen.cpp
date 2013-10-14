#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

int main(){
    srand(time(NULL));
    long b = 1000 + rand() % 500000000;
    
    cout << "1000000000 1000000 " << b << endl;
    
    for(long i = 1; i <= 1000000; ++i){
        cout << i * 999 << " " << rand() % 500000000 + 500000000 << endl;
    }
    
    return 0;
}
