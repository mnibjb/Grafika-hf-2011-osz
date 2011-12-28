//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2011-tol.
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk.
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D,
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi,
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : MICHELBERGER TAMAS
// Neptun : CPIUX4
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#include <math.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// MsWindows-on ez is kell
#include <windows.h>
#endif // Win32 platform

#include <GL/gl.h>
#include <GL/glu.h>
// A GLUT-ot le kell tolteni: http://www.opengl.org/resources/libraries/glut/
#include <GL/glut.h>

#define new new_nelkul_is_meg_lehet_csinalni

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

const float PI = 3.14159265f;
const float maxLengthOfSnake = 0.4f;
const float minLengthOfSnake = 0.2f;
const float animationLengthInMillisec = 5000.0f;
const float elevatorLevels[] = {-0.99f, -0.33f, 0.33f, 1.0f};
const float SNAKELEVELS[] = {-0.925f, -0.265f, 0.4f};

enum Direction {LEFT, RIGHT};
enum VerticalState {STEADY, UP, DOWN};

bool gameOver = false;

bool fequal(float a, float b, float accuracy = 0.001f) {
    if (fabs(a-b) < accuracy) return true;
    return false;
}

class Elevator {
    private:
        float x;
        float y;
        VerticalState state;
        int currentLevel;
    public:
        float getX() { return x; }
        float getY() { return y; }
        float getLevel() { return currentLevel; }
        VerticalState getState() { return state; }
        void init (float x, float y, int level = 1) {
            this->x = x;
            this->y = y;
            currentLevel = level;
            state = STEADY;
        }

        static const float elevatorMovementDuration = 1000.0f;
        static const float width = 0.55;
        Elevator(){
            init(0.0, 0.0);
        }
        Elevator(float x, float y) {
            init(x,y);
        }
        void moveUp(){
            state = UP;

        }
        void moveDown() {
            state = DOWN;

        }
        void draw() {
            glBegin(GL_POLYGON);
                glVertex2f(x, y);
                glVertex2f(x, y-0.01f);
                glVertex2f(x+width, y-0.01f);
                glVertex2f(x+width, y);
            glEnd();
        }
        void animate(float dt){
            switch(state) {
                case STEADY:
                    break;
                case UP:
                    currentLevel = currentLevel == 3 ? 3 : currentLevel + 1;
                    y = elevatorLevels[currentLevel];
                    state = STEADY;
                    break;
                case DOWN:
                    currentLevel = currentLevel == 0 ? 0 : currentLevel - 1;
                    y = elevatorLevels[currentLevel];
                    state = STEADY;
                    break;
            }
        }
};

class Field {
    private:
        Elevator leftElevator;
        Elevator rightElevator;
    public:
        Field() {
            leftElevator.init(-0.7f, elevatorLevels[1],1);
            rightElevator.init(0.15f, elevatorLevels[2],2);
        }
        Elevator& getLeftElevator() {
            return leftElevator;
        }
        Elevator& getRightElevator() {
            return rightElevator;
        }
        void draw(){
            glColor3f(1,1,1);
            glBegin(GL_LINES);
                glVertex2f(-1, -0.33f);
                glVertex2f(-0.7f, -0.33f);
            glEnd();

            glBegin(GL_LINES);
                glVertex2f(-0.15, -0.33f);
                glVertex2f(0.15f, -0.33f);
            glEnd();

            glBegin(GL_LINES);
                glVertex2f(0.7, -0.33f);
                glVertex2f(1.0f, -0.33f);
            glEnd();

            glBegin(GL_LINES);
                glVertex2f(-1, 0.33f);
                glVertex2f(-0.7f, 0.33f);
            glEnd();

            glBegin(GL_LINES);
                glVertex2f(-0.15, 0.33f);
                glVertex2f(0.15f, 0.33f);
            glEnd();

            glBegin(GL_LINES);
                glVertex2f(0.7f, 0.33f);
                glVertex2f(1.0f, 0.33f);
            glEnd();

            leftElevator.draw();
            rightElevator.draw();
        }

        void animate(float dt) {
            leftElevator.animate(dt);
            rightElevator.animate(dt);
        }
} field;

class  AbstractSnake {
    private:
        float amplitude;
        float length;
        float xHead;
        float yHead;
        float k;
        float I;
        float animPos;
        Direction dir;
        int currLevel;
        VerticalState verticalState;

        void turn() {
            switch(dir) {
                case LEFT:
                    dir = RIGHT;
                    break;
                case RIGHT:
                    dir = LEFT;
                    break;
            }
            length = minLengthOfSnake;
        }

        void drawHeadLeft(float x, float y) {
            glColor3f(1,1,1);
            glBegin(GL_LINE_STRIP);
                glVertex2f(x, y);
                glVertex2f(x - 0.05f, y + 0.03f);
                glVertex2f(x - 0.1f, y);
                glVertex2f(x - 0.05f, y - 0.03f);
                glVertex2f(x, y);
            glEnd();
            colorit();
            glBegin(GL_TRIANGLES);
                glVertex2f(x, y);
                glVertex2f(x - 0.05f, y + 0.03f);
                glVertex2f(x - 0.05f, y - 0.03f);
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex2f(x - 0.1f, y);
                glVertex2f(x - 0.05f, y + 0.03f);
                glVertex2f(x - 0.05f, y - 0.03f);
            glEnd();

        }

        void drawHeadRight(float x, float y) {
            glColor3f(1,1,1);
            glBegin(GL_LINE_STRIP);
                glVertex2f(x, y);
                glVertex2f(x + 0.05f, y + 0.03f);
                glVertex2f(x + 0.1f, y);
                glVertex2f(x + 0.05f, y - 0.03f);
                glVertex2f(x, y);
            glEnd();
            colorit();
            glBegin(GL_TRIANGLES);
                glVertex2f(x, y);
                glVertex2f(x + 0.05f, y + 0.03f);
                glVertex2f(x + 0.05f, y - 0.03f);
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex2f(x + 0.1f, y);
                glVertex2f(x + 0.05f, y + 0.03f);
                glVertex2f(x + 0.05f, y - 0.03f);
            glEnd();
        }

        void liftUp() {
            if (verticalState != UP) return;
            if (currLevel == 2) die();
            yHead = SNAKELEVELS[++currLevel];
            verticalState = STEADY;
        }

        void liftDown() {
            if (verticalState != DOWN) return;
            currLevel = currLevel == 0 ? 0 : currLevel-1;
            yHead = SNAKELEVELS[currLevel];
            verticalState = STEADY;
        }


    protected:
        void virtual colorit() = 0;
    public:
        AbstractSnake(){
            amplitude = .05f;
            length = 0.5f;
            k = 3.0f;
            xHead = .5f;
            yHead = 0.0f;
            I = 2.0f;
            animPos = 0.0f;
            dir = LEFT;
            currLevel = 1;
        }

        void die() {
            gameOver = true;
        }

        float getxHead() { return xHead; }

        void setxHead(float x) { xHead = x; }

        void setVerticalState(VerticalState vs) { verticalState = vs; }

        void setPos(float x, float y, int level) { xHead = x; yHead = y; currLevel = level;}

        float getLength() { return length; }

        void setLength(float l) { length = l; }

        void draw(){
            colorit();
            glBegin(GL_LINE_STRIP);
            for (float x = 0.0; x < length; x += 0.01f) {
                float y = amplitude * sin((2*k*PI)/length*x);
                glVertex2f(xHead + x, yHead + y);
            }
            glEnd();

            switch (dir) {
                case LEFT:
                    drawHeadLeft(xHead, yHead);
                    break;
                case RIGHT:
                    drawHeadRight(xHead + length, yHead);
                    break;
            }
        }

        void animate(float dt) {
            switch(dir) {
                case LEFT :
                    if (animPos < 1000.0f && length >= minLengthOfSnake) {
                        length -= dt/animationLengthInMillisec;
                    } else if (animPos >= 1000.0f && length < maxLengthOfSnake) {
                        length += dt/animationLengthInMillisec;
                        xHead -= dt/animationLengthInMillisec;
                    } else {
                        animPos = 0.0f;
                    }
                    break;
                case RIGHT:
                    if (animPos < 1000.0f && length < maxLengthOfSnake) {
                        length += dt/animationLengthInMillisec;
                    } else if (animPos >= 1000.0f && length >= minLengthOfSnake) {
                        length -= dt/animationLengthInMillisec;
                        xHead += dt/animationLengthInMillisec;
                    } else {
                        animPos = 0.0f;
                    }
                    break;
            }
            float a = length*length > maxLengthOfSnake*maxLengthOfSnake ? maxLengthOfSnake*maxLengthOfSnake : length*length;
            amplitude = (sqrtf(maxLengthOfSnake*maxLengthOfSnake - a ) / 4);
            animPos += dt;
        }

        void control(float dt) {
            switch(dir) {
                case LEFT:
                    if (xHead + length + dt/animationLengthInMillisec < -1.0f) {
                        turn();
                    }
                break;
                case RIGHT:
                    if (xHead > 1.0f + dt/animationLengthInMillisec) {
                        turn();
                    }
                    break;
            }

            if (isAboveElevatorHole(field.getLeftElevator()) || isAboveElevatorHole(field.getRightElevator()))
                yHead = SNAKELEVELS[--currLevel];

            liftUp();
        }

        bool isAboveElevatorHole(Elevator& e) {
            switch(dir) {
                case LEFT:
                    if ((e.getX() < xHead - 0.1f) && (e.getX() + Elevator::width > xHead - 0.1f)) {
                        if ((e.getX() < xHead + length) && (e.getX()+Elevator::width > xHead+length)) {
                            if (currLevel != e.getLevel() && currLevel != 0) {
                                return true;
                            }
                        }
                    }
                    break;
                case RIGHT:
                    if ((e.getX() < xHead + length + 0.1f) && (e.getX() + Elevator::width > xHead + length + 0.1f)) {
                        if ((e.getX() < xHead) && (e.getX() + Elevator::width > xHead)) {
                            if (currLevel != e.getLevel() && currLevel != 0) {
                                return true;
                            }
                        }
                    }
                    break;
            }
            return false;
        }

        bool isAboveElevator(Elevator& e) {
            switch(dir) {
                case LEFT:
                    if (((e.getX() < xHead - 0.1f) && (e.getX() + Elevator::width > xHead - 0.1f)) ||
                        ((e.getX() < xHead + length) && (e.getX()+Elevator::width > xHead+length))) {
                            if (currLevel == e.getLevel()) {
                                return true;
                            }
                    }
                    break;
                case RIGHT:
                    if (((e.getX() < xHead + length + 0.1f) && (e.getX() + Elevator::width > xHead + length + 0.1f)) ||
                        ((e.getX() < xHead) && (e.getX() + Elevator::width > xHead))) {
                            if (currLevel == e.getLevel()) {
                                return true;
                            }
                        }
                    break;
            }
            return false;
        }

        bool isUnderElevator(Elevator& e) {
            switch(dir) {
                case LEFT:
                    if (((e.getX() < xHead - 0.1f) && (e.getX() + Elevator::width > xHead - 0.1f)) ||
                        ((e.getX() < xHead + length) && (e.getX()+Elevator::width > xHead+length))) {
                            if (currLevel == e.getLevel() - 1) {
                                return true;
                            }
                    }
                    break;
                case RIGHT:
                    if (((e.getX() < xHead + length + 0.1f) && (e.getX() + Elevator::width > xHead + length + 0.1f)) ||
                        ((e.getX() < xHead) && (e.getX() + Elevator::width > xHead))) {
                            if (currLevel == e.getLevel() - 1) {
                                return true;
                            }
                        }
                    break;
            }
            return false;
        }

        bool isReachedSnake(AbstractSnake& s) {
            if (this->currLevel != s.currLevel) return false;
            if (this->dir == LEFT) {
                if (this->xHead-0.1f > s.xHead && this->xHead-0.1f < s.xHead+s.length) {
                    return true;
                }
            } else {
                if (this->xHead+0.1f + this->length > s.xHead && this->xHead+0.1f + this->length < s.xHead+s.length) {
                    return true;
                }
            }
            return false;
        }
};

class RedSnake : public AbstractSnake {
    protected:
        void virtual colorit() {
            glColor3f(1,0,0);
        }
} rSnake;

class GreenSnake : public AbstractSnake {
    protected:
        void virtual colorit(){
            glColor3f(0,1,0);
        }
} gSnake;


// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
    gSnake.setPos(0.5f, SNAKELEVELS[0], 0);
    rSnake.setPos(0.7f, SNAKELEVELS[2], 2);
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

    if (!gameOver) {
        rSnake.draw();
        gSnake.draw();
        field.draw();
    }
    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'q') {
        field.getLeftElevator().moveUp();
        if (rSnake.isAboveElevator(field.getLeftElevator()))
            rSnake.setVerticalState(UP);
        if (gSnake.isAboveElevator(field.getLeftElevator()))
            gSnake.setVerticalState(UP);

    }
    if (key == 'a') {
        field.getLeftElevator().moveDown();
        if (rSnake.isUnderElevator(field.getLeftElevator()))
            rSnake.die();
        if (gSnake.isUnderElevator(field.getLeftElevator()))
            gSnake.die();
    }

    if (key == 'o'){
        field.getRightElevator().moveUp();
        if (rSnake.isAboveElevator(field.getRightElevator()))
            rSnake.setVerticalState(UP);
        if (gSnake.isAboveElevator(field.getRightElevator()))
            gSnake.setVerticalState(UP);
    }
    if (key == 'l') {
        field.getRightElevator().moveDown();
        if (rSnake.isUnderElevator(field.getRightElevator()))
            rSnake.die();
        if (gSnake.isUnderElevator(field.getRightElevator()))
            gSnake.die();

    }

    if (key == 27)  exit(0);
    glutPostRedisplay( );
}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN);  // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
}

float lastChanged = 0.0;

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
    long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido
    float dt = time-lastChanged;
    if (dt > 50.0f) {
        rSnake.control(dt);
        gSnake.control(dt);

        if (rSnake.isReachedSnake(gSnake)) {
            gSnake.die();
        }

        gSnake.animate(dt);
        rSnake.animate(dt);

        field.animate(dt);
        lastChanged = time;
    }
    glutPostRedisplay();
}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv); 				// GLUT inicializalasa
    glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel
    glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

    glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

    glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();

    onInitialization();					// Az altalad irt inicializalast lefuttatjuk

    glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
    glutMouseFunc(onMouse);
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);

    glutMainLoop();					// Esemenykezelo hurok

    return 0;
}
