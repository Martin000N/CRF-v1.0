#include <windows.h>
#include <any>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>
#include <omp.h>

namespace statusIgre{
    class igra{
    private:
        int statusIgre;
        float xSmer, ySmer, zSmer, te3, te2;
        int x_res, y_res;
    public:
        // Privzeta konstruktorja.
        igra():statusIgre(1), x_res(100), y_res(100), xSmer(0.0f), ySmer(0.0f), zSmer(1.0f), te3(0.0f), te2(0.0f){}
        igra(int statusIgre, int x_res, int y_res, float xSmer, float ySmer, float zSmer, float te3):statusIgre(statusIgre), x_res(x_res), y_res(y_res), xSmer(xSmer), ySmer(ySmer), zSmer(zSmer), te3(te3), te2(te2){} 
    
    public:
        // Ni permanentno, samo za debugganje. Na koncu bo cistejsa koda.
        //==============================================================
        int getx(){return x_res;}
        int gety(){return y_res;}

        float getzs(){return zSmer;}
        float getxs(){return xSmer;}
        float getys(){return ySmer;}

        float gz(){return te3;}
        float gz2(){return te2;}

        void x(float s){xSmer+=s;}
        void y(float s){ySmer+=s;}
        void gzs(float s){te3+=s;}
        void gzs2(float s){te2+=s;}

        void z(float s){zSmer+=s;}

        //==============================================================
        // Ni permanentno, samo za debugganje. Na koncu bo cistejsa koda.


        // Dolocevanje resolucije (100x100 default).
        void dolociResolucijo(int x, int y){this->x_res=x; this->y_res=y;}

        // Vrne status igre.
        int pridobiStatus() const {return this->statusIgre;}

        // Negira status igre (0->1/1->0).
        void negirajStatus(){this->statusIgre=(this->statusIgre?0:1);}
    };
    
}

namespace branjeInputa{
    class input{
    public:
        static void preverjanjeInputa(statusIgre::igra* stanje, std::unordered_map<int, std::any>* funkcijeZaPremikeIgralca, std::unordered_map<int, std::any>* funkcijeZaPremikePogleda){
            // Pregleda vse tipke.
            for (int tipka=0; tipka<256; ++tipka){
                SHORT stanjeTipke=GetAsyncKeyState(tipka);

                // High-order bit, ki pove če je tipka stisnjena. Preveri tudi unordered-map za vrednost kljuca.
                if (stanjeTipke&0x8000) {
                    int asciiKoda=MapVirtualKey(tipka, MAPVK_VK_TO_CHAR);

                    // Preveri ce obstaja.
                    if (funkcijeZaPremikeIgralca->find(asciiKoda)!=funkcijeZaPremikeIgralca->end()){

                    }
                }
        
            }

            // Escape.
            if (GetAsyncKeyState(27)&0x8000) {
                stanje->negirajStatus();
            }
        }
    };
}

/*
    class kvaternion {
    private:
        float w, x, y, z;
    public:
        kvaternion(float kot, float vx, float vy, float vz) {
            w=cos(kot/2.0f);
            float sinpolovicniKot=sin(kot/2.0f);
            x=vx*sinpolovicniKot;
            y=vy*sinpolovicniKot;
            z=vz*sinpolovicniKot;
        }

        kvaternion operator*(const kvaternion& q) const {return kvaternion(w*q.w-x*q.x-y*q.y-z*q.z,w*q.x+x*q.w+y*q.z-z*q.y,w*q.y-x*q.z+y*q.w+z*q.x,w*q.z+x*q.y-y*q.x+z*q.w);}

        static kvaternion konjugiraj(kvaternion kv) {
            return kvaternion(kv.w, -kv.x, -kv.y, -kv.z);
        }

        static tocka3D rotate(kvaternion kv, float vx, float vy, float& vz) {
            kvaternion p(0, vx, vy, vz);
            kvaternion result=(kv)*p*konjugiraj(kv);
            return tocka3D(result.x, result.y, result.z);
        }
    };*/


namespace primitivi{
    
    class tocka3D{
    private:
        float x, y, z;

    public:
        tocka3D():x(0), y(0), z(0){}
        tocka3D(float x, float y, float z):x(x), y(y), z(z){}

    public:

        // Navadne funkije za obdelovanje posameznih atributov.
        float pridobiX() const {return x;}
        float pridobiY() const {return y;}
        float pridobiZ() const {return z;}

        void spremeniX(float x) {this->x=x;}
        void spremeniY(float y) {this->y=y;}
        void spremeniZ(float z) {this->z=z;}

        // Dereffrenca this, ker mora.
        void spremeniKoordinate(tocka3D tocka) {(*this)=tocka;}

        // dodal bom kvaternione.
        tocka3D rotX(float faktor) {return tocka3D(this->x, this->y*cos(faktor)-this->z*sin(faktor), this->y*sin(faktor)+this->z*cos(faktor));}
        tocka3D rotY(float faktor) {return tocka3D(this->x*cos(faktor)+this->z*sin(faktor), this->y, -this->x*sin(faktor)+this->z*cos(faktor));}
        tocka3D rotZ(float faktor) {return tocka3D(this->x*cos(faktor)-this->y*sin(faktor), this->x*sin(faktor)+this->y*cos(faktor), this->z);}
    };

    class vektorTock3D{
    private:
        std::vector<tocka3D> vektorTock;

    public:
        vektorTock3D()=default;

        // Metode tocke.
        tocka3D operator[](size_t idx) const {return vektorTock[idx];}
        void operator+=(tocka3D tocka) {vektorTock.emplace_back(tocka);}

    };

    class nKotnik{
    private:
        std::vector<tocka3D> tocke;

    public:
        nKotnik()=default;

        // Metode nKotnika.
        tocka3D operator[] (const size_t idx) const {return tocke[idx];}
        void operator+=(const tocka3D tocka) {tocke.emplace_back(tocka);}

        // Uporablja se za iteriranje v for loopih.
        std::vector<tocka3D>::iterator begin() {return tocke.begin();}
        std::vector<tocka3D>::iterator end() {return tocke.end();}

    public:
        // Pocisti tocke.
        void pocistiTocke() {tocke.clear();}

        // Vrne vse tocke v nKotniku.
        std::vector<tocka3D> pridobiTocke() const {return tocke;}

        // Vrne stevilo vogalov.
        size_t pridobiN() const {return tocke.size();}
    };

    class model{
    private:
        std::vector<nKotnik> modelF;

    public:
        void operator+=(nKotnik tempKotnik) {modelF.emplace_back(tempKotnik);}

        // Uporablja se za iteriranje v for loopih.
        std::vector<nKotnik>::iterator begin() {return modelF.begin();}
        std::vector<nKotnik>::iterator end() {return modelF.end();}
    };

    class objekti{
    private:
        std::vector<model> vektorObjektov;
    public:
        // Metode objektov.
        model operator[] (const size_t idx) const {return vektorObjektov[idx];}
        void operator+=(model model) {vektorObjektov.emplace_back(model);}

        // Uporablja se za iteriranje v for loopih.
        std::vector<model>::iterator begin() { return vektorObjektov.begin();}
        std::vector<model>::iterator end() {return vektorObjektov.end();}
    };
}

namespace renderanje{
    class projekcija{

    public:
        // Uporabi samo na zacetku.
        static void hardResetZaslon() {printf("\033[1;1H\033[2J");}

        static void pocistiZaslon() {printf("\x1b[d");}

        static void prikaz(primitivi::objekti& vektorObjektov, statusIgre::igra& status, char* buffer){
            for (size_t idy=0;idy<=status.gety();idy++){
                buffer[idy*status.getx()]='\n';
            }
            int x, y;

            for (auto& model:vektorObjektov){
                for (auto& ploskev:model){
                    for (auto& tocka:ploskev){
                        primitivi::tocka3D temp=tocka;

                        x=round(tocka.pridobiX()*status.getzs()*2+status.getxs())+90;
                        y=round(tocka.pridobiY()*status.getzs()+status.getys())+200;

                        if (0<=x && x<status.getx() && 0<=y && y<status.gety()){
                            buffer[(status.getx())*y+x]='*';

                        }
                    }
                }
            }
            puts(buffer);

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
                    vektorTock+=primitivi::tocka3D(x, y, z);
                }

                // Shranjuje ploskve. 
                else if (tip_podatka=="f"){

                    std::string zeton;
                    primitivi::nKotnik ploskev;

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
