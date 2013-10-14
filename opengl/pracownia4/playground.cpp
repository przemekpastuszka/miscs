/*
Zadanie na pracownię numer 5 - Grafika Komputerowa
Code by Przemysław Pastuszka
*/
#include <GL/glfw.h>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <list>
#include <set>
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
float cubeVerts[] = {-1.0f, -1.0f,  1.0f, 1.0f, -1.0f, 1.0f,1.0f,  1.0f,  1.0f,-1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,-1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f,-1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f, 
        -1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f,-1.0f,  1.0f,  1.0f,-1.0f,  1.0f, -1.0f
        };

//struktura opisująca sześcian
struct Cube{
    private:
    float* vertices;
    int nrOfVertices;
    int drawingMode;

    public:
    Point translation;
    Point color;
    
    Cube(){
        vertices = cubeVerts;
        nrOfVertices = 24;
        drawingMode = GL_QUADS;
        color = Point(1, 1, 1);
    }
    
    void Draw(); //rysuje obiekt
};
void Cube :: Draw(){ //rysowanie sześcianu za pomocą VA - nic ciekawego się tu nie dzieje
    glPushMatrix();
    glTranslatef(translation.x, translation.y, translation.z);
    
    glColor3f(color.x, color.y, color.z);
        
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(drawingMode, 0, nrOfVertices);
    
    glPopMatrix();
}
/************************************************************************************/

/************************************************************************************/
//klasa abstrakcyjna będąca bazą dla brył otaczających
class BoundingVolume{
    protected:
    enum VolumeType {BBOX, FRUSTUM}; //dostępne 2 bryły otaczające - AABB i frustum
    
    public:
    VolumeType type;
    virtual bool Intersects(BoundingVolume*) = 0; //najważniejszym zadaniem brył jest sprawdzanie przecięć między sobą
};

//Axis Aligned Bounding Box
class AABB : public BoundingVolume{
    private:
    Point upperRight, lowerLeft; //prawy górny i lewy dolny róg
    
    public:
    AABB(){
        type = BBOX;
    }
    AABB(Cube* c){ //można stworzyć bryłę otaczającą na podstawie sześcianu o dł. krawędzi 2
        type = BBOX;
        upperRight = Point(1, 1, 1) + c -> translation;
        lowerLeft = Point(-1, -1, -1) + c -> translation;
    }
    AABB(const Point& ur, const Point& ll) : upperRight(ur), lowerLeft(ll){
        type = BBOX;
    }
    
    bool Intersects(BoundingVolume* volume);
    
    //akcesory
    Point Maximum() const {return upperRight;}
    Point Minimum() const {return lowerLeft;}
};
bool AABB :: Intersects(BoundingVolume* volume){
    if(volume -> type == FRUSTUM){
        return volume -> Intersects(this);
    }
    if(volume -> type == BBOX){
        AABB* box = (AABB*)volume;
        return (lowerLeft.x < box -> Maximum().x && upperRight.x > box -> Minimum().x
            && lowerLeft.y < box -> Maximum().y && upperRight.y > box -> Minimum().y
            && lowerLeft.z < box -> Maximum().z && upperRight.z > box -> Minimum().z);
    }
    return false;
}

//klasa reprezentująca płaszczyznę daną równaniem ax + by + cz + d = 0 
class Plane{
    public:
    float a, b, c, d;
    
    Plane(float A, float B, float C, float D) : a(A), b(B), c(C), d(D){}
    
    //iloczyn skalarny - oblicza odległość punktu od płaszczyzny (o ile ta została znormalizowana)
    float Dot(const Point& p){
        return a * p.x + b * p.y + c * p.z + d;
    }
    void Normalize(){ //normalizacja płaszczyzny
        float length = sqrt(a * a + b * b + c * c);
        a /= length;
        b /= length;
        c /= length;
        d /= length;
    }
};

//frustum, czyli ścięta piramida reprezentująca pole widzenia kamery
class Frustum : public BoundingVolume{
    private:
    Plane** planes; //opisana jest sześcioma płaszczyznami
    
    public:
    Frustum(float* m);
    ~Frustum(){
        for(int i = 0; i < 6; ++i){
            if(planes[i] != NULL){
                delete planes[i];
            }
        }
        delete [] planes;
    }
    bool Intersects(BoundingVolume*);
};
bool Frustum :: Intersects(BoundingVolume* volume){
    if(volume -> type == FRUSTUM){ //program nie obsługuje przecięć typu frustum - frustum
        cout << "Frustum cannot be intersected with another frustum" << endl;
        return false;
    }
    if(volume -> type == BBOX){ //przecięcie frustum - AABB
        AABB* box = (AABB*)volume;
        
        //testujemy czy prostopadłościan leży po odpowiedniej stronie każdej z płaszczyzn
        for(int i = 0; i < 6; ++i){
            //w zależności od ustawienia płaszczyzny dobieramy wierzchołek p prostopadłościanu, który będziemy testować
            Point p = box -> Minimum();
            if(planes[i] -> a >= 0){
                p.x = box -> Maximum().x;
            }
            if(planes[i] -> b >= 0){
                p.y = box -> Maximum().y;
            }
            if(planes[i] -> c >= 0){
                p.z = box -> Maximum().z;
            }
            //jeśli odległość od płaszczyzny < 0 to znaczy, że punkt leży "za" nią - zatem prostopadłościan jest poza widokiem
            if(planes[i] -> Dot(p) < 0){
                return false;
            }
        }
        return true;
    }
    return false;
}
Frustum :: Frustum(float* m){
    type = FRUSTUM;
    planes = new Plane*[6];
    //na podstawie macierzy m = macierz widoku * macierz projekcji tworzę sześć płaszczyzn
    //poniższe wzory znalazłem w sieci
    planes[0] = new Plane(m[0 * 4 + 3] + m[0 * 4 + 0], m[1 * 4 + 3] + m[1 * 4 + 0], m[2 * 4 + 3] + m[2 * 4 + 0], m[3 * 4 + 3] + m[3 * 4 + 0]);
    planes[1] = new Plane(m[0 * 4 + 3] - m[0 * 4 + 0], m[1 * 4 + 3] - m[1 * 4 + 0], m[2 * 4 + 3] - m[2 * 4 + 0], m[3 * 4 + 3] - m[3 * 4 + 0]);
    planes[2] = new Plane(m[0 * 4 + 3] - m[0 * 4 + 1], m[1 * 4 + 3] - m[1 * 4 + 1], m[2 * 4 + 3] - m[2 * 4 + 1], m[3 * 4 + 3] - m[3 * 4 + 1]);
    planes[3] = new Plane(m[0 * 4 + 3] + m[0 * 4 + 1], m[1 * 4 + 3] + m[1 * 4 + 1], m[2 * 4 + 3] + m[2 * 4 + 1], m[3 * 4 + 3] + m[3 * 4 + 1]);
    planes[4] = new Plane(m[0 * 4 + 2], m[1 * 4 + 2], m[2 * 4 + 2], m[3 * 4 + 2]);
    planes[5] = new Plane(m[0 * 4 + 3] - m[0 * 4 + 2], m[1 * 4 + 3] - m[1 * 4 + 2], m[2 * 4 + 3] - m[2 * 4 + 2], m[3 * 4 + 3] - m[3 * 4 + 2]);
    
    //płaszczyzny zostają znormalizowane
    for(int i = 0; i < 6; ++i){
        planes[i] -> Normalize();
    }
}
/************************************************************************************/

/************************************************************************************/
//drzewo ósemkowe
class Octree{
    private:
    struct Node{ //poddrzewo drzewa ósemkowego
        AABB* bbox; //jego bryła otaczająca
        bool isLeaf; //czy jest liściem
        Octree :: Node **children; //osiem poddrzew (jeśli nie jest liściem)
        list<Cube*>* objects; //obiekty (jeśli jest liściem)
        
        //głębokość drzewa, "promień" i środek w przestrzeni
        Node(int depth, float size, const Point& center);
        ~Node();
        
        void FindIntersectingObjects(BoundingVolume* volume, set<Cube*>* set);
        void Add(Cube*, BoundingVolume*);
    };
    
    Node* root; //korzeń drzewa
    
    public:
    Octree(int depth, float size){
        root = new Node(depth, size, Point(0, 0, 0));
    }
    ~Octree(){
        delete root;
    }
    
    void FindIntersectingObjects(BoundingVolume* volume, set<Cube*>* set){
        root -> FindIntersectingObjects(volume, set);
    }
    void Add(Cube* c){
        AABB volume(c);
        root -> Add(c, &volume);
    }
};
Octree :: Node :: Node(int depth, float size, const Point& center){
    bbox = new AABB(Point(size, size, size) + center, Point(-size, -size, -size) + center);
    if(depth > 0){
        isLeaf = false;
        children = new Node*[8];
        for(int i = 0; i < 8; ++i){ //kreacja ośmiu poddrzew
            int a = (i & 1) * 2 - 1; //i = 4c + 2b + a 
            int b = ((i >> 1) & 1) * 2 - 1; //innymi słowy - c b a jest binarną reprezentacją i
            int c = ((i >> 2) & 1) * 2 - 1;
            Point n = (size / 2) * Point(a, b, c); //przesunięcie środka dziecka względem środka ojca
            children[i] = new Node(depth - 1, size / 2, n + center);
        }
    }
    else{
        isLeaf = true;
        objects = new list<Cube*>();
    }
}
Octree :: Node :: ~Node(){
    delete bbox;
    if(!isLeaf){
        for(int i = 0; i < 8; ++i){
            delete children[i];
        }
        delete [] children;
    }
    else{
        delete objects;
    }
}
void Octree :: Node :: Add(Cube* c, BoundingVolume* volume){ //dodaje sześcian do drzewa
    if(isLeaf){
        objects -> push_back(c);
        return;
    }
    for(int i = 0; i < 8; ++i){
        if(volume -> Intersects(children[i] -> bbox)){
            children[i] -> Add(c, volume);
        }
    }
}
//dodaje do zbioru sześcianów te, które przecinają się z volume
void Octree :: Node :: FindIntersectingObjects(BoundingVolume* volume, set<Cube*>* set){
    if(isLeaf){ 
        for(list<Cube*> :: iterator it = objects -> begin(); it != objects-> end(); it++){
        //dodaję wszystkie obiekty, które znajdują się w liściu - dodatkowe sprawdzenie powodowało spadek wydajności
          //  AABB c(*it);
          //  if(volume -> Intersects(&c)){
                set -> insert(*it);
           // }
        }
        return;
    }
    
    for(int i = 0; i < 8; ++i){
        if(volume -> Intersects(children[i] -> bbox)){
            children[i] -> FindIntersectingObjects(volume, set);
        }
    }
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
    
    void SetLookAt();
    void SetProjection();
    
    public:
    Camera(){
        glfwGetMousePos(&lastMouseX, &lastMouseY);
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
    
    Frustum* Update(double time);
};
void Camera :: SetLookAt(){
    Point lookAt = position + lookDir; 
    gluLookAt(position.x, position.y, position.z, lookAt.x, lookAt.y, lookAt.z,
        up.x, up.y, up.z);
}
void Camera :: SetProjection(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, WIDTH / HEIGHT, 0.2, 2100);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
Frustum* Camera :: Update(double time){

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
    
    GLfloat m[16];
    glGetFloatv(GL_PROJECTION_MATRIX, m); //m to macierz widoku * macierz projekcji
    return new Frustum(m);
}
/************************************************************************************/

//generuje losowe sześciany
void GenerateCubes(int nrOfCubes, int genSize, list<Cube*>* cubes){
    for(int i = 0; i < nrOfCubes; ++i){
        Cube* tmp = new Cube();
        tmp -> translation = Point(-genSize + rand() % (2 * genSize), -genSize + rand() % (2 * genSize), -genSize + rand() % (2 * genSize));
        tmp -> color = Point((100 + rand() % 155) / (255.0), (100 + rand() % 155) / (255.0), (100 + rand() % 155) / (255.0));
        cubes -> push_back(tmp);
    }
}

int main( void ){
    srand(time(NULL));
    
    int running = GL_TRUE;
    if(!glfwInit()){
        exit( EXIT_FAILURE );
    }
    
    if(!glfwOpenWindow(WIDTH, HEIGHT, 0,0,0,0,0,0, GLFW_WINDOW)){
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    
    /********************************************/
    glEnable(GL_DEPTH_TEST); //włącz bufor głębokości
    glDepthFunc(GL_LEQUAL); 
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwDisable(GLFW_MOUSE_CURSOR);
    /********************************************/

    //diff to różnica pomiędzy dwoma ostatnimi klatkami, zaś last to czas jaki upłynął od włączenia aplikacji do poprzedniej klatki
    double diff = 0, last = glfwGetTime();

    /********************************************/
    //zmienne pomocnicze dla obliczania FPS
    int num = 0;
    double sum = 0;
    /********************************************/
    
    /********************************************/
    const int depth = 4; //głębokość drzewa ósemkowego
    const int size = 1024; //w takim "promieniu" generowane będą sześciany
    int nrOfCubes = 40000; //ile sześcianów wygenerować
    Octree tree(depth, size);
    list<Cube*>* cubes = new list<Cube*>();
    GenerateCubes(nrOfCubes, size, cubes);
    for(list<Cube*> :: iterator it = cubes -> begin(); it != cubes -> end(); it++){
        tree.Add(*it);
    }
    set<Cube*>* toDraw = new set<Cube*>();
    /********************************************/
    
    /********************************************/
    //zmienne dotyczące trybu rysowania / widoczności obiektów
    bool useFrustum = true;
    bool fPressed = false;
    /********************************************/
    
    Camera camera;
    while(running){        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        double current = glfwGetTime();
        diff = current - last; //ustal deltę czasu
        last = current;
        
        Frustum* frustum = camera.Update(diff); //update kamery
        
        /********************************************/
        //co sto klatek wypisuje aktualny FPS i liczbę wyświetlanych obiektów
        if(num == 100){
            system("clear");
            if(useFrustum){
                cout << "Using frustum culling" << endl;
            }
            else{
                cout << "Frustum culling off" << endl;
            }
            std :: cout << "FPS: " << 100 / sum << std :: endl;
            std :: cout << "Objects: " << toDraw -> size() << std :: endl;
            num = sum = 0;
        }
        sum += diff;
        ++num;
        /********************************************/
        
        if(!fPressed && glfwGetKey('F') == GLFW_PRESS){
            useFrustum = !useFrustum;
            fPressed = true;
        }
        if(glfwGetKey('F') == GLFW_RELEASE){
            fPressed = false;
        }
        
        //rysuj z użyciem drzewa ósemkowego
        if(useFrustum){
            toDraw -> clear();
            tree.FindIntersectingObjects(frustum, toDraw);
            for(set<Cube*> :: iterator it = toDraw -> begin(); it != toDraw -> end(); it++){
                (*it) -> Draw();
            }
        }
        else{
            for(list<Cube*> :: iterator it = cubes -> begin(); it != cubes -> end(); it++){
                (*it) -> Draw();
            }
        }
        delete frustum;
        
        glfwSwapBuffers();
        running = !glfwGetKey( GLFW_KEY_ESC ) &&
        glfwGetWindowParam( GLFW_OPENED );
    }
    
    for(list<Cube*> :: iterator it = cubes -> begin(); it != cubes -> end(); it++){
        delete *it;
    }
    delete cubes;
    delete toDraw;
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
/************************************************************************************/
