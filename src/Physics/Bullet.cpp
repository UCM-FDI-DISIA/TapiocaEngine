#include <iostream>
#include "PhysicsManager.h"
#include "Utilities/Singleton.h"

int main() {

    std::cout << "Bullet\n";
   
    Tapioca::PhysicsManager* physMng= Tapioca::PhysicsManager::create();
    int i = 1;
    while (true) {
        physMng->update(1/60.f);
    }
    return 0;
}
