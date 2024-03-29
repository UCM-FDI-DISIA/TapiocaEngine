#include "PhysicsDebugDrawer.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "GraphicsManager.h"
#include "RenderNode.h"
#include "checkML.h"

namespace Tapioca {

PhysicsDebugDrawer::PhysicsDebugDrawer()
    : mode(DBG_DrawWireframe), node(GraphicsManager::instance()->createSelfManagedNode()) { }

PhysicsDebugDrawer::~PhysicsDebugDrawer() { }

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    auto g = GraphicsManager::instance();
    Ogre::ManualObject* line = g->createManualObject(node);
    line->begin("white", Ogre::RenderOperation::OT_LINE_LIST);
    line->position(from.x(), from.y(), from.z());
    line->position(to.x(), to.y(), to.z());
    line->colour(color.x(), color.y(), color.z());
    line->end();
    lines.push_back(line);
}

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor,
                                  const btVector3& toColor) { }

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString) {
#ifdef _DEBUG
    std::cout << warningString << '\n';
#endif
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance,
                                          int lifeTime, const btVector3& color) {
    auto g = GraphicsManager::instance();
    Ogre::ManualObject* line = g->createManualObject(node);
    line->begin("white", Ogre::RenderOperation::OT_POINT_LIST);
    line->position(PointOnB.x(), PointOnB.y(), PointOnB.z());
    btVector3 to = PointOnB + normalOnB * distance;
    line->position(to.x(), to.y(), to.z());
    line->colour(color.x(), color.y(), color.z());
    line->end();
    lines.push_back(line);
}

//void PhysicsDebugDrawer::drawTransform(const btTransform& transform, btScalar orthoLen) { }

void PhysicsDebugDrawer::clearLines() {
    auto g = GraphicsManager::instance();
    for (std::list<Ogre::ManualObject*>::iterator it = lines.begin(); it != lines.end();) {
        g->destroyManualObject(*it);
        it = lines.erase(it);
    }
}


}
