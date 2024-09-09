#include <vector>
#include "CRF.hpp"
#include <iostream>

int main(){
    int x_res=500;
    int y_res=100;
    
    statusIgre::igra status;
    status.dolociResolucijo(x_res, y_res);

    renderanje::bufferArray buffer(x_res, y_res);
    renderanje::zBufferArray zBuffer(x_res, y_res);

    renderanje::projekcija::hardResetZaslon(status, buffer, zBuffer);
    std::vector<primitivi::model> origMod;
    std::vector<primitivi::model> temp;

    origMod.emplace_back(nalagalnik::nalaganjeTock::naloziOBB("C:\\Users\\user\\Desktop\\raster\\obj\\textures\\test.obj"));

    while (status.pridobiStatus()){

        renderanje::projekcija::pocistiZaslon(status, buffer, zBuffer);
        
        branjeInputa::input::preverjanjeInputa(status, origMod);
        temp=origMod;
        rasterizacija::transofrmacije::clip(origMod, status);
        rasterizacija::transofrmacije::projekcijskaMat(temp);

        renderanje::renderanjePrimitiv::renderObjektov(temp, buffer, zBuffer, status);
        puts(buffer.pridobiBuffer());
    }
   return 0;
}
