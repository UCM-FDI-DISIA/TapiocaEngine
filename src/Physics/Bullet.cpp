#include <iostream>
#include "PhysicsManager.h"
#include "../Graphics/Singleton.h" 

int main() {

    std::cout << "Bullet\n";
   
    Tapioca::PhysicsManager* physMng= Tapioca::PhysicsManager::Singleton::init();
    int i = 1;
    while (true) {
        physMng->update(1.0f / 60.0f);
    }
    return 0;
}
