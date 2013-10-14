#include <iostream>
#include <cstring>

using namespace std;

enum Action {KOPIUJ, ZASTAP, ZAMIEN, WYRZUC, USUN, WSTAW};

int OdlRedakcyjna(string x, string y, int m, int n, int kopiuj, int zastap, int zamien, int wyrzuc, int usun, int wstaw)
{
	++m; ++n;
	int** tmp = new int*[m];
	for(int i = 0; i < m; ++i)
		tmp[i] = new int[n];
	
	for(int i = 0; i < m; ++i)
		tmp[i][0] = i * usun;
	for(int j = 1; j < n; ++j)
		tmp[0][j] = j * wstaw;
	
	for(int i = 1; i < m; ++i)
		for(int j = 1; j < n; ++j)
		{
			tmp[i][j] = usun + tmp[i - 1][j];
			if(tmp[i][j] > wstaw + tmp[i][j - 1])
				tmp[i][j] = wstaw + tmp[i][j - 1];
			if(x[i - 1] == y[j - 1] && tmp[i][j] > kopiuj + tmp[i - 1][j - 1])
				tmp[i][j] = kopiuj + tmp[i - 1][j - 1];
			if(tmp[i][j] > zastap + tmp[i - 1][j - 1])
				tmp[i][j] = zastap + tmp[i - 1][j - 1];
			if(i - 1 > 0 && j - 1 > 0 && x[i - 2] == y[j - 1] && x[i - 1] == y[j - 2] &&
					tmp[i][j] > zamien + tmp[i - 2][j - 2])
				tmp[i][j] = zamien + tmp[i - 2][j - 2];
		}
	
	int min = tmp[m - 1][n - 1];
	for(int i = 0; i < m - 1; ++i)
		if(tmp[i][n - 1] + wyrzuc < min)
			min = tmp[i][n - 1] + wyrzuc;
	
	for(int i = 0; i < m; ++i)
		delete [] tmp[i];
	delete [] tmp;
	
	return min;
}
const int MAX_NUMBER = 1000000;
int OcenaUliniowienia(string a, string b)
{
	return -OdlRedakcyjna(a, b, a.length(), b.length(), -1, 1, MAX_NUMBER, MAX_NUMBER, 2, 2);
}

int main()
{
	string a = "gatcggcat", b = "caatgtgaatc";
	cout << OcenaUliniowienia(a, b) << endl;
	return 0;
}
