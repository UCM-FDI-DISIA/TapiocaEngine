#pragma once
#include "RenderObject.h"
#include <string>
#include <functional>
#include <imgui.h>
#include "Utilities/Vector2.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
/*
* @brief Clase que representa un botón en la interfaz grafica con la que el usuario puede interactuar para realizar cierta accion
*/
class TAPIOCA_API Button {
private:
    ImVec2 position;                 // Posicion del boton
    std::string text;                // Texto que se muestra en el boton
    std::function<void()> onClick;   // Funcion que se ejecuta cuando se hace click en el boton
    ImVec2 size;                     // Tamano del boton
    bool* canCloseWindow;            // Puntero a booleano que indica si se puede cerrar la ventana
    ImGuiWindowFlags flags;          // Flags de la ventana

public:
    /*
    * @brief Inicializa un boton con los parametros dados
    * @param scnMgr Puntero al SceneManager de Ogre
    * @param node Nodo de la escena al que se asocia el boton
    * @param position Posicion del boton
    * @param text Texto que se muestra en el boton
    * @param onClick Funcion que se ejecuta cuando se hace click en el boton
    * @param size Tamano del boton
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
    * @param flags Flags de la ventana
    */
    Button(Ogre::SceneManager* const scnMgr, RenderNode* const node, const ImVec2 position, const std::string& text,
           std::function<void()> onClick, const ImVec2 size = ImVec2(130, 40), bool* canCloseWindow = nullptr,
           ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
    virtual ~Button() { }

    /*
    * @brief Establece la posicion del boton
    */
    inline void setPosition(const ImVec2 position) { this->position = position; }

    /*
    * @brief Establece el texto del boton
    */
    inline void setText(const std::string& text) { this->text = text; }

    /*
    * @brief Establece la funcion que se ejecuta cuando se hace click en el boton
    */
    inline void setOnClick(std::function<void()> onClick) { this->onClick = onClick; }

    /*
    * @brief Establece el tamano del boton
    */
    inline void setSize(const ImVec2& size) { this->size = size; }

    /*
    * @brief Establece el puntero a booleano que indica si se puede cerrar la ventana
	*/
    inline void setCanCloseWindow(bool* canCloseWindow) { this->canCloseWindow = canCloseWindow; }

    /*
    * @brief Establece los flags de la ventana
	*/
    inline void setFlags(ImGuiWindowFlags flags) { this->flags = flags; }

    /*
    * @brief Devuelve la posicion del boton
    * @return Posicion del boton
	*/
    inline ImVec2 getPosition() const { return position; }

    /*
    * @brief Devuelve el texto del boton
    * @return Texto del boton
    */
    inline std::string getText() const { return text; }

    /*
    * @brief Devuelve la funcion que se ejecuta cuando se hace click en el boton
    * @return Funcion que se ejecuta cuando se hace click en el boton
	*/
    inline std::function<void()> getOnClick() const { return onClick; }

    /*
    * @brief Devuelve el tamano del boton
    * @return Tamano del boton
    */
    inline ImVec2 getSize() const { return size; }

    /*
    * @brief Devuelve el puntero a booleano que indica si se puede cerrar la ventana
    * @return Puntero a booleano que indica si se puede cerrar la ventana
    */
    inline bool* getCanCloseWindow() const { return canCloseWindow; }

    /*
    * @brief Devuelve los flags de la ventana
    * @return Flags de la ventana
	*/
    inline ImGuiWindowFlags getFlags() const { return flags; }
};
}