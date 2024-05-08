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
    * @brief Modulo cargado en la memoria del proceso
    */
    HMODULE module;

    /**
    * @brief Carga el modulo desde la ruta especificada (gamePath) a partir del nombre del juego (gameName)
    * @param gameName Nombre del juego
    * @return Si se ha podido cargar correctamente o no
    */
    bool load(std::string const& gameName);

public:
    /**
    * @brief Constructor por defecto
    */
    DynamicLibraryLoader();
    /**
    * @brief Libera el modulo cargado
    */
    ~DynamicLibraryLoader();

    /**
    * @brief Carga la dll del juego y llama a su funcion de inicializacion
    * @param gameName Nombre del juego
    * @return Si se ha podido cargar correctamente o no
    */
    bool initGame(std::string const& gameName = "game");

    /**
    * @brief Devuelve el modulo
    */
    inline HMODULE getModule() const { return module; }
};
}
