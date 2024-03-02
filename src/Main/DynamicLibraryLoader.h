#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;

namespace Tapioca {
/**
* @brief Carga de bibliotecas din�micas
*/
class DynamicLibraryLoader {
private:
    string gameName; // Nombre del juego
    string gamePath; // Ruta del juego con formato
    HMODULE module; // M�dulo cargado en la memoria del proceso

    /**
    * @brief Libera el m�dulo
    */
    void free();

public:
    /**
    * @brief Constructor: Inicializa el nombre del juego y el mod�lo
    * @param gameName: Nombre del juego
    */
    DynamicLibraryLoader(const string& gameName = "game");
    /**
    * @brief Libera la memoria usada del m�dulo si el m�dulo est� cargado
    */
    ~DynamicLibraryLoader();

    /**
    * @brief Devuelve el m�dulo
    * @return module
    */
    inline HMODULE getModule() const { return module; }

    /**
    * @brief Carga el m�dulo desde la ruta especificada (gamePath) a partir del nombre del juego (gameName)
    * @return Si se ha podido cargar correctamente o no
    */
    bool load();
};
}
