/********************************************************************************************
 * Przemysław Pastuszka
 * Nr albumu 233 186
 * Analiza numeryczna 2010/2011 - pracownia nr 3, zad. 23
********************************************************************************************/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>

using namespace std;
/*******************************************************************************************
 * Niestety, C++ nie pozwala na użycie lambda wyrażeń, dlatego muszę posiłkować się takimi
 * pośrednimi rozwiązaniami. 
 * Poniższe klasy pozwalają na zasymulowanie użycia lamda wyrażeń.
********************************************************************************************/
class Function{ //abstrakcyjna klasa przedstawiająca funkcję
	public:
	virtual double At(double x) const = 0; //policz wartość f. w punkcie x
};
//pozwala na stworzenie obiektu typu Function na podstawie wskaźnika do funkcji
class Anything : public Function{ 
	private:
	double (*f)(double);
	
	public:
	Anything(double (*fun)(double x)){
		f = fun;
	}
	double At(double x) const {return f(x);}
};
class Substract : public Function{ //dla danych funkcji f i g tworzy funkcję f - g
	private:
	Function *a, *b;
	
	public:
	Substract(Function *f, Function *g){
		a = f;
		b = g;
	}
	double At(double x) const {return a -> At(x) - b -> At(x);}
};
/********************************************************************************************/

/*******************************************************************************************
 * Poniższa klasa pozwala na łatwe i intuicyjne operacje na wielomianach. Umożliwia między innymi:
 *  dodawanie wielomianów
 *  mnożenie wielomianów
 *  całkowanie symboliczne wielomianów
********************************************************************************************/
class Polynomial : public Function{
    private:
    int n; //stopień
    vector<double> coeffs; //współczynniki przy potęgach (od najmniej do najbardziej znaczącej)
    
    public:
    Polynomial(double c = 0) : n(0){ //stwórz stały wielomian
        coeffs.push_back(c);
    }
    Polynomial(double a, double b) : n(1){ //wielomian stopnia 1: ax + b
        coeffs.push_back(b);
        coeffs.push_back(a);
    }
    //wielomian stopnia size - 1 o współczynnikach danych w tablicy values
    Polynomial(double* values, int size) : n(size - 1){
        coeffs.assign(values, values + size);
    }
    
    Polynomial operator+(const Polynomial& b) const;
    Polynomial operator*(const Polynomial& b) const;
    Polynomial operator-(const Polynomial& b) const{
		return operator+(-1 * b);
	}
    double At(double x) const;
    double operator()(double x) const {return At(x);}
    int Degree() const {return n;}
    
    double Integral(double a, double b) const;
    
    friend Polynomial operator*(double c, const Polynomial& b);
    
    void Print(){ //wypisuje wielomian na ekran
        for(int i = 0; i <= n; ++i){
            cout << coeffs[i] << "x^" << i << " + ";
        }
        cout << endl;
    }
};
//symboliczne całkowanie wielomianów 
//korzystam z tego, że integral x^i dx = x^(i + 1) / (i + 1)
double Polynomial :: Integral(double a, double b) const{
    Polynomial tmp;
    tmp.n = n + 1;
    tmp.coeffs.resize(tmp.n + 1);
    for(int i = 0; i <= n; ++i){
        tmp.coeffs[i + 1] = coeffs[i] / (i + 1);
    }
    return tmp(b) - tmp(a);
}
//mnożenie wielomianu przez stałą
Polynomial operator*(double c, const Polynomial& b){
    Polynomial result;
    result.n = b.n;
    result.coeffs.resize(result.n + 1);
    for(int i = 0; i <= result.n; ++i){
        result.coeffs[i] = c * b.coeffs[i];
    }
    return result;
}
//dodawanie wielomianów
Polynomial Polynomial :: operator+(const Polynomial& b) const{
    Polynomial result;
    result.n = max(n, b.n);
    result.coeffs.resize(result.n + 1, 0);
    for(int i = 0; i <= result.n; ++i){
        result.coeffs[i] = i <= n ? coeffs[i] : 0;
        result.coeffs[i] += i <= b.n ? b.coeffs[i] : 0;
    }
    return result;
}
//mnożenie wielomianów
Polynomial Polynomial :: operator*(const Polynomial& b) const{
    Polynomial result;
    result.n = n + b.n;
    result.coeffs.resize(result.n + 1, 0);
    for(int i = 0; i <= result.n; ++i){
        for(int j = 0; j <= i; ++j){
            if(j <= n && i - j <= b.n){
                result.coeffs[i] += coeffs[j] * b.coeffs[i - j];
            }
        }
    }
    return result;
}
//oblicza wartość wielomianu w punkcie x korzystając ze schematu Hornera
double Polynomial :: At(double x) const{
    double result = coeffs[n];
    for(int i = n - 1; i >= 0; --i){
        result *= x;
        result += coeffs[i];
    }
    return result;
}
/********************************************************************************************/

/********************************************************************************************
 * Klasa zajmująca się 'produkcją' wielomianów ortogonalnych dla podanej funkcji wagowej i przedziału
********************************************************************************************/
class OrthoPolynomials{
	private:
	vector<Polynomial> orthos; //przetrzymuje obliczone wielomiany ortogonalne
	int n; //stopień najwyższego obliczonego już wielomianu
	double a, b; //przedział
	Polynomial p, tmp; //p jest funkcją wagową
	
	public:
	OrthoPolynomials(const Polynomial& p, double a, double b) : n(1){
		this -> p = p;
		this -> a = a;
		this -> b = b;
		tmp = Polynomial(1, 0);
		orthos.push_back(Polynomial(1));
		orthos.push_back(Polynomial(1, -(p * tmp).Integral(a, b) / p.Integral(a, b)));
	}
	
	Polynomial operator[](unsigned int k);
};
//generuje wielomiany ortogonalne dla podanej funkcji wagowej p (będącej wielomianem) w przedziale a, b
Polynomial OrthoPolynomials :: operator[](unsigned int k){
	if(k <= n){
		return orthos[k]; //wielomian już obliczony
	}
	
    //oblicz wielomian k - tego stopnia
    for(int i = n + 1; i <= k; ++i){
        double m = (orthos[i - 1] * orthos[i - 1] * p).Integral(a, b); //<P_{k-1}, P_{k-1}>
        double q = (orthos[i - 2] * orthos[i - 2] * p).Integral(a, b); //<P_{k-2}, P_{k-2}>
        double w = (tmp * orthos[i - 1] * orthos[i - 1] * p).Integral(a, b); //<xP_{k-1}, P_{k-1}>
        double z = (tmp * orthos[i - 1] * orthos[i - 2] * p).Integral(a, b); //<xP_{k-1}, P_{k-2}>
        orthos.push_back(Polynomial(1, -w / m) * orthos[i - 1] + (-z / q) * orthos[i - 2]);
    }
    n = k;
    return orthos[k];
}
/********************************************************************************************/

/********************************************************************************************
 * Całkowanie złożonym wzorem Simpsona. Nieużywane, więc zakomentowane
********************************************************************************************/
/*double Simpson(double a, double b, int n, Function **f, int m){
	double result = 0;
	double step = (b - a) / n;
	for(int i = 0; i < n; ++i){
		double p = 1, k = 1, s = 1;
		for(int j = 0; j < m; ++j){
			p *= f[j] -> At(a + i * step);
			k *= f[j] -> At(a + i * step + step);
			s *= f[j] -> At(a + i * step + step / 2);
		}
		result += p + 4 * s + k;
	}
	return step * result / 6;
}
double Simpson(double a, double b, int n, Function* f, Function* g, Function* h){
	Function** funs = new Function*[3];
	funs[0] = f;
	funs[1] = g;
	funs[2] = h;
	double result = Simpson(a, b, n, funs, 3);
	delete [] funs;
	return result;
}
double SimpsonSub(double a, double b, int n, Function* f, Function* g, Function* h){
	Function *tmp = new Substract(g, h);
	double result = Simpson(a, b, n, f, tmp, tmp);
	delete tmp;
	return result;
}*/
/********************************************************************************************/

/********************************************************************************************
 * Całkowanie za pomocą kwadratur Gaussa - Legendre'a z użyciem 34 węzłów.
 * Współczynniki pochodzą ze strony: http://www.ii.uni.wroc.pl/~pkl/GL.dat
 * 
 * Co prawda znalazłem na tej stronie: 
 * http://keisan.casio.com/has10/SpecExec.cgi?path=08500000.Numerical analysis%2f07000000.Numerical integration%2f10030500.Nodes and Weights of Gauss-Legendre%2fdefault.xml&charset=utf-8
 * współczynniki dla 50 węzłow, jednakże były one mało dokładne (całkowanie działało słabiej
 * niż metoda Simpsona!)
********************************************************************************************/
double x[50], w[50];
void Init(){
    x[0] = -9.97571753790841919243372e-01;   w[0] = 6.22914055590868471860646e-03;
    x[1] = -9.87227816406309485049750e-01;   w[1] = 1.44501627485950354152022e-02;
    x[2] = -9.68708262533344281764647e-01;   w[2] = 2.25637219854949700840941e-02;
    x[3] = -9.42162397405107091631676e-01;   w[3] = 3.04913806384461318094424e-02;
    x[4] = -9.07809677718324468800900e-01;   w[4] = 3.81665937963875163217659e-02;
    x[5] = -8.65934638334564469263572e-01;   w[5] = 4.55256115233532724538226e-02;
    x[6] = -8.16884227900933664591579e-01;   w[6] = 5.25074145726781061682460e-02;
    x[7] = -7.61064876629873014187409e-01;   w[7] = 5.90541358275244931939610e-02;
    x[8] = -6.98939113216262907933000e-01;   w[8] = 6.51115215540764113785444e-02;
    x[9] = -6.31021727080528545317776e-01;   w[9] = 7.06293758142557249990388e-02;
    x[10] = -5.57875500669746642736460e-01;   w[10] = 7.55619746600319312708340e-02;
    x[11] = -4.80106545190327034194103e-01;   w[11] = 7.98684443397718447388188e-02;
    x[12] = -3.98359277758645940631495e-01;   w[12] = 8.35130996998456551870202e-02;
    x[13] = -3.13311081339463247458317e-01;   w[13] = 8.64657397470357497842469e-02;
    x[14] = -2.25666691616449483868641e-01;   w[14] = 8.87018978356938692870765e-02;
    x[15] = -1.36152357259182975894429e-01;   w[15] = 9.02030443706407295739422e-02;
    x[16] = -4.55098219531025427490757e-02;   w[16] = 9.09567403302598736153376e-02;
    x[17] =  4.55098219531025427490757e-02;   w[17] = 9.09567403302598736153376e-02;
    x[18] =  1.36152357259182975894429e-01;   w[18] = 9.02030443706407295739422e-02;
    x[19] =  2.25666691616449483868641e-01;   w[19] = 8.87018978356938692870765e-02;
    x[20] =  3.13311081339463247458317e-01;   w[20] = 8.64657397470357497842469e-02;
    x[21] =  3.98359277758645940631495e-01;   w[21] = 8.35130996998456551870202e-02;
    x[22] =  4.80106545190327034194103e-01;   w[22] = 7.98684443397718447388188e-02;
    x[23] =  5.57875500669746642736460e-01;   w[23] = 7.55619746600319312708340e-02;
    x[24] =  6.31021727080528545317776e-01;   w[24] = 7.06293758142557249990388e-02;
    x[25] =  6.98939113216262907933000e-01;   w[25] = 6.51115215540764113785444e-02;
    x[26] =  7.61064876629873014187409e-01;   w[26] = 5.90541358275244931939610e-02;
    x[27] =  8.16884227900933664591579e-01;   w[27] = 5.25074145726781061682460e-02;
    x[28] =  8.65934638334564469263572e-01;   w[28] = 4.55256115233532724538226e-02;
    x[29] =  9.07809677718324468800900e-01;   w[29] = 3.81665937963875163217659e-02;
    x[30] =  9.42162397405107091631676e-01;   w[30] = 3.04913806384461318094424e-02;
    x[31] =  9.68708262533344281764647e-01;   w[31] = 2.25637219854949700840941e-02;
    x[32] =  9.87227816406309485049750e-01;   w[32] = 1.44501627485950354152022e-02;
    x[33] =  9.97571753790841919243372e-01;   w[33] = 6.22914055590868471860646e-03;
}
//Oblicza całkę funkcji f[0] * f[1] * .... * f[m - 1]
double Legendre(Function **f, int m){
	double result = 0;
	for(int i = 0; i < 34; ++i){
		double p = w[i];
		for(int j = 0; j < m; ++j){
            //następuje mapowanie przedziału [-1, 1] na [0, 1] za pomocą
            //funkcji (x + 1) / 2
			p *= f[j] -> At((x[i] + 1) / 2);
		}
		result += p;
	}
    //przedział całkowania zmniejszył się dwukrotnie, więc to samo należy zrobić z wynikiem
	return result / 2;
}
//oblicza całkę funkcji f * g * h
double Legendre(Function* f, Function* g, Function* h){
	Function** funs = new Function*[3];
	funs[0] = f;
	funs[1] = g;
	funs[2] = h;
	double result = Legendre(funs, 3);
	delete [] funs;
	return result;
}
//oblicza całkę funkcji f * (g - h)^2
double LegendreSub(Function* f, Function* g, Function* h){
	Function *tmp = new Substract(g, h);
	double result = Legendre(f, tmp, tmp);
	delete tmp;
	return result;
}
/********************************************************************************************/

/********************************************************************************************/
OrthoPolynomials generator(Polynomial(1, 0), 0, 1); //generuje wielomiany ortogonalne

/*
 * Znajduje wielomian optymalny w najniższego stopnia taki, że
 * zachodzi ||f - w||^2 < epsilon.
 * Całkowanie odbywa się za pomocą przekształceń symbolicznych
*/
Polynomial FindOptimal(Polynomial f, double epsilon){
	Polynomial p(1, 0);
	Polynomial optimal;
	for(int i = 0; (p * (f - optimal) * (f - optimal)).Integral(0, 1) >= epsilon; ++i){
		optimal = optimal + //korzystam ze wzoru na wielomian optymalny wyprowadzonego w sprawozdaniu
			((p * f * generator[i]).Integral(0, 1))/((p * generator[i] * generator[i]).Integral(0, 1)) * generator[i];
	}
	return optimal;
}
//jak wyżej tylko całkowanie odbywa się metodą Gaussa - Legendre'a
Polynomial FindOptimal(Function *f, double epsilon){
	Polynomial p(1, 0);
	Polynomial optimal;
	for(int i = 0; LegendreSub(&p, f, &optimal) >= epsilon; ++i){
		Polynomial gen = generator[i];
		optimal = optimal +
			Legendre(&p, f, &gen)/((p * gen * gen).Integral(0, 1)) * gen;
	}
	return optimal;
}
/********************************************************************************************/

//generuje losowy wielomian stopnia nie większego niż 25
Polynomial Generate(){
    double q[25];
    int a = 2 + rand() % 23;
    for(int j = 0; j < a; ++j){
		double m = rand() % 20000 - 10000;
		double n = rand() % 10000;
        q[j] = m != 0 && n != 0 ? m / n : 1;
    }
    return Polynomial(q, a);
}

//oblicza funkcję g(n) zdefiniowaną w sprawozdaniu
double Test1(int n){
    Polynomial p(1, 0);
    double max = 0;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < i; ++j){
            double res = (p * generator[i] * generator[j]).Integral(0, 1);
            res = abs(res);
            max = res > max ? res : max;
        }
    }
    return max;
}

//wykonuje drugi test opisany w sprawozdaniu z użyciem całkowania symbolicznego
void Test2A(int n, double epsilon, bool printErrors){
	int errors = 0;
	for(int i = 0; i < n; ++i){
		Polynomial tmp = Generate();
		Polynomial opt = FindOptimal(tmp, epsilon);
		if(opt.Degree() > tmp.Degree()){
			++errors;
			if(printErrors){
				cout << "Stopien losowego: " << tmp.Degree() << endl;
				cout << "Stopien obliczonego: " << opt.Degree() << endl;
				tmp.Print();
				cout << endl;
			}
		}
	}
	cout << "Liczba bledow: " << errors << endl;
}
//to samo co wyżej tylko przy użyciu kwadratur Gaussa - Legendre'a
void Test2B(int n, double epsilon, bool printErrors){
	int errors = 0;
	for(int i = 0; i < n; ++i){
		Polynomial tmp = Generate();
		Polynomial opt = FindOptimal(&tmp, epsilon);
		if(opt.Degree() > tmp.Degree()){
			++errors;
			if(printErrors){
				cout << "Stopien losowego: " << tmp.Degree() << endl;
				cout << "Stopien obliczonego: " << opt.Degree() << endl << endl;
			}
		}
	}
	cout << "Liczba bledow: " << errors << endl;
}
//test 3 ze sprawozdania
void Test3(Function* f, int n){
    Polynomial p(1, 0);
	Polynomial optimal;
	for(int i = 0; i < n; ++i){
		Polynomial gen = generator[i];
		optimal = optimal +
			Legendre(&p, f, &gen)/((p * gen * gen).Integral(0, 1)) * gen;
        cout << i << endl;
        optimal.Print();
        cout << setprecision(18) << fixed << "Blad: " << LegendreSub(&p, f, &optimal) << endl << endl;
	}
}

/********************************************************************************************/
int main(){
    srand(time(NULL));
    Init();
    
    //tu należy wywoływać funkcje testowe
    return 0;
}
/********************************************************************************************/
