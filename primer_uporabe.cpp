#include <vector>
#include "CRF.hpp"
#include <iostream>

int main(){
    int x_res=200;
    int y_res=100;
    
    statusIgre::igra status(1, x_res, y_res, M_PI/6);

    renderanje::bufferArray buffer(x_res, y_res);

    renderanje::projekcija::hardResetZaslon(status, buffer);
    std::vector<primitivi::objekt> origMod;

    origMod.emplace_back(nalagalnik::nalaganjeTock::naloziOBJ("C:\\Users\\marti\\Desktop\\raster\\obj\\wooden watch tower2.obj"));

    while (status.status()){

        renderanje::projekcija::pocistiZaslon(status, buffer);
        
        branjeInputa::input::preverjanjeInputa(status, origMod);

        std::vector<primitivi::objekt> temp=transformacije::clippanje::clipLinij(origMod, status);
        
        transformacije::projekcijskaTransformacija::izracunProjekcije(temp, status);

        renderanje::renderanjePrimitiv::renderObjektov(temp, buffer, status);

        puts(buffer.pridobiBuffer());
    }
   return 0;
}
