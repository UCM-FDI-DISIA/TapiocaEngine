#pragma once
#include "Core.h"
namespace Tapioca {
class TAPIOCA_API ProgressBar { 
private:
    //valor entre 0 y 1 que indica como de llena esta la barra
    float progress = 0;
    //El color de llebado de la barra r , g , b ,a
    Vector4 color;
    // x e y
    Vector2 position;
    //width y heitgh
    Vector2 size;
    std::string backgroundtext;


public:
    /*
    * @brief constructora de barra de progreso toma como argumentos:
    * @param initialprog initialprog valor entre 0 y 1 que establece como de llena est ala barra
    * @param col el color de llenado de la barra r g b a
    * @param pos la posicion en pantalla
    * @param siz las dimensiones de la barra
    * @param backtext texto que apareceria de fondo en la barra
    */
    ProgressBar(float initialprog, Vector4 col, Vector2 pos, Vector2 siz, std::string backtext = "");
    
    float getProgress() { return progress; }
    void addProgress(float p) { progress += p; };

    Vector4 getColor() { return color; }
    Vector2 getPosition() { return position; }
    Vector2 getSize() { return size; }
    std::string getText() { return backgroundtext; }
    
};
}