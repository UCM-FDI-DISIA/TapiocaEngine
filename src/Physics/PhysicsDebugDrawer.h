#pragma once
#include <LinearMath/btIDebugDraw.h>
#include <list>
#include "defs.h"

namespace Ogre {
class ManualObject;
}

namespace Tapioca {
class RenderNode;

class TAPIOCA_API PhysicsDebugDrawer : public btIDebugDraw {
    int mode;
    std::list<Ogre::ManualObject*> lines;
    RenderNode* node;

public:
    /*
    * @brief Constructora de la clase PhysicsDebugDrawer.
    */
    PhysicsDebugDrawer();
    /*
    * @brief Destructora de la clase PhysicsDebugDrawer.
    */
    ~PhysicsDebugDrawer();

    /*
    * @brief Dibuja una linea
    * @param from Origen de la linea
    * @param to Fin de la linea
    * @param color Color de la linea
    */
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) override;
    /*
    * @brief Dibuja un linea desde el punto de contacto al objeto que ha contactado
    * @param PointOnB Punto donde ha contactado
    * @param normalOnB Normal que con direccion formada por el punto de contacto y el objeto contactado
    * @param distance Distancia entre el punto de contacto y el objeto contactado
    * @param lifetime Tiempo que se va a dibujar la linea (No se usa)
    * @param color Color de la linea
    */
    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
    /*
    * @brief Avisa de un error por consola
    * @param warningString Error que se va a avisar
    */
    void reportErrorWarning(const char* warningString) override;
    void draw3dText(const btVector3& location, const char* textString) override {};
    //void drawTransform(const btTransform& transform, btScalar orthoLen) override;
    /*
    * @brief Elimina las lines dibujadas
    */
    void clearLines() override;
    /*
    * @brief Cambia el modo de debug
    * @param debugMode Modo al que se quiere cambiar
    */
    inline void setDebugMode(const int debugMode) override { mode = DebugDrawModes(debugMode); }
    /*
    * @brief Devuelve el modo de debug
    * @return Modo de debug
    */
    inline int getDebugMode() const override { return mode; }
};

}