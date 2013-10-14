/*******************************************************************************
 * Analiza numeryczna - pracownia nr 2
 * Zadanie 14
 * Przemysław Pastuszka
 * nr albumu 233 186
*******************************************************************************/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

/*******************************************************************************
* Klasa macierz pozwalająca na przeprowadzenie eliminacji Gaussa
* z pełnym lub częściowym wyborem elementów głównych
*******************************************************************************/
class Matrix{
    private:
    unsigned long m, n; //wymiary macierzy
    double **elements; //elementy
    
    public:
    Matrix(unsigned long m, unsigned long n);
    Matrix(const Matrix&);
    ~Matrix();
    
    //funkcje pomocnicze eliminacji Gaussa
    void SwapRows(unsigned long a, unsigned long b); //zamienia wiersze
    void SwapColumns(unsigned long a, unsigned long b); //zamienia kolumny
    void MultiplyAndSubstract(unsigned long a, unsigned long b, double c); //odejmuje od wiersza b wiersz a przemnożony przez c
    void MultiplyBy(unsigned long a, double c); //mnoży wiersz a przez skalar c
    
    double InftyNorm() const; //oblicza normę macierzy (nieskończoność)
    unsigned long GaussRank(bool fullChoice); //oblicza rząd macierzy przy pomocy eliminacji Gaussa
    
    //operator [] pozwala na longuicyjny dostęp do elementów macierzy
    double* operator[](unsigned long k){
        return elements[k];
    }
    void Prlong(unsigned long prec, unsigned long width) const; //wypisuje macierz na ekran (z ustaloną precyzją i długością)
};
/*******************************************************************************/
void Matrix :: SwapRows(unsigned long a, unsigned long b){
    double* tmp = elements[a];
    elements[a] = elements[b];
    elements[b] = tmp;
}
/*******************************************************************************/
void Matrix :: SwapColumns(unsigned long a, unsigned long b){
    for(long i = 0; i < m; ++i){
        double tmp = elements[i][a];
        elements[i][a] = elements[i][b];
        elements[i][b] = tmp;
    }
    
}
/*******************************************************************************/
Matrix :: Matrix(unsigned long m, unsigned long n){
    this -> m = m;
    this -> n = n;
    elements = new double*[m];
    for(unsigned long i = 0; i < m; ++i)
        elements[i] = new double[n];
}
/*******************************************************************************/
Matrix :: Matrix(const Matrix& matrix){
    this -> m = matrix.m;
    this -> n = matrix.n;
    elements = new double*[m];
    for(unsigned long i = 0; i < m; ++i)
        elements[i] = new double[n];
    for(unsigned long i = 0; i < m; ++i){
        for(unsigned long j = 0; j < n; ++j)
            elements[i][j] = matrix.elements[i][j];
    }
}
/*******************************************************************************/
Matrix :: ~Matrix(){
    for(unsigned long i = 0; i < m; ++i)
        delete [] elements[i];
    delete [] elements;
}
/*******************************************************************************/
void Matrix :: MultiplyAndSubstract(unsigned long a, unsigned long b, double c){
    for(unsigned long i = 0; i < n; ++i)
        elements[b][i] -= c * elements[a][i];
}
/*******************************************************************************/
void Matrix :: MultiplyBy(unsigned long a, double c){
    for(unsigned long i = 0; i < n; ++i)
        elements[a][i] *= c;
}
/*******************************************************************************/
void Matrix :: Prlong(unsigned long prec, unsigned long width) const{
    cout.precision(prec);
    cout.setf(ios :: fixed, ios :: floatfield);
    for(long i = 0; i < m; ++i){
        for(long j = 0; j < n; ++j){
            std :: cout << setw(width + prec) << elements[i][j] << " ";
        }
        cout << endl;
    }
}
/*******************************************************************************/
//Norma macierzy wyliczana jest według wzoru, który można odnaleźć w notatkach do wykładu
double Matrix :: InftyNorm() const{
    double norm = 0;
    for(long i = 0; i < m; ++i){
        double sum = 0;
        for(long j = 0; j < n; ++j){
            sum += abs(elements[i][j]);
        }
        if(sum > norm)
            norm = sum;
    }
    return norm;
}
/*******************************************************************************/
long epsilonPower = 0;
//jeśli useNorm jest ustawione na true to wykorzystywana jest druga postać funkcji E (patrz sprawozdanie, wzór 2),
//wpp wykorzystywana jest pierwsza wersja
bool useNorm = false;
/*******************************************************************************/
unsigned long Matrix :: GaussRank(bool fullChoice){
    unsigned long q = std :: min(m, n);
    unsigned long i = 0, mv = 0;
    double epsilon = pow(2, -epsilonPower);
    double inftyNorm = InftyNorm();
    for(; i + mv < q; ++i){
        
        //znajdź element główny
        double mValue = abs(elements[i][i + mv]); //aktualna maksymalna wartość
        long bestRow = i, bestCol = i + mv;
        unsigned long bound = fullChoice ? n : (i + mv < n ? i + mv + 1 : i + mv);
        for(unsigned long z = i + mv; z < bound; ++z){ 
            for(unsigned long j = i; j < m; ++j){
                if(abs(elements[j][z]) > mValue){
                    mValue = abs(elements[j][z]);
                    bestRow = j;
                    bestCol = z;
                }
            }
        }
        
        //sprawdzenie czy należy zakończyć działanie algorytmu
        if(useNorm && mValue < epsilon * inftyNorm){
            if(fullChoice){
                break;
            }
            else{
                ++mv;
                --i;
                continue;
            }
        }
        if((!useNorm) && mValue < epsilon){
            if(fullChoice){
                break;
            }
            else{
                ++mv;
                --i;
                continue;
            }
        }
            
        if(i != bestRow){
            SwapRows(i, bestRow); //zamień aktualny wiersz z wierszem zawierającym element główny
        }
        if(i + mv != bestCol){
            SwapColumns(i + mv, bestCol); //zamień aktualną kolumnę z kolumną zawierającą element główny
        }
        
        //jeśli wyraz wiodący wiersza jest różny od 0 to można wykonać krok eliminacji
        if(elements[i][i + mv] != 0){
            MultiplyBy(i, 1 / elements[i][i + mv]);
            for(unsigned long j = i + 1; j < m; ++j){
                MultiplyAndSubstract(i, j, elements[j][i + mv]);
            }
        }
    }
    return i;
}
/*******************************************************************************/
/*******************************************************************************/

/*******************************************************************************
* Testuj przy pomocy n macierzy o wymiarach mxm o wartościach w zbiorze {cp, ..., ck}
* o rzędzie rank, wykonując na nich p przekształceń elementarnych
*******************************************************************************/
void Tester(unsigned long n, unsigned long m, unsigned long cp, unsigned long ck, unsigned long p, unsigned long rank,
    bool prlongErrors){
        
    Matrix test(m, m);
    //poniższe dwie zmienne zliczają ilość niepoprawnie obliczonych macierzy metodą Gaussa z odpowiednio częściowym i pełnym wyborem
    unsigned long errors = 0, errors2 = 0;
    for(unsigned long i = 0; i < n; ++i){
        //losuj wartości macierzy (na i powyżej głównej przekątnej)
        for(unsigned long k = 0; k < m; ++k){
            for(unsigned long j = 0; j < m; ++j){
                if(j >= k && k < rank) //tylko pewne wiersze winny zostać niezerowe
                    test[k][j] = cp + rand() % (ck + 1 - cp);
                else
                    test[k][j] = 0; //zeruj pod główną przekątną
            }
        }
        //wykonaj p przekształceń elementarnych (przemnóż wiersz przez skalar albo dodaj jeden wiersz do drugiego mnożąc przez pewien skalar)
        //te skalary też są z przedziału {cp, ..., ck}
        for(unsigned long j = 0; j < p; ++j){
            long a = rand() % m, b = rand() % m;
            double c = (cp + rand() % (ck + 1 - cp));
            if(a == b)
                test.MultiplyBy(a, c);
            else
                test.MultiplyAndSubstract(a, b, -c);
        }
        
        Matrix copy(test);
        Matrix copy2(test);
       // unsigned long result = copy.GaussRank(false);
        unsigned long result2 = copy2.GaussRank(true);
        /*if(result != rank){
            if(prlongErrors){
                cout << "Metoda Gaussa z czesciowym wyborem elementow " << endl;
                cout << "Poprawny rzad: " << rank << "\nRzad obliczony: " << result << endl;
                test.Prlong(0, 5);
                std :: cout << std :: endl;
                copy.Prlong(2, 5);
                std :: cout << std :: endl;
                std :: cout << std :: endl;
            }
            ++errors;
        }*/
        if(result2 != rank){
            if(prlongErrors){
                cout << "Metoda Gaussa z pelnym wyborem elementow " << endl;
                cout << "Poprawny rzad: " << rank << "\nRzad obliczony: " << result2 << endl;
                test.Prlong(0, 5);
                std :: cout << std :: endl;
                copy2.Prlong(2, 5);
                std :: cout << std :: endl;
                std :: cout << std :: endl;
            }
            ++errors2;
        }
    }
    //~ cout << "Niepoprawnie obliczono rzad " << errors << " macierzy (czesciowy wybor elementow)" << endl;
    //~ cout << "Niepoprawnie obliczono rzad " << errors2 << " macierzy (pelny wybor elementow)" << endl;
    //cout << errors << "\t" << errors2 << endl;
    cout << errors2 << endl;
} 
/*******************************************************************************/
/*******************************************************************************/

int main(){
    srand(time(NULL));
    
    /***************************************************************************
     * Aby wykonać żądane testy wystarczy zmienić poniższe współczynniki
    ****************************************************************************/
    unsigned long nrOfMatricesToTest = 200000; //liczba macierzy testowych
    unsigned long matricesSize = 15; //wymiary macierzy testowych
    unsigned long cp = 1, ck = 100000; //skalary dla przekształceń elementarnych będą losowane ze zbioru {cp, ..., ck}
    unsigned long nrOfElementaryOperations = 4; //liczba operacji elementarnych jakie zostaną wykonane
    unsigned long desiredRank = 12; //będą sprawdzane macierze o takim rzędzie
    unsigned long verboseMode = false; //czy wypisywać dokładne informacje o błędach
    useNorm = true; //czy używać drugiej postaci funkcji E
    unsigned long pEpsilon = 5, kEpsilon = 75; //zostaną przetestowane epsilony w tym przedziale
    
    //właściwa pętla testująca
    for(epsilonPower =  pEpsilon; epsilonPower <= kEpsilon; ++epsilonPower){
        cout << epsilonPower << " ";
        Tester(nrOfMatricesToTest, matricesSize, cp, ck, nrOfElementaryOperations, desiredRank, verboseMode);
    }
    
    return 0;
}
