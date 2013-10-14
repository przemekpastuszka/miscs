/*
Zadanie na pracownię numer 4 - Grafika Komputerowa
Code by Przemysław Pastuszka
*/
#include <GL/glew.h>
#include <GL/glfw.h>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>

using namespace std;

//rozmiar okna
const float WIDTH = 1024;
const float HEIGHT = 768;

/************************************************************************************/
//Przechowuje trójkę floatów (może być interpretowane jako punkt lub wektor w 3d)
struct Point{
    float x, y, z;
    Point() : x(0), y(0), z(0) {}
    Point(float r, float g, float b) : x(r), y(g), z(b) {}
    Point(const Point& a) : x(a.x), y(a.y), z(a.z) {}
    
    //iloczyn wektorowy
    static Point CrossProduct(const Point& a, const Point& b){
        return Point(a.y * b.z - a.z * b.y, -a.x * b.z + a.z * b.x, a.x * b.y - a.y * b.x);
    }
    
    friend const Point operator+(const Point& a, const Point& b);
    friend const Point operator*(double c, const Point& b);
    Point& operator+=(const Point& a){
        x += a.x;
        y += a.y;
        z += a.z;
        return *this;
    }
    Point RotateY(float angle);
    Point RotateX(float angle);
    float Length(){ 
        return sqrt(x * x + y * y + z * z);
    }
    Point Normalize(){ //zwraca znormalizowany wektor
        float l = Length();
        return Point(x / l, y / l, z / l);
    }
};
//dodawanie dwóch wektorów
const Point operator+(const Point& a, const Point& b){
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}
const Point operator*(double c, const Point& b){
    return Point(b.x * c, b.y * c, b.z * c);
}
Point Point :: RotateY(float angle){
    return Point(x * (float)cos(angle) - z * (float)sin(angle), y, x * (float)sin(angle) + z * (float)cos(angle));
}
Point Point :: RotateX(float angle){
    return Point(x, y * (float)cos(angle) - z * (float)sin(angle), y * (float)sin(angle) + z * (float)cos(angle));
}
/************************************************************************************/

/************************************************************************************/
//klasa zajmująca się obsługą kamery
class Camera{
    private:
    double speed, rotationSpeed;
    double rotationX, rotationY;
    //wektory opisujące podstawowe kierunki w przestrzeni
    Point left, right, forward, backward, lookDir, up, top, position;
    int lastMouseX, lastMouseY; //ostatnia pozycja myszy
    bool perspectiveProjection; //czy używamy projekcji z perspektywą
    
    void SetLookAt();
    void SetProjection();
    
    
    public:
    Camera(bool perspective){
        glfwGetMousePos(&lastMouseX, &lastMouseY);
        perspectiveProjection = perspective;
        //ustawia wektory podstawowych kierunków
        position = Point(0, 0, -50);
        left = Point(1, 0, 0);
        right = Point(-1, 0, 0);
        forward = Point(0, 0, 1);
        backward = Point(0, 0, -1);
        top = Point(0, 1, 0);
        rotationX = rotationY = 0;
        rotationSpeed = 0.01; //arbitralnie wybrana prędkość obrotu kamery
        speed = 15; //prędkość ruchu kamery
    }
    
    void Update(double time);
};
void Camera :: SetLookAt(){
   // Point up = Point :: CrossProduct(lookDir, right);
    Point lookAt = position + lookDir; 
    gluLookAt(position.x, position.y, position.z, lookAt.x, lookAt.y, lookAt.z,
        up.x, up.y, up.z);
}
void Camera :: SetProjection(){
    if(perspectiveProjection){
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45, WIDTH / HEIGHT, 0.2, 1000);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }
}
void Camera :: Update(double time){

    //zajmuje się ruchem kamery
    if(glfwGetKey('A') == GLFW_PRESS)
        position += time * speed * left.RotateX(rotationX).RotateY(rotationY);
    if(glfwGetKey('D') == GLFW_PRESS)
        position += time * speed * right.RotateX(rotationX).RotateY(rotationY);
    if(glfwGetKey('S') == GLFW_PRESS)
        position += time * speed * backward.RotateX(rotationX).RotateY(rotationY);
    if(glfwGetKey('W') == GLFW_PRESS)
        position += time * speed * forward.RotateX(rotationX).RotateY(rotationY);
    
    //ustaw kierunek patrzenia i wektor określający obrót kamery (up vector)
    lookDir = forward.RotateX(rotationX).RotateY(rotationY);
    up = top.RotateX(rotationX).RotateY(rotationY);
    
    //określa zmianę w pozycji myszy
    int mouseX, mouseY;
    glfwGetMousePos(&mouseX, &mouseY);
    int deltaX, deltaY;
    deltaX = lastMouseX - mouseX;
    deltaY = lastMouseY - mouseY;
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    
    rotationY -= deltaX * rotationSpeed;
    rotationX -= deltaY * rotationSpeed;
    
    SetProjection();
    SetLookAt();
}
/************************************************************************************/

/************************************************************************************/
//Przechowuje informacje o obiekcie
class Object{
    private:
    float* vertices; //przetrzymuje pozycje wierzchołków
    float* colors; //kolory kolejnych wierzchołków
    int nrOfVertices; //liczba wierzchołków
    int drawingMode; //GL_TRIANGLES, GL_QUADS, itede
    
    bool buffersInitialized; //czy informacje o wierzchołkach i kolorach zostały przesłane do karty graficznej
    GLuint vertBuffer, colorBuffer; //numery używanych buforów
    
    Object(int nrOfElements);
    
    void GenerateBuffers();
    
    static Point CalculateColor(float height, float maxH, float minH);
    public:
    
    ~Object(){
        delete [] vertices;
        delete [] colors;
    }
    
    //funkcje rysujące
    void DrawBeginEnd();
    void DrawVA();
    void DrawVBO();
    
    //funkcje generujące teren
    static Object* GenerateRandomTriangles(int nrOfTriangles); //jak mówi nazwa
    //generuje teren o wymiarach gridSize x gridSize przy użyciu mapy wysokości o rozmiarze n x n
    static Object* GenerateTerrain(short int heights[], int n, int gridSize);
    static Object* GenerateRandomTerrain(int gridSize); //dla celów testowych
};
void Object :: GenerateBuffers(){ //przesyła informacje o wierzchołkach do karty graficznej
    if(buffersInitialized) //informacje mogą zostać przesłane tylko raz
        return;
    buffersInitialized = true;
    
    //stwórz bufor pozycji wierzchołków
    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * nrOfVertices, vertices, GL_STATIC_DRAW);
    
    //stwórz bufor kolorów wierzchołków
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * nrOfVertices, colors, GL_STATIC_DRAW);
}
Object :: Object(int nrOfElements){
    vertices = new float[nrOfElements];
    colors = new float[nrOfElements];
    buffersInitialized = false;
}
/************************************************************************************/

/************************************************************************************/
//FUNKCJE RYSUJĄCE OBIEKTY NA EKRANIE
void Object :: DrawBeginEnd(){
    glBegin(drawingMode);
    
    for(int i = 0; i < nrOfVertices; ++i){ //narysuj każdy wierzchołek
        glColor3f(colors[3 * i], colors[3 * i + 1], colors[3 * i + 2]);
        glVertex3f(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]);
    }
    
    glEnd();
}
void Object :: DrawVBO(){
    GenerateBuffers(); //generujemy bufory, jeśli jeszcze nie wygenerowane
    
    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glColorPointer(3, GL_FLOAT, 0, NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
    
	glDrawArrays(drawingMode, 0, nrOfVertices);
	
    glBindBuffer(GL_ARRAY_BUFFER, 0); //zwalniamy binding bufora (ważne!)
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
void Object :: DrawVA(){
    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glColorPointer(3, GL_FLOAT, 0, colors);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(drawingMode, 0, nrOfVertices);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
/************************************************************************************/

/************************************************************************************/
//FUNKCJE TWORZĄCE OBIEKTY
//tworzy losowe trójkąty w przestrzeni
Object* Object :: GenerateRandomTriangles(int nrOfTriangles){
    Object* result = new Object(3 * 3 * nrOfTriangles); //wynikiem będzie pewien obiekt składający się z trójkątów
    result -> drawingMode = GL_TRIANGLES;
    result -> nrOfVertices = 3 * nrOfTriangles;
    
    for(int i = 0; i < nrOfTriangles; ++i){
        for(int j = 0; j < 3; ++j){
            for(int z = 0; z < 3; ++z){
                //losowane są zarówno kolory, jak i pozycje
                result -> colors[9 * i + 3 * j + z] = 1 - (rand() % 155) / 255.0;
                result -> vertices[9 * i + 3 * j + z] = (rand() % 100) - 50;
            }
        }
    }
    
    return result;
}

//dla celów testowych - generuje losowy teren
Object* Object :: GenerateRandomTerrain(int n){
    short int *heights = new short int[n * n];
    
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            heights[i + n * j] = rand() % 30;
        }
    }
    
    Object* obj = Object :: GenerateTerrain(heights, n, n);
    delete [] heights;
    return obj;
}

//najmniejsza wysokość terenu
const int minValue = -10;
//generuje teren
Object* Object :: GenerateTerrain(short int heights[], int n, int gridSize){
    int q = gridSize - 1;
    int nrOfQuads = q * q; //z ilu kwadratów będzie się składał model
    Object* result = new Object(3 * 4 * nrOfQuads);
    result -> drawingMode = GL_QUADS;
    result -> nrOfVertices = 4 * nrOfQuads;
    
    //obliczanie amplitudy wysokości
    int maxH = minValue;
    int minH = 32000;
    for(int i = 0; i < gridSize; ++i){
        for(int j = 0; j < gridSize; ++j){
            maxH = heights[i + j * n] > maxH ? heights[i + j * n] : maxH;
            minH = heights[i + j * n] < minH ? heights[i + j * n] : minH;
        }
    }
    minH = minH > minValue ? minH : minValue;
    //cout << minH << " " <<maxH << endl;
    float* tmp1 = result -> vertices, *tmp2 = result -> colors; //pomocnicze wskaźniki
    for(int i = 0; i < q; ++i){
        for(int j = 0; j < q; ++j){ //i, j wyznaczają lewy dolny róg aktualnie egzaminowanego kwadratu
            for(int k = 0; k < 2; ++k){
                for(int m = 0; m < 2; ++m){
                    int l = (k + m) % 2;
                    
                    int h = heights[i + k + (j + l) * n];
                    //cout << h << endl;
                    h = h < minValue ? minValue : h; //przycinamy wysokość do sensownej wartości
                    //h *= 0.6; //skalowanie wysokości
                    
                    tmp1[0] = i + k;
                    tmp1[1] = h;
                    tmp1[2] = j + l;
                    tmp1 += 3;
                    
                    Point color = CalculateColor(h, maxH, minH);
                    tmp2[0] = color.x;
                    tmp2[1] = color.y;
                    tmp2[2] = color.z;
                    tmp2 += 3;
                }
            }
        }
    }
    
    return result;
}
Point Object :: CalculateColor(float height, float maxH, float minH){
    height -= minH; //wydobywa z cienia doliny
    //float c = log(4 * height / (maxH - minH));
    //float c = height / (maxH - minH);
    
    float amplitude = maxH - minH;
    float step = amplitude / 4;
    if(height < step){
        return Point(0, 0, height / step);
    }
    if(height < 2 * step){
        return Point(0, (height - step) / step, 1 - ((height - step) / step));
    }
    if(height < 3 * step){
        return Point((height - 2 * step) / step, 1 - ((height - 2 * step) / step), 0);
    }
    return Point(1, (height - 3 * step) / step, (height - 3 * step) / step);
    
    //return Point(c, c, c); //zwracamy pewien odcień szarości
}
/************************************************************************************/

/************************************************************************************/
//wczytuje plik w formacie PNM (z lub bez nagłówka)
void ParseFile(short int nums[], char fileName[], int n){
    FILE *in = fopen(fileName, "rb");

    char buf[1024];
    fgets(buf, 1000,  in);
    //printf("%s", buf);
    fgets(buf, 1000,  in);
    //printf("%s", buf);
    fgets(buf, 1000,  in);
    //printf("%s", buf);
 
    fread(nums, 2, n * n, in);
    fclose(in);

    //odwraca kolejność bitów
    char *ptr = (char*)nums;
    for (int i = 0; i < n * n; i++) {
       char tmp   = ptr[2 * i];
       ptr[2 * i]   = ptr[2 * i + 1];
       ptr[2 * i + 1] = tmp;
       //cout << nums[i] << endl;
    }
    
}
/************************************************************************************/

enum Mode {BEGIN_END = 0, VBO, VA}; //możliwe tryby rysowania
char* modes[] = {"Begin end", "VBO", "VA"}; //i ich nazwy

int main(void){
    srand(time(NULL));
    
    /********************************************/
    //inicjalizacja grafiki
    int running = GL_TRUE;
    if(!glfwInit()){
        exit( EXIT_FAILURE );
    }
    if(!glfwOpenWindow(WIDTH, HEIGHT, 0,0,0,0,0,0, GLFW_WINDOW)){
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    glewInit();
    glEnable(GL_DEPTH_TEST); //włącz bufor głębokości
    glDepthFunc(GL_LEQUAL); 
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwDisable(GLFW_MOUSE_CURSOR);
    /********************************************/
    
    //diff to różnica pomiędzy dwoma ostatnimi klatkami, zaś last to czas jaki upłynął od włączenia aplikacji do poprzedniej klatki
    double diff = 0, last = glfwGetTime();
    
    Camera camera(true);
    
    /********************************************/
    //tworzenie obiektów
    const int n = 1000;
    short int *nums = new short int[n * n];
    ParseFile(nums, "tmp", n);
    Object* terrain = Object :: GenerateTerrain(nums, n, 300); //utworzenie terenu
    //Object* terrain = Object :: GenerateRandomTerrain(300);
    Object* bush = Object :: GenerateRandomTriangles(1000); //utworzenie "krzaka"
    delete [] nums;
    /********************************************/
    
    /********************************************/
    //zmienne dotyczące trybu rysowania / widoczności obiektów
    bool showTerrain = true, showBush = true; //czy teren/krzak są widoczne
    bool bPressed = false, tPressed = false, mPressed = false; //stan klawiszy B, T, M
    Mode mode = VBO; //aktualny tryb rysowania
    /********************************************/
    
    /********************************************/
    //zmienne pomocnicze dla obliczania FPS
    int num = 0;
    double sum = 0;
    /********************************************/
    
    while(running){        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        double current = glfwGetTime();
        diff = current - last; //ustal deltę czasu
        last = current;
        
        camera.Update(diff); //update kamery
        
        /********************************************/
        //co sto klatek wypisuje aktualny FPS
        if(num == 100){
            std :: cout << 100 / sum << std :: endl;
            num = sum = 0;
        }
        sum += diff;
        ++num;
        /********************************************/
        
        /********************************************/
        //obsługa klawiszy
        glMatrixMode(GL_MODELVIEW);
        if(!bPressed && glfwGetKey('B') == GLFW_PRESS){
            showBush = !showBush;
            bPressed = true;
            cout << "Bush visibility toggled" << endl;
        }
        if(glfwGetKey('B') == GLFW_RELEASE){
            bPressed = false;
        }
        
        if(!tPressed && glfwGetKey('T') == GLFW_PRESS){
            showTerrain = !showTerrain;
            tPressed = true;
            cout << "Terrain visibility toggled" << endl;
        }
        if(glfwGetKey('T') == GLFW_RELEASE){
            tPressed = false;
        }
        
        if(!mPressed && glfwGetKey('M') == GLFW_PRESS){
            mode = (Mode)((mode + 1) % 3);
            mPressed = true;
            cout << "Drawing mode toggled. \nUsing: " << modes[mode] <<  endl;
        }
        if(glfwGetKey('M') == GLFW_RELEASE){
            mPressed = false;
        }
        /********************************************/
        
        
        /********************************************/
        //rendering
        if(showBush){
            glLoadIdentity();
            glTranslatef(150, 80, 150); //przesuwamy krzak na środek terenu
            switch(mode){
                case BEGIN_END:
                    bush -> DrawBeginEnd();
                    break;
                case VA:
                    bush -> DrawVA();
                    break;
                case VBO:
                    bush -> DrawVBO();
                    break;
            }
        }
        if(showTerrain){
            glLoadIdentity();
            switch(mode){
                case BEGIN_END:
                    terrain -> DrawBeginEnd();
                    break;
                case VA:
                    terrain -> DrawVA();
                    break;
                case VBO:
                    terrain -> DrawVBO();
                    break;
            }
        }
        /********************************************/
        
        glfwSwapBuffers();
        running = !glfwGetKey( GLFW_KEY_ESC ) &&
        glfwGetWindowParam( GLFW_OPENED );
    }
    
    delete bush; //czyszczenie
    delete terrain;
    glfwTerminate();
    return 0;
}
/************************************************************************************/
