#pragma once
#include "Structure/Component.h"
#include <unordered_map>

namespace Tapioca {
class NodeAnimator;
class Vector3;

class TAPIOCA_API KeyFrameAnimator : public Component {
private:
    struct KeyFramesInfo {
        int size = 0;
        std::unordered_map<int, Vector3> pos;
        std::unordered_map<int, Vector3> scale;
        std::unordered_map<int, Vector3> rot;
    };
    KeyFramesInfo info;
    NodeAnimator* nodeAnimator;
    float speed;
    float duration;
    bool playByDefault;
    bool loop;

public:
    COMPONENT_ID("KeyFrameAnimator")

    KeyFrameAnimator();
    ~KeyFrameAnimator();

    bool initComponent(const CompMap& variables) override;

    void start() override;

    void update(uint64_t delt) override;

    void play(const bool loop);

    void enable(const bool enabled);

    void setLoop(const bool loop);

    bool isLooping() const;

    bool isEnabled() const;

    bool hasEnded() const;
};
}