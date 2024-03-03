#pragma once
#include "Structure/Component.h"
#include <string>

namespace TapiocaGame {
class EjemploComponent : public Tapioca::Component {
private:
    int a;
    float b;
    bool c;
    std::string d;

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
