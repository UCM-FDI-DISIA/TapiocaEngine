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
    * @param module Modulo cargado en la memoria del proceso
    * @param gameName Nombre del juego
    * @return Si se ha podido cargar correctamente o no
    */
    static bool load(HMODULE& module, std::string const& gameName);

    /**
    * @brief Libera el modulo
    */
    static void freeModule(HMODULE module);

public:
    DynamicLibraryLoader() = delete;
    ~DynamicLibraryLoader() = delete;

    /**
    * @brief Carga la dll del juego y llama a su funcion de inicializacion
    * @return Si se ha podido cargar correctamente o no
    */
    static bool initGame(std::string const& gameName = "game");
};
}
