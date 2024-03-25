#pragma once
#include <string>
#include <functional>
#include <imgui.h>
#include "Utilities/Vector2.h"
#include "Utilities/Vector3.h"
#include "Components/Transform.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
class Transform;

/*
* @brief Clase que representa la base de un widget en la interfaz grafica
*/
class TAPIOCA_API BaseWidget {
protected:
    std::string name;               // Nombre del widget
    Transform* transform;           // Puntero al transform del widget
    bool* canCloseWindow;           // Puntero a booleano que indica si se puede cerrar la ventana
    ImGuiWindowFlags windowFlags;   // Flags de la ventana

public:
    /*
    * @brief Constructor por defecto
    */
    BaseWidget();

    /*
    * @brief Destructor por defecto
    */
    virtual ~BaseWidget() { }

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
    inline void setPosition(const ImVec2& position) { transform->setPosition(Vector3(position.x, position.y, 0)); }

    /*
    * @brief Establece el tamano del boton
    * @param size Tamano del boton
    */
    inline void setSize(const ImVec2& size) { transform->setScaleXY(Vector2(size.x, size.y)); }

    /*
    * @brief Establece el puntero a booleano que indica si se puede cerrar la ventana
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
	*/
    inline void setCanCloseWindow(bool* canCloseWindow) { this->canCloseWindow = canCloseWindow; }

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
    inline ImVec2 getPosition() const { return ImVec2(transform->getPosition().x, transform->getPosition().y); }

    /*
    * @brief Devuelve el tamano del widget
    * @return Tamano del widget
    */
    inline ImVec2 getSize() const { return ImVec2(transform->getScale().x, transform->getScale().y); }

    /*
    * @brief Devuelve el puntero a booleano que indica si se puede cerrar la ventana
    * @return Puntero a booleano que indica si se puede cerrar la ventana
    */
    inline bool* getCanCloseWindow() const { return canCloseWindow; }

    /*
    * @brief Devuelve los flags de la ventana
    * @return Flags de la ventana
	*/
    inline ImGuiWindowFlags getWindowFlags() const { return windowFlags; }
};
}