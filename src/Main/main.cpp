#pragma once
#include "checkML.h"
#include "Exports.h"
#include "Utilities/Quaternion.h"
#include "Utilities/Vector3.h"

int main(int argc, char** argv) {
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    Tapioca::initEngine();
    Tapioca::runEngine();
    Tapioca::deleteEngine();

#ifdef _DEBUG
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif
    return 0;
}
