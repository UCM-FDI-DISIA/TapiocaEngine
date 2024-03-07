#pragma once
#include "RenderObject.h"
#include "Utilities/Vector3.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {

class TAPIOCA_API Skyplane {
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
    Skyplane(Ogre::SceneManager* const scnMgr, std::string const& materialName, const bool enable = true,
             const Vector3 rkNormal = {0.0f, 0.0f, 1.0f}, const float fConstant = -20.0f, const float scale = 0.3f,
             const float tiling = 1.0f, const bool drawFirst = true, const float bow = 1.0f, const int xsegments = 100,
             const int ysegments = 100);

public:
    void setEnable(const bool enable);
    bool isEnabled();
};
}
