#pragma once
#include <LinearMath/btIDebugDraw.h>

namespace Tapioca {

class PhysicsDebugDrawer : public btIDebugDraw {
    int mode;

public:
    PhysicsDebugDrawer();
    ~PhysicsDebugDrawer();
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) override;
    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime,
        const btVector3& color) override;
    void reportErrorWarning(const char* warningString) override;
    void draw3dText(const btVector3& location, const char* textString) override;
    void drawTransform(const btTransform& transform, btScalar orthoLen) override;
    inline void setDebugMode(int debugMode) override { mode = DebugDrawModes(debugMode); }
    inline int getDebugMode() const override { return mode; }
};

}