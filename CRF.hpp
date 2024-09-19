#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <windows.h>

namespace statusIgre{
    class igra{
        private:
            int potekIgre, x_res, y_res;
            float FOVx2, zPloskve;

        private:
            static float izracunZPloskve(int x_res, float FOVx2){return (x_res/2)/(2*tan(FOVx2));}

        public:
        
            // FOVx2 je polovica FOV v x smeri (v radianih).
            igra(): potekIgre(1), x_res(100), y_res(100), FOVx2(45.0f) {zPloskve=izracunZPloskve(x_res, FOVx2);}
            // FOVx2 je polovica FOV v x smeri (v radianih).
            igra(int statusIgre, int x_res, int y_res, float FOVx2): potekIgre(potekIgre), x_res(x_res), y_res(y_res), FOVx2(FOVx2){zPloskve=izracunZPloskve(x_res, FOVx2);} 
        
        public:

            int resolucijaX() const {return x_res;}
            int resolucijaY() const {return y_res;}

            // Dolocevanje resolucije (100x100 default).
            void dolociResolucijo(int x, int y) {x_res=x; y_res=y;}

            // Negira status igre (0->1/1->0). Uporabno za izstop iz while loop-a.
            void negirajStatus() {potekIgre=(potekIgre?0:1);}

            int status() const {return potekIgre;}

            float FOV() const {return FOVx2;}

            float ZP(){return zPloskve;}
    };
}

namespace primitivi{
    
    // Struct točke v 3 dimenzijah.
    struct tocka3D {
        
        float x, y, z;
        
        tocka3D(): x(0), y(0), z(0) {}
        tocka3D(float x, float y, float z): x(x), y(y), z(z) {}

        tocka3D operator+ (tocka3D& tocka) {return tocka3D(x+tocka.x, y+tocka.y, z+tocka.z);}
        tocka3D operator- (tocka3D& tocka) {return tocka3D(x-tocka.x, y-tocka.y, z-tocka.z);}
        
    };

    // Večinoma se uporablja kot način shranjevanja ploskev.
    class vektorTock3D{
        private:
            std::vector<primitivi::tocka3D> tocke;

        public:

            vektorTock3D()=default;

            tocka3D operator[] (const size_t idx) const {return tocke[idx];}
            void operator+= (const tocka3D tocka) {tocke.emplace_back(tocka);}

            std::vector<primitivi::tocka3D>::iterator begin() {return tocke.begin();}
            std::vector<primitivi::tocka3D>::iterator end() {return tocke.end();}

        public:

            void pocistiTocke() {tocke.clear();}

            std::vector<primitivi::tocka3D> pridobiTocke() const {return tocke;}

            size_t pridobiN() const {return tocke.size();}
    };

    // Shranjevanje ploskev ==> objekt.
    class objekt{
        private:
            std::vector<vektorTock3D> ploskevobjekta;

        public:
            void operator+=(vektorTock3D ploskev) {ploskevobjekta.emplace_back(ploskev);}

            std::vector<vektorTock3D>::iterator begin() {return ploskevobjekta.begin();}
            std::vector<vektorTock3D>::iterator end() {return ploskevobjekta.end();}
    };
}

namespace renderanje{

    class bufferArray{
        private:
            int x, y;
            char* buffer;

        public:
            // Ustvari array char-ov z dimenzijama (x, y). Uporablja tudi destructor zaradi "new char[...]".
            bufferArray(int x, int y):x(x), y(y) {buffer=new char[x*y+y];}
            ~bufferArray() {delete[] buffer;}

            char& operator[] (size_t idx) const {return buffer[idx];}

        public:

            char* pridobiBuffer() const {return buffer;}

            size_t velikost() const {return x*y+y;}

            void bufferMemset(char znak){memset(this->buffer, znak, velikost());}

            void postaviPiksel (char znak, int x, int y, int sirina, int visina){
                if (x>=-(sirina/2)&&y>=-(visina/2)&&x<(sirina/2)&&y<(visina/2)){
                    buffer[(y+visina/2)*(sirina+1)+(x+sirina/2)]='*';
                }
            }
    };

    class renderanjePrimitiv{
        public:

            // Brensenhamov algoritem z int-i. (Koordinate z 0 in 1 sta začetna in končna točka).
            static void renderLinij(int x0, int y0, int x1, int y1, renderanje::bufferArray& buffer, int sirina, int visina){

                int dx=abs(x1-x0),sx=x0<x1?1:-1;
                int dy=-abs(y1-y0),sy=y0<y1?1:-1; 
                int err=dx+dy, e2;

                for(;;){
                    buffer.postaviPiksel('.', x0, y0, sirina, visina);
                    if(x0==x1&&y0==y1)break;
                    e2=2*err;
                    (e2>=dy)?(err+=dy,x0+=sx):(err+=dx,y0+=sy);
                }
            }

            // Rendera ploskev (vektorTock3D).
            static void renderPloskve(primitivi::vektorTock3D& ploskev, renderanje::bufferArray& buffer, int sirina, int visina){
                for (size_t index=0; index<ploskev.pridobiN(); ++index){
                    float X1=-(ploskev[index%ploskev.pridobiN()].x),
                    Y1=-(ploskev[index%ploskev.pridobiN()].y),
                    X2=-(ploskev[(index+1)%ploskev.pridobiN()].x), 
                    Y2=-(ploskev[(index+1)%ploskev.pridobiN()].y);

                    renderLinij(X1,Y1,X2,Y2,buffer,sirina,visina);
                }
            }

            // Gre skozi vektor objektov. Sirina in visina se pridobi iz statusa.
            static void renderObjektov(std::vector<primitivi::objekt>& vektorObjektov, renderanje::bufferArray& buffer, statusIgre::igra& status){
                int sirina=status.resolucijaX();
                int visina=status.resolucijaY();

                for (primitivi::objekt& objekt:vektorObjektov){
                    for (primitivi::vektorTock3D& ploskev:objekt){
                        renderanje::renderanjePrimitiv::renderPloskve(ploskev, buffer, sirina, visina);
                    }
                }
            }

            // Uporaba za debugganje. Ni še dokončana funkcija.
            static void debugRenderObjektov(std::vector<primitivi::objekt>& vektorObjektov, renderanje::bufferArray& buffer, statusIgre::igra& status){
                for (primitivi::objekt& objekt:vektorObjektov){
                    for (primitivi::vektorTock3D& ploskev:objekt){
                        printf("BO ŠE DODANO!");
                    }
                }
            }
    };

    class projekcija{

        private:
            static void vstaviNewline(int visina, int sirina, renderanje::bufferArray& buffer) {for (size_t idy=0;idy<visina;++idy){buffer[(idy+1)*(sirina+1)-1]='\n';}}

        public:
            // Uporabi samo na zacetku, da zbriše začetni tekst, če obstaja.
            static void hardResetZaslon(statusIgre::igra& status, renderanje::bufferArray& buffer){
                buffer.bufferMemset(' ');

                printf("\033[1;1H\033[2J");

                vstaviNewline(status.resolucijaY(), status.resolucijaX(), buffer);
            }

            // Premakne cursor na (1,1). Je hitrejše of hardResetZaslon().
            static void pocistiZaslon(statusIgre::igra& status, renderanje::bufferArray& buffer){
                buffer.bufferMemset(' ');

                printf("\033[1;1H");

                vstaviNewline(status.resolucijaY(), status.resolucijaX(), buffer);
            }
    };
}

namespace transformacije{
    class clippanje{

        private:
            struct Presecisce{
                float x, y;
            }; 

        private:
            static Presecisce izracunPresecisca(primitivi::tocka3D& T1, primitivi::tocka3D& T2, float zPloskve){
                float deltaX=T2.x-T1.x;
                float deltaY=T2.y-T1.y;
                float deltaZ=T2.z-T1.z;
 
                return (Presecisce){((zPloskve-T2.z)/deltaZ)*deltaX+T2.x, ((zPloskve-T2.z)/deltaZ)*deltaY+T2.y};
            }

        public:
            // Kliči *za* transformacijo loadanih modelov (premiki, rotacije...) in *pred* projekcijsko transformacijo.
            static std::vector<primitivi::objekt> clipLinij(std::vector<primitivi::objekt>& vektorObjektov, statusIgre::igra& status){
                std::vector<primitivi::objekt> objektiZaReturn;
                int sirina=status.resolucijaX(), visina=status.resolucijaY();

                Presecisce tempTockaPresecisca;
                float ZP = 10.0f;//status.ZP();
                for (primitivi::objekt& objekt:vektorObjektov){

                    primitivi::objekt tempObjekt;
                    for (primitivi::vektorTock3D& ploskev:objekt){

                        primitivi::vektorTock3D ploskevTemp;
                        for (size_t index=0; index<ploskev.pridobiN(); ++index){
                            
                            primitivi::tocka3D T1=ploskev[index%ploskev.pridobiN()];
                            primitivi::tocka3D T2=ploskev[(index+1)%ploskev.pridobiN()];        

                            if (T1.z>=ZP&&T2.z>=ZP){
                                ploskevTemp+=T1;
                                ploskevTemp+=T2;
                            } else if (T1.z>=ZP&&T2.z<ZP){
                                tempTockaPresecisca=izracunPresecisca(T2, T1, ZP);
                                ploskevTemp+=primitivi::tocka3D(tempTockaPresecisca.x, tempTockaPresecisca.y, ZP);
                            } else if (T1.z<ZP&&T2.z>=ZP){
                                tempTockaPresecisca=izracunPresecisca(T1, T2, ZP);
                                ploskevTemp+=primitivi::tocka3D(tempTockaPresecisca.x, tempTockaPresecisca.y, ZP);
                            }
                        }
                        tempObjekt+=ploskevTemp;
                    }
                    objektiZaReturn.emplace_back(tempObjekt);
                }
                
                return objektiZaReturn;
            }
    };

    class projekcijskaTransformacija{
        private:

        public:
            static void izracunProjekcije(std::vector<primitivi::objekt>& vektorObjektov, statusIgre::igra& status){
                float z=status.ZP();
                for (primitivi::objekt& objekt:vektorObjektov){
                    for (primitivi::vektorTock3D& ploskev:objekt){
                        for (primitivi::tocka3D& tocka:ploskev){
                            float tx=tocka.x;
                            float ty=tocka.y;
                            float tz=tocka.z;
                            tocka=primitivi::tocka3D((tx*z/tz), (ty*z/tz), tz);
                        }
                    }
                }
            }
    };

    class premik{
        public:
            static void prem(std::vector<primitivi::objekt>& vektorObjektov, primitivi::tocka3D vektor){

                for (primitivi::objekt& objekt:vektorObjektov){
                    for (primitivi::vektorTock3D& ploskev:objekt){
                        for (primitivi::tocka3D& tocka:ploskev){
                            tocka.x+=vektor.x;
                            tocka.y+=vektor.y;
                            tocka.z+=vektor.z;
                        }
                    }
                }
            }
    };
}

namespace branjeInputa{
    class input{
    public:
        static void preverjanjeInputa(statusIgre::igra& status, std::vector<primitivi::objekt>& testT){

            // ESC
            if (GetAsyncKeyState(27)&0x8000){
                status.statusIgre::igra::negirajStatus();
            }
            // W
            else if (GetAsyncKeyState(87)&0x8000){
                transformacije::premik::prem(testT, primitivi::tocka3D(0, -1, 0));
            }
            // S
            else if (GetAsyncKeyState(83)&0x8000){
                transformacije::premik::prem(testT, primitivi::tocka3D(0, 1, 0));
            }
            // A
            else if (GetAsyncKeyState(65)&0x8000){
                transformacije::premik::prem(testT, primitivi::tocka3D(-1, 0, 0));
            }
            // D
            else if (GetAsyncKeyState(68)&0x8000){
                transformacije::premik::prem(testT, primitivi::tocka3D(1, 0, 0));
            }
            // T
            else if (GetAsyncKeyState(84)&0x8000){
                transformacije::premik::prem(testT, primitivi::tocka3D(0, 0, -2));
            }
            // R
            else if (GetAsyncKeyState(82)&0x8000){
                transformacije::premik::prem(testT, primitivi::tocka3D(0, 0, 2));
            }
        }
    };
}

namespace nalagalnik{
    class nalaganjeTock{
        public:
            static primitivi::objekt naloziOBJ(const std::string& pot){

                std::ifstream obj_datoteka(pot);
                std::string vrstica;

                // Inicializirani arraji primitiv.
                primitivi::vektorTock3D vektorTock;
                primitivi::objekt objekt;

                while (std::getline(obj_datoteka, vrstica)){
                    std::istringstream iss(vrstica);
                    std::string tip_podatka;

                    // Pridobi prvi znak v vrstici.
                    iss >> tip_podatka;

                    // "If" hranjuje točke, "else if" pa ploskve.
                    if (tip_podatka=="v"){
                        float x, y, z;
                        iss >> x >> y >> z;

                        // Množenje dodano samo zato ker se modeli še ne morejo transformirati popolnoma. Funkcija še bo dodana.
                        vektorTock+=primitivi::tocka3D(100.0f*x, 100.0f*y, 100.0f*z);
                    } else if (tip_podatka=="f"){

                        std::string delStringa;
                        primitivi::vektorTock3D ploskev;

                        while (iss >> delStringa) {

                            // Išče "/", ki ločuje številke.
                            size_t pos=delStringa.find('/');
                            
                            // Dokler ni konec stringa.
                            if (pos!=std::string::npos) {
                                std::string numberStr=delStringa.substr(0, pos);

                                // Typecast v int.
                                ploskev+=vektorTock[std::stoi(numberStr)-1];
                            }
                        }

                        // Objektu se doda ploskev.
                        objekt+=ploskev;
                    }
                }

                return objekt;
            }
    };
}
