#include <iostream>

using namespace std;
int main(){
    
    int masks[] = {0, 32, 16, 8, 4, 2, 1};
    
    for(int i = 0; i < 64; ++i){
        
        int result = 0;
        if(i & masks[1]){
            result |= masks[2] | masks[6];
        }
        if(i & masks[2]){
            result |= masks[1] | masks[3];
        }
        if(i & masks[3]){
            result |= masks[2] | masks[4];
        }
        if(i & masks[4]){
            result |= masks[5];
        }
        if(i & masks[5]){
            result |= masks[4] | masks[6];
        }
        if(i & masks[6]){
            result |= masks[5];
        }
        if( ((i & masks[1]) && (i & masks[6])) || ((i & masks[3]) && (i & masks[4])) ){
            result = -1;
        }
        cout << result << ", ";
    }
    cout << endl;
    
    for(int i = 0; i < 64; ++i){
        
        int result = 0;
        for(int j = 1; j <= 6; ++j){
            if((i & masks[j]) != 0){
                ++result;
            }
            
        }
        cout << result << ", ";
    }
    cout << endl;
    cout << (16 & masks[4]) << endl;
    return 0;
}
