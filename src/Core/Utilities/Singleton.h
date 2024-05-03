#pragma once
#include <memory>
#include "defs.h"

namespace Tapioca {
/**
* @brief Patron de diseno Singleton para crear una unica instancia de una clase
*/
template<typename T>
class Singleton {
private:
    static T* instance_; // Instancia unica

protected:
    /**
    * @brief Constructor vacio
    */
    Singleton() {};

public:
    Singleton<T>& operator=(const Singleton<T>& s) = delete;
    Singleton(const Singleton<T>& s) = delete;

    /**
    * @brief Destructor vacio
    */
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
