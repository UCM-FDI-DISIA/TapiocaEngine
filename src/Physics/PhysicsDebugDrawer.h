#pragma once
#include <LinearMath/btIDebugDraw.h>
#include <list>
#include "defs.h"

namespace Ogre {
class ManualObject;
}

namespace Tapioca {
class GraphicsManager;
class RenderNode;

// warning C4275 se ha utilizado una interfaz no de DLL class 'btIDebugDraw'
// como base para la interfaz DLL class 'Tapioca::PhysicsDebugDrawer'
#ifdef _MSC_VER
#pragma warning(disable : 4275)
#endif
/**
* @brief Clase que se encarga de dibujar las lineas de debug de Bullet
*/
class TAPIOCA_API PhysicsDebugDrawer : public btIDebugDraw {
#ifdef _MSC_VER
#pragma warning(default : 4275)
#endif
private: 
    int mode;   // Modo de debug
    GraphicsManager* graphicsManager;   // Puntero al GraphicsManager
    RenderNode* node;                   // Nodo de renderizado

    // warning C4251 'Tapioca::PhysicsDebugDrawer::lines' :
    // class 'std::list<Ogre::ManualObject *,std::allocator<Ogre::ManualObject *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::PhysicsDebugDrawer'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::list<Ogre::ManualObject*> lines;   // Lista de lineas
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

public:
    /**
    * @brief Constructor por defecto
    */
    PhysicsDebugDrawer();
    /**
    * @brief Destructor por defecto
    */
    ~PhysicsDebugDrawer();

    /**
    * @brief Dibuja una linea
    * @param from Origen de la linea
    * @param to Fin de la linea
    * @param color Color de la linea
    */
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
    /**
    * @brief Dibuja un linea desde el punto de contacto al objeto que ha contactado
    * @param PointOnB Punto donde ha contactado
    * @param normalOnB Normal que con direccion formada por el punto de contacto y el objeto contactado
    * @param distance Distancia entre el punto de contacto y el objeto contactado
    * @param lifetime Tiempo que se va a dibujar la linea (No se usa)
    * @param color Color de la linea
    */
    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime,
                          const btVector3& color) override;
    /**
    * @brief Avisa de un error por consola
    * @param warningString Error que se va a avisar
    */
    void reportErrorWarning(const char* warningString) override;
    /**
    * @brief Dibuja un texto en 3D
    * @param location Posicion donde se va a dibujar el texto
    * @param textString Texto que se va a dibujar
    */
    void draw3dText(const btVector3& location, const char* textString) override {};

    /** 
    * @brief Elimina las lines dibujadas
    */
    void clearLines() override;
    /**
    * @brief Cambia el modo de debug
    * @param debugMode Modo al que se quiere cambiar
    */
    inline void setDebugMode(const int debugMode) override { mode = DebugDrawModes(debugMode); }
    /**
    * @brief Devuelve el modo de debug
    * @return Modo de debug
    */
    inline int getDebugMode() const override { return mode; }
};
}
