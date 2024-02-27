#pragma once
#include <cstdint>

namespace Tapioca {
class Module {
private:

public:
    Module();
    virtual ~Module() { }

    virtual void init() { }
    virtual void update(const uint64_t deltaTime) { }
    virtual void handleEvents() { }
    virtual void fixedUpdate() { }
    virtual void render() { }
};
}