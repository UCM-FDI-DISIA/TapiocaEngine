#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;

namespace Tapioca {
/**
* @brief Carga de bibliotecas dinámicas
*/
class DynamicLibraryLoader {
private:
    string gameName; // Nombre del juego
    string gamePath; // Ruta del juego con formato
    HMODULE module; // Módulo cargado en la memoria del proceso

    /**
    * @brief Libera el módulo
    */
    void free();

public:
    /**
    * @brief Constructor: Inicializa el nombre del juego y el modúlo
    * @param gameName: Nombre del juego
    */
    DynamicLibraryLoader(const string& gameName = "game");
    /**
    * @brief Libera la memoria usada del módulo si el módulo está cargado
    */
    ~DynamicLibraryLoader();

    /**
    * @brief Devuelve el módulo
    * @return module
    */
    inline HMODULE getModule() const { return module; }

    /**
    * @brief Carga el módulo desde la ruta especificada (gamePath) a partir del nombre del juego (gameName)
    * @return Si se ha podido cargar correctamente o no
    */
    bool load();
};
}
