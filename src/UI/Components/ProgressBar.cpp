#include "ProgressBar.h"

Tapioca::ProgressBar::ProgressBar(float initialprog, Vector4 col, Vector2 pos, Vector2 siz, std::string backtext) {
    progress = initialprog;
    color = col;
    position = pos;
    size = siz;
    backgroundtext = backtext;
}
