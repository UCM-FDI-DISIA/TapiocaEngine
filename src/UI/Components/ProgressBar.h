#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"
#include "Core.h"

namespace Tapioca {
class TAPIOCA_API ProgressBar : public BaseWidget, public Component  { 
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
    std::string name;


public:
    COMPONENT_ID("ProgressBar")
    //@brief constructora vacia
    ProgressBar();
    //@brief destructora por defecto
    ~ProgressBar();
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
    * @brief devuelve el progreso actual
    */
    float getProgress() { return progress; }
    /*
    * @brief aumenta en p el progreso
    */
    void addProgress(float p);

    /*
    *  @brief devuelve el color
    */
    Vector4 getColor() { return color; }
    /*
    * @brief devuelve el texto de fondo
    */
    std::string getText() { return backgroundtext; }
    
};
}