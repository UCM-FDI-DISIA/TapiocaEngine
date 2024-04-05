#pragma once
#include "BaseWidget.h"
#include "Core.h"
#include <string>

namespace Tapioca {
class TAPIOCA_API Slider : public BaseWidget {
private:
    bool vertical;
    float currentValue;
    float max;
    float min;
    //a e y
    Vector2 position;
    //width y heitgh
    Vector2 size;

public:
    Slider(std::string title, bool verticalMode, float initValue, float maxValue, float minValue, Vector2 pos,
           Vector2 siz);
    Vector2 getPosition() { return position; }
    Vector2 getSize() { return size; }
    float* getCurrentValue() { return &currentValue; }
    float getMax() { return max; }
    float getMin() { return min; }
    bool isVertical() { return vertical; }
};
}
