#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>

namespace statusIgre{
    class igra{
    private:
        int statusIgre, x_res, y_res;
        float xSmer, ySmer, zSmer;
    public:
        // Privzeta konstruktorja.
        igra():statusIgre(1), x_res(100), y_res(100), xSmer(0.0f), ySmer(0.0f), zSmer(1.0f){}
        igra(int statusIgre, int x_res, int y_res, float xSmer, float ySmer, float zSmer, float te3):statusIgre(statusIgre), x_res(x_res), y_res(y_res), xSmer(xSmer), ySmer(ySmer), zSmer(zSmer){} 
    
    public:
        // Ni permanentno, samo za debugganje. Na koncu bo cistejsa koda.
        //==============================================================
        int xDimenzija(){return x_res;}
        int yDimenzija(){return y_res;}

        float getzs(){return zSmer;}
        float getxs(){return xSmer;}
        float getys(){return ySmer;}

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
        static void preverjanjeInputa(statusIgre::igra* stanje/*, std::unordered_map<int, std::any>* funkcijeZaPremikeIgralca, std::unordered_map<int, std::any>* funkcijeZaPremikePogleda*/){

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

            }
            // A
            else if (GetAsyncKeyState(65)&0x8000){

            }
            // D
            else if (GetAsyncKeyState(68)&0x8000){

            }
            // W
            else if (GetAsyncKeyState(87)&0x8000){

            }
            // S
            else if (GetAsyncKeyState(83)&0x8000){

            }
            // T
            else if (GetAsyncKeyState(84)&0x8000){

            }
            // R
            else if (GetAsyncKeyState(82)&0x8000){

            }
            // E
            else if(GetAsyncKeyState(69)&0x8000){

            }
            // Q
            else if(GetAsyncKeyState(81)&0x8000){

            }
        }
    };
}

namespace transformacije{
    class kvaternioni{
    private:
        float w, x, y, z;
    public:
        kvaternioni(float kot, float xKomponenta, float yKomponenta, float zKomponenta) {
            w=cos(kot/2.0f);
            float sinPolovicniKot=sin(kot/2.0f);
            x=xKomponenta*sinPolovicniKot;
            y=yKomponenta*sinPolovicniKot;
            z=zKomponenta*sinPolovicniKot;
        }

        kvaternioni operator*(const kvaternioni& kvaternion) const {return kvaternioni(w*kvaternion.w-x*kvaternion.x-y*kvaternion.y-z*kvaternion.z,w*kvaternion.x+x*kvaternion.w+y*kvaternion.z-z*kvaternion.y,w*kvaternion.y-x*kvaternion.z+y*kvaternion.w+z*kvaternion.x,w*kvaternion.z+x*kvaternion.y-y*kvaternion.x+z*kvaternion.w);}

        static kvaternioni konjugiraj(kvaternioni kvaternion) {return kvaternioni(kvaternion.w, -kvaternion.x, -kvaternion.y, -kvaternion.z);}

        // Kvaternion p se rotira okoli osi kvaterniona. q*p*q^-1.
        static primitivi::tocka3D rotate(kvaternioni kvaternion, primitivi::tocka3D tocka) {
            kvaternioni p(0, tocka.pridobiX(), tocka.pridobiY(), tocka.pridobiZ());
            kvaternioni rezultat=(kvaternion)*p*konjugiraj(kvaternion);
            return primitivi::tocka3D(rezultat.x, rezultat.y, rezultat.z);
        }
    };
}

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
    class bufferArray{
        private:
            int x, y;
            char* buffer;
        public:
            bufferArray(int x, int y):x(x), y(y) {
                buffer=new char[x*y+y];
            }


            // Metoda buffera.
            char& operator[] (size_t idx) {return buffer[idx];}

        public:
    
            char* pridobiBuffer() const {return buffer;}
            size_t velikost() const {return x*y+y;}
    };

    class zBufferArray{
        private:
            int x, y;
            float* buffer;
        public:
            zBufferArray(int x, int y):x(x), y(y) {
                buffer=new float[x*y+y];
            }
            // Metoda buffera.
            float& operator[] (size_t idx) {return buffer[idx];}

        public:

            size_t velikost() const {return x*y+y;}
    };
    
    
    class bufferArrayMemset{
    public:
        static void bufferMemsetC(bufferArray& buffer, char znak, size_t maxVelikost){
            for (size_t idx=0; idx<maxVelikost; ++idx){
                buffer[idx]=znak;
            }
        }
        static void bufferMemsetI(zBufferArray& buffer, float znak, size_t maxVelikost){
            for (size_t idx=0; idx<maxVelikost; ++idx){
                buffer[idx]=znak;
            }
        }
    };

    class projekcija{

    public:
        // Uporabi samo na zacetku.
        static void hardResetZaslon() {printf("\033[1;1H\033[2J");}

        static void pocistiZaslon() {printf("\x1b[d");}

        static void prikaz(primitivi::objekti& vektorObjektov, statusIgre::igra& status, renderanje::bufferArray& buffer, renderanje::zBufferArray& zBuffer){


            int x, y;
            int sirina=status.xDimenzija();
            int visina=status.yDimenzija();
            bufferArrayMemset::bufferMemsetC(buffer, ' ', buffer.velikost());
            bufferArrayMemset::bufferMemsetI(zBuffer, 1000.0f, buffer.velikost());


            for (size_t idy=0;idy<=visina;idy++){
                buffer[idy*sirina]='\n';
            }

            for (auto& model:vektorObjektov){
                for (auto& ploskev:model){
                    for (auto& tocka:ploskev){


                        x=round(tocka.pridobiX());
                        y=round(tocka.pridobiY());

                        if (x>=0&&x<sirina&&y>=0&&y<visina&&tocka.pridobiZ()>0&&tocka.pridobiZ()<zBuffer[y*sirina+x]) {
                            zBuffer[y*sirina+x]=tocka.pridobiZ();
                            buffer[y*sirina+x]='*';
                        }
                    }
                }
            }

            puts(buffer.pridobiBuffer());
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
