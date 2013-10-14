/*
Zadanie na pracownię numer 2 - Grafika Komputerowa
Code by Przemysław Pastuszka
*/
#include <GL/glfw.h>
#include <cstdlib>
#include <algorithm>
#include <iostream>

//rozmiar okna
const float WIDTH = 1024;
const float HEIGHT = 768;

/************************************************************************************/
//Przechowuje trójkę floatów (może być interpretowane jako punkt lub wektor w 3d)
struct Point{
    float x, y, z;
    Point() : x(0), y(0), z(0) {}
    Point(float r, float g, float b) : x(r), y(g), z(b) {}
    
    //iloczyn wektorowy
    static Point CrossProduct(const Point& a, const Point& b){
        return Point(a.y * b.z - a.z * b.y, -a.x * b.z + a.z * b.x, a.x * b.y - a.y * b.x);
    }
    
    friend const Point operator+(const Point& a, const Point& b);
};
//dodawanie dwóch wektorów
const Point operator+(const Point& a, const Point& b){
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}
/************************************************************************************/

/************************************************************************************/
//przechowuje podstawowe informacje o modelu (wierzchołki, ich liczbę oraz sposób rysowania)
struct ObjectModel{
    float* vertices;
    //float* normals;
    int nrOfVertices;
    //int nrOfFaces;
    int drawingMode; //GL_LINES, GL_TRIANGLES and so on
    
    ObjectModel() : vertices(NULL), nrOfVertices(0) {}
    ObjectModel(float* verts, int nr, int mode) : vertices(verts), nrOfVertices(nr), drawingMode(mode) {} 
};

//klasa abstrakcyjna opisująca konkretną instancję obiektu
struct Object{
    //umiejscowienie w przestrzeni opisane skalowaniem obiektu, translacją i rotacją
    Point scale, translation, rotation;
    
    void ScaleAndTranslate(); //skaluje, przesuwa i obraca (rotacja gdzieś się zagubiła i nie ma jej w nazwie)
    virtual void Draw() = 0; //rysuje obiekt
};
void Object :: ScaleAndTranslate(){
    glTranslatef(translation.x, translation.y, translation.z);
    glScalef(scale.x, scale.y, scale.z);
    
    //zmienna rotation przechowuje wartość obrotu w stopniach dla poszczególnych osi
    //należy tę informację odpowiednio przygotować dla funkcji glRotatef, co czynione jest poniżej
    float maxAngle = std :: max(rotation.x, std :: max(rotation.y, rotation.z));
    if(maxAngle != 0)
        glRotatef(maxAngle, rotation.x / maxAngle, rotation.y / maxAngle, rotation.z / maxAngle);
}

//prosty obiekt to po prostu pokolorowany model umiejscowiony w przestrzeni
struct SimpleObject : public Object{
    ObjectModel* model;
    Point color;
    bool wireframe; //czy rysować tylko siatkę zamiast wypełnienia kolorem
    
    //duuużo konstruktorów (ułatwiają życie przy tworzeniu obiektów)
    SimpleObject() : wireframe(false), model(NULL), color(1, 1, 1){
        scale = Point(1, 1, 1);
    } 
    SimpleObject(ObjectModel* m, Point c) : color(c), wireframe(false), model(m){
        scale = Point(1, 1, 1);
    }
    SimpleObject(ObjectModel* m, Point c, Point s) : color(c), wireframe(false), model(m){
        scale = s;
    }
    SimpleObject(ObjectModel* m, Point c, Point s, Point t) : color(c), wireframe(false), model(m){
        scale = s;
        translation = t;
    }
    SimpleObject(ObjectModel* m, Point c, Point s, Point t, Point r) : color(c), wireframe(false), model(m){
        scale = s;
        translation = t;
        rotation = r;
    }
    SimpleObject(ObjectModel* m, Point c, Point s, Point t, Point r, bool w) : color(c), wireframe(w), model(m){
        scale = s;
        translation = t;
        rotation = r;
    }
    void Draw();
};
void SimpleObject :: Draw(){ //rysowanie prostego obiektu - nic ciekawego się tu nie dzieje
    glPushMatrix();
    ScaleAndTranslate(); //przeskaluj, obróć i przesuń
    
    glColor3f(color.x, color.y, color.z);
    //float color[] = {obj -> color.x, obj -> color.y, obj -> color.z, 1};
    //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);

    if(wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
    glVertexPointer(3, GL_FLOAT, 0, model -> vertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    //glNormalPointer(GL_FLOAT, 0, obj -> model -> nomals);
    //glEnableClientState(GL_NORMAL_ARRAY);
    glDrawArrays(model -> drawingMode, 0, model -> nrOfVertices);
    
    glPopMatrix();
}

//obiekt złożony z innych obiektów (prostych lub również złożonych)
struct ComplexObject : public Object{
    int nrOfObjects;
    Object** objects;

    //tworzy tablicę na wskaźniki do obiektów i ustawia standardowe współczynniki skalowania
    ComplexObject(unsigned int k){
        scale = Point(1, 1, 1);
        nrOfObjects = k;
        objects = new Object*[k];
    }
    ~ComplexObject(){ //usuwa utworzoną wcześniej tablicę
        delete [] objects;
    }
    
    void Draw();
};
void ComplexObject :: Draw(){
    glPushMatrix();
    
    ScaleAndTranslate(); //przesuń, obróć i przeskaluj dla wszystkich obiektów w objects
    for(int i = 0; i < nrOfObjects; ++i) //narysuj wszystkie obiekty z object
        objects[i] -> Draw();
        
    glPopMatrix();
}
/************************************************************************************/

/************************************************************************************/
//model sześcianu (najczęściej wykorzystywany)
float cubeVerts[] = {-1.0f, -1.0f,  1.0f, 1.0f, -1.0f, 1.0f,1.0f,  1.0f,  1.0f,-1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,-1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f,-1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f, 
        -1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f,-1.0f,  1.0f,  1.0f,-1.0f,  1.0f, -1.0f
        };
ObjectModel cube(cubeVerts, 24, GL_QUADS);
//float cubeNormals[] = {0, 0, 1,  0, 0, -1,  0, 1, 0,  0, -1, 0,  1, 0, 0,  -1, 0, 0};
/************************************************************************************/

/************************************************************************************/
//model płaszczyzny
float planeVerts[] = {-1.0f, -1.0f,  1.0f, 1.0f, -1.0f, 1.0f,1.0f,  1.0f,  1.0f,-1.0f,  1.0f,  1.0f};
ObjectModel planeModel(planeVerts, 4, GL_QUADS);
/************************************************************************************/

/************************************************************************************/
//model toru, czyli po prostu linii
float railVerts[] = {-1, 0, 0,  1, 0, 0 };
ObjectModel railModel(railVerts, 2, GL_LINES);
/************************************************************************************/

/************************************************************************************/
//drzewko
SimpleObject trunk(&cube, Point(0.62, 0.32, 0.17), Point(0.3, 1, 0.3));
SimpleObject leafs(&cube, Point(0, 1, 0), Point(1, 1, 1), Point(0, 2, 0));
struct Tree : public ComplexObject{
    Tree(Point s, Point t) : ComplexObject(2){
        objects[0] = &trunk;
        objects[1] = &leafs;
        translation = t;
        scale = s;
    }
};
/************************************************************************************/

/************************************************************************************/
//człowiek
SimpleObject head(&cube, Point(1, 0, 0), Point(0.3, 0.3, 0.25), Point(0, 1.9, 0));
SimpleObject body(&cube, Point(0, 0, 1), Point(0.6, 1, 0.3), Point(0, 0.3, 0));
SimpleObject larm(&cube, Point(0, 1, 0), Point(0.1, 0.8, 0.18), Point(-0.8, 0.5, 0));
SimpleObject rarm(&cube, Point(0, 1, 0), Point(0.1, 0.8, 0.18), Point(0.8, 0.5, 0));
SimpleObject lleg(&cube, Point(0, 1, 0), Point(0.15, 0.7, 0.2), Point(-0.3, -1.5, 0));
SimpleObject rleg(&cube, Point(0, 1, 0), Point(0.15, 0.7, 0.2), Point(0.3, -1.5, 0));
struct Dummy : public ComplexObject{
    double direction; //kierunek, w którym aktualnie się porusza wzdłuż osi X (-1 lub 1)
    double speed; //prędkość
    
    Dummy(Point s, Point t, Point r) : ComplexObject(6), direction(1), speed(2){
        objects[0] = &head;
        objects[1] = &body;
        objects[2] = &larm;
        objects[3] = &rarm;
        objects[4] = &lleg;
        objects[5] = &rleg;
        scale = s;
        translation = t;
        rotation = r;
    }
    //funkcja pomocnicza przycinająca x do przedziału [m, M]
    double Clamp(double x, double m, double M){
        return std :: min(M, std :: max(m, x));
    }
    void Update(double time);
};
void Dummy :: Update(double time){
    translation.x += time * speed * direction; //przesuń
    if(translation.x > 1.5 || translation.x < -1.5) //jeśli zaszedł za daleko to zmień kierunek
        direction *= -1;
    translation.x = Clamp(translation.x, -1.5, 1.5); //przytnij do dozwolonego przedziału
}
/************************************************************************************/

/************************************************************************************/
//"Kawałek" terenu - tj. płaszczyzna + tory + drzewka
SimpleObject railOne(&railModel, Point(1, 1, 1), Point(5, 1, 1), Point(0, -4.98, 0.4));
SimpleObject railTwo(&railModel, Point(1, 1, 1), Point(5, 1, 1), Point(0, -4.98, -0.4));
SimpleObject plane(&planeModel, Point(0, 0.6, 0), Point(5, 5, 5), Point(0, 0, 0), Point(90, 0, 0));
Tree tree1(Point (0.6, 0.6, 0.6), Point(-4, -4.5, 4)); 
Tree tree2(Point (0.6, 0.6, 0.6), Point(0, -4.5, 4)); 
Tree tree3(Point (0.6, 0.6, 0.6), Point(-4, -4.5, -4)); 
Tree tree4(Point (0.6, 0.6, 0.6), Point(0, -4.5, -4));
struct TerrainPatch : public ComplexObject{
    TerrainPatch(Point t) : ComplexObject(7){
        objects[0] = &plane;
        objects[1] = &tree1;
        objects[2] = &tree2;
        objects[3] = &tree3;
        objects[4] = &tree4;
        objects[5] = &railOne;
        objects[6] = &railTwo;
        translation = t;
    }
};
/************************************************************************************/

/************************************************************************************/
//pociąg
Dummy passenger(Point(0.4, 0.4, 0.4), Point(0, -0.3, 0), Point(0, 90, 0));
SimpleObject trainPart1(&cube, Point(1, 0, 0), Point(2, 1, 1), Point(-5, 0, 0));
SimpleObject trainPart2(&cube, Point(1, 0, 0), Point(2, 1, 1), Point(0, 0, 0), Point(0, 0, 0), true);
SimpleObject trainPart3(&cube, Point(1, 0, 0), Point(2, 1, 1), Point(5, 0, 0));
struct Train : public ComplexObject{
    double totalTime; //całkowity czas jaki upłynął od startu pociągu
    bool update; //czy pociąg jedzie
    Train(Point s, Point t) : ComplexObject(4), totalTime(0), update(false){
        objects[0] = &passenger;
        objects[1] = &trainPart1;
        objects[2] = &trainPart2;
        objects[3] = &trainPart3;
        scale = s;
        translation = t;
    }
    void Update(double time);
};
void Train :: Update(double time){
    if(update){ //jeśli pociąg jedzie
        totalTime += time; //to zwiększ całkowity czas
        //i oblicz pozycję ze wzoru na drogę w ruchu jednostajnie przyspieszonym
        translation.x = 10 + 0.3 * totalTime * totalTime; //0.3 to wartość przyspieszenia
    }
}
/************************************************************************************/

/************************************************************************************/
//Kompletny świat
TerrainPatch patches[] = { //całkowity teren składa się z pięciu kawałków
    TerrainPatch(Point(0, 0, 0)),
    TerrainPatch(Point(10, 0, 0)),
    TerrainPatch(Point(20, 0, 0)),
    TerrainPatch(Point(30, 0, 0)),
    TerrainPatch(Point(40, 0, 0)),
    TerrainPatch(Point(50, 0, 0))
};
Train train(Point(0.8, 0.8, 0.8), Point(10, -4, 0));
struct World : public ComplexObject{
    int last; //nr kawałka terenu, który jest na końcu (tj. w najbliższym czasie zniknie on z ekranu i zostanie przeniesiony do przodu)
    double pos; //jaka była pozycja pociągu podczas ostatniego przeniesienia kawałka terenu
    World() : ComplexObject(7), last(0), pos(train.translation.x){
        objects[0] = &patches[0];
        objects[1] = &patches[1];
        objects[2] = &patches[2];
        objects[3] = &patches[3];
        objects[4] = &patches[4];
        objects[5] = &patches[5];
        objects[6] = &train;
    }
    void Update();
};
void World :: Update(){
    if(train.translation.x - pos > 10){ //pociąg przejechał 10 jednostek od ostatniego przeniesienia
        pos = train.translation.x;
        objects[last] -> translation.x += 60; //przenieś ostatni kawałek terenu
        last = (last + 1) % 6; //wyznacz nowy ostatni kawałek terenu
    }
}
/************************************************************************************/

/************************************************************************************/
//ustawia widok przy pomocy pozycji obserwatora, kierunku w którym patrzy i kierunku jego prawej strony
void SetLookAt(Point position, Point lookDir, Point right){
    Point up = Point :: CrossProduct(lookDir, right); //wyznacz "up" wektor
    Point lookAt = position + lookDir; //punkt, na który patrzy
    gluLookAt(position.x, position.y, position.z, lookAt.x, lookAt.y, lookAt.z,
        up.x, up.y, up.z);
}
/************************************************************************************/

/************************************************************************************/
int main( void ){
    int running = GL_TRUE;
    if(!glfwInit()){
        exit( EXIT_FAILURE );
    }
    
    if(!glfwOpenWindow(WIDTH, HEIGHT, 0,0,0,0,0,0, GLFW_WINDOW)){
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    float lightAmbient[] = {0.2, 0.2, 0.2, 1};
    float lightDiffuse[] = {1, 1, 1, 1};
    float lightPosition[] = {-1, 0, 0, 0};
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse); 
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    
    glShadeModel(GL_FLAT);*/
    
    glEnable(GL_DEPTH_TEST); //włącz bufor głębokości
    glDepthFunc(GL_LEQUAL); 

    //diff to różnica pomiędzy dwoma ostatnimi klatkami, zaś last to czas jaki upłynął od włączenia aplikacji do poprzedniej klatki
    double diff = 0, last = glfwGetTime();
    World w; //tworzymy świat
    while(running){        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //RÓB UPDATE
        if(glfwGetKey('A') == GLFW_PRESS) //"włącz" pociąg
            train.update = true;
        
        double current = glfwGetTime();
        diff = current - last; //ustal deltę czasu
        last = current;
        passenger.Update(diff); //update pasażera
        train.Update(diff); //update pociągu
        w.Update(); //update terenu
        //SKOŃCZ UPDATE
        
        /************************************************************************************/
        //widok z "boku"
        glViewport(0, 0, WIDTH, HEIGHT / 2);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45, WIDTH / (HEIGHT / 2), 0.2, 100);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        SetLookAt(Point(train.translation.x, 0, -14), Point(0, -0.2, 1), Point(1, 0, 0));
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //ZACZNIJ RYSOWAĆ
        w.Draw();
        //SKOŃCZ RYSOWAĆ
        /************************************************************************************/
        
        /************************************************************************************/
        //widok z przodu pociągu
        glViewport(0, HEIGHT / 2, WIDTH, HEIGHT / 2);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45, WIDTH / (HEIGHT / 2), 0.2, 100);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        SetLookAt(Point(train.translation.x, -0.8, 0), Point(1, 0, 0), Point(0, 0, -1));
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //ZACZNIJ RYSOWAĆ
        w.Draw();
        //SKOŃCZ RYSOWAĆ
        /************************************************************************************/
        
        glfwSwapBuffers();
        running = !glfwGetKey( GLFW_KEY_ESC ) &&
        glfwGetWindowParam( GLFW_OPENED );
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
/************************************************************************************/
