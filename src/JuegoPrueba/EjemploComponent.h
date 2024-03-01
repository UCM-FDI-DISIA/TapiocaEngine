#pragma once
#include <string>
#include "Structure/Component.h"

namespace Tapioca {
class EjemploComponent : public Component {
private:
    char a;
    int b;
    float c;
    bool d;
    std::string name;

public:
    COMPONENT_ID("EjemploComponent")

    EjemploComponent();
    ~EjemploComponent() { }
    void initComponent(const CompMap& map) override;
    // void update(const uint64_t deltaTime) override;
    // void handleEvents() override;
    // void fixedUpdate() override;
};
}
