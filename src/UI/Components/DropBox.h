#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"
#include "Core.h"
#include <string>


namespace Tapioca {
 class TAPIOCA_API DropBox : public BaseWidget, public Component {

private:
   // std::string name;
    std::vector< std::string> items;
    int currentItem;
    //a e y
    Vector2 position;
    //width y heitgh
    Vector2 size;

public:
    COMPONENT_ID("DropBox")
   //@brief constructora vacia
     DropBox();
   //@brief destructora por defecto
    ~DropBox();
     /*
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * @param variables unordered_map con los parametros iniciales
    */
    virtual bool initComponent(const CompMap& variables) override;

    /*
    * @brief Asigna el transform del objeto al texto e inicializa la fuente del texto
    */
    virtual void start() override;

    /*
    * @brief Metodo que se usa para renderizar 
    */
    virtual void render() const override;
    /*
    * @brief devuelve el nombre
    */
    std::string getName() { return name; }
    /*
    * @brief delvueve un vector con las distintas opciones
    */
    std::vector<std::string> getContent() { return items; }
    /*
    * @brief devuelve el indice del elemento seleccionado
    */
    int* getCurrent() { return &currentItem;}
};
}

