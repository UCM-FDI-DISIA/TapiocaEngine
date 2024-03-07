#pragma once
#include "Utilities/defs.h"

namespace Tapioca {
class TAPIOCA_API Module {
public:
    Module() { }
    virtual ~Module() { }

    virtual bool init() { return true; }
    virtual void start() { }
    virtual void update(const uint64_t deltaTime) { }
    virtual void sendEvents() { }
    virtual void handleEvent(std::string const& id, void* info) { }
    virtual void fixedUpdate() { }
    virtual void render() { }
    virtual void refresh() { }
};
}