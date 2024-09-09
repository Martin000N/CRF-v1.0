#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <windows.h>

#define epsilon 0.3f

namespace statusIgre{
    class igra{

        public:
            // Privzeta konstruktorja.
            igra():potekIgre(1), x_res(100), y_res(100), xSmer(0), ySmer(0), zSmer(0){}
            igra(int statusIgre, int x_res, int y_res):potekIgre(potekIgre), x_res(x_res), y_res(y_res), xSmer(xSmer), ySmer(ySmer), zSmer(zSmer){} 
        
        public:

            int pridobiX(){return x_res;}
            int pridobiY(){return y_res;}
            // Dolocevanje resolucije (100x100 default).
            void dolociResolucijo(int x, int y){x_res=x; y_res=y;}

            // Vrne status igre.
            int pridobiStatus() const {return potekIgre;}

            // Negira status igre (0->1/1->0).
            void negirajStatus(){potekIgre=(potekIgre?0:1);}

            float getzs(){return zSmer;}
            float getxs(){return xSmer;}
            float getys(){return ySmer;}

            void xSmerS(float s){xSmer+=s;}
            void ySmerS(float s){ySmer+=s;}
            void zSmerS(float s){zSmer+=s;}
        private:
            int potekIgre, x_res, y_res;
            float xSmer, ySmer, zSmer;
    };
}

namespace primitivi{
    
    class tocka3D{
        public:
            tocka3D():x(0), y(0), z(0){}
            tocka3D(float x, float y, float z):x(x), y(y), z(z){}
            void operator+= (const tocka3D& tocka) {x+=tocka.x; y+=tocka.y; z+=tocka.z;}

        public:
            // Navadne funkije za obdelovanje posameznih atributov.
            float pridobiX() const {return x;}
            float pridobiY() const {return y;}
            float pridobiZ() const {return z;}

            void spremeniX(float x) {this->x=x;}
            void spremeniY(float y) {this->y=y;}
            void spremeniZ(float z) {this->z=z;}

            void spremeniKoordinate(tocka3D tocka) {(*this)=tocka;}

            // dodal bom kvaternione.
            tocka3D rotX(float faktor) {return tocka3D(this->x, this->y*cos(faktor)-this->z*sin(faktor), this->y*sin(faktor)+this->z*cos(faktor));}
            tocka3D rotY(float faktor) {return tocka3D(this->x*cos(faktor)+this->z*sin(faktor), this->y, -this->x*sin(faktor)+this->z*cos(faktor));}
            tocka3D rotZ(float faktor) {return tocka3D(this->x*cos(faktor)-this->y*sin(faktor), this->x*sin(faktor)+this->y*cos(faktor), this->z);}
        
        private:
            float x, y, z;};

    class vektorTock3D{
        private:
            std::vector<tocka3D> tocke;

        public:
            vektorTock3D()=default;
            tocka3D operator[] (const size_t idx) const {return tocke[idx];}
            void operator+=(const tocka3D tocka) {tocke.emplace_back(tocka);}
            std::vector<tocka3D>::iterator begin() {return tocke.begin();}
            std::vector<tocka3D>::iterator end() {return tocke.end();}

        public:
            // Pocisti tocke.
            void pocistiTocke() {tocke.clear();}
            std::vector<tocka3D> pridobiTocke() const {return tocke;}
            size_t pridobiN() const {return tocke.size();}
    };

    class model{
        private:
            std::vector<vektorTock3D> ploskevModela;

        public:
            void operator+=(vektorTock3D ploskev) {ploskevModela.emplace_back(ploskev);}

            // Uporablja se za iteriranje v for loopih.
            std::vector<vektorTock3D>::iterator begin() {return ploskevModela.begin();}
            std::vector<vektorTock3D>::iterator end() {return ploskevModela.end();}
    };
}

namespace renderanje{

    class bufferArray{

        public:
            bufferArray(int x, int y):x(x), y(y) {buffer=new char[x*y+y];}
            char& operator[] (size_t idx) {return buffer[idx];}
            ~bufferArray(){for(int i=0; i<y; ++i){delete[] buffer;}}

        public:
            char* pridobiBuffer() const {return buffer;}
            size_t velikost() const {return x*y+y;}
            void bufferMemset(char znak){
                for (size_t idx=0; idx<x*y+y; ++idx){
                    this->buffer[idx]=znak;
                }
                //memset(buffer, znak, sizeof(buffer));
            }
        
        private:
            int x, y;
            char* buffer;
    };
    
/*
================================================
Zbuffer array je int*, zato ne bo točen.
================================================
*/
    class zBufferArray{

        public:
            zBufferArray(int x, int y):x(x), y(y) {buffer=new int[x*y+y];}
            int& operator[] (size_t idx) {return buffer[idx];}
            ~zBufferArray(){for(int i=0; i<y; ++i){delete[] buffer;}}

        public:
            size_t velikost() const {return x*y+y;}
            void zBufferMemset(int znak){
                memset(buffer, znak, sizeof(buffer));
            }

        private:
            int x, y;
            int* buffer;
    };
/*
================================================
Zbuffer array je int*, zato ne bo točen.
================================================
*/

    class renderanjePrimitiv{
        public:
            static void renderPloskev(primitivi::vektorTock3D& ploskev, renderanje::bufferArray& buffer, renderanje::zBufferArray& zBuffer, statusIgre::igra& status){
                for (size_t index=0; index<ploskev.pridobiN(); ++index){
                    if (!(ploskev[index%ploskev.pridobiN()].pridobiZ()<0||ploskev[(index+1)%ploskev.pridobiN()].pridobiZ()<0)){
                        float X1=(ploskev[index%ploskev.pridobiN()].pridobiX()), 
                        Y1=(ploskev[index%ploskev.pridobiN()].pridobiY()), 
                        X2=(ploskev[(index+1)%ploskev.pridobiN()].pridobiX()),  
                        Y2=(ploskev[(index+1)%ploskev.pridobiN()].pridobiY());

                        renderLinij(
                            X1,
                            Y1,
                            X2,
                            Y2,
                            buffer,
                            zBuffer,
                            status);
                    }
                }
            }

            // Brensenhamov algoritem z int-i.
            static void renderLinij(int x0, int y0, int x1, int y1, renderanje::bufferArray& buffer, renderanje::zBufferArray& zBuffer, statusIgre::igra& status){

                int dx=abs(x1-x0),sx=x0<x1?1:-1;
                int dy=-abs(y1-y0),sy=y0<y1?1:-1; 
                int err=dx+dy, e2;
                
                int sirina=status.pridobiX();
                int visina=status.pridobiY();

                for(;;){
                    if (x0>=-(sirina/2)&&y0>=-(visina/2)&&x0<(sirina/2)&&y0<(visina/2)){
                        buffer[(visina-(y0+visina/2))*(sirina+1)+(x0+sirina/2)+((visina-(y0+visina/2))==0?0:1)]='*';
                    }
                    if(x0==x1&&y0==y1)break;
                    e2=2*err;
                    (e2>=dy)?(err+=dy,x0+=sx):(err+=dx,y0+=sy);
                }
            }

            static void renderObjektov(std::vector<primitivi::model>& vektorObjektov, renderanje::bufferArray& buffer, renderanje::zBufferArray& zBuffer, statusIgre::igra& status){

                for (auto& model:vektorObjektov){
                    for (auto& ploskev:model){
                        renderanje::renderanjePrimitiv::renderPloskev(ploskev, buffer, zBuffer, status);
                    }
                }
            }           


    };

    class projekcija{

    public:
        // Uporabi samo na zacetku, da zbriše začetni tekst, če obstaja.
        static void hardResetZaslon(statusIgre::igra& status, renderanje::bufferArray& buffer, renderanje::zBufferArray& zBuffer){
            buffer.bufferMemset('.');
            zBuffer.zBufferMemset(10000000);

            printf("\033[1;1H\033[2J");

            vstaviNewline(status.pridobiX(), status.pridobiX(), buffer);
        }

        // Premakne cursor na (1,1). Je hitrejše of hardResetZaslon().
        static void pocistiZaslon(statusIgre::igra& status, renderanje::bufferArray& buffer, renderanje::zBufferArray& zBuffer){
            buffer.bufferMemset(' ');
            zBuffer.zBufferMemset(10000000);

            printf("\033[1;1H");

            vstaviNewline(status.pridobiX(), status.pridobiX(), buffer);
        }

        static void vstaviNewline(int visina, int sirina, renderanje::bufferArray& buffer){
            for (size_t idy=1;idy<=visina;++idy){
                buffer[idy*(sirina+1)]='\n';

            }
        }
    };
}

namespace rasterizacija{
    class transofrmacije{
        public:
            static void premik(std::vector<primitivi::model>& modeli, primitivi::tocka3D vektorPremika){
                for (primitivi::model& model:modeli){
                    for (primitivi::vektorTock3D& ploskve:model){
                        for (primitivi::tocka3D& tocka:ploskve){
                            tocka+=vektorPremika;
                        }
                    }
                }
                
            }

            static void clip(std::vector<primitivi::model>& modeli, statusIgre::igra& status){
                for (primitivi::model& model:modeli){
                    for (primitivi::vektorTock3D& ploskev:model){
                        for (size_t index=0; index<ploskev.pridobiN(); ++index){
                            primitivi::tocka3D T1=ploskev[index%ploskev.pridobiN()];
                            primitivi::tocka3D T2=ploskev[(index+1)%ploskev.pridobiN()];        

                            if (T1.pridobiZ()<0&&T2.pridobiZ()<0){
                                 continue;
                            } else if (T1.pridobiZ()<0){
                                Presecisce test=iskanjePresecisca(status, T1, T2);
                                T1.spremeniX(test.x);
                                T1.spremeniX(test.y);
                                T1.spremeniZ(0.1f);

                            } else if (T2.pridobiZ()<0){
                                Presecisce test=iskanjePresecisca(status, T2, T1);
                                T2.spremeniX(test.x);
                                T2.spremeniX(test.y);
                                T2.spremeniZ(0.1f);

                            }
                        }
                    }
                }
            }

            static void projekcijskaMat(std::vector<primitivi::model>& modeli){
                float zc=9.0f;
                for (primitivi::model& model:modeli){
                    for (primitivi::vektorTock3D& ploskve:model){
                        for (primitivi::tocka3D& tocka:ploskve){
                            float tx=tocka.pridobiX();
                            float ty=tocka.pridobiY();
                            float tz=(abs(tocka.pridobiZ())<epsilon)?0.1f:tocka.pridobiZ();
                            //printf("%f, %f, %f, %f, %f, %f\n", (tx*zc/tz), (ty*zc/tz), tx, ty, tz);
                            tocka = primitivi::tocka3D((tx*zc/tz), (ty*zc/tz), tz);
                        }
                    }
                }
            }

        private:
            struct Presecisce{
                float x;
                float y;
            }; 

            static Presecisce iskanjePresecisca(statusIgre::igra& status, primitivi::tocka3D T1, primitivi::tocka3D T2){
                float tx1=T1.pridobiX(), 
                ty1=T1.pridobiY(), 
                tz1=T1.pridobiZ(), 
                tx2=T2.pridobiX(), 
                ty2=T2.pridobiY(), 
                tz2=T2.pridobiZ();
                if (abs(tx2-tx1)<epsilon||abs(ty2-ty1)<epsilon||abs(tz2-tz1)<epsilon){
                    //printf("%f, %f,%f, %f\n",ty1, ty2, tx1, tx2);
                    return (Presecisce){tx1, ty1};
                } else{
                    int x_res=status.pridobiX(), y_res=status.pridobiY();
                    float k=(-tz1)/(tz2-tz1);
                    float x=(tx1+k*(tx2-tx1)), y=(ty1+k*(ty2-ty1));
                    //printf("drug: %f, %f\n", x,y);
                    return (Presecisce){x, y};
                }                  
            }
    };
}

namespace branjeInputa{
    class input{
    public:
        static void preverjanjeInputa(statusIgre::igra& status, std::vector<primitivi::model>& testT/*, std::unordered_map<int, std::any>* funkcijeZaPremikeIgralca, std::unordered_map<int, std::any>* funkcijeZaPremikePogleda*/){

            // Del naslednje posodobitve.
            //==================================================================================================
            /*// Pregleda vse tipke.
            for (int tipka=0; tipka<256; ++tipka){
                SHORT stanjeTipke=GetAsyncKeyState(tipka);

                // High-order bit, ki pove če je tipka stisnjena. Preveri tudi unordered-map za vrednost kljuca.
                if (stanjeTipke&0x8000) {
                    int asciiKoda=MapVirtualKey(tipka, MAPVK_VK_TO_CHAR);

                    // Preveri ce obstaja.
                    if (funkcijeZaPremikeIgralca->find(asciiKoda)!=funkcijeZaPremikeIgralca->end()){

                    }
                }
        
            }*/
            //===================================================================================================
            // Del naslednje posodobitve.

            // ESC
            if (GetAsyncKeyState(27)&0x8000){
                status.statusIgre::igra::negirajStatus();
            }/**/
            // W
            else if (GetAsyncKeyState(87)&0x8000){
                //status.ySmerS(1.0f);
                rasterizacija::transofrmacije::premik(testT, primitivi::tocka3D(0, -1, 0));
            }
            // S
            else if (GetAsyncKeyState(83)&0x8000){
                //status.ySmerS(-1.0f);
                rasterizacija::transofrmacije::premik(testT, primitivi::tocka3D(0, 1, 0));
            }
            // A
            else if (GetAsyncKeyState(65)&0x8000){
                //status.xSmerS(1.0f);
                rasterizacija::transofrmacije::premik(testT, primitivi::tocka3D(1, 0, 0));
            }
            // D
            else if (GetAsyncKeyState(68)&0x8000){
                //status.xSmerS(-1.0f);
                rasterizacija::transofrmacije::premik(testT, primitivi::tocka3D(-1, 0, 0));
            }
            // T
            else if (GetAsyncKeyState(84)&0x8000){
                //status.zSmerS(1.0f);
                rasterizacija::transofrmacije::premik(testT, primitivi::tocka3D(0, 0, -0.01));
            }
            // R
            else if (GetAsyncKeyState(82)&0x8000){
                //status.zSmerS(-1.0f);
                rasterizacija::transofrmacije::premik(testT, primitivi::tocka3D(0, 0, 0.01));
            }/*
            // E
            else if(GetAsyncKeyState(69)&0x8000){
                status.zSmerS(0.01f);
            }
            // Q
            else if(GetAsyncKeyState(81)&0x8000){
                status.zSmerS(-0.01f);
            }*/
        }
    };
}

namespace nalagalnik{
    class nalaganjeTock{
    public:
        static primitivi::model naloziOBB(const std::string& pot){

            std::ifstream obj_datoteka(pot);
            std::string vrstica;

            // Inicializirani arraji primitiv.
            primitivi::vektorTock3D vektorTock;
            primitivi::model ploskve;

            while (std::getline(obj_datoteka, vrstica)){
                std::istringstream iss(vrstica);
                std::string tip_podatka;
                iss >> tip_podatka;

                // Shranjuje vertexe.
                if (tip_podatka=="v"){
                    float x, y, z;
                    iss >> x >> y >> z;
                    vektorTock+=primitivi::tocka3D(10.0f*x, 10.0f*y, z+0.9f);
                }

                // Shranjuje ploskve. 
                else if (tip_podatka=="f"){

                    std::string zeton;
                    primitivi::vektorTock3D ploskev;

                    while (iss >> zeton) {
                        size_t pos=zeton.find('/');

                        if (pos!=std::string::npos) {
                            std::string numberStr=zeton.substr(0, pos);

                            // Typecast v int.
                            ploskev+=vektorTock[std::stoi(numberStr)-1];
                        }
                    }
                    ploskve+=ploskev;
                }
            }

            return ploskve;
        }
    };
}
