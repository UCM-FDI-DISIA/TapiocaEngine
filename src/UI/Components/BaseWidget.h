#pragma once
#include <string>
#include "Utilities/Vector2.h"
#include "Utilities/Vector3.h"
#include "Components/Transform.h"

typedef int ImGuiWindowFlags;

namespace Tapioca {
class UIManager;
class LuaManager;

/*
* @brief Clase que representa la base de un widget en la interfaz grafica
*/
class TAPIOCA_API BaseWidget {
protected:
    UIManager* uiManager;     // Puntero al UIManager
    LuaManager* luaManager;   // Puntero al LuaManager

    std::string name;               // Nombre del widget
    Transform* transform;           // Puntero al transform del widget
    ImGuiWindowFlags windowFlags;   // Flags de la ventana

public:
    /*
    * @brief Constructor por defecto
    */
    BaseWidget();

    /*
    * @brief Destructor por defecto
    */
    ~BaseWidget();

    /*
    * @brief Genera un nombre aleatorio para el widget
    * @return Nombre aleatorio
    */
    std::string generateRandomName();

    /*
    * @brief Establece el nombre del widget
    * @param name Nombre del widget
    */
    inline void setName(const std::string& name) { this->name = name; }

    /*
    * @brief Establece el puntero al transform del widget
    * @param transform Puntero al transform del widget
    */
    inline void setTransform(Transform* transform) { this->transform = transform; }

    /*
    * @brief Establece la posicion del boton
    * @param position Posicion del boton
    */
    inline void setPosition(const Vector2& position) { transform->setPosition(Vector3(position.x, position.y, 0)); }

    /*
    * @brief Establece el tamano del boton
    * @param size Tamano del boton
    */
    inline void setSize(const Vector2& size) { transform->setScaleXY(size); }

    /*
    * @brief Establece los flags de la ventana
    * @param windowFlags Flags de la ventana
	*/
    inline void setWindowFlags(ImGuiWindowFlags windowFlags) { this->windowFlags = windowFlags; }

    /*
    * @brief Devuelve el nombre del widget
    * @return Nombre del widget
    */
    inline std::string getName() const { return name; }

    /*
    * @brief Devuelve el puntero al transform del widget
    * @return Puntero al transform del widget
	*/
    inline Transform* getTransform() const { return transform; }

    /*
    * @brief Devuelve la posicion del widget
    * @return Posicion del widget
    */
    inline Vector2 getPosition() const { return transform->getPositionXY(); }

    /*
    * @brief Devuelve el tamano del widget
    * @return Tamano del widget
    */
    inline Vector2 getSize() const { return transform->getScaleXY(); }

    /*
    * @brief Devuelve los flags de la ventana
    * @return Flags de la ventana
	*/
    inline ImGuiWindowFlags getWindowFlags() const { return windowFlags; }
};
}