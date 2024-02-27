#pragma once
#include <string>
namespace Ogre { 
class Camera;
class SceneManager;
}
namespace Tapioca { 
class GraphicsEngine;
class Node;
class Vector3;
}
namespace Tapioca {
class Camera { 
public:
    friend GraphicsEngine;

private:
    Ogre::Camera* mCam;
    Node* mNode;
    Camera(Ogre::SceneManager* scnMgr, Node* n, std::string name, Vector3 look, int clipdst, bool autoAR);
    Ogre::Camera* getCamera() { return mCam; };
   

public:
void setLookAt(Vector3 look);
void setNearClipDistance(float dst);


};
}