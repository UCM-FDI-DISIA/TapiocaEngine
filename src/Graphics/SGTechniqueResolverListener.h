#pragma once





#include <OgreRTShaderSystem.h>
#include <OgreMaterialManager.h>


/*
// @brief  Clase encargada de generar Shaders para los materiales que no tengan
*/
class SGTechniqueResolverListener: public Ogre::MaterialManager::Listener {
public:
    explicit SGTechniqueResolverListener(Ogre::RTShader::ShaderGenerator* pShaderGenerator);

    /** This is the hook point where shader based technique will be created.
        It will be called whenever the material manager won't find appropriate technique
        that satisfy the target scheme name. If the scheme name is out target RT Shader System
        scheme name we will try to create shader generated technique for it.
    */
    Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String& schemeName,
        Ogre::Material* originalMaterial, unsigned short lodIndex, const Ogre::Renderable* rend);

    bool afterIlluminationPassesCreated(Ogre::Technique* tech);

    bool beforeIlluminationPassesCleared(Ogre::Technique* tech);

protected:
    Ogre::RTShader::ShaderGenerator* mShaderGenerator;   // The shader generator instance.
};

/** @} */
/** @} */
