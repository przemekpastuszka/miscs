#include <algorithm>
#include <iostream>


using namespace std;

struct Kupa{
    long nr;
    bool left;
};

bool Cmp(Kupa a, Kupa b){
    if(a.nr == b.nr){
        return a.left;
    }
    return a.nr < b. nr;
}



int main(){
    
    long i;
    cin >> i;
    
    Kupa* kupa = new Kupa[2 * i];
    
    for(long j = 0; j < i; ++j){
        cin >> kupa[2 * j].nr;
        cin >> kupa[2 * j + 1].nr;
        kupa[2 * j].left = true;
        kupa[2 * j + 1].left = false;
    }
    
    sort(kupa, &kupa[2 * i], Cmp);
    
    int wynik = 0;
    int max = 0;
    for(long j = 0; j < 2 * i; ++j){
        if(kupa[j].left){
            ++wynik;
        }
        else{
            --wynik;
        }
        if(wynik > max){
            max = wynik;
        }
    }
    cout << max << endl;
    return 0;
}
