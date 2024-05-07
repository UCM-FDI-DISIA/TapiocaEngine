#include "PhysicsDebugDrawer.h"
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "GraphicsManager.h"
#include "Utilities/RenderNode.h"
#include "checkML.h"

namespace Tapioca {
PhysicsDebugDrawer::PhysicsDebugDrawer() : mode(DBG_DrawWireframe) {
    graphicsManager = GraphicsManager::instance();
    if (graphicsManager != nullptr) node = graphicsManager->createSelfManagedNode();
    else
        node = nullptr;
}

PhysicsDebugDrawer::~PhysicsDebugDrawer() {
    graphicsManager = nullptr;
    node = nullptr;
}

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    auto g = GraphicsManager::instance();
    Ogre::ManualObject* line = g->createManualObject(node);
    line->begin("whiteDebug", Ogre::RenderOperation::OT_LINE_LIST);
    line->position(from.x(), from.y(), from.z());
    line->position(to.x(), to.y(), to.z());
    line->colour(color.x(), color.y(), color.z());
    line->end();
    line->setCastShadows(false);
    lines.push_back(line);
}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString) {
#ifdef _DEBUG
    std::string message = "PhysicsDebugDrawer: " + std::string(warningString);
    logWarn(message.c_str());
#endif
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance,
                                          int lifeTime, const btVector3& color) {
    auto g = GraphicsManager::instance();
    Ogre::ManualObject* line = g->createManualObject(node);
    line->begin("whiteDebug", Ogre::RenderOperation::OT_POINT_LIST);
    line->position(PointOnB.x(), PointOnB.y(), PointOnB.z());
    btVector3 to = PointOnB + normalOnB * distance;
    line->position(to.x(), to.y(), to.z());
    line->colour(color.x(), color.y(), color.z());
    line->end();
    line->setCastShadows(false);
    lines.push_back(line);
}

void PhysicsDebugDrawer::clearLines() {
    auto g = GraphicsManager::instance();
    for (std::list<Ogre::ManualObject*>::iterator it = lines.begin(); it != lines.end();) {
        g->destroyManualObject(*it);
        it = lines.erase(it);
    }
}
}
