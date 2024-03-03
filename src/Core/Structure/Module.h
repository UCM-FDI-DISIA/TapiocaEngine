#pragma once
#include <cstdint>

namespace Tapioca {
class Module {
public:
    Module();
    virtual ~Module() { }

    virtual bool init() { return true; }
    virtual void start() { }
    virtual void update(const uint64_t deltaTime) { }
    virtual void handleEvents() { }
    virtual void fixedUpdate() { }
    virtual void render() { }
    virtual void refresh() { }
};
}