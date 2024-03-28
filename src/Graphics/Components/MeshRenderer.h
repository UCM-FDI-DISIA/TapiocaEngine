#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class Mesh;
class RenderNode;
class Transform;
class Animator;

class TAPIOCA_API MeshRenderer : public Component {
    friend Animator;

private:
    RenderNode* node;
    Mesh* mesh;
    Transform* transform;
    // warning C4251 'Tapioca::MeshRenderer::meshName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::MeshRenderer'
    // warning C4251 'Tapioca::MeshRenderer::materialName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::MeshRenderer'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string meshName;
    std::string materialName;
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    Mesh* getMesh() const;
    Vector3 initialRotation;

public:
    COMPONENT_ID("MeshRenderer")
    MeshRenderer();
    ~MeshRenderer();

    bool initComponent(const CompMap& variables) override;
    // se necesita una awake para que el animator puede obtener el mesh
    void awake() override;
    void handleEvent(std::string const& id, void* info) override;

    void setMeshName(const std::string& n);
    void setMaterialName(const std::string& n);
};
}