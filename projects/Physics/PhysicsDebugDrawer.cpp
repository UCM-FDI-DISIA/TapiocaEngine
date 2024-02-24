#include "PhysicsDebugDrawer.h"
#include <Ogre.h>
#include "../Graphics/GraphicsEngine.h"

void Tapioca::PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) { 
	Ogre::ManualObject* line;
}

void Tapioca::PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, 
	const btVector3& fromColor, const btVector3& toColor) { }

void Tapioca::PhysicsDebugDrawer::reportErrorWarning(const char* warningString) { }

void Tapioca::PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, 
	btScalar distance, int lifeTime, const btVector3& color) { }

void Tapioca::PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString) { }

void Tapioca::PhysicsDebugDrawer::drawTransform(const btTransform& transform, btScalar orthoLen) { }
