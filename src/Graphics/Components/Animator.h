#pragma once
#include <string>
#include "Structure/Component.h"

namespace Tapioca {
class MeshRenderer;
class AnimationHelper;

class TAPIOCA_API Animator : public Component {

private:
    MeshRenderer* meshRenderer;
    AnimationHelper* anim;

    float speed;

public:
    COMPONENT_ID("Animator")
    Animator();
    ~Animator();

    bool initComponent(const CompMap& variables) override;
    void start() override;
    void update(uint64_t delt) override;

    void playAnim(std::string const& animName);

    void setSpeed(const float s);
    void setLoop(const bool l);
    void setPlaying(const bool p);

    float getSpeed() const;
    bool getLoop() const;
    bool getPlaying() const;
};
}