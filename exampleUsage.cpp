#include <vector>
#include <any>
#include <unordered_map>
#include "test.hpp"
#include <windows.h>
#include <omp.h>

int main(){
    int x_res=1500;
	int y_res=400;
    
    std::unordered_map<int, std::any> test;

    statusIgre::igra stanje;
    stanje.dolociResolucijo(x_res, y_res);

    primitivi::objekti objekt;

    int moveX=0;
    int moveY=0;
    float zoom=10.0f;

	int st_pikslov=y_res*x_res+y_res;
    char original[st_pikslov];
 
    float fakt=1.0f;
    float fakt2=1.0f;

	renderanje::projekcija::pocistiZaslon();
    memset(original, ' ', sizeof(original));
    objekt+=(nalagalnik::nalaganjeTock::naloziOBB("C:\\Users..."));
    objekt+=(nalagalnik::nalaganjeTock::naloziOBB("C:\\Users..."));

    while (stanje.pridobiStatus()){
        if (GetAsyncKeyState(65)&0x8000){
            stanje.x(10.0f);
        }
        else if (GetAsyncKeyState(68)&0x8000){
            stanje.x(-10.0f);
        }
        else if (GetAsyncKeyState (0x57)){
			stanje.y(10.0f);
		}
		else if (GetAsyncKeyState (0x53)){
		    stanje.y(-10.0f);
		}

        else if (GetAsyncKeyState(84)&0x8000){
            stanje.z(0.1f);
        }
        else if (GetAsyncKeyState(82)&0x8000) {
            stanje.z(-0.1f);
        }
        else if(GetAsyncKeyState(69)&0x8000){
            stanje.gzs(0.2f);
        }
        else if(GetAsyncKeyState(81)&0x8000){
            stanje.gzs2(0.2f);
        }
        renderanje::projekcija::pocistiZaslon();
        renderanje::projekcija::prikaz(objekt, stanje, original);
        memset(original, ' ', sizeof(original));
        
        

        branjeInputa::input::preverjanjeInputa(&stanje, &test, &test);
        
    }
    
}
