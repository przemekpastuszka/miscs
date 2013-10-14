/*
Przemysław Pastuszka
Nr indeksu: 233 186

Program realizujący zadanie nr 20 z 1 pracowni Analizy Numerycznej
*/
#include <iostream>
#include <cmath>
using namespace std;

/************************************************************************
Prosta implementacja macierzy o wymiarach 2x2 razem z operacją mnożenia.
Wykorzystywana w operacji obliczania n-tej liczby Fibonacciego
za pomocą potęgowania macierzy:
	|1 1|
	|1 0|
************************************************************************/
struct Matrix
{
	long a11, a12, a21, a22;

	Matrix() {}
	Matrix(long a, long b, long c, long d) : a11(a), a12(b), a21(c), a22(d) {}
	
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
/************************************************************************/


/************************************************************************
Funkcja pomocna w testowaniu kolejnych metod obliczania wyrazów ciągu.
Jej zadaniem jest znalezienie takiej stałej c należącej do [cp, ck], że błąd względny/bezwzględny
jest jak najmniejszy
Argumenty:
-exactValue - wskaźnik do funkcji zwracającej dokładną wartość żądanego wyrazu ciągu
-p - początek badanego przedziału wyrazów
-k - koniec badanego przedziału wyrazów
-approx - wskaźnik do funkcji zwracającej przybliżoną wartość żądanego wyrazu ciągu w zależności od pewnej stałej typu double
-relativeError - jeśli true to obliczany jest średni błąd względny; wpp średni błąd bezwzględny
-cp - początek przedziału, z którego wybierane będą stałe do eksperymentów
-ck - koniec przedziału, z którego wybierane będą stałe do eksperymentów
-printComparision - wypisz porównanie dokładnych wartości z wartościami przybliżonymi
************************************************************************/
double Tester(double (*exactValue)(unsigned int), unsigned int p, unsigned int k,
		double (*approx)(unsigned int, double), bool relativeError, double cp, double ck, bool printComparision)
{	
	double min = 10000000000000;
	double c = cp;
	for(double j = cp; j <= ck; j += 0.001) //testuj stałe z przedziału [cp, ck]. Krok: 0,001
	{
		double sum = 0; //przechowuje sumę błędów
		for(int i = p; i <= k; ++i) //liczy błąd dla każdego wyrazu z przedziału [p, k]
			sum += abs(approx(i, j) - exactValue(i)) / (relativeError ? abs(exactValue(i)) : 1);
		if(sum < min)
		{
			min = sum;
			c = j;
		}
	}
	
	if(printComparision)
	{
		cout << "Exact value \t Best approximation" << endl;
		for(int i = p; i <= k; ++i)
			cout << exactValue(i) << "\t" << approx(i, c) << endl;
	}
	cout << endl << "Average error: " << min / (k - p + 1) << endl;
	
	return c;
}
/************************************************************************/


/************************************************************************
Rekurencyjna implementacja operacji szybkiego potęgowania
Dzięki zastosowaniu szablonów możliwe jest podniesienie do k-tej
potęgi dowolnego elementu, dla którego zdefiniowano operację mnożenia,
np: liczb typu float, double czy macierzy reprezentowanej przez Matrix

Drugim argumentem funkcji jest element neutralny mnożenia
(dla liczb jest to 1, dla macierzy macierz identycznościowa)

Zastosowanie szybkiego potęgowania pozwala na przyspieszenie obliczeń i
jednoczesne zmniejszenie błędu, jakim obarczony jest końcowy wynik
************************************************************************/
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
/************************************************************************/

/************************************************************************
Funkcja F oblicza k-ty wyraz ciągu Fibonacciego za pomocą jawnego wzoru
************************************************************************/
template <class T>
T F(unsigned int k)
{
	T sqrt5 = sqrt(5);
	T a = QuickPowering<T>((1 + sqrt5) / 2, 1, k);
	T b = QuickPowering<T>((1 - sqrt5) / 2, 1, k);
	return (a - b) / sqrt5;
}
/************************************************************************/


/************************************************************************
Funkcja F oblicza k-ty wyraz ciągu G za pomocą jawnego wzoru
************************************************************************/
template <class T>
T G(unsigned int k)
{
	return QuickPowering<T>((1 - sqrt(5)) / 2, 1, k);
}
/************************************************************************/


/************************************************************************
Ponieważ dla ciągów F oraz G zachodzi ta sama zależność rekurencyjna:
F_n = F_{n-1} + F_{n-2}
G_n = G_{n-1} + G_{n-2}
z dokładnością do pierwszych dwóch początkowych wyrazów, można obliczyć
je za pomocą tej samej funkcji.

Jako drugi argument należy przekazać zerowy wyraz ciągu
Trzecim argumentem powinien być pierwszy wyraz ciągu
************************************************************************/
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
/************************************************************************/

/************************************************************************
Oblicza k-ty wyraz ciągu G za pomocą wzoru rekurencyjnego
************************************************************************/
template <class T>
T GRecursive(unsigned int k)
{
	return Recursive<T>(k, 1, (1 - sqrt(5)) / 2);
}
/************************************************************************/

/************************************************************************
Funkcja Tester wymaga przekazania wskaźnika do funkcji o sygnaturze f(unsigned int, double). Niestety, GRecursive nie spełnia tego wymagania,
dlatego zostaje opakowana w funkcję Dummy, która przyjmuje nadmiarowy argument c
************************************************************************/
template <class T>
T Dummy(unsigned int k, double c)
{
	return GRecursive<T>(k);
}
/************************************************************************/


/************************************************************************
Funkcja obliczająca k-ty wyraz ciągu G przy pomocy metody ilorazów. Stała c jest k-tym ilorazem
************************************************************************/
double QuotientMethod(unsigned int k, double c)
{
	double tab[k+1];
	tab[k] = c;
	for(int i = k - 1; i >= 1; --i)
		tab[i] = 1 / (tab[i + 1] - 1); //policz kolejno wszystkie ilorazy
	
	double scalingValue = (1 - sqrt(5)) / (2 * tab[1]); //wartość rho
	
	double previous = 1;
	for(int i = 1; i <= k; ++i)
		previous *= tab[i]; //policz wartości wyrazów ciągu przy pomocy ilorazów
	return previous * scalingValue; //przeskaluj i zwróć wynik
}
/************************************************************************/


/************************************************************************
Funkcja obliczająca k-ty wyraz ciągu G przy użyciu ciągu rekurencyjnego w kolejności malejących indeksów (używany jest algorytm opisany w
sprawozdaniu). Wartości początkowe to G_{2 * k + 1} = 1 i G_{2 * k + 2} = c
************************************************************************/
double ReversedMethod(unsigned int k, double c)
{
	double tab[2 * k + 2];
	tab[2 * k + 1] = 1 * c;
	tab[2 * k ] = 1;

	for(int i = 2 * k - 1; i >= 0; --i)
		tab[i] = tab[i + 2] - tab[i + 1]; 
		
	double scalingValue = 1 / tab[0];
	return tab[k] * scalingValue;
}
/************************************************************************/


/************************************************************************
Funkcja obliczająca wartość wzoru nr 6 dla argumentu k
************************************************************************/
double Equation6(unsigned int k)
{
	double tab[k + 1];
	tab[1] = 5;
	for(int i = 2; i <= k; ++i)
		tab[i] = tab[i/2] + i * i + 4;
	return tab[k];
}
/************************************************************************/

/************************************************************************
Szacowanie wzoru nr 6 poprzez c * n^2
************************************************************************/
double Equation6Approx(unsigned int k, double c)
{
	return c * k * k;
}
/************************************************************************/

/************************************************************************
Szacowanie wzoru przy użyciu interpolacji wielomianowej (współczynniki obliczone
dla wartości w 3 punktach)
************************************************************************/
double Equation6Approx2(unsigned int k, double c)
{
	double n = k;
	return 2 * n * n / 3 + 6 * n - 5 / (3.0);
}
/************************************************************************/

/************************************************************************
Szacowanie wzoru przy użyciu interpolacji wielomianowej (współczynniki obliczone
dla wartości w 5 punktach)
************************************************************************/
double Equation6Approx3(unsigned int k, double c)
{
	double n = k;
	return 419 * n * n /210 
  		-616 * n /45  
  		+ 8069/(90.0)  
		-49163/(315 * n) 
   		+ 748/(n * n * 9); 
}
/************************************************************************/


/************************************************************************
Funkcja obliczająca wartość wzoru nr 7 dla argumentu k
************************************************************************/
double Equation7(unsigned int k)
{
	double tab[k + 1];
	tab[1] = 5;
	for(int i = 2; i <= k; ++i)
	{
		double tmp = i;
		tab[i] = 2 * tab[i / 2] + 5 / tmp;
	}
	return tab[k];
}
/************************************************************************/

/************************************************************************
Szacowanie wzoru nr 7 poprzez c * n
************************************************************************/
double Equation7Approx(unsigned int k, double c)
{
	return c * k;
}
/************************************************************************/


/************************************************************************
Oblicz k-ty wyraz ciągu G metodą potęgowania macierzy
************************************************************************/
double MatrixPoweringMethod(unsigned int k)
{
	Matrix m = QuickPowering(Matrix(1, 1, 1, 0), Matrix :: Identity(), k);
	double s = (1 - sqrt(5)) / 2;
	return m.a21 * s + m.a22;
}
/************************************************************************/

/************************************************************************
Podobnie jak w wypadku GRecursive - tutaj opakowana zostaje funkcja MatrixPoweringMethod
************************************************************************/
double Dummy2(unsigned int k, double c) { return MatrixPoweringMethod(k); }
/************************************************************************/

/************************************************************************
Tutaj znajdują się wszystkie przeprowadzone testy.
Aby wybranego testu wystarczy go odkomentować
************************************************************************/
int main()
{	
	cout.precision(19);
	cout.setf(ios :: fixed, ios :: floatfield);
	
	/************************************************************************
	TWIERDZENIE O REKURSJI UNIWERSALNEJ (uwaga - czas działania testów jest dość długi!)
	************************************************************************/
	//Średnie błędy przybliżeń i najlepsze stałe dla wzoru 6 na kolejnych przedziałach
	/*cout << "c = " << Tester(Equation6, 1, 100, Equation6Approx, true, 0, 2, false) << endl;
	cout << "c = " << Tester(Equation6, 1, 500, Equation6Approx, true, 0, 2, false) << endl;
	cout << "c = " << Tester(Equation6, 1, 1000, Equation6Approx, true, 0, 2, false) << endl;
	cout << "c = " << Tester(Equation6, 1, 2000, Equation6Approx, true, 0, 2, false) << endl;*/
	
	//Średnie błędy przybliżeń i najlepsze stałe dla wzoru 7 na kolejnych przedziałach
	/*cout << "c = " << Tester(Equation7, 1, 100, Equation7Approx, true, 3, 5, false) << endl;
	cout << "c = " << Tester(Equation7, 1, 500, Equation7Approx, true, 3, 5, false) << endl;
	cout << "c = " << Tester(Equation7, 1, 1000, Equation7Approx, true, 3, 5, false) << endl;
	cout << "c = " << Tester(Equation7, 1, 2000, Equation7Approx, true, 3, 5, false) << endl;*/
	/************************************************************************/
	
	/************************************************************************
	INTERPOLACJA WIELOMIANOWA
	************************************************************************/
	//Średnie błędy przybliżeń dla wzoru 6 (interpolacja na podstawie wartości w 3 punktach) 
	/*Tester(Equation6, 1, 100, Equation6Approx2, true, 0, 0, false);
	Tester(Equation6, 1, 500, Equation6Approx2, true, 0, 0, false);
	Tester(Equation6, 1, 2000, Equation6Approx2, true, 0, 0, false);*/
	
	//Średnie błędy przybliżeń dla wzoru 6 (interpolacja na podstawie wartości w 5 punktach) 
	/*Tester(Equation6, 1, 100, Equation6Approx3, true, 0, 0, false);
	Tester(Equation6, 1, 500, Equation6Approx3, true, 0, 0, false);
	Tester(Equation6, 1, 2000, Equation6Approx3, true, 0, 0, false);*/
	/************************************************************************/
	
	/************************************************************************
	UŻYWANIE WZORU REKURENCYJNEGO
	************************************************************************/
	//Porównanie wartości dokładnych i przybliżonych (w kolejności rosnących indeksów) na przedziale 1 - 50
	//Tester(G<double>, 1, 50, Dummy<double>, true, 0, 0, true);
	
	//Porównanie wartości dokładnych i przybliżonych (w kolejności malejących indeksów) dla G101 = 1 i G102 = -1 na przedziale 1 - 50
	//Tester(G<double>, 1, 50, ReversedMethod, true, -1, -1, true);
	
	//Średni błąd względny dla metody używania wzoru rekurencyjnego w kolejności malejących indeksów dla G101 = 1 i G102 = -1 na przedziale 10 - 50
	//Tester(G<double>, 10, 50, ReversedMethod, true, -1, -1, false);
	
	//Poszukiwanie takiego c, że dla wartości G101 = 1 i G102 = c metoda używania wzoru rekurencyjnego w kolejności malejących indeksów
	//daje najlepsze wyniki
	//cout << "c = " << Tester(G<double>, 1, 50, ReversedMethod, true, -1, -0.1, false) << endl;
	/************************************************************************/
	
	/************************************************************************
	METODA ILORAZÓW
	************************************************************************/
	//Średnie błędy względne i bezwzględne dla pewnych stałych na przedziale 1 - 50
	/*Tester(G<double>, 1, 50, QuotientMethod, false, -0.8, -0.8, false);
	Tester(G<double>, 1, 50, QuotientMethod, true, -0.8, -0.8, false);
	Tester(G<double>, 1, 50, QuotientMethod, false, -0.7, -0.7, false);
	Tester(G<double>, 1, 50, QuotientMethod, true, -0.7, -0.7, false);
	Tester(G<double>, 1, 50, QuotientMethod, false, -0.62, -0.62, false);
	Tester(G<double>, 1, 50, QuotientMethod, true, -0.62, -0.62, false);
	Tester(G<double>, 1, 50, QuotientMethod, false, -0.618, -0.618, false);
	Tester(G<double>, 1, 50, QuotientMethod, true, -0.618, -0.618, false);
	Tester(G<double>, 1, 50, QuotientMethod, false, -0.6, -0.6, false);
	Tester(G<double>, 1, 50, QuotientMethod, true, -0.6, -0.6, false);
	Tester(G<double>, 1, 50, QuotientMethod, false, -0.4, -0.4, false);
	Tester(G<double>, 1, 50, QuotientMethod, true, -0.4, -0.4, false);*/
	
	//Porównanie wartości dokładnych i przybliżonych dla stałej -0.618 na przedziale 1 - 50
	//Tester(G<double>, 1, 50, QuotientMethod, true, -0.618, -0.618, true);
	
	//Porównanie wartości dokładnych i przybliżonych dla stałej -0.7 na przedziale 1 - 50
	//Tester(G<double>, 1, 50, QuotientMethod, true, -0.7, -0.7, true);
	/************************************************************************/
	
	/************************************************************************
	METODA POTĘGOWANIA MACIERZY
	************************************************************************/
	//Porównanie wartości dokładnych i przybliżonych na przedziale 1 - 50
	//Tester(G<double>, 1, 50, Dummy2, true, 0, 0, true);
	/************************************************************************/
	return 0;
}
