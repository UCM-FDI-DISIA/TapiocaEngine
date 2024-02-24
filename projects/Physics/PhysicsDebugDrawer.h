#pragma once
#include <LinearMath/btIDebugDraw.h>

namespace Tapioca {

class PhysicsDebugDrawer : public btIDebugDraw {
public:
    PhysicsDebugDrawer() { btIDebugDraw::setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe); }
    ~PhysicsDebugDrawer() {}
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
    void reportErrorWarning(const char* warningString);
    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
    void draw3dText(const btVector3& location, const char* textString);
    void drawTransform(const btTransform& transform, btScalar orthoLen);
    inline void setDebugMode(int mode) { btIDebugDraw::setDebugMode(mode); }
    inline int getDebugMode() { return btIDebugDraw::getDebugMode(); }
};

}