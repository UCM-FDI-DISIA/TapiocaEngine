#include "PhysicsDebugDrawer.h"
#include <Ogre.h>
#include "../Graphics/GraphicsEngine.h"
#include "Utilities/checkML.h"

Tapioca::PhysicsDebugDrawer::PhysicsDebugDrawer()
    : mode(DBG_DrawWireframe) { }

Tapioca::PhysicsDebugDrawer::~PhysicsDebugDrawer() {  }

void Tapioca::PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) { 
	Ogre::ManualObject* line = GraphicsEngine::instance()->getSceneManager()->createManualObject();
    line->begin("white", Ogre::RenderOperation::OT_LINE_LIST);
    line->position(from.x(), from.y(), from.z());
    line->position(to.x(), to.y(), to.z());
    line->colour(color.x(), color.y(), color.z());
    line->end();
    GraphicsEngine::instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode()->attachObject(line);
    lines.push_back(line);
}

void Tapioca::PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, 
	const btVector3& fromColor, const btVector3& toColor) { }

void Tapioca::PhysicsDebugDrawer::reportErrorWarning(const char* warningString) { }

void Tapioca::PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, 
	btScalar distance, int lifeTime, const btVector3& color) { }

void Tapioca::PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString) { }

void Tapioca::PhysicsDebugDrawer::drawTransform(const btTransform& transform, btScalar orthoLen) { }

void Tapioca::PhysicsDebugDrawer::clearLines() {
    for (std::list<Ogre::ManualObject*>::iterator it = lines.begin(); it != lines.end();) {
        GraphicsEngine::instance()->getSceneManager()->destroyManualObject(*it);
        it = lines.erase(it);
    }
    
}
