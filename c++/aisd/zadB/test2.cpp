#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

int main(int argc, char** argv){
    srand(time(NULL));
    
    ofstream kupa(argv[1]);
    
    int c = rand() % 21;
    
    kupa << 200000 << endl;
    for(int i = 0; i < 200000; ++i){
        for(int j = 0; j < 3; ++j){
            if(rand() % 100 < 40 + c){
                kupa << 'x';
            }
            else{
                kupa << '.';
            }
        }
        kupa << endl;
    }
    
    
    kupa.close();
    
    return 0;
}
