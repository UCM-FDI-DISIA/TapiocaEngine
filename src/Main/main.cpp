#pragma once
#include "defs.h"
#include "checkML.h"
#include "Exports.h"
#include "../Core/Core.h"


int main(int argc, char** argv) {
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    Tapioca::Quaternion q = Tapioca::Quaternion(0,Tapioca::Vector3(0, 1, 0));
    Tapioca::Quaternion q2 = Tapioca::Quaternion(0, Tapioca::Vector3(0, 0, 1));
    Tapioca::Quaternion q3 = Tapioca::Quaternion(1,0,0,0.0000000017);
    Tapioca::Quaternion qc = q * q2*q3;
    Tapioca::Vector3 p = qc.rotatePoint(Tapioca::Vector3(0, 0, 1));
 
    Tapioca::initEngine();
    
    Tapioca::runEngine();
   // Tapioca::SoundEngine::instance()->testsample();
    Tapioca::deleteEngine();

#ifdef _DEBUG
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif
    return 0;
}
