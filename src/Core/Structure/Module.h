#pragma once

namespace Tapioca {
class Module {
private:

public:
    Module() { }
    virtual ~Module() { }

    virtual void init() { }
    virtual void update() { }
    virtual void handleEvents() { }
    virtual void fixedUpdate() { }
    virtual void render() { }
};
}