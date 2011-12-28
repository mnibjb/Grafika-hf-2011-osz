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
enum Metszes {BELSO_PONT, VEGPONT, NEM_METSZENEK};

bool fequal(float a, float b, float accuracy = 0.001f) {
    return (fabs(a-b) < accuracy);
}

class Shape {
    protected:
        float red,green,blue;
    public:
        Shape() {red=0.0; green=0.0; blue=0.0; }
        ~Shape() {};
        virtual void draw() = 0;
        void setColor(float r, float g, float b) {
            this->red = r;
            this->green = g;
            this->blue = b;
        }
};

class Vector : public Shape {
    private:
        float x;
        float y;
        float z;
    public:
        Vector() { x = 0.0f; y = 0.0f; z=0.0f; }
        Vector(float x, float y) { this->x = x; this->y = y; z = 0.0f; }
        Vector(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
        Vector(const Vector& v) { x = v.x; y = v.y; z=v.z; }
        ~Vector() {};
        float getX() { return x; }
        float getY() { return y; }
        float getZ() { return z; }
        void set(float x, float y) { this->x = x; this->y = y; }
        void set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
        void virtual draw(){
            glColor3f(0.0,0.0,0.0);
            glBegin(GL_TRIANGLES);
                glVertex2f(x,y);
                glVertex2f(x-0.2f,y-0.2f);
                glVertex2f(x+0.2f,y-0.2f);
            glEnd();
        }
        float distance(const Vector& v) {
            float xpart = (v.x-x) * (v.x-x);
            float ypart = (v.y-y) * (v.y-y);
            return sqrtf(xpart + ypart);
        }
        float length() {
            return sqrtf(x*x + y*y);
        }
        float meredekseg_elojel(Vector& v){
            if ( ((v.x >= x) && (v.y >= y)) || ((v.x < x) && (v.y < y)) ) {
                return 1.0;
            }
            return -1.0;
        }

        Vector operator+ (const Vector& other) const { return Vector(this->x + other.x, this->y + other.y ); }
        Vector operator- (const Vector& other) const { return Vector(this->x - other.x, this->y - other.y ); }
        bool operator== (const Vector& other) const { return fequal(this->x, other.x) && fequal(this->y, other.y); }
        Vector operator* (const float& other) const { return Vector(this->x * other, this->y * other); }
        Vector& operator= (const Vector& other) { this->x = other.x; this->y = other.y; this->z = other.z; return *this; }
};

class Matrix {
    private:
        float a, b, c, d;
    public:
        static Matrix EgysegMatrix() {
            Matrix m(1.0,0.0,0.0,1.0);
            return m;
        }
        Matrix() {
            a = 0.0;
            b = 0.0;
            c = 0.0;
            d = 0.0;
        }
        Matrix(float a, float b, float c, float d) {
            this->a = a;
            this->b = b;
            this->c = c;
            this->d = d;
        }

        Vector szorzas(Vector& v) {
            Vector res;
            float newx = v.getX() * a + v.getY() * c;
            float newy = v.getX() * b + v.getY() * d;
            res.set(newx, newy, v.getZ());
            return res;
        }

        Matrix& operator= (const Matrix& other) {
            this->a = other.a;
            this->b = other.b;
            this->c = other.c;
            this->d = other.d;
            return *this;
        }
};

class AbstractCurve : public Shape {
    protected:
        static const unsigned int NR_OF_CONTROL_POINTS = 100;
        Vector referencia[NR_OF_CONTROL_POINTS];
        Vector v[NR_OF_CONTROL_POINTS];
        bool showCtrlPts;
        int np;
        Matrix sebesseg;
        Vector gyorsulas;

        float length;

        virtual void rotate() {
            for (int i=0; i<np;i++) {
                v[i] = sebesseg.szorzas(referencia[i]);
            }
        }

        virtual void deform() {
            for (int i=0; i<np; i++) {
                Vector r = gyorsulas * v[i].getZ();
                v[i] = v[i] + r;
            }
        }

    public:
        AbstractCurve() : showCtrlPts(false) {
            sebesseg = Matrix::EgysegMatrix();
            length = 0.0;
        }
        virtual void draw() {
            rotate();
            deform();
            if (showCtrlPts) {
                for (int i = 0; i< np; ++i) {
                    v[i].draw();
                }
            }
        }
        Vector& operator[](int idx) {
            return v[idx];
        }
        void setShowControlPoints(bool b) { showCtrlPts = b;}
        void setSebesseg(Matrix& m) {sebesseg = m;}
        Vector getGyorsulas() {return gyorsulas;}
        void setGyorsulas(Vector& a) {gyorsulas = a;}
        float getLength() { return length; }
        int getNP() {return np;}

        void setReferencia(Vector& v, int idx){
            referencia[idx].set(v.getX(), v.getY());
        }

        virtual void copyToReferencia() {
            for (int i=0; i<np; i++) {
                referencia[i] = v[i];
            }
        }

        bool rajtavanazegyenesen(Vector& p, Vector& A, Vector& B) {
            Vector r = A-B;
            if (fequal(r.getX(), 0.0f)) {
                if (fequal(A.getX(), p.getX())) {
                    return ( (p.getY() >= fminf(A.getY(), B.getY())) && ( p.getY() <= fmaxf(A.getY(), B.getY()) ) );
                }
            }
            float m = r.getY() / r.getX();
            float y = m * (p.getX() - A.getX()) + A.getY();
            return fequal(y, p.getY());
        }

        bool counterClockwise(Vector& a, Vector& b, Vector& c) {
            return (( c.getY() - a.getY() ) * ( b.getX() - a.getX() )) > (( b.getY() - a.getY() ) * ( c.getX()-a.getX() ));
        }

        // forras: http://compgeom.cs.uiuc.edu/~jeffe/teaching/373/notes/x06-sweepline.pdf
        bool metszeneke(Vector& A, Vector& B, Vector& C, Vector& D) {
            if (counterClockwise(A,C,D) == counterClockwise(B,C,D))
                return false;
            else if (counterClockwise(A,B,C) == counterClockwise(A,B,D))
                return false;
            else
                return true;

        }


        virtual bool zartgorbe() {
            return (v[0] == v[np-1]);
        }

        bool belsoponte(Vector& p) {
            if (!zartgorbe()) { return false; }
            Vector p2(p.getX()+30.0f, p.getY());
            int db=0;
            for (int i=0; i<np-1; i++) {
                bool m = metszeneke(p,p2,v[i],v[i+1]);
                if (m == true) {
                        ++db;
                }
            }
            return db%2==1;
        }


        bool atlo_e(Vector& v1, Vector& v2) {
            Vector h = (v1+v2) * 0.5f;
            if (!belsoponte(h)) return false;
            for (int i=0; i<np-1; i++) {
                if (v1 == v[i] || v1 == v[i+1]) continue;
                if (v2 == v[i] || v2 == v[i+1]) continue;
                if (metszeneke(v1,v2,v[i],v[i+1]) == true) {
                    return false;
                }
            }
            return true;
        }

        void fulvago(Vector v[], int n) {
            if (n == 3) {
                glBegin(GL_TRIANGLES);
                for(int k=0;k<n;k++){
                    glVertex2f(v[k].getX(), v[k].getY());
                }
                glEnd();
                return;
            }
            bool volt_atlo = false;
            int i=1;
            for (; i<n-1; i++) {
                if ( atlo_e(v[i-1], v[i+1]) ) {
                    volt_atlo = true;
                    break;
                }
            }

            if (volt_atlo) {
                glBegin(GL_TRIANGLES);
                    glVertex2f(v[i-1].getX(), v[i-1].getY());
                    glVertex2f(v[i].getX(), v[i].getY());
                    glVertex2f(v[i+1].getX(), v[i+1].getY());
                glEnd();
            }
            Vector t[n-1];
            for (int j=0;j<n;j++) {
                if (j<i) {
                    t[j] = v[j];
                } else if (j>i) {
                    t[j-1] = v[j];
                }
            }
            fulvago(t, n-1);
        }

};

class CatmullRomCurve : public AbstractCurve {
    private:
        float dt;
        bool szines;
        Vector r(float t, int i) {
            Vector res(0,0);
            res = ((v[i]*(-1.0f) + v[i+1] *3.0f + v[i+2]*(-3.0f) + v[i+3])*t*t*t
                + (v[i]*(2.0f) + v[i+1]*(-5.0f) + v[i+2]*4.0f - v[i+3])*t*t
                + (v[i]*(-1.0f)+v[i+2])*t
                + v[i+1] * 2.0f) * 0.5f;
            return res;
        }
        Vector elsoDerivalt(float t, int i) {
            Vector res;
            res = ((v[i]*(-1.0f) + v[i+1] *3.0f + v[i+2]*(-3.0f) + v[i+3])*t*t*3
                + (v[i]*(2.0f) + v[i+1]*(-5.0f) + v[i+2]*4.0f - v[i+3])*t*2
                + (v[i]*(-1.0f)+v[i+2])) * 0.5f;
            return res;
        }
        Vector masodikDerivalt(float t, int i) {
            Vector res;
            res = ((v[i]*(-1.0f) + v[i+1] *3.0f + v[i+2]*(-3.0f) + v[i+3])*t*6
                + (v[i]*(2.0f) + v[i+1]*(-5.0f) + v[i+2]*4.0f - v[i+3])*2) * 0.5f;
            return res;
        }
    public:
        Vector speed;
        Vector accerelation;

        CatmullRomCurve() { dt = 0.025f; np = 0; szines = false;}
        CatmullRomCurve(int n) {
            np = n;
            dt = 0.025f;
            szines = false;
        }
        void setSzines(bool b) {
            szines = b;
        }
        virtual void draw() {
            AbstractCurve::draw();
            length = 0.0;
            Vector prev(v[0]);
            glColor3f(this->red, this->green, this->blue);
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < np - 3; ++i) {
                for (float t = 0.0f; t < 1.0f; t += dt) {
                    Vector v = r(t,i);
                    glVertex2f(v.getX(), v.getY());
                    length += prev.distance(v);
                    prev = v;
                }
            }
            glEnd();
            if (szines) {
                fulvago(v,np);
            }

        }

        Vector nextPosition(const Vector& p, float dt, float v) {
            float ut = dt * v;
            float hossz = 0.0f;
            bool innentol = false;
            Vector prev;
            for (int i = 0; i < np - 3; ++i) {
                for (float t = 0.0f; t < 1.0f; t += this->dt) {
                    Vector temp = r(t,i);
                    if (innentol) {
                        hossz += prev.distance(temp);
                    }
                    if (hossz >= ut) {
                        if (temp.distance(this->v[np-1]) < 1.0f) {
                            return this->v[0];
                        }
                        speed = elsoDerivalt(t,i);
                        speed = speed * (1/speed.length());

                        accerelation = masodikDerivalt(t,i);
                        return temp;
                    }
                    if (p == temp) {
                        innentol = true;
                    }
                    prev = temp;
                }
            }
            return p;
        }
};

class BezierCurve : public AbstractCurve {
    private:
        float B(int i, float t){
            float ch = 1.0;
            for (int j=1; j<=i; j++){
                ch*= (float)(np-j+1)/j;

            }
            return ch * pow(t,i) * pow(1-t,np-i);
        }

        Vector r(float t){
            Vector res(0,0);
            for (int i=0; i<=np; i++){
                res = res + v[i]*B(i,t);
            }
            return res;
        }

        virtual void rotate() {
            for (int i=0; i<=np;i++) {
                v[i] = sebesseg.szorzas(referencia[i]);
            }
        }
        virtual void deform() {
            for (int i=0; i<=np; i++) {
                Vector r = gyorsulas * v[i].getZ();
                v[i] = v[i] + r;
            }
        }
     public:
        BezierCurve() { np = 0; }
        BezierCurve(int n) {
            np = n-1;
        }
        virtual void draw(){
            AbstractCurve::draw();

            glBegin(GL_POLYGON);
            glColor3f(this->red,this->green,this->blue);
            float dt = 0.02f;
            for (float t=0.0; t<1.0; t+=dt){
                Vector v = r(t);
                glVertex2f(v.getX(), v.getY());
            }
            glEnd();

        }

        void virtual copyToReferencia() {
            for (int i=0; i<=np; i++) {
                referencia[i] = v[i];
            }
        }

        virtual bool zartgorbe(){
            return v[0] == v[np];
        }


};

class CatmullClark : public AbstractCurve{
    private:
        void refine(const Vector r1[], int n, Vector r2[]) {

            r2[0] = r1[0]*0.5f + ((r1[n-1] + r1[0]) * 0.5f)*0.25f + ((r1[0] + r1[1]) * 0.5f) * 0.25f;
            for (int i=1;i<n;++i) {
                Vector h1 = (r1[i-1] + r1[i]) * 0.5f;
                Vector h2 = (r1[i] + r1[i+1<n?i+1:0]) * 0.5f;
                Vector r1mod = r1[i]*0.5f + h1*0.25f + h2*0.25f;

                r2[i*2] = r1mod;
                r2[i*2-1] = h1;
                r2[i*2+1] = h2;
            }
        }

    public:
        CatmullClark() {np = 0;}
        CatmullClark(int n) {
            np = n;
        }

        virtual void draw() {
            AbstractCurve::draw();

            int n = 2*np;
            Vector r[n];
            refine(v, np, r);

            int m = 2*n;
            Vector p[m];
            refine(r, n, p);

            int o = 2*m;
            Vector w[o];
            refine(p, m, w);

            glColor3f(this->red,this->green,this->blue);
            glBegin(GL_POLYGON);
            for (int i=0; i<o; i++) {
                glVertex2f(w[i].getX(), w[i].getY());
            }
            glEnd();
        }
};

class Snail : public Shape{
    private:
        CatmullRomCurve body;
        CatmullClark shell_layer1;
        CatmullClark shell_layer2;
        BezierCurve leftEye;
        BezierCurve rightEye;

        Vector pivot;
    public:
        Snail() {
            body = CatmullRomCurve(31);
            body[0] = Vector(-0.386667, 1.920000);
            body[1] = Vector(-0.386667, 1.920000);
            body[2] = Vector(-0.560000, 1.506667);
            body[3] = Vector(-0.326667, 1.060000);
            body[4] = Vector(-0.620000, 0.733333);
            body[5] = Vector(-0.706667, 0.313333);
            body[6] = Vector(-0.806667, -0.293333);
            body[7] = Vector(-0.833333, -0.826667);
            body[8] = Vector(-0.526667, -1.446667);
            body[9] = Vector(-0.193333, -1.633333);
            body[10] = Vector(0.320000, -1.780000);
            body[11] = Vector(0.680000, -1.706667);
            body[12] = Vector(0.860000, -1.213333);
            body[13] = Vector(0.840000, -0.866667);
            body[14] = Vector(0.720000, -0.520000);
            body[15] = Vector(0.600000, -0.100000);
            body[16] = Vector(0.506667, 0.306667);
            body[17] = Vector(0.433333, 0.593333);
            body[18] = Vector(0.433333, 0.973333);
            body[19] = Vector(0.500000, 1.313333);
            body[20] = Vector(0.613333, 1.600000);
            body[21] = Vector(0.320000, 1.860000);
            body[22] = Vector(0.086667, 1.846667);
            body[23] = Vector(0.140000, 1.620000);
            body[24] = Vector(0.200000, 1.426667);
            body[25] = Vector(0.240000, 1.226667);
            body[26] = Vector(0.153333, 1.086667);
            body[27] = Vector(0.013333, 1.293333);
            body[28] = Vector(-0.200000, 1.786667);
            body[29] = Vector(-0.386667, 1.920000);
            body[30] = Vector(-0.386667, 1.920000);
            body.setColor(1.0,0.5, 0.0);
            body.copyToReferencia();
            body.setSzines(true);

            rightEye = BezierCurve(6);
            rightEye[0]=Vector(0.280000, 2.183334, 0.0);
            rightEye[1]=Vector(0.113333, 1.908334, 0.0);
            rightEye[2]=Vector(0.466667, 1.591667, 0.0);
            rightEye[3]=Vector(1.033333, 1.608334, 0.0);
            rightEye[4]=Vector(0.573333, 2.075001, 0.0);
            rightEye[5]=Vector(0.280000, 2.183334, 0.0);
            rightEye.setColor(0.0,0.0,0.0);
            rightEye.copyToReferencia();

            leftEye = BezierCurve(7);
            leftEye[0]=Vector(-0.446667, 2.358333, 0.0);
            leftEye[1]=Vector(-0.140000, 2.100000, 0.0);
            leftEye[2]=Vector(-0.293333, 1.850000, 0.0);
            leftEye[3]=Vector(-0.633333, 1.750000, 0.0);
            leftEye[4]=Vector(-0.826667, 1.950000, 0.0);
            leftEye[5]=Vector(-0.626667, 2.208333, 0.0);
            leftEye[6]=Vector(-0.446667, 2.358333, 0.0);
            leftEye.setColor(0.0,0.0,0.0);
            leftEye.copyToReferencia();

            shell_layer1 =  CatmullClark(4);
            shell_layer1[0] = Vector(0.0, 1.5, 0.1);
            shell_layer1[1] = Vector(0.85, -0.5, 0.1);
            shell_layer1[2] = Vector(0.0, -2.0, 0.1);
            shell_layer1[3] = Vector(-1.0, -0.3, 0.1);
            shell_layer1.setColor(1.0,1.0,0.0);
            shell_layer1.copyToReferencia();

            shell_layer2 = CatmullClark(4);
            shell_layer2[0] = Vector(0.0, 1.0, 0.2);
            shell_layer2[1] = Vector(0.6, -0.5, 0.2);
            shell_layer2[2] = Vector(0.0, -1.6, 0.2);
            shell_layer2[3] = Vector(-0.6, -0.3, 0.2);
            shell_layer2.setColor(0.0,1.0,1.0);
            shell_layer2.copyToReferencia();

            pivot = Vector(10.0,10.0);
        }

        virtual void draw() {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(pivot.getX(),pivot.getY(), 0.0);

            body.draw();
            leftEye.draw();
            rightEye.draw();
            shell_layer1.draw();
            shell_layer2.draw();

        }
        void animate(float dt, CatmullRomCurve& track) {
            pivot = track.nextPosition(pivot, dt, track.getLength() / 5000);
            Matrix m(track.speed.getY(), -1.0*track.speed.getX(), track.speed.getX(), track.speed.getY());
            setSebesseg(m);
            setGyorsulas(track.accerelation);
        }

        Vector getPosition() {return pivot;}

        void setPosition(Vector& v) {
            pivot = v;
        }

        void setPosition(float x, float y) {
            pivot.set(x,y);
        }

        void setSebesseg(Matrix& m) {
            body.setSebesseg(m);
            shell_layer1.setSebesseg(m);
            shell_layer2.setSebesseg(m);
            leftEye.setSebesseg(m);
            rightEye.setSebesseg(m);
        }

        void setGyorsulas(Vector& m) {
            body.setGyorsulas(m);
            shell_layer1.setGyorsulas(m);
            shell_layer2.setGyorsulas(m);
            leftEye.setGyorsulas(m);
            rightEye.setGyorsulas(m);
        }
};

class Track : public Shape {
    private:
        CatmullRomCurve track;
        Snail snail;
        bool z;
    public:
        Track() {
            track = CatmullRomCurve(32);
            track[0]=Vector(29.100000, 15.500000);
            track[1]=Vector(29.100000, 15.500000);
            track[2]=Vector(27.500000, 19.450001);
            track[3]=Vector(24.650000, 21.850000);
            track[4]=Vector(21.100000, 23.350000);
            track[5]=Vector(18.150000, 22.200001);
            track[6]=Vector(13.050000, 23.200001);
            track[7]=Vector(11.000000, 25.750000);
            track[8]=Vector(9.300000, 27.299999);
            track[9]=Vector(6.750000, 27.500000);
            track[10]=Vector(6.600000, 23.900000);
            track[11]=Vector(10.000000, 21.100000);
            track[12]=Vector(8.200000, 20.000000);
            track[13]=Vector(3.850000, 19.600000);
            track[14]=Vector(3.650000, 17.450001);
            track[15]=Vector(7.500000, 14.900000);
            track[16]=Vector(10.000000, 13.750000);
            track[17]=Vector(10.400000, 11.500000);
            track[18]=Vector(8.400000, 8.600000);
            track[19]=Vector(7.900000, 6.750000);
            track[20]=Vector(8.150000, 4.700000);
            track[21]=Vector(11.800000, 2.750000);
            track[22]=Vector(16.750000, 2.500000);
            track[23]=Vector(19.600000, 3.250000);
            track[24]=Vector(20.000000, 6.350000);
            track[25]=Vector(22.150000, 7.600000);
            track[26]=Vector(25.100000, 5.500000);
            track[27]=Vector(26.700001, 5.450000);
            track[28]=Vector(27.299999, 8.400000);
            track[29]=Vector(27.850000, 12.600000);
            track[30]=Vector(28.549999, 13.350000);
            track[30]=Vector(29.100000, 15.500000);
            track[31]=Vector(29.100000, 15.500000);

            track.copyToReferencia();

            track.setColor(1.0,1.0,1.0);
            snail.setPosition(track[0]);

            z = false;
        }

        virtual void draw() {
            if (z) {
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluOrtho2D
                (
                           snail.getPosition().getX() - 5
                           ,snail.getPosition().getX() + 5
                           ,snail.getPosition().getY() - 5
                           ,snail.getPosition().getY() + 5
                );
            } else {
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluOrtho2D( 0.0, 30.0, 0.0, 30.0 );
            }
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            track.draw();
            snail.draw();
        }

        void animate(float dt) {
            snail.animate(dt, track);
        }

        void zoom() {
            z = z ? false : true;
        }

        Vector& operator[](int idx) {
            return track[idx];
        }

        CatmullRomCurve& getSpline() {return track; }

        bool getZoom() {return z;}

};

Track t;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 30.0, 0.0, 30.0 );
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.0f, 0.45f, 0.0f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

    t.draw();

    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet
    if (key == 27) exit(0);

    if (key == 'j') {
        t.animate(52.0f);
    }

    if (key == 'z') {
        t.zoom();
    }

    glutPostRedisplay();

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN && !t.getZoom()) {
        float new_x = (30.0/600) * x;
        float new_y = ((30.0/600) * y - 30 ) * -1.0f ;

        Vector r(new_x, new_y);

        float minDist = r.distance(t[0]);
        int minIdx = 0;

        for (int i=1; i<t.getSpline().getNP(); i++) {
            float d = r.distance(t[i]);
            if (d < minDist) {
                minIdx = i;
                minDist = d;
            }
        }

        t.getSpline().setReferencia(r, minIdx);
        t[minIdx] = r;
        glutPostRedisplay();

    }
}

float last_changed = 0.0;

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
     long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido
     float dt = time - last_changed;
     if ( dt > 50.0f) {
        t.animate(dt);
        last_changed = time;
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

