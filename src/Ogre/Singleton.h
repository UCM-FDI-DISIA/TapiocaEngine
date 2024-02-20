#pragma once

#include <memory>
#include <cassert>


// Clase singleton para hacer que las clases hijas sean singleton
template<typename T>
class Singleton {
private:
	// Instancia única
	static std::unique_ptr<T> instance_;

protected:
	Singleton() {};

public:
	// Evita hacer copias de un singleton
	Singleton<T>& operator=(const Singleton<T>& s) = delete;
	Singleton(const Singleton<T>& s) = delete;

	virtual ~Singleton() {};

	// Inicializa la instancia con los parámetros deseados (... args)
	template<typename ...T_args>
	inline static T* init(T_args &&...args) {
		assert(instance_.get() == nullptr, "Instance already exists");
		instance_.reset(new T(std::forward<T_args>(args)...));
		
		return instance_.get();
	}

	// Para cerrar singletons en un orden específico
	inline static void close() { instance_.reset(); }


	// Obtener el puntero a la instancia
	inline static T* instance() {
		// Si no existe, se crea
		if (instance_.get() == nullptr) init();

		// Entonces, devuelve el puntero
		return instance_.get();
	}
};

template<typename T>
std::unique_ptr<T> Singleton<T>::instance_;


/*
* Ejemplo de uso:
*
*	class A : public Singleton<A> {
*		friend Singleton<A>;
*
*	private:
*		// Las constructoras tienen que ser privadas y la clase debe tener al menos 
		// un constructor sin argumentos por si init() es llamado sin argumentos
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
*		// La destructora también puede ser virtual
*		~A() { ... }
*
*		method3() { ... };
*		method4() { ... };
*		...
*	}
*
*
* La clase hija debe ser instanciada así:
*	// args depende de los argumentos de la(s) constructora(s) de A
*	A::init(args);
*
*/
