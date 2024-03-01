#pragma once
#include <string>
#include "Structure/Component.h"
using namespace std;
using namespace Tapioca;

namespace TapiocaGame {
class EjemploComponent : public Component {
private:
    int a;
    float b;
    bool c;
    string d;

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
