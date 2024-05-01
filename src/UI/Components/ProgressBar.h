#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"
#include <string>
#include "Utilities/Vector4.h"

namespace Tapioca {
/*
* @brief Clase que representa una barra de progreso en la interfaz grafica
*/
class TAPIOCA_API ProgressBar : public BaseWidget, public Component {
private:
    float progress;               // Progreso actual
    Vector4 barColor;             // Color de la barra de progreso
    std::string backgroundText;   // Texto de fondo

public:
    COMPONENT_ID("ProgressBar")

    /*
    * @brief Constructor por defecto
    */
    ProgressBar();

    /*
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * @param Variables unordered_map con los parametros iniciales
    */
    bool initComponent(const CompMap& variables) override;

    /*
    * @brief Asigna el transform del objeto al texto e inicializa la fuente del texto
    */
    void start() override;

    /*
    * @brief Metodo que se usa para renderizar 
    */
    void render() const override;

    /*
    * @brief Establece el progreso actual
    * @param progress Progreso actual
    */
    inline void setProgress(float progress) { this->progress = progress; }

    /*
    * @brief Establece el color de la barra de progreso
    * @param barColor Color de la barra de progreso
    */
    inline void setBarColor(const Vector4& barColor) { this->barColor = barColor; }

    /*
    * @brief Establece el texto de fondo
    * @param backgroundText Texto de fondo
    */
    inline void setBackgroundText(const std::string& backgroundText) { this->backgroundText = backgroundText; }

    /*
    * @brief Devuelve el progreso actual
    * @return Progreso actual
    */
    inline float getProgress() const { return progress; }

    /*
    * @brief Aumenta en p el progreso
    * @param p Cantidad de progreso a aumentar
    */
    void addProgress(float p);

    /*
    * @brief Devuelve el color de la barra de progreso
    * @return Color de la barra de progreso
    */
    inline Vector4 getBarColor() const { return barColor; }

    /*
    * @brief Devuelve el texto de fondo
    * @return Texto de fondo
    */
    inline std::string getBackgroundText() const { return backgroundText; }
};
}