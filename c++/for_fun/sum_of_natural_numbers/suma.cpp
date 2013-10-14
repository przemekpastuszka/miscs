#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;


void Recursive(int n, int p, string kupa)
{
    ostringstream ss;
    ss << kupa << " + " << p;
    
    if(n == p)
        cout << kupa << " + " << n << endl;
    for(int i = p + 1; i <= n - p; ++i)
        Recursive(n - p, i, ss.str());
}

int main()
{
    int szukane;
    cin >> szukane;
    for(int i = 1; i <= szukane; ++i)
        Recursive(szukane, i, "");
    
}
