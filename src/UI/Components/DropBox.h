#pragma once
#include "Core.h"
#include <string>
namespace Tapioca {
class TAPIOCA_API  DropBox {

private:
    std::string name;
    std::vector< std::string> items;
    int currentItem;
    //a e y
    Vector2 position;
    //width y heitgh
    Vector2 size;

public:
    DropBox(std::string title, std::vector<std::string> content,int intialselec,Vector2 pos,Vector2 siz);
    Vector2 getPosition() { return position; }
    Vector2 getSize() { return size; }
    std::string getName() { return name; }
    std::vector<std::string> getContent() { return items; }
    int* getCurrent() { return &currentItem;}
};
}

