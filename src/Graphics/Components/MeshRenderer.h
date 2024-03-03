#pragma once
#include "Structure/Component.h"

namespace Tapioca {
class Mesh;

class MeshRenderer : public Component {
private:
	Mesh* mesh;
	std::string meshName, materialName;
public:
	COMPONENT_ID("MeshRenderer")
	MeshRenderer();
	~MeshRenderer();

	bool initComponent(const CompMap& variables) override;
	void start() override;
};
}