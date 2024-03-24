#pragma once
#include <string>
#include <functional>
#include <imgui.h>
#include "Utilities/Vector2.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
/*
* @brief Clase que representa la base de un widget en la interfaz grafica
*/
class TAPIOCA_API BaseWidget {
private:
    std::string name;               // Nombre del widget
    ImVec2 position;                // Posicion del widget
    bool* canCloseWindow;           // Puntero a booleano que indica si se puede cerrar la ventana
    ImGuiWindowFlags windowFlags;   // Flags de la ventana

public:
    /*
    * @brief Estructura que contiene las opciones para inicializar un widget
    * @param name Nombre del widget
    * @param position Posicion del widget
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
    * @param windowFlags Flags de la ventana
    */
    struct WidgetOptions {
        std::string name;
        ImVec2 position = ImVec2(0, 0);
        bool* canCloseWindow = nullptr;
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings;
    };

    /*
    * @brief Inicializa el widget con los parametros dados
    * @param options Opciones del widget
    */
    BaseWidget(const WidgetOptions& options);

    /*
    * @brief Inicializa un widget
    * @param name Nombre del widget
    * @param position Posicion del widget
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
    * @param windowFlags Flags de la ventana
    */
    BaseWidget(const std::string& name, const ImVec2& position, bool* canCloseWindow, ImGuiWindowFlags windowFlags);

    virtual ~BaseWidget() { }

    /*
    * @brief Establece el nombre del widget
    * @param name Nombre del widget
    */
    inline void setName(const std::string& name) { this->name = name; }

    /*
    * @brief Establece la posicion del widget
    * @param position Posicion del widget
    */
    inline void setPosition(const ImVec2 position) { this->position = position; }

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
    * @brief Devuelve la posicion del widget
    * @return Posicion del widget
	*/
    inline ImVec2 getPosition() const { return position; }

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