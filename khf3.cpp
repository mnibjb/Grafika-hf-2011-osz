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
// Nev    : <VEZETEKNEV(EK)> <KERESZTNEV(EK)>
// Neptun : <NEPTUN KOD>
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

#define WIDTH 600
#define HEIGHT 600
#define EPSILON 1e-4
#define NR_OF_PHOTONS 10000

bool fequal(float a, float b, float accuracy = 0.001f) {
    return (fabs(a-b) < accuracy);
}

class Color {
    private:
        float r,g,b;
    public:
        float getR() {return r;}
        float getG() {return g;}
        float getB() {return b;}
        Color() {r = 0.0f;g = 0.0f;b = 0.0f;}
        Color(float rr, float gg, float bb) {r = rr; g = gg; b = bb;}
        Color operator+(const Color& o) { return Color(r+o.r, g+o.g, b+o.b); }
        Color operator*(const Color& o) { return Color(r*o.r, g*o.g, b*o.b); }
        Color operator*(const float f) { return Color(r*f, g*f, b*f); }
        Color& operator +=(const Color& o) {
            *this = *this + o;
            return *this;
        }
        Color& operator= (const Color& o) {
            this->r = o.r;
            this->g = o.g;
            this->b = o.b;
            return *this;
        }
        static Color Black() {return Color();}
        static Color White() {return Color(1,1,1);}

} const AMBIENT(0.1f,0.4f,0.8f);

Color operator-(float f, Color& c) {
    return Color(f-c.getR(), f-c.getG(), f-c.getB());
}

class Vector {
    private:
        float x;
        float y;
        float z;
    public:
        Vector() { x = 0.0f; y = 0.0f; z=0.0f; }
        Vector(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
        Vector(const Vector& v) { x = v.x; y = v.y; z=v.z; }
        ~Vector() {};

        static Vector X() {return Vector(1.0f, 0.0f, 0.0f);}
        static Vector Y() {return Vector(0.0f, 1.0f, 0.0f);}
        static Vector Z() {return Vector(0.0f, 0.0f, 1.0f);}

        float getX() { return x; }
        float getY() { return y; }
        float getZ() { return z; }
        void set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
        float distance(const Vector& v) const {
            float xpart = (v.x-x) * (v.x-x);
            float ypart = (v.y-y) * (v.y-y);
            float zpart = (v.z-z) * (v.z-z);
            return sqrtf(xpart + ypart + zpart);
        }
        float length() {
            return sqrtf(x*x + y*y + z*z);
        }

        float dot(const Vector& v) {
            return x*v.x + y*v.y + z*v.z;
        }

        Vector getNormalized() {
            Vector v = *this;
            return v * (1/v.length());
        }

        Vector cross(const Vector& v) {
            return Vector(
                this->y * v.z - this->z * v.y,
                this->z * v.x - this->x * v.z,
                this->x * v.y - this->y * v.x
            );
        }

        static Vector Infinity() {
            float f = 100000000.0f;
            return Vector(f,f,f);
        }

        Vector operator+ (const Vector& other) const { return Vector(this->x + other.x, this->y + other.y, this->z + other.z ); }
        Vector operator- (const Vector& other) const { return Vector(this->x - other.x, this->y - other.y, this->z - other.z ); }
        bool operator== (const Vector& other) const { return fequal(this->x, other.x) && fequal(this->y, other.y) && fequal(this->z, other.z); }
        Vector operator* (const float& other) const { return Vector(this->x * other, this->y * other, this->z * other); }
        Vector& operator= (const Vector& other) { this->x = other.x; this->y = other.y; this->z = other.z; return *this; }
};

class Eye {
    public:
        Vector getPosition() {
            return Vector(0.0,1.5,0.0);
        }
};


class ViewPlane {
    public:
        static const float Z = -1.0f;
        Vector getRayStartCoordinate(unsigned int u, unsigned int v) {
            float x = (2.0f*u - WIDTH ) / WIDTH;
            float y = (2.0f*v - HEIGHT) / HEIGHT;
            return Vector(x,y,Z);
        }
};

class Ray {
    protected:
        Vector start;
        Vector dir;
    public:
        Ray() {}
        Ray(Vector raystart, Vector raydir) {
            start = raystart;
            dir = raydir.getNormalized();
        }
        Vector& getStart() {return start; }
        Vector& getDir() { return dir; }

};

class Material {
    protected:
        Color ka, ks, kd, kr;
    public:
        Color getKa() {return ka;}
        Color getKs() {return ks;}
        Color getKd() {return kd;}
        Color getKr() {return kr;}
        virtual Color BRDF(Vector& L, Vector& N, Vector& V) = 0;
};

class Photon : public Ray {
    private:
        Vector impact;
        Color color;
    public:
        Photon() {}
        Photon(Vector& start, Vector& dir) {
            this->start = start;
            this->dir = dir.getNormalized();
        }
        Photon(Vector& start, Vector& dir, Color& c) {
            this->start = start;
            this->dir = dir.getNormalized();
            this->color = c;
        }
        Vector& getImpact() {return impact;}
        Color& getColor() {return color;}
        void setColor(Color& c) {color = c;}
        void setImpact(Vector& v) {impact = v;}

        Photon& operator=(const Photon& other) {
            this->dir = other.dir;
            this->impact = other.impact;
            this->start = other.start;
            this->color = other.color;
            return *this;
        }
};

class Emitter {
    private:
        Color c;
        Vector pos;
    public:
        Emitter() {};
        Emitter(Color c, Vector pos) {
            this->c = c;
            this->pos = pos;
        }
        const Color& getColor() const {return c;}
        const Vector& getPos() const{return pos;}
};


class DiffuseMaterial : public Material {
    private:
        float tenyezo1, tenyezo2;

    public:
        DiffuseMaterial(Color c) {
            kd = c;
            ka = kd * (1/M_PI);
        }
        Color BRDF(Vector& L, Vector& N, Vector& V) {
            return kd * (sinf(tenyezo1*4.0) * sinf(tenyezo2*4.0));
//            return Color(0.0,0.2,0);
        }

        void setTextureVariant(float t1, float t2) {
            tenyezo1 = t1;
            tenyezo2 = t2;
        }
};

#define nr 0.2f
#define ng 1.1f
#define nb 1.2f
#define kappar 3.6f
#define kappag 2.6f
#define kappab 2.3f

class SmoothSurface : public Material{
    private:
// Réz (n/k)	0.2/3.6     1.1/2.6	    1.2/2.3
// Kr(r) ~ [ (fr(r) – 1)2 + (kappa(r)2) + (1 – cosθ)5 * 4fr(r) ] / [(fr(r) + 1)2 + (kappa(r)2) ]
        Color F0;
    public:
        SmoothSurface(Color c)  {
            ks = ka = Color(0.0,0.0,0.0);
            kd =Color (0.72,0.45,0.2);
        }
        Color BRDF(Vector& L, Vector& N, Vector& V) {
            Vector n = N.getNormalized();
            Vector v = V.getNormalized();
            float cosa = n.dot(v);
            fresnel(cosa);
            return kr * (1/ cosa);
        }
        void fresnel(float cost) {
            float fr = ((nr-1)*(nr-1) + 4*nr*powf(1-cost,5) + kappar*kappar) / (((nr + 1)*(nr + 1)) + kappar*kappar);
            float fg = ((ng-1)*(ng-1) + 4*ng*powf(1-cost,5) + kappag*kappag) / (((ng + 1)*(ng + 1)) + kappag*kappag);
            float fb = ((nb-1)*(nb-1) + 4*nb*powf(1-cost,5) + kappab*kappab) / (((nb + 1)*(nb + 1)) + kappab*kappab);
            kr = Color(fr,fg,fb);
        }
        Vector reflectionDir(Vector& N, Vector& V) {
            Vector n = N.getNormalized();
            Vector v = V.getNormalized();
            return v - (n * n.dot(v) * 2);
        }

};

class Object {
    protected:
        bool infinite;
    public:
        static const float NONINTERSECT = -1.0f;
        virtual float intersect(Ray& r);
        virtual Vector normal(Vector& V) =0;
        void setInfinite(bool i) {infinite = i;}
};

float Object::intersect(Ray& r) {
    return Object::NONINTERSECT;
}

class Plane : public Object, public DiffuseMaterial {
    public:
        Vector N;
        Vector P0; // kozeppont
        float maxDepth;
        float width, depth;
        Plane() : DiffuseMaterial(Color::Black()) {};
        Plane(Vector p0) : DiffuseMaterial(Color::White()){
            P0 = p0;
            N = Vector::Y();
            maxDepth = 10.0f;
            width = 10.0f;
            depth = 5.0f;
            infinite = false;
        }
        Vector normal(Vector& V) {
            return N;
        }
        float intersect(Ray& r) {
            float t = Object::NONINTERSECT;
            float denominator = r.getDir().dot(N);
            float numerator = (P0 - r.getStart()).dot(N);
            if (fequal(0.0f, denominator)) {
                if (fequal(0.0f, numerator)) {
                    return 0.0f;
                } else {
                    return Object::NONINTERSECT;
                }
            }

            t = numerator / denominator;



            if (!infinite) {
                Vector ip = r.getStart() + (r.getDir() * t);
                setTextureVariant(ip.getX(), ip.getZ());

                float xDist = fabs(ip.getX() - P0.getX());
                float zDist = fabs(ip.getZ() - P0.getZ());
                if (t>0 && (xDist > width/2.0 || zDist > depth / 2.0)) {
                    return Object::NONINTERSECT;
                }
            }
            return t;
        }
};

class Cylinder : public Object, public SmoothSurface{
    private:
        Vector P,Q;
        float R;
        float h;
        bool lastHitInside;
    public:
        Cylinder(): SmoothSurface(Color(1.0f,1.0f,1.0f)){
            R = 1.0f;
        }
        bool isLastHitInside() {
            return lastHitInside;
        }
        void setCenter(Vector& c) {
            P = c;
            Q.set(P.getX(), P.getY() + h, P.getZ());
        }
        void setHeight(float h) {
            this->h = h;
            setCenter(P);
        }
        void setR(float R) {
            this->R = R;
        }
        Vector normal(Vector& V) {
            Vector v1(Q.getX(),V.getY(), Q.getZ());
            return (V-v1);

        }
        float intersect(Ray& r) {
            Vector n = r.getDir();
            Vector m = r.getStart() - P;
            Vector d1 = (Q-P);
            Vector d = d1.getNormalized();
            float height = d1.length();

            // (n*n - (n*d)^2)t^2 + 2(m*n-(n*d)(m*d))t + (m*m-r^2)-(m*d)^2 = 0
            float nd = n.dot(d);
            float md = m.dot(d);

            float a = n.dot(n) - (nd*nd);
            float b = m.dot(n) - (nd*md);
            float c = (m.dot(m) - (R*R)) - (md*md);

            if (fabsf(a) < EPSILON) {
                return Object::NONINTERSECT;
            }

            float discriminant = b*b - a*c;
            float t = Object::NONINTERSECT;
            if (fequal(0.0f, discriminant)) {
                t = (-b + sqrtf(discriminant))/a;
            } else if (discriminant < 0) {
                return Object::NONINTERSECT;
            } else {
                float s1 = (-b + sqrtf(discriminant))/a;
                float s2 = (-b - sqrtf(discriminant))/a;
                Vector higherP = Q.getY() < P.getY() ? P : Q;
                Plane p = Plane();
                p.N = d;
                p.P0 = higherP;
                p.setInfinite(true);
                p.maxDepth = 1000.0f;
                float tp = p.intersect(r);
                Vector pip = r.getStart() + r.getDir() * tp;
                if ( tp > 0.0f && higherP.distance(pip) < R ) {
                    t = fmaxf(s1,s2);
                    lastHitInside = true;
                } else {
                    t = fminf(s1,s2);
                    lastHitInside = false;
                }
            }

            Vector ip = r.getStart() + r.getDir() * t;
            Vector wQ = ip-Q;
            float distQ = fabsf(wQ.dot(d)/ d.length());
            Vector wP = ip-P;
            float distP = fabsf(wP.dot(d)/ d.length());
            if (distQ > height || distP > height) {return Object::NONINTERSECT;}

            return t;
        }
};

int compare (const void * a, const void * b)
{
    Photon* p1 = (Photon*) a;
    Photon* p2 = (Photon*) b;

    Vector i1 = p1->getImpact();
    Vector i2 = p2->getImpact();

    int comp;
    if (fequal(i1.getX(), i2.getX())) {
        comp = 0;
    }
    else if (i1.getX() < i2.getX() ) {
        comp = -1;
    }
    else if (i1.getX() > i2.getX()) {
        comp = 1;
    }
    return comp;

}

class Scene {
    private:
        Eye eye;
        ViewPlane vp;
        Plane p;
        Cylinder c;
        Emitter light;
        Photon photonMap[NR_OF_PHOTONS];

        static const int MAXDEPTH = 3;
        Color trace(Ray& r, int d) {
            Color color;
            if (d > Scene::MAXDEPTH) {
                return AMBIENT;
            }
            ++d;

            float t = c.intersect(r);


            if (t > 0.0f) {
                color += c.getKa() * AMBIENT;

                Vector ip = r.getStart() + r.getDir() * t;
                Vector N = c.normal(ip).getNormalized();
                Vector Rin = r.getDir();
                if (c.isLastHitInside()) { N = N * -1.0f;}
                Vector V = c.reflectionDir(N,Rin);
                V = V.getNormalized();
                ip = ip + (N * EPSILON);
                Ray newRay(ip, V);

                color += directLight(r, N, c,t);


                color += trace(newRay, d+1)* c.BRDF(Rin,N,V);

            } else {

                t = p.intersect(r);
                if (t > 0.0f) {

                    Vector dummy;
                    Vector ip = r.getStart() + r.getDir() * t;
                    Vector N = p.normal(dummy).getNormalized();

                    color += (p.getKa() * AMBIENT);
                    color += directLight(r,N,p,t);
                    int pIdx = searchPhoton(ip);
                    if ( pIdx != -1 ) {
                        return color + photonMap[pIdx].getColor();
                    }
                    color += p.BRDF(N,N,N);
                }
            }

            if (t<0.0f) {return AMBIENT;}
            return color;
        }
        Color directLight(Ray& ray, Vector& N, Material& m, float t ) {
            Color returnColor;
            Vector intersection = ray.getStart() + (ray.getDir() * t);

            Vector dir = light.getPos() - intersection;

            dir = dir.getNormalized();
            N = N.getNormalized();
            intersection = intersection + (N * EPSILON);
            Ray toLight(intersection, dir);
            float tp = p.intersect(toLight);
            float tc = c.intersect(toLight);

            if (tp < 0.0f && tc < 0.0f) {
                // "ralat a fenyre"
                float cost = toLight.getDir().dot(N);
                if (cost < 0.0f){
                    cost=0.0f;
                }
                Vector raydir = ray.getDir();
                Color lightColor = light.getColor();
                returnColor += m.getKd()* (lightColor * cost);
            }
            return returnColor;
        }

        Vector getRandomDirection() {
            float y = -3.0f;
            float x = rand() * (-10.0f / RAND_MAX);
            float z = rand() * (-5.0f / RAND_MAX) - 3.5f;
            Vector v(x,y,z);
            return v.getNormalized();
        }

        void shoot(int i, Photon photon, int depth = 0) {
            if (depth > 3) return;
            ++depth;

            float tc = c.intersect(photon);
            float tp = p.intersect(photon);

            Vector impact = Vector::Infinity();

            if (tc > 0.0f) {
                impact = photon.getStart() + (photon.getDir() * tc);
                Vector N = c.normal(impact);
                N = N.getNormalized();
                if (c.isLastHitInside()) { N = N * -1.0f;}
                Vector V = c.reflectionDir(N,photon.getDir());
                V = V.getNormalized();
                impact = impact + (N * EPSILON);
                Color photonColor = c.BRDF(photon.getDir(), N, V);
                photon.setColor(photonColor);
                photon.setImpact(impact);
                shoot(i, photon, depth);
            } if (tp > 0.0f) {
                impact = photon.getStart() + (photon.getDir() * tp);
                photon.setImpact(impact);
            }
            photonMap[i] = photon;
        }

        int searchPhoton(Vector& impact) {

            for (int i=0;i<NR_OF_PHOTONS; ++i) {
                if (photonMap[i].getImpact().distance(impact) < 0.01f) {
                    return i;
                }
            }
            return -1;

        }

        void toneMapping() {
            for (int i=0;i<WIDTH; ++i) {
                for (int j = 0; j<HEIGHT; ++j) {
                    for (int k = 0; k<3;++k) {
                        pixels[i][j][k] = pixels[i][j][k] / (1+pixels[i][j][k]);
                    }
                }
            }
        }
    public:
        Scene() {
            light = Emitter(Color::White(), Vector(5,1,0));
            Vector p0 = Vector(0.0, -2.0, -6.0 );
            p = Plane(p0);

            c = Cylinder();
            c.setHeight(0.5f);
            c.setR(1.5f);
            Vector p1 = Vector(0,-1.5,-6);
            c.setCenter(p0);
        }
        float pixels[WIDTH][HEIGHT][3];
        void prepareRender() {
            Color lightColor = Color::Black();
            Vector lightPos = light.getPos();
            for (int i = 0; i< NR_OF_PHOTONS; ++i) {
                Vector dir = getRandomDirection();
                Photon ray(lightPos, dir, lightColor);

                shoot(i, ray);
            }

            for (int i=0; i<HEIGHT; ++i) {
                for (int j=0; j<WIDTH; ++j) {
                    Ray r(eye.getPosition(), vp.getRayStartCoordinate(j,i));
                    Color c = trace(r,0);
                    pixels[i][j][0] = c.getR();
                    pixels[i][j][1] = c.getG();
                    pixels[i][j][2] = c.getB();
                }
            }

            toneMapping();
        }
};

Scene s;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {

    s.prepareRender();
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, s.pixels );

    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet
    if (key == 27) exit(0);

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN);  // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) { }

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
