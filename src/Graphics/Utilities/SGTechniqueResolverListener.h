#pragma once
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <OgreRTShaderSystem.h>
#include <OgreMaterialManager.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

/*
// @brief  Clase encargada de generar Shaders para los materiales que no tengan
*/
class SGTechniqueResolverListener : public Ogre::MaterialManager::Listener {
public:
    explicit SGTechniqueResolverListener(Ogre::RTShader::ShaderGenerator* const pShaderGenerator);

    /** This is the hook point where shader based technique will be created.
        It will be called whenever the material manager won't find appropriate technique
        that satisfy the target scheme name. If the scheme name is out target RT Shader System
        scheme name we will try to create shader generated technique for it.
    */
    Ogre::Technique* handleSchemeNotFound(const unsigned short schemeIndex, Ogre::String const& schemeName,
                                          Ogre::Material* const originalMaterial, const unsigned short lodIndex,
                                          const Ogre::Renderable* const rend);

    bool afterIlluminationPassesCreated(Ogre::Technique* const tech);

    bool beforeIlluminationPassesCleared(Ogre::Technique* const tech);

protected:
    Ogre::RTShader::ShaderGenerator* mShaderGenerator;   // The shader generator instance.
};

/** @} */
/** @} */