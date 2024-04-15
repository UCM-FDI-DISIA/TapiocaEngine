#pragma once
#include "checkML.h"
#include "Exports.h"
#include "Utilities/Quaternion.h"
#include "Utilities/Vector3.h"

int main(int argc, char** argv) {
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    
    Tapioca::Vector3 v1 = Tapioca::Vector3(0, 1, 0);
    Tapioca::Quaternion q1 = Tapioca::Quaternion(90 ,Tapioca::Vector3(0,0,1));
    Tapioca::Vector3 v2 = Tapioca::Vector3(0, 0, 1);
    Tapioca::Quaternion q2 = Tapioca::Quaternion(0.7071,0,0.7071,0.0);
    Tapioca::Quaternion q3 = q2 * q1;
    Tapioca::Quaternion q4 = Tapioca::Quaternion(Tapioca::Vector3(20.0f,85.0f,60.0f));
    //coicide con el resultado de lso ejes en ZYX 
    https://www.andre-gaschler.com/rotationconverter/
    Tapioca::Vector3 v3 = q4.taitBryan();
    Tapioca::Vector3 v4 = q4.eulerAxis();

    Tapioca::initEngine();
    Tapioca::runEngine();
    Tapioca::deleteEngine();

#ifdef _DEBUG
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif
    return 0;
}
