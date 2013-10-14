/*
Przemysław Pastuszka
Nr indeksu: 233 186

Program realizujący zadanie nr 20 z 1 pracowni Analizy Numerycznej
*/
#include <iostream>
#include <cmath>
using namespace std;

/*
Prosta implementacja macierzy o wymiarach 2x2 razem z operacją mnożenia.
Wykorzystywana w operacji obliczania n-tej liczby Fibonacciego
za pomocą potęgowania macierzy:
	|1 1|
	|1 0|
*/
struct Matrix
{
	int a11, a12, a21, a22;

	Matrix() {}
	Matrix(int a, int b, int c, int d) : a11(a), a12(b), a21(c), a22(d) {}
	
	static Matrix Identity() {return Matrix(1, 0, 0, 1);}
	
	friend const Matrix operator*(const Matrix& left, const Matrix& right);
};
const Matrix operator*(const Matrix& left, const Matrix& right)
{
	Matrix result;
	result.a11 = left.a11 * right.a11 + left.a12 * right.a21;
	result.a12 = left.a11 * right.a12 + left.a12 * right.a22;
	result.a21 = left.a21 * right.a11 + left.a22 * right.a21;
	result.a22 = left.a21 * right.a12 + left.a22 * right.a22;
	return result;
}

/*
Rekurencyjna implementacja operacji szybkiego potęgowania
Dzięki zastosowaniu szablonów możliwe jest podniesienie do k-tej
potęgi dowolnego elementu, dla którego zdefiniowano operację mnożenia,
np: liczb typu float, double czy macierzy reprezentowanej przez Matrix

Drugim argumentem funkcji jest element neutralny mnożenia
(dla liczb jest to 1, dla macierzy macierz identycznościowa)

Zastosowanie szybkiego potęgowania pozwala na przyspieszenie obliczeń i
jednoczesne zmniejszenie błędu, jakim obarczony jest końcowy wynik
*/
template <class T>
T QuickPowering(T element, T defValue, unsigned int exponent)
{
	if(exponent == 0)
		return defValue;
	if(exponent % 2)
		return element * QuickPowering(element, defValue, --exponent);
	T temp = QuickPowering(element, defValue, exponent / 2);
	return temp * temp;
}

/*
Funkcja F oblicza k-ty wyraz ciągu Fibonacciego za pomocą wzoru podanego w zadaniu
*/
template <class T>
T F(unsigned int k)
{
	T sqrt5 = sqrt(5);
	T a = QuickPowering<T>((1 + sqrt5) / 2, 1, k);
	T b = QuickPowering<T>((1 - sqrt5) / 2, 1, k);
	return (a - b) / sqrt5;
}

/*
Funkcja G oblicza k-ty wyraz ciągu G za pomocą wzoru podanego w zadaniu
*/
template <class T>
T G(unsigned int k)
{
	return QuickPowering<T>((1 - sqrt(5)) / 2, 1, k);
}

/*
Ponieważ dla ciągów F oraz G zachodzi ta sama zależność rekurencyjna:
Fn = Fn-1 + Fn-2
Gn = Gn-1 + Gn-2
z dokładnością do pierwszych dwóch początkowych wyrazów, można obliczyć
je za pomocą tej samej funkcji.

Jako drugi argument należy przekazać zerowy wyraz ciągu
Trzecim argumentem powinien być pierwszy wyraz ciągu
*/
template <class T>
T Recursive(unsigned int k, T f2, T f1)
{
	if(k == 0)
		return f2;
	if(k == 1)
		return f1;
	for(int i = 2; i <= k; ++i)
	{
		f1 += f2;
		f2 = f1 - f2;
	}
	return f1;
}



int main()
{	
	cout.precision(19);
	/*cout.setf(ios::fixed,ios::floatfield);*/
	long double G0 = 1, G1 = (1 - sqrt(5)) / 2;
	for(int i = 0; i < 50; ++i)
		cout << Recursive<float>(i, G0, G1) << "\t" << Recursive<double>(i, G0, G1) 
			<< "\t" << G<float>(i) << "\t" << G<double>(i) << endl;
	return 0;
}
