#include "Skybox.h"
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

Skybox::Skybox(Ogre::SceneManager* const scnMgr, RenderNode* const node, std::string const& texture, const float distC,
               const bool orderC)
    : RenderObject(node, scnMgr), texture(texture), distC(distC), orderC(orderC) {
    scnM = scnMgr;

    Ogre::MaterialPtr m = Ogre::MaterialManager::getSingleton().getByName(
        texture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (!m) {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Sky box material '" + texture + "' not found.",
                    "SceneManager::setSkyBox");
    }
    // Ensure loaded
    m->load();

    bool valid = m->getBestTechnique() && m->getBestTechnique()->getNumPasses();
    if (valid) {
        Ogre::Pass* pass = m->getBestTechnique()->getPass(0);
        valid = valid && pass->getNumTextureUnitStates() &&
            pass->getTextureUnitState(0)->getTextureType() == Ogre::TEX_TYPE_CUBE_MAP;
    }

    if (!valid) {
        Ogre::LogManager::getSingleton().logWarning("skybox material " + texture + " is not supported, defaulting");
        m = Ogre::MaterialManager::getSingleton().getDefaultSettings();
    }

    // Create object
    if (!mSkyBoxObj) {
        mSkyBoxObj = std::make_unique<Ogre::ManualObject>("SkyBox");
        mSkyBoxObj->setCastShadows(false);
    }
    else {
        if (!mSkyBoxObj->isAttached()) {
        }
        mSkyBoxObj->clear();
    }

    mSkyBoxObj->setRenderQueueGroup(0);
    mSkyBoxObj->begin(texture, Ogre::RenderOperation::OT_TRIANGLE_STRIP,
                      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    // rendering cube, only using 14 vertices
    const Ogre::Vector3 cube_strip[14] = {
        {-1.f, 1.f, 1.f},     // Front-top-left
        {1.f, 1.f, 1.f},      // Front-top-right
        {-1.f, -1.f, 1.f},    // Front-bottom-left
        {1.f, -1.f, 1.f},     // Front-bottom-right
        {1.f, -1.f, -1.f},    // Back-bottom-right
        {1.f, 1.f, 1.f},      // Front-top-right
        {1.f, 1.f, -1.f},     // Back-top-right
        {-1.f, 1.f, 1.f},     // Front-top-left
        {-1.f, 1.f, -1.f},    // Back-top-left
        {-1.f, -1.f, 1.f},    // Front-bottom-left
        {-1.f, -1.f, -1.f},   // Back-bottom-left
        {1.f, -1.f, -1.f},    // Back-bottom-right
        {-1.f, 1.f, -1.f},    // Back-top-left
        {1.f, 1.f, -1.f}      // Back-top-right
    };

    for (const auto& vtx : cube_strip) {
        mSkyBoxObj->position(Ogre::Vector3(0, 0, 1) * (vtx * distC));
        // Note UVs mirrored front/back
        mSkyBoxObj->textureCoord(vtx.normalisedCopy() * Ogre::Vector3(1, 1, -1));
    }

    init(mSkyBoxObj.get());

    mSkyBoxObj->end();    
}

void Skybox::setEnable(const bool enable) { scnM->setSkyBoxEnabled(false); }

bool Skybox::isEnabled() { return scnM->isSkyBoxEnabled(); }
}
