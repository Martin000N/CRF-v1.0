(SLO)
CRF (CPU Rendering Framework) KNJIZNICA V C++. Deluje le single-threaded in je **single-header library**.
**Delujoče funkcije:**

    1.  wireframe rendering,
    
    2.  clippanje n-kotnikov (za linije še bom dodal),
    
    3. izris n-kotnikov,
    
    4. branje .obj datotek, 
    
    5. določanje raznih aspektov projekcije (FOV, resolucija...),
    
    6. enostavno manipuliranje posameznih enot (točke, n-kotniki, modeli...)...

Uporaba knjižnice.

Trenutno se vsi modeli (tudi če obstaja samo en) naložijo v std::vector<primitivi::model>. Tega se ne ureja in na njem lahko samo izvajajo modelove transformacije.

1. V main funkciji se ustvarijo 3 objekti (statusIgre::igra status, renderanje::bufferArray buffer(x_res, y_res), std::vector<primitivi::objekt>), nato v zadnjega naloži vse modele na naslednji način:

        "origMod.emplace_back(nalagalnik::nalaganjeTock::naloziOBJ("C:\\Users\\marti\\Desktop\\raster\\obj\\wooden watch tower2.obj"));"
   
2. Pred glavnim while loopom se še uporabi;

        "renderanje::projekcija::hardResetZaslon(status, buffer);"

3. V glavnem loop-u se uporabijo naslednje funkcije v določenem vrstem redu. Njihova funkcija je trivijalna in je ne bom razlagal. Obenem ja vsaka izmed teh komentar, ki jo razloži ali pa je popolnoma samoumevna:

       "renderanje::projekcija::pocistiZaslon(status, buffer);
        
        branjeInputa::input::preverjanjeInputa(status, origMod);

        std::vector<primitivi::objekt> temp=transformacije::clippanje::clipLinij(origMod, status);
        
        transformacije::projekcijskaTransformacija::izracunProjekcije(temp, status);

        renderanje::renderanjePrimitiv::renderObjektov(temp, buffer, status);

        puts(buffer.pridobiBuffer());"

4. Zadeva se kompilira sz naslednjo komando

        "g++ -IPathToInclude example.cpp"
(ENG)
This is a **single-header library**. If you want to try the example program, you can compile it with 

    "g++ -IPathToInclude example.cpp"
