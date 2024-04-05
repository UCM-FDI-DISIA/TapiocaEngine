#include "Slider.h"

Tapioca::Slider::Slider(std::string title, bool verticalMode, float initValue, float maxValue, float minValue,
                        Vector2 pos, Vector2 siz) {
    name = title;
    vertical = verticalMode;
    currentValue = initValue;
    max = maxValue;
    min = minValue;
    position = pos;
    size = siz;
}
