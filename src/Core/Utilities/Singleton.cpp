#include "Singleton.h"

#include "Structure/Game.h"
#include "WindowManager.h"
#include "GraphicsEngine.h"
#include "UIManager.h"
#include "InputManager.h"
#include "Structure/FactoryManager.h"
#include "PhysicsManager.h"
#include "SceneManager.h"
#include "SoundEngine.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<Game>;
template class TAPIOCA_API Singleton<WindowManager>;
template class TAPIOCA_API Singleton<GraphicsEngine>;
template class TAPIOCA_API Singleton<UIManager>;
template class TAPIOCA_API Singleton<InputManager>;
template class TAPIOCA_API Singleton<FactoryManager>;
template class TAPIOCA_API Singleton<PhysicsManager>;
template class TAPIOCA_API Singleton<SceneManager>;
template class TAPIOCA_API Singleton<SoundEngine>;

template<>
Game* Singleton<Game>::instance_ = nullptr;

template<>
WindowManager* Singleton<WindowManager>::instance_ = nullptr;

template<>
GraphicsEngine* Singleton<GraphicsEngine>::instance_ = nullptr;

template<>
UIManager* Singleton<UIManager>::instance_ = nullptr;

template<>
InputManager* Singleton<InputManager>::instance_ = nullptr;

template<>
FactoryManager* Singleton<FactoryManager>::instance_ = nullptr;

template<>
PhysicsManager* Singleton<PhysicsManager>::instance_ = nullptr;

template<>
SceneManager* Singleton<SceneManager>::instance_ = nullptr;

template<>
SoundEngine* Singleton<SoundEngine>::instance_ = nullptr;
}
