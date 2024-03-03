#pragma once
#include <memory>

#ifdef _DEBUG
#include <iostream>
#endif

namespace Tapioca {
// Clase singleton para hacer que las clases hijas sean singleton
template<typename T> class Singleton {
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
    inline static T* create(T_args&&... args) {
        //assert(instance_.get() == nullptr, "Instance already exists");
        if (instance_ == nullptr) instance_ = new T(std::forward<T_args>(args)...);
#ifdef _DEBUG
        else std::cout << "Instance already exists\n";
#endif
        return instance_;
    }

    // Obtener el puntero a la instancia
    inline static T* instance() {
        // Si no existe, se crea
        if (instance_ == nullptr) create();

        // Entonces, devuelve el puntero
        return instance_;
    }
};

template<typename T> T* Singleton<T>::instance_ = nullptr;
}


/*
* Ejemplo de uso:
*
*	class A : public Singleton<A> {
*		friend Singleton<A>;
*
*	private:
*		// Las constructoras tienen que ser privadas y la clase debe tener al menos 
		// un constructor sin argumentos por si create() es llamado sin argumentos
*		A() { ... };
*		A(args1) { ... };
*		A(args2) { ...};
*
*		method1() { ... };
*		method2() { ... };
*		...
*
*	public:
*		// Para evitar copiar/mover la instancia
*		A(A&) = delete;
*		A(A&&) = delete;
*		A& operator=(A&) = delete;
*		A& operator=(A&&) = delete;
* 
*		// La destructora tambien puede ser virtual
*		~A() { ... }
*
*		method3() { ... };
*		method4() { ... };
*		...
*	}
*
*
* La clase hija debe ser instanciada asi:
*	// args depende de los argumentos de la(s) constructora(s) de A
*	A::create(args);
*
*/
