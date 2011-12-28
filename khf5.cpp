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

#define EPSILON 0.001f
#define MAX_NR_OF_PARTS 5
#define W_HEIGHT 600
#define W_WIDHT 600



bool fequal(float a, float b, float accuracy = EPSILON) {
    return (fabs(a-b) < accuracy);
}

float degToRad(float deg) {
    return deg * (M_PI / 180);
}

float zeroLevel = 0;

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

        float getX() const { return x; }
        float getY() const { return y; }
        float getZ() const { return z; }
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

        void setX(float x) {this->x=x;}
        void setY(float y) {this->y=y;}
        void setZ(float z) {this->z=z;}
        Vector operator+ (const Vector& other) const { return Vector(this->x + other.x, this->y + other.y, this->z + other.z ); }
        Vector operator- (const Vector& other) const { return Vector(this->x - other.x, this->y - other.y, this->z - other.z ); }
        bool operator== (const Vector& other) const { return fequal(this->x, other.x) && fequal(this->y, other.y) && fequal(this->z, other.z); }
        Vector operator* (const float& other) const { return Vector(this->x * other, this->y * other, this->z * other); }
        Vector& operator= (const Vector& other) { this->x = other.x; this->y = other.y; this->z = other.z; return *this; }
        Vector& operator-=(const Vector &rhs) {
            this->x = this->x - rhs.x;
            this->y = this->y - rhs.y;
            this->z = this->z - rhs.z;
            return *this;
        }
};

Vector lightPos(4,10,1);

class Shape {
    private:
        bool drawingShadow;
    protected:
        static const float d = 0.02f;
        Vector P0;
        float ka[4], kd[4], ks[4];
        float shine;
        Vector rotateDir, animRotateDir;
        float angle, animAngle;
        float size;

        bool texturazott;
        unsigned int texId;
        bool castShadow;

        virtual void drawVertexes() = 0;
        virtual void doScale(){glScalef(size,size,size);};
        virtual void drawSubParts() {}
        virtual void init() {
            angle = 0.0f;
            animAngle = 0.0f;
            shine = 0.0f;
            size = 1.0f;
            drawingShadow = false;
            castShadow = false;
       }
    public:

        Shape() { init(); }
        void draw() {

            glMaterialfv( GL_FRONT, GL_AMBIENT, ka );
            glMaterialfv( GL_FRONT, GL_DIFFUSE, kd );
            glMaterialfv( GL_FRONT, GL_SPECULAR,ks );
            glMaterialf( GL_FRONT, GL_SHININESS, shine );

            if (texturazott) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texId);
            }

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(P0.getX(), P0.getY(), P0.getZ());
            glRotatef(angle, rotateDir.getX(), rotateDir.getY(), rotateDir.getZ());
            glRotatef(animAngle,animRotateDir.getX(), animRotateDir.getY(), animRotateDir.getZ());

            doScale();
            drawVertexes();

            drawSubParts();

            glPopMatrix();

            if (texturazott) {
                glDisable(GL_TEXTURE_2D);
            }

            if (!drawingShadow && castShadow) {
                drawingShadow = true;
                glPushMatrix();
                glTranslatef(0,zeroLevel+0.05,0);

                float shadow_mtx[4][4] = {{1,0,0,0},
                                          {-lightPos.getX()/lightPos.getY(),0,-lightPos.getZ()/lightPos.getY(),0},
                                          {0,0,1,0},
                                          {0,0.001,0,1}};
                glMultMatrixf( &shadow_mtx[0][0] );
                glDisable(GL_LIGHTING);
                glColor3f(0,0,0);
                this->draw();
                glPopMatrix();
                glEnable(GL_LIGHTING);
            } else { drawingShadow = false; }
        }

        virtual void animate(float dt) {}
        Vector& getP0() {return P0;}
        void setP0(const Vector& p0) { P0 = p0; }
        void setP0(const float x, const float y, const float z) { P0.set(x,y,z); }
        float* Ka() {return ka;}
        float* Kd() {return kd;}
        float* Ks() {return ks;}
        void setKa(float r, float g, float b, float alpha = 1.0f) {
            ka[0] = r;
            ka[1] = g;
            ka[2] = b;
            ka[3] = alpha;
        }
        void setKd(float r, float g, float b, float alpha = 1.0f) {
            kd[0] = r;
            kd[1] = g;
            kd[2] = b;
            kd[3] = alpha;
        }
        void setKs(float r, float g, float b, float alpha = 1.0f) {
            ks[0] = r;
            ks[1] = g;
            ks[2] = b;
            ks[3] = alpha;
        }
        float getAngle() const {return angle;}

        void setShine(const float s) {shine = s;}
        void setAngle(const float a) {angle = a;}
        void setRotateDir(const Vector& v) {rotateDir = v;}
        void setAnimAngle(const float a) {animAngle = a;}
        void setAnimRotateDir(const Vector& v) {animRotateDir = v;}
        void setSize(const float s) {size = s;}
        void setTexId(const unsigned int id) {texId = id;}
        void setTexturazott(const bool tex) {texturazott = tex;}
        void setCastShadow(const bool cs) { castShadow = cs;}

        void setMaterial(const Shape& base) {
            this->setKa(base.ka[0], base.ka[1], base.ka[2]);
            this->setKd(base.kd[0], base.kd[1], base.kd[2]);
            this->setKs(base.ks[0], base.ks[1], base.ks[2]);
            this->shine = base.shine;
        }

        static void drawCircle(float y, float yDir) {
            glDisable(GL_CULL_FACE);
            glBegin(GL_TRIANGLE_FAN);
            glNormal3f(0,yDir,0);

            for(float u=0.0f; u<1.0f; u+=d) {
                float x = cosf(2*M_PI*u);
                float z = -sinf(2*M_PI*u);
                glTexCoord2f(x,y);
                glVertex3f(x,y,z);

                x = cosf(2*M_PI*(u+d));
                z = -sinf(2*M_PI*(u+d));
                glTexCoord2f(x,y);
                glVertex3f(x,y,z);

                glTexCoord2f(0,0);
                glVertex3f(0,y,0);

            }
            glEnd();
            glEnable(GL_CULL_FACE);
        }
};

class Sphere : public Shape {
    private:
        float R;
    protected:
        void doScale() {
            glScalef(R,R,R);
        }
        void drawVertexes() {
            glBegin(GL_QUADS);
            for (float u=0.0f; u<1.0f; u+=d) {
                for (float v=0.0f;v<1.0f;v+=d) {
                    float x = cosf(2*M_PI*u) * sinf(M_PI*v);
                    float y = sinf(2*M_PI*u) * sinf(M_PI*v);
                    float z = cosf(M_PI*v);
                    glNormal3f(x,y,z);
                    glVertex3f(x,y,z);

                    x = cosf(2*M_PI*(u)) * sinf(M_PI*(v+d));
                    y = sinf(2*M_PI*(u)) * sinf(M_PI*(v+d));
                    z = cosf(M_PI*(v+d));
                    glNormal3f(x,y,z);
                    glVertex3f(x,y,z);

                    x = cosf(2*M_PI*(u+d)) * sinf(M_PI*(v+d));
                    y = sinf(2*M_PI*(u+d)) * sinf(M_PI*(v+d));
                    z = cosf(M_PI*(v+d));
                    glNormal3f(x,y,z);
                    glVertex3f(x,y,z);

                    x = cosf(2*M_PI*(u+d)) * sinf(M_PI*v);
                    y = sinf(2*M_PI*(u+d)) * sinf(M_PI*v);
                    z = cosf(M_PI*v);
                    glNormal3f(x,y,z);
                    glVertex3f(x,y,z);
                }
            }
            glEnd();
        }
    public:
        Sphere() { R = 1.0f; }
        float getR() {return R;}
        void setR(const float r) {R = r; }


};

class Cylinder : public Shape {
    private:
        float R;
        float h;
    protected:
        void doScale() {
            glScalef(R,h,R);
        }
        void drawVertexes() {
            glBegin(GL_QUADS);
            for (float u = 0.0f; u<=1.0f; u+=d) {
                for (float hi = 0.0f; hi <= 1.0; hi+=d) {

                    float x = cosf(2*M_PI*u);
                    float z = -1.0f*sinf(2*M_PI*u);
                    float y = hi;
                    glTexCoord2f(x,y);
                    glNormal3f(x,y,z);
                    glVertex3f(x,y,z);

                    x = cosf(2*M_PI*(u+d));
                    z = -1.0f*sinf(2*M_PI*(u+d));
                    y = hi;
                    glTexCoord2f(x,y);
                    glNormal3f(x,y,z);
                    glVertex3f(x,y,z);

                    x = cosf(2*M_PI*(u+d));
                    z = -1.0f*sinf(2*M_PI*(u+d));
                    y = hi + d;
                    glTexCoord2f(x,y);
                    glNormal3f(x,y,z);
                    glVertex3f(x,y,z);

                    x = cosf(2*M_PI*u);
                    z = -1.0f*sinf(2*M_PI*u);
                    y = hi + d;
                    glTexCoord2f(x,y);
                    glNormal3f(x,y,z);
                    glVertex3f(x,y,z);
                }
            }
            glEnd();

            Shape::drawCircle(0.0f, -1.0f);
            Shape::drawCircle(1.0f, 1.0f);

        }
    public:
        Cylinder() {
            R = 1.0f;
            h = 1.0f;
        }
        float getR() {return R;}
        void setR(const float r) {R = r; }
        float getH() {return h;}
        void setH(const float h) {this->h = h; }
};

class Cuboid : public Shape {
    private:
        float height, width, depth;

    protected:
        void doScale() {
            glScalef(width,height,depth);
        }
        void drawVertexes() {
            glDisable(GL_CULL_FACE);
            glBegin(GL_QUADS);
                for (float u=0.0f;u<1.0f; u+=d) {
                    for (float v=0.0f; v<1.0f; v+=d) {
                        float un = u+d;
                        float vn = v+d;

                        glNormal3f(0,0,1);
                        glVertex3f(u,v,0);
                        glVertex3f(un,v,0);
                        glVertex3f(un,vn,0);
                        glVertex3f(u,vn,0);

                        glNormal3f(0,0,-1);
                        glVertex3f(u,v,-1);
                        glVertex3f(un,v,-1);
                        glVertex3f(un,vn,-1);
                        glVertex3f(u,vn,-1);

                        glNormal3f(0,-1,0);
                        glVertex3f(u,0,-v);
                        glVertex3f(un,0,-v);
                        glVertex3f(un,0,-vn);
                        glVertex3f(u,0,-vn);

                        glNormal3f(0,1,0);
                        glVertex3f(u,1,-v);
                        glVertex3f(un,1,-v);
                        glVertex3f(un,1,-vn);
                        glVertex3f(u,1,-vn);

                        glNormal3f(-1,0,0);
                        glVertex3f(0,u,-v);
                        glVertex3f(0,un,-v);
                        glVertex3f(0,un,-vn);
                        glVertex3f(0,u,-vn);

                        glNormal3f(1,0,0);
                        glVertex3f(1,u,-v);
                        glVertex3f(1,un,-v);
                        glVertex3f(1,un,-vn);
                        glVertex3f(1,u,-vn);

                    }
                }
            glEnd();
            glEnable(GL_CULL_FACE);
        }

    public:
        Cuboid() { width=height=depth=1.0f; }
        float getWidth() const {return width;}
        void setWidth(float w) {width = w;}
        float getHeight() const {return height;}
        void setHeight(float h) {height = h;}
        float getDepth() const {return depth;}
        void setDepth(float d) {depth = d;}
        void setDimensions(float w, float h, float d) {width = w; height = h; depth = d;}
};

class Plane : public Shape {
    private:
        float width, height;
    protected:
        void doScale() {glScalef(width, 1, height);}
        void drawVertexes() {
            glBegin(GL_QUADS);
            glNormal3f(0,1,0);

            for (float x=-1.0f; x<1.0f; x+=d) {
                for (float z=-1.0f; z<1.0f; z+=d) {
                    glTexCoord2f(x,z);
                    glVertex3f(x,0,-z);

                    glTexCoord2f(x+d,z);
                    glVertex3f(x+d,0,-z);

                    glTexCoord2f(x+d,z+d);
                    glVertex3f(x+d,0,-(z+d));

                    glTexCoord2f(x,z+d);
                    glVertex3f(x,0,-(z+d));
                }
            }

            glEnd();
        }
        void init() {
            Shape::init();
            width = height = 1.0f;
            castShadow = false;
        }
    public:
        Plane() {init();}
        float getWidth() const {return width;}
        void setWidth(float w) {width = w;}
        float getHeight() const {return height;}
        void setHeight(float h) {height = h;}
};

class Pyramid: public Shape {
    private:
        float baseWidth, height;
    protected:
        void doScale() {glScalef(baseWidth, height, baseWidth);}
        void drawVertexes() {
            glBegin(GL_TRIANGLE_FAN);
                glNormal3f(0,1,0);
                glVertex3f(0.0f,  1.0f, 0.0f);
                glVertex3f(-1.0f, 0, 1.0f);
                glVertex3f( 1.0f, 0, 1.0f);
                glVertex3f( 1.0f, 0, -1.0f);
                glVertex3f( -1.0f, 0, -1.0f);
                glVertex3f(-1.0f, 0, 1.0f);
            glEnd();

            glBegin(GL_QUADS);
                glVertex3f(-1.0f, 0, 1.0f);
                glVertex3f( -1.0f, 0, -1.0f);
                glVertex3f( 1.0f, 0, -1.0f);
                glVertex3f( 1.0f, 0, 1.0f);
            glEnd();
        }
        void init() {
            Shape::init();
            height = 1.0f;
            baseWidth = 1.0f;
        }
    public:
        Pyramid(){init();}
        float getBaseWidth() const {return baseWidth;}
        void setBaseWidth(const float w) {baseWidth = w;}
        float getHeight() const {return height;}
        void setHeight(const float h) {height = h;}


};

class Cone : public Shape {
    private:
        float R, height;
    protected:
        void doScale() {glScalef(R,height,R);}
        void drawVertexes() {
            glBegin(GL_QUADS);
              for(float s=0;s<2*M_PI;s+=d) {
                for(float t=0;t<1.0;t+=d) {
                        float x = t * cosf(s);
                        float z = -t * sinf(s) ;
                        float y = t;
                        glNormal3f(x,y,z);
                        glVertex3f(x,y,z);

                        x = t * cosf(s+d);
                        z = -t * sinf(s+d) ;
                        y = t;
                        glNormal3f(x,y,z);
                        glVertex3f(x,y,z);

                        x = (t+d) * cosf(s+d);
                        z = -(t+d) * sinf(s+d) ;
                        y = (t+d);
                        glNormal3f(x,y,z);
                        glVertex3f(x,y,z);

                        x = (t+d) * cosf(s);
                        z = -(t+d) * sinf(s) ;
                        y = (t+d);
                        glNormal3f(x,y,z);
                        glVertex3f(x,y,z);

                    }
                }
            glEnd();

            drawCircle(1,1);
        }
        void init() {
            Shape::init();
            R = 1.0f;
            height=1.0f;
        }
    public:
        Cone() {init();}
        void setR(const float r) {R = r;}
        void setHeight(const float h) {height = h;}
};

class Leg : public Shape {
    private:
        Cylinder thigh;
        Cylinder shin;
        Cylinder foot;
        Sphere knee;
        Sphere ankle;

    protected:
        void init() {
            Shape::init();

            this->setKa(0.1,0.1,0.1);
            this->setKd(1,1,0);
            this->setKs(0.2,0.2,0.2);
            this->setShine(5);

            thigh.setMaterial(*this);
            knee.setMaterial(*this);
            shin.setMaterial(*this);
            ankle.setMaterial(*this);
            foot.setMaterial(*this);

            thigh.setP0(Vector(0,0,0));
            thigh.setAngle(40);
            thigh.setRotateDir(Vector::Z());
            thigh.setR(.2);
            thigh.setH(2);

            knee.setR(.3);

            shin.setP0(Vector(0,0,0));
            shin.setAngle(140);
            shin.setRotateDir(Vector::Z());
            shin.setR(.2);
            shin.setH(1.5);

            ankle.setP0(Vector(-1, -1.2, 0) );
            ankle.setR(0.3);

            foot.setP0(Vector(-1.25,-1.2,0));
            foot.setAngle(90);
            foot.setRotateDir(Vector::Z());
            foot.setR(.2);
            foot.setH(1);

        }
        virtual void drawVertexes() {}
        void drawSubParts() {
            thigh.draw();
            knee.draw();
            shin.draw();
            ankle.draw();
            foot.draw();
        }
    public:
        Leg() {init();}
};

class Head : public Shape {
    private:
        Sphere leftEye, rightEye, head;
        Cylinder neck;
        Cone beak;
        Cone taraj1, taraj2, taraj3;
    protected:
        void drawVertexes() {}
        void init() {
            Shape::init();

            this->setKa(0.1,0.1,0.1);
            this->setKd(1,1,0);
            this->setKs(0.2,0.2,0.2);
            this->setShine(5);

            head.setMaterial(*this);
            head.setP0(P0);
            head.setRotateDir(Vector::Y());

            leftEye.setKa(0.1,0.1,0.1);
            leftEye.setKd(0.1,0.2,0.9);
            leftEye.setKs(0.1,0.1,0.3);
            leftEye.setShine(8);

            rightEye.setMaterial(leftEye);

            beak.setKa(0.1,0.1,0.1);
            beak.setKd(0.9,0.2,0.3);
            beak.setKs(0.3,0.1,0.05);
            beak.setShine(15);

            taraj1.setMaterial(beak);
            taraj2.setMaterial(beak);
            taraj3.setMaterial(beak);

            neck.setMaterial(*this);

            float r = 1;
            head.setR(r);
            leftEye.setR(r/5);
            leftEye.setP0(0.4, 0.5, 0.9);

            rightEye.setR(r/5);
            rightEye.setP0(-0.4, 0.5, .9);

            beak.setRotateDir(Vector::X());
            beak.setAngle(270);
            beak.setR(r/5);
            beak.setHeight(1);
            beak.setP0(0,0,2);

            taraj1.setP0(0,1.55,-0.9);
            taraj1.setR(r/5);
            taraj1.setHeight(0.8);
            taraj1.setRotateDir(Vector::X());
            taraj1.setAngle(-210);

            taraj2.setP0(0,1.8,0);
            taraj2.setR(r/5);
            taraj2.setHeight(0.8);
            taraj2.setAngle(180);
            taraj2.setRotateDir(Vector::X());

            taraj3.setP0(0,1.5,0.9);
            taraj3.setR(r/5);
            taraj3.setHeight(0.8);
            taraj3.setRotateDir(Vector::X());
            taraj3.setAngle(205);

            neck.setH(1);
            neck.setR(0.35);
            neck.setP0(0,-1.8,0);
            neck.setAngle(0);
            neck.setRotateDir(Vector::X());
        }
        void drawSubParts() {
            head.draw();
            leftEye.draw();
            rightEye.draw();
            beak.draw();
            neck.draw();
            taraj1.draw();
            taraj2.draw();
            taraj3.draw();
        }
    public:
        Head() {init();}
};

class Chicken : public Shape {
    private:
        float headAnimDir;
        Leg leftLeg, rightLeg;
        Sphere body;
        Head head;
        Cone tail;
    protected:
        void drawVertexes() {}
        void init(){
            Shape::init();

            headAnimDir = 1.0f;

            this->castShadow = true;
            this->setKa(0.1,0.1,0.1);
            this->setKd(1,1,0);
            this->setKs(0.2,0.2,0.2);
            this->setShine(5);

            size = 1.0f;

            body.setMaterial(*this);
            body.setR(1.5);

            head.setP0(0,3,0.8);
            head.setRotateDir(Vector::Y());

            leftLeg.setP0(-0.8,-2.5,-1);
            leftLeg.setRotateDir(Vector::Y());
            leftLeg.setAngle(90);

            rightLeg.setP0(0.8,-2.5,-1);
            rightLeg.setRotateDir(Vector::Y());
            rightLeg.setAngle(90);

            tail.setKa(0.1,0.1,0.1);
            tail.setKd(0.9,0.5,0.3);
            tail.setKs(0.3,0.1,0.05);
            tail.setShine(15);
            tail.setP0(0,0.5,-1);
            tail.setR(0.5);
            tail.setHeight(2);
            tail.setRotateDir(Vector::X());
            tail.setAngle(-60);
        }

        void drawSubParts() {
            leftLeg.draw();
            rightLeg.draw();
            head.draw();
            body.draw();
            tail.draw();
        }
    public:
        Chicken() {init();}
        void animate(float dt) {
            float a = head.getAngle();
            if (a > 45.0f) {headAnimDir = -1.0f;}
            if (a < -45.0f) {headAnimDir = 1.0f;}
            a+=(dt/100)*headAnimDir;
            head.setAngle(a);
        }
        Vector getViewDir() {
            Vector v(head.getP0().getX(),head.getP0().getY(),head.getP0().getZ()+1);
            v = v - head.getP0();
            float a = head.getAngle() * (M_PI/180) *-1.0f;
            float x1 = v.getX() * cosf(a) - v.getZ() * sinf(a);
            float z1 = v.getX() * sinf(a) + v.getZ() * cosf(a);
            return Vector(x1,v.getY(),z1);
        }

};

enum State {
    Speed, Slow, Steady
};

class RoadRoller : public Shape {
    private:
        Cylinder frontWheel, backWheel;
        Cuboid frontFork, backFork;
        Cuboid chassis, tummy;
        Cuboid frontWindow, backWindow;
        Cuboid roof;
        Cylinder chimney;

        float a;
        Vector v;
        State state;

    protected:
        void drawVertexes(){}
        void init() {
            Shape::init();

            a = 0.1f;
            state = Steady;

            this->castShadow = true;

            this->setKa(0.1,0.1,0.1);
            this->setKd(0.2,0.2,0.2);
            this->setKs(0.9,0.9,0.9);
            this->setShine(30);

            frontWheel.setKa(0.1,0.1,0.1);
            frontWheel.setKd(0.9,0.9,0.9);
            frontWheel.setKs(0.1,0.1,0.1);
            frontWheel.setShine(5);


            backWheel.setMaterial(frontWheel);
            frontFork.setMaterial(*this);
            backFork.setMaterial(*this);
            chassis.setMaterial(*this);
            tummy.setMaterial(*this);
            frontWindow.setMaterial(*this);
            backWindow.setMaterial(*this);
            roof.setMaterial(*this);
            chimney.setMaterial(*this);

            frontWheel.setH(4);
            frontWheel.setR(2);
            frontWheel.setRotateDir(Vector::X());
            frontWheel.setAngle(-90);
            frontWheel.setP0(-5, -2, 0);

            frontFork.setWidth(5);
            frontFork.setHeight(2.5);
            frontFork.setDepth(0.5);
            frontFork.setRotateDir(Vector::Y());
            frontFork.setAngle(90);
            frontFork.setP0(-4.7,-2,0.5);


            backWheel.setH(4);
            backWheel.setR(2);
            backWheel.setRotateDir(Vector::X());
            backWheel.setAngle(-90);
            backWheel.setP0(5, -2, 0);

            backFork.setWidth(5);
            backFork.setHeight(2.5);
            backFork.setDepth(0.5);
            backFork.setRotateDir(Vector::Y());
            backFork.setAngle(90);
            backFork.setP0(5.2,-2,0.5);

            chassis.setWidth(12);
            chassis.setHeight(0.5);
            chassis.setDepth(3.8);
            chassis.setP0(-6, 0.5, 0);

            tummy.setDimensions(4.2,3.5,3.8);
            tummy.setP0(-2.2,-2.6,0);

            frontWindow.setDimensions(0.5, 3, 3.8);
            frontWindow.setP0(-2.5,1,0);

            backWindow.setDimensions(0.5, 3, 3.8);
            backWindow.setP0(2.5,1,0);

            roof.setP0(-3.5, 4, 0);
            roof.setDimensions(7.5,0.5,3.8);

            chimney.setH(6);
            chimney.setR(0.3);
            chimney.setP0(-5,0,-1);



        }
        void drawSubParts() {
            frontWheel.draw();
            frontFork.draw();

            backWheel.draw();
            backFork.draw();

            chassis.draw();
            tummy.draw();
            frontWindow.draw();
            backWindow.draw();
            roof.draw();

            chimney.draw();

        }
    public:
        RoadRoller(){init();}
        void setWheelTexture(const unsigned int id) {
            backWheel.setTexturazott(true);
            backWheel.setTexId(id);
            frontWheel.setTexturazott(true);
            frontWheel.setTexId(id);
        }

        void setState(State s) {
            this->state = s;
        }

        void animate(float dt) {
            float vx = 0.0f;
            switch(state) {
                case Speed:
                    vx = v.getX() + a*dt;
                break;
                case Slow:
                    vx = v.getX() - a*dt;
                break;
                case Steady:
                    vx = v.getX();
                break;
            }
            v.set(vx,v.getY(),v.getZ());
            P0 -= v;
        }
};

class Camera {
    public:
        float ref,r;
        Vector center;
        Vector pos, dir, up;
        Camera() {
            up = Vector::Y();
            ref = 0.0f;
        }

        void animate(float dt) {
            r = (center-pos).length();
            float v = dt/1500.0f;
            ref += v;
            pos.setX(cosf(ref)*r + center.getX());
            pos.setZ(sinf(ref)*r + center.getZ());
            dir = (center - pos);
        }
};

int currentChicken = 0;
class WorldView {
    public:
        Chicken** chickens;
        RoadRoller* r;
        Plane* ground;
        Plane* road;
        Camera cam;

        float x,y,width,height;

        WorldView() {
            x=y=width=height=0.0f;
            cam.pos.set(0,5,0);
            cam.dir.set(0,0,-1);
        }

        void draw() {

            glViewport(x,y,width,height);
            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            gluPerspective(60.0,width/height,0.1,100.0);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            gluLookAt( cam.pos.getX(), cam.pos.getY(), cam.pos.getZ(),
                   cam.pos.getX() + cam.dir.getX(), cam.pos.getY() + cam.dir.getY(), cam.pos.getZ() + cam.dir.getZ(),
                   cam.up.getX(), cam.up.getY(), cam.up.getZ() );

            ground->draw();
            road->draw();
            r->draw();
            for(int i=0;i<3;++i) {
                chickens[i]->draw();
            }
        }

        void adjustCamToRoadRoller() {
            cam.pos.set(r->getP0().getX()-1.2, r->getP0().getY()+1, r->getP0().getZ()-1);
        }

        void adjustCamToChicken(int i) {
            Vector v(chickens[currentChicken]->getP0().getX(),
                     chickens[currentChicken]->getP0().getY()+1.3,
                     chickens[currentChicken]->getP0().getZ());//+.85);

            cam.pos = chickens[currentChicken]->getViewDir() + v;
            cam.dir = chickens[currentChicken]->getViewDir();
        }

};

Chicken c1,c2,c3;

Chicken* chicken[3];
RoadRoller r;
Plane ground;
Plane road;

Camera cam;

WorldView v1,v2,v3,vref;

float *sunColor,sunPos[4];

float noise (int x, int y) {
    int n=x+y*57;
    n=(n<<13)^n;
    int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
    return 1.0-((float)nn/1073741824.0);
}

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {

    vref.cam.pos.set(-3,5,-10);
    vref.cam.dir.set(0,0,-1);

    int level = 0, border = 0, width = 256, height = 256;
	unsigned char image[256][256][3];
	unsigned int tex[2];
    glGenTextures(2,tex);

    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    for (int x=0;x<width;++x) {
        for (int y=0; y<height;++y) {
            float n = noise(x,y);
            image[x][y][0] = 56 - n*10;
            image[x][y][1] = 56 - n*50;
            image[x][y][2] = 56 - n*15;
        }
    }
    glTexImage2D( GL_TEXTURE_2D, level, GL_RGB, width, height, border,
              GL_RGB, GL_UNSIGNED_BYTE, image );

    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    for (int x=0;x<width;++x) {
        for (int y=0; y<height;++y) {
            float n = noise(x,y);
            image[x][y][0] = 210 - n*20;
            image[x][y][1] = 210 - n*50;
            image[x][y][2] = 210 - n*10;
        }
    }
    glTexImage2D( GL_TEXTURE_2D, level, GL_RGB, width, height, border,
              GL_RGB, GL_UNSIGNED_BYTE, image );

	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_NORMALIZE );

	glShadeModel(GL_SMOOTH);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(60.0,1.0,0.1,100.0);

    chicken[0] = &c1;
    chicken[0]->setP0(-5,1.6,-23);
    chicken[0]->setRotateDir(Vector::Y());
    chicken[0]->setAngle(0);
    chicken[0]->setSize(0.4);

    chicken[1] = &c2;
    chicken[1]->setP0(-7,1.6,-23);
    chicken[1]->setRotateDir(Vector::Y());
    chicken[1]->setAngle(0);
    chicken[1]->setSize(0.4);

    chicken[2] = &c3;
    chicken[2]->setP0(-9,1.6,-23);
    chicken[2]->setRotateDir(Vector::Y());
    chicken[2]->setAngle(0);
    chicken[2]->setSize(0.4);

    r.setP0(5,1.99,-17.5);
    r.setRotateDir(Vector(0,1,0));
    r.setAngle(0);
    r.setSize(0.5);
    r.setWheelTexture(tex[1]);

    ground.setKa(0.1,0.5,0.1);
    ground.setKd(0.3, 0.7, 0.4);
    ground.setKs(0.1,0.1,0.1);
    ground.setShine(2);
    ground.setP0(0,zeroLevel-0.1,0);
    ground.setWidth(50);
    ground.setHeight(50);

    road.setKa(0.1,0.1,0.1);
    road.setKd(0.1, 0.1, 0.1);
    road.setKs(0.1,0.1,0.1);
    road.setShine(2);
    road.setP0(0,zeroLevel+0.005,-18);
    road.setWidth(50);
    road.setHeight(5);
    road.setRotateDir(Vector::Y());
    road.setAngle(0);
    road.setTexId(tex[0]);
    road.setTexturazott(true);

    vref.chickens=chicken;
    vref.road = &road;
    vref.ground = &ground;
    vref.r = &r;
    vref.width = W_WIDHT; vref.height=W_HEIGHT/2;

    v1.chickens=chicken;
    v1.road = &road;
    v1.ground = &ground;
    v1.r = &r;
    v1.x=0;v1.y=0;
    v1.width=W_WIDHT/2.0f;
    v1.height=W_HEIGHT/2.0f;
    v1.cam.pos.set(r.getP0().getX()-1.2, r.getP0().getY()+1, r.getP0().getZ()-1);
    v1.cam.dir.set(-1,0,0);

    v2.chickens=chicken;
    v2.road = &road;
    v2.ground = &ground;
    v2.r = &r;
    v2.x=0;v2.y=300;
    v2.width=W_WIDHT;
    v2.height=W_HEIGHT/2.0f;
    v2.cam.pos.set(chicken[0]->getP0().getX(),
                   chicken[0]->getP0().getY()+1.3,
                   chicken[0]->getP0().getZ()+.85);
    v2.cam.dir.set(0,0,1);

    v3.chickens=chicken;
    v3.road = &road;
    v3.ground = &ground;
    v3.r = &r;
    v3.x=300;v3.y=0;
    v3.width=W_WIDHT/2.0f;
    v3.height=W_HEIGHT/2.0f;
    v3.cam.center.set(-7,4,-23);
    v3.cam.pos.set(-7,4,-27);
    v3.cam.dir.set(0,0,1);

    float sunColor[]={0,0,0,1};
    sunPos = { lightPos.getX(),lightPos.getY(),lightPos.getZ(),0 };
    float ambient[] = {0.1,0.1,0.1,1};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunColor);
	glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sunColor);

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

    v1.draw();
    v2.draw();
    v3.draw();
//    vref.draw();
    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet
    if (key == 27) exit(0);

    if (key == 'f') { r.setState(Speed);}
    if (key == 'n') { r.setState(Steady);}
    if (key == 'b') { r.setState(Slow);}


}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN);  // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
}

float last_changed = 0;
float daystart = 0;
float day = 1.0f;

float sunRed[] = {1,0.73,0,1};
float sunWhite[] = {1,1,1,1};
float sunDark[] = {0,0.015,0.3,1};

float ref = 0;

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
    long time = glutGet(GLUT_ELAPSED_TIME);
    float dt = time - last_changed;
    if (dt > 20.0f) {

        r.animate(dt/1000.0f);
        for (int i=0;i<3;++i) {
            chicken[i]->animate(dt);
        }

        v1.adjustCamToRoadRoller();
        v2.adjustCamToChicken(currentChicken);
        v3.cam.animate(dt);
        last_changed = time;
    }

    if (daystart + 1000 > time) {
        sunColor = sunRed;
    } else if (daystart + 5000 > time) {
        sunColor = sunWhite;
    } else if (daystart + 6000 > time) {
        sunColor = sunRed;
    } else if (daystart + 10000 > time) {
        sunColor = sunDark;
    } else {
        sunColor = sunRed;
        daystart = time;
    }
    Vector center;
    Vector pos(sunPos[0],sunPos[1],sunPos[2]);
    float r = (center-pos).length();
    float v = dt/2000.0f;
    ref += v;
    pos.setX(cosf(ref)*r + center.getX());
    pos.setY(sinf(ref)*r + center.getY());
    sunPos[0] = pos.getX();
    sunPos[1] = pos.getY();

    glClearColor(sunColor[0],sunColor[1],sunColor[2], 1.0f);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunColor);
    glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunColor);

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
