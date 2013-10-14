#include <iostream>
#include <cstring>
#include <list>

using namespace std;

char kupa[500][501]; //tu są słowa
int leny[500]; //długości słów
    
int wynik[500][500]; //wynik[i][j] - ile wystaje słowo j poza i

int ustawienie[500];
int n;

bool Check(int i){
    int akt = wynik[ustawienie[i]][ustawienie[i + 1]];
    if(i > 0){
        akt += wynik[ustawienie[i - 1]][ustawienie[i]];
    }
    if(i < n - 2){
        akt += wynik[ustawienie[i + 1]][ustawienie[i + 2]];
    }
    
    int prob = wynik[ustawienie[i + 1]][ustawienie[i]];
    if(i > 0){
        prob += wynik[ustawienie[i - 1]][ustawienie[i + 1]];
    }
    if(i < n - 2){
        prob += wynik[ustawienie[i]][ustawienie[i + 2]];
    }
    
    return prob < akt;
}

int main(){
    
    cin >> n;
    
    for(int i = 0; i < n; ++i){
        cin >> kupa[i];
        leny[i] = strlen(kupa[i]);
        ustawienie[i] = i;
    }
    
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(i == j)
                continue;
            int dl = 0;
            int k = 0;
            int w = 0;
            for(; k < leny[i] && w < leny[j]; ++k){
                if(kupa[i][k] == kupa[j][w]){
                    ++dl;
                    ++w;
                }
                else{
                    dl = 0;
                    w = 0;
                }
                if(dl == leny[j]){
                    break;
                }
            }
            wynik[i][j] = leny[j] - dl;
        }
    } 
    //tu koniec liczenia wynik

    for(int i = 0; i <= n - 2; ++i){
        for(int j = n - 2; j >= i; --j){
            if(Check(j)){
                int tmp = ustawienie[j];
                ustawienie[j] = ustawienie[j + 1];
                ustawienie[j + 1] = tmp;
            }
        }
    }
    
    int dl = leny[ustawienie[0]];
    int prev = ustawienie[0];
    for(int i = 1; i < n; ++i){
        int curr = ustawienie[i];
        dl += wynik[prev][curr];
        prev = curr;
    }
    
    cout << dl << endl;
    
    cout << kupa[ustawienie[0]];
    
    prev = ustawienie[0];
    for(int i = 1; i < n; ++i){
        int curr = ustawienie[i];
        cout << &kupa[curr][leny[curr] - wynik[prev][curr]];
        prev = curr;
    }
    
    cout << endl;
    return 0;
}
