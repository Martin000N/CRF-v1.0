#include <vector>
#include "test.hpp"
#include <windows.h>

int main(){
    int x_res=1500;
	int y_res=400;

    statusIgre::igra stanje;
    stanje.dolociResolucijo(x_res, y_res);

    primitivi::objekti objekt;

    renderanje::bufferArray buffer(x_res, y_res);
    renderanje::zBufferArray zBuffer(x_res, y_res);

    objekt+=(nalagalnik::nalaganjeTock::naloziOBB("C:\\Users\\marti\\Desktop\\scene\\scene.obj"));
    objekt+=(nalagalnik::nalaganjeTock::naloziOBB("C:\\Users\\marti\\Desktop\\www\\scene.obj"));

    renderanje::projekcija::hardResetZaslon();

    while (stanje.pridobiStatus()){

        renderanje::projekcija::pocistiZaslon();
        renderanje::projekcija::prikaz(objekt, stanje, buffer, zBuffer);
        branjeInputa::input::preverjanjeInputa(&stanje);
        
    } 
}
