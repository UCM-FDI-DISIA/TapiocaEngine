// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.
// SPDX-License-Identifier: MIT

#ifndef __GLSLangProgram_H__
#define __GLSLangProgram_H__

#include <OgreGLSLangProgramManagerExports.h>
#include "OgreHighLevelGpuProgram.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgrePlugin.h"

namespace Ogre
{
/** \addtogroup Plugins
 *  @{
 */
/** \defgroup GLSLangProgramManager GLSLangProgramManager
 *  GLslang can be used to compile common, high-level GLSL/ HLSL code down to SPIRV assembler
 *  language for both GL and Vulkan.
 *  @{
 */
/** Specialisation of HighLevelGpuProgram to provide support for the GLSLang compiler by Khronos.

    The program will negotiate with the renderer to compile the appropriate program
    for the API and graphics card capabilities.
*/
class GLSLangProgram : public HighLevelGpuProgram
{
    void createLowLevelImpl() override;
    void unloadHighLevelImpl() override;
    void loadFromSource() override;
    void buildConstantDefinitions() override;
    void prepareImpl() override;

    std::vector<uint32> mAssembly;
public:
    GLSLangProgram(ResourceManager* creator, const String& name, ResourceHandle handle, const String& group,
                   bool isManual, ManualResourceLoader* loader);
    ~GLSLangProgram();
    const String& getLanguage(void) const override;

    bool isSupported() const override;
};

/** Factory class for GLSLang programs. */
class GLSLangProgramFactory : public HighLevelGpuProgramFactory
{
public:
    GLSLangProgramFactory();
    ~GLSLangProgramFactory();
    /// Get the name of the language this factory creates programs for
    const String& getLanguage(void) const override;
    GpuProgram* create(ResourceManager* creator, const String& name, ResourceHandle handle,
                       const String& group, bool isManual, ManualResourceLoader* loader) override;
};

/** Plugin instance for Cg Program Manager */
class _OgreGLSLangProgramManagerExport GLSLangPlugin : public Plugin
{
public:
    const String& getName() const override;

    void install() override;

    void initialise() override;

    void shutdown() override;

    void uninstall() override;

protected:
    std::unique_ptr<GLSLangProgramFactory> mProgramFactory;
};
/** @} */
/** @} */
} // namespace Ogre

#endif
