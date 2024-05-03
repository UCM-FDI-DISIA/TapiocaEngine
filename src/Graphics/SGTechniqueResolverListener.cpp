#include "SGTechniqueResolverListener.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <OgreTechnique.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

SGTechniqueResolverListener::SGTechniqueResolverListener(Ogre::RTShader::ShaderGenerator* const pShaderGenerator)
    : mShaderGenerator(pShaderGenerator) { }

Ogre::Technique* SGTechniqueResolverListener::handleSchemeNotFound(const unsigned short schemeIndex,
                                                                   const Ogre::String& schemeName,
                                                                   Ogre::Material* const originalMaterial,
                                                                   const unsigned short lodIndex,
                                                                   const Ogre::Renderable* const rend) {
    if (!mShaderGenerator->hasRenderState(schemeName)) return NULL;

    bool techniqueCreated = mShaderGenerator->createShaderBasedTechnique(
        *originalMaterial, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, schemeName);

    if (!techniqueCreated) return NULL;

    mShaderGenerator->validateMaterial(schemeName, *originalMaterial);

    for (auto* t : originalMaterial->getTechniques()) {
        if (t->getSchemeName() == schemeName) return t;
    }
    return NULL;
}

bool SGTechniqueResolverListener::afterIlluminationPassesCreated(Ogre::Technique* const tech) {
    if (mShaderGenerator->hasRenderState(tech->getSchemeName())) {
        Ogre::Material* mat = tech->getParent();
        mShaderGenerator->validateMaterialIlluminationPasses(tech->getSchemeName(), mat->getName(), mat->getGroup());
        return true;
    }
    return false;
}

bool SGTechniqueResolverListener::beforeIlluminationPassesCleared(Ogre::Technique* const tech) {
    if (mShaderGenerator->hasRenderState(tech->getSchemeName())) {
        Ogre::Material* mat = tech->getParent();
        mShaderGenerator->invalidateMaterialIlluminationPasses(tech->getSchemeName(), mat->getName(), mat->getGroup());
        return true;
    }
    return false;
}
