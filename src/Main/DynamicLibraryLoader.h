#pragma once
#include <string>
#include <Windows.h>

namespace Tapioca {
/**
* @brief Carga de bibliotecas dinamicas
*/
class DynamicLibraryLoader {
private:
    std::string gameName;   // Nombre del juego
    std::string gamePath;   // Ruta del juego con formato
    HMODULE module; // Modulo cargado en la memoria del proceso

    /**
    * @brief Libera el modulo
    */
    void freeModule();

public:
    /**
    * @brief Constructor: Inicializa el nombre del juego y el modulo
    * @param gameName: Nombre del juego
    */
    DynamicLibraryLoader(const std::string& gameName = "game");
    /**
    * @brief Libera la memoria usada del modulo si el modulo esta cargado
    */
    ~DynamicLibraryLoader();

    /**
    * @brief Devuelve el modulo
    * @return module
    */
    inline HMODULE getModule() const { return module; }

    /**
    * @brief Carga el modulo desde la ruta especificada (gamePath) a partir del nombre del juego (gameName)
    * @return Si se ha podido cargar correctamente o no
    */
    bool load();
};
}
