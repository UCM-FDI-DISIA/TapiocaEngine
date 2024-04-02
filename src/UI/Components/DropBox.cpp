#include "DropBox.h"

Tapioca::DropBox::DropBox(std::string title, std::vector<std::string> content, int intialselec, Vector2 pos,Vector2 siz) { 
	name = title;
    items = content;
    currentItem = intialselec;
    position = pos;
    size = siz;
}
