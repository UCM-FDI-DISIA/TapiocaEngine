#pragma once
#include <string>
#include <Windows.h>

namespace Tapioca {
/**
* @brief Carga de bibliotecas dinamicas
*/
class DynamicLibraryLoader {
private:
    /**
    * @brief Carga el modulo desde la ruta especificada (gamePath) a partir del nombre del juego (gameName)
    * @param gameName Nombre del juego
    * @return Si se ha podido cargar correctamente o no
    */
    static bool load(std::string const& gameName);

public:
    static HMODULE module;   // Modulo cargado en la memoria del proceso
    
    DynamicLibraryLoader() = delete;
    ~DynamicLibraryLoader() = delete;

    /**
    * @brief Carga la dll del juego y llama a su funcion de inicializacion
    * @param gameName Nombre del juego
    * @return Si se ha podido cargar correctamente o no
    */
    static bool initGame(std::string const& gameName = "game");

    /**
    * @brief Libera el modulo
    */
    static void freeModule();
};
}
