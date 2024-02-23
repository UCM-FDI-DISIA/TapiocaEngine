#pragma once
#include "../Utilities/Singleton.h"

namespace Tapioca {
class Factory : public Singleton<Factory> {
    friend Singleton<Factory>;

};
}
