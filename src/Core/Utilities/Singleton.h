#pragma once
#include <memory>
#include "defs.h"

namespace Tapioca {
/*
* @brief Clase singleton para hacer que las clases hijas sean singleton
*/
template<typename T>
class Singleton {
private:
    // Instancia unica
    static T* instance_;

protected:
    Singleton() {};

public:
    // Evita hacer copias de un singleton
    Singleton<T>& operator=(const Singleton<T>& s) = delete;
    Singleton(const Singleton<T>& s) = delete;

    virtual ~Singleton() {};

    // Inicializa la instancia con los parametros deseados (... args)
    template<typename... T_args>
    static T* create(T_args&&... args) {
        if (instance_ == nullptr) instance_ = new T(std::forward<T_args>(args)...);
        else
            logInfo("Singleton: Instance already exists");
        return instance_;
    }

    // Obtener el puntero a la instancia
    static T* instance() {
        // Si no existe, se crea
        if (instance_ == nullptr) create();
        // Entonces, devuelve el puntero
        return instance_;
    }
};
}
