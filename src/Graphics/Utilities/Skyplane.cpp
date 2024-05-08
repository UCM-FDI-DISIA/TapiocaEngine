#include "Skyplane.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "RenderNode.h"
#include "checkML.h"

namespace Tapioca {
Skyplane::Skyplane(Ogre::SceneManager* const scnMngr, RenderNode* const node, Ogre::MeshManager* const mshMgr,
    std::string const& materialName, std::string const& skyplaneName, const bool enable,
    const Vector3 rkNormal, const float fConstant, const float scale, const float tiling,
    const bool drawFirst, const float bow, const int xsegments, const int ysegments) : RenderObject(node, scnMngr), 
    sceneManager(scnMngr), material(materialName), rkNormal(rkNormal), fConstant(fConstant), scale(scale), 
    tiling(tiling), drawFirst(drawFirst), bow(bow), xSegments(xsegments), ySegments(ysegments) 
{
    mSkyPlaneAux = Ogre::Plane(Ogre::Vector3(rkNormal.x, rkNormal.y, rkNormal.z), Ogre::Real(0));

    Ogre::MaterialPtr m = Ogre::MaterialManager::getSingleton().getByName(
        materialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (!m) {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Sky plane material '" + materialName + "' not found.",
                    "setSkyPlane");
    }
    
    m->setDepthWriteEnabled(false);
    m->load();

    Ogre::MeshPtr planeMesh = Ogre::MeshManager::getSingleton()
        .getByName(skyplaneName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (planeMesh) Ogre::MeshManager::getSingleton().remove(planeMesh);

    Ogre::Vector3 up = mSkyPlaneAux.normal.crossProduct(Ogre::Vector3::UNIT_X);
    if (up == Ogre::Vector3::ZERO) up = mSkyPlaneAux.normal.crossProduct(-Ogre::Vector3::UNIT_Z);

    if (bow > 0) {
        planeMesh = Ogre::MeshManager::getSingleton()
            .createCurvedPlane(skyplaneName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
                               mSkyPlaneAux, scale * 100, scale * 100, scale * bow * 100, xsegments, 
                               ysegments, false, 1, tiling, tiling, up);
    }
    else {
        planeMesh = Ogre::MeshManager::getSingleton()
            .createPlane(skyplaneName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mSkyPlaneAux, 
                        scale * 100, scale * 100, xsegments, ysegments, false, 1, tiling, tiling, up);
    }

    if (mSkyPlane) {
        sceneManager->destroyEntity(skyplaneName);
        mSkyPlane = 0;
    }

    mSkyPlane = sceneManager->createEntity(skyplaneName);
    init(mSkyPlane);
}

void Skyplane::setEnable(const bool enable) { sceneManager->setSkyPlaneEnabled(enable); }

bool Skyplane::isEnabled() { return sceneManager->isSkyPlaneEnabled(); }
}
