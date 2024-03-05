#pragma once
#include "RenderObject.h"
#include <string>
#include "Utilities/Vector3.h"


namespace Ogre {
class SceneManager;
}

namespace Tapioca {
class Node;

class Skyplane : public RenderObject {
protected:
    Ogre::SceneManager* scnM;
    std::string material;
    Vector3 rkNormal;
    float fConstant;
    float scale;
    float tiling;
    bool drawFirst;
    float bow;
    int xSegments;
    int ySegments;

    // Construye un skyplane
    Skyplane(Ogre::SceneManager* scnMgr, Node* node, std::string const& materialName, bool enable = true,
             Vector3 rkNormal = {0.0f, 0.0f, 1.0f}, float fConstant = -20.0f, float scale = 0.3f, float tiling = 1.0f,
             bool drawFirst = true, float bow = 1.0f, int xsegments = 100, int ysegments = 100);

public:
    virtual ~Skyplane() {/*if (mSkyplane != nullptr) delete mSkyplane;*/};
    void setEnable(bool enable);
    bool isEnable();
};
}
