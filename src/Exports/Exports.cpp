#include "Exports.h"
#include <lua.hpp>
#include <sstream>
#include "Structure/MainLoop.h"
#include "Structure/DynamicLibraryLoader.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "Structure/FactoryManager.h"
#include "SceneLoader.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "UIManager.h"
#include "SoundManager.h"
#include "LuaManager.h"
#include "LuaRegistry.h"
#include "Structure/BasicBuilder.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Components/MeshRenderer.h"
#include "Components/CameraComponent.h"
#include "Components/Animator.h"
#include "Components/ParticleSystemComponent.h"
#include "Components/PlaneComponent.h"
#include "Components/SpriteRenderer.h"
#include "Components/LightDirComp.h"
#include "Components/LightPointComp.h"
#include "Components/LightRectComp.h"
#include "Components/LightSpotComp.h"
#include "Components/KeyFrameAnimator.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ImageTextButton.h"
#include "Components/InputText.h"
#include "Components/Line.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/Text.h"
#include "Components/AudioListenerComponent.h"
#include "Components/AudioSourceComponent.h"
#include "checkML.h"

namespace Tapioca {
void initEngine() {
    mainLoop = MainLoop::create();
    createModules(mainLoop);
}

void deleteEngine() {
    if (mainLoop != nullptr) delete mainLoop;
    mainLoop = nullptr;
}

void runEngine(std::string const& gameName) {
    if (mainLoop->init()) {
        createEngineBuilders();
        registerLuaFunctions();
        if (mainLoop->loadGame(gameName)) mainLoop->run();
        else logError("Exports: runEngine: Error al cargar el juego.");
    }
    else logError("Exports: runEngine: Error al inicializar un modulo.");
}

static void createModules(MainLoop* mainLoop) {
    window = WindowManager::create();
    mainLoop->addModule(window);

    graphics = GraphicsManager::create();
    mainLoop->addModule(graphics);

    ui = UIManager::create();
    mainLoop->addModule(ui);

    input = InputManager::create();
    mainLoop->addModule(input);
    mapInput();

    factories = FactoryManager::create();
    mainLoop->addModule(factories);

    physics = PhysicsManager::create();
    mainLoop->addModule(physics);

    scenes = SceneLoader::create();
    mainLoop->addModule(scenes);

    sound = SoundManager::create();
    mainLoop->addModule(sound);

    lua = LuaManager::create();
    mainLoop->addModule(lua);

    // Importante: UI tiene que suscribirse antes que input
    // para ignorar eventos de input que interactuen con la UI
    // y ambos deberian ser los ultimos para que el resto de
    // eventos de ventana no se ignoren por los eventos de UI
    if (graphics != nullptr) window->subscribeModule(graphics);
    if (ui != nullptr) window->subscribeModule(ui);
    if (window != nullptr) window->subscribeModule(input);
}

static void createEngineBuilders() {
    logInfo("RunEngine: Anadiendo contructores del motor.");
    FactoryManager* manager = FactoryManager::instance();
    // Core
    manager->addBuilder(new BasicBuilder<Transform>());
    // Physiscs
    manager->addBuilder(new BasicBuilder<RigidBody>());
    // Graphics
    manager->addBuilder(new BasicBuilder<MeshRenderer>());
    manager->addBuilder(new BasicBuilder<CameraComponent>());
    manager->addBuilder(new BasicBuilder<Animator>());
    manager->addBuilder(new BasicBuilder<KeyFrameAnimator>());
    manager->addBuilder(new BasicBuilder<ParticleSystemComponent>());
    manager->addBuilder(new BasicBuilder<SpriteRenderer>());
    manager->addBuilder(new BasicBuilder<PlaneComponent>());
    manager->addBuilder(new BasicBuilder<LightDirComp>());
    manager->addBuilder(new BasicBuilder<LightPointComp>());
    manager->addBuilder(new BasicBuilder<LightRectComp>());
    manager->addBuilder(new BasicBuilder<LightSpotComp>());
    // UI
    manager->addBuilder(new BasicBuilder<Button>());
    manager->addBuilder(new BasicBuilder<Image>());
    manager->addBuilder(new BasicBuilder<ImageTextButton>());
    manager->addBuilder(new BasicBuilder<InputText>());
    manager->addBuilder(new BasicBuilder<Line>());
    manager->addBuilder(new BasicBuilder<ProgressBar>());
    manager->addBuilder(new BasicBuilder<Slider>());
    manager->addBuilder(new BasicBuilder<Text>());
    // Sound
    manager->addBuilder(new BasicBuilder<AudioListenerComponent>());
    manager->addBuilder(new BasicBuilder<AudioSourceComponent>());
}

static void registerLuaFunctions() {
    LuaRegistry* reg = lua->getRegistry();

    luabridge::getGlobalNamespace(lua->getLuaState())
        .beginNamespace("Tapioca")
            .addFunction("loadScene",
                +[](std::string name) -> bool {
                    return scenes->loadScene(name) != nullptr;
                })
            .addFunction("exit",
                +[]() -> void {
                    mainLoop->exit();
                })
            .addFunction("deleteScene",
                +[](std::string name) -> void {
                    mainLoop->deleteScene(name);
                })
            .addFunction("getScene",
                +[](std::string name) -> Scene* {
                    return mainLoop->getScene(name);
                })
            .addFunction("getLoadedScenes",
                +[]() -> std::unordered_map<std::string, Scene*> {
                    return mainLoop->getLoadedScenes();
                })
            //.addFunction("", +[]() -> void {})
            .deriveClass<Transform, Component>("Transform")
                .addProperty("position",
                    [](Transform* trans) -> Vector3 { return trans->getPosition(); },
                    [](Transform* trans, const Vector3& position) { trans->setPosition(position); })
                .addFunction("getGlobalPositionWithoutRotation", &Transform::getGlobalPositionWithoutRotation)
                .addFunction("getGlobalPosition", &Transform::getGlobalPosition)
                .addFunction("setGlobalPosition", &Transform::setGlobalPosition)
                .addProperty("rotation",
                    [](Transform* trans) -> Quaternion { return trans->getRotation(); },
                    [](Transform* trans, const Quaternion& rotation) { trans->setRotation(rotation); })
                .addFunction("getGlobalRotation", &Transform::getGlobalRotation)
                .addProperty("scale",
                    [](Transform* trans) -> Vector3 { return trans->getScale(); },
                    [](Transform* trans, const Vector3& scale) { trans->setScale(scale); })
                .addFunction("getGlobalScale", &Transform::getGlobalScale)
                .addFunction("translate", &Transform::translate)
                .addFunction("rotate", luabridge::overload<const Quaternion&>(&Transform::rotate))
                .addFunction("right", &Transform::right)
                .addFunction("up", &Transform::up)
                .addFunction("forward", &Transform::forward)
                .addProperty("parent", &Transform::getParent, &Transform::setParent)
                .addFunction("getChildren", &Transform::getChildren)
                .addFunction("getAllChildren", &Transform::getAllChildren)
            .endClass()
            .deriveClass<Animator, Component>("Animator")
                .addFunction("playAnim", &Animator::playAnim)
                .addProperty("speed", &Animator::getSpeed, &Animator::setSpeed)
                .addProperty("looping", &Animator::getLoop, &Animator::setLoop)
                .addProperty("playing", &Animator::getPlaying, &Animator::setPlaying)
                .addFunction("hasEnded", &Animator::hasEnded)
                .addFunction("getAnimName", &Animator::getAnimName)
            .endClass()
            .deriveClass<CameraComponent, Component>("CameraComponent")
                .addFunction("lookAt", &CameraComponent::lookAt)
                .addFunction("setDirection", &CameraComponent::setDirection)
                .addFunction("setNearClipDistance", &CameraComponent::setNearClipDistance)
                .addFunction("setFarClipDistance", &CameraComponent::setFarClipDistance)
                .addFunction("setFOVYRadians", &CameraComponent::setFOVYRadians)
                .addFunction("setFOVYDegrees", &CameraComponent::setFOVYDegrees)
                .addFunction("setDimensions", &CameraComponent::setDimensions)
                .addFunction("getWidthInPixels", &CameraComponent::getWidthInPixels)
                .addFunction("getHeightInPixels", &CameraComponent::getHeightInPixels)
                .addFunction("setBackground", &CameraComponent::setBackground)
                .addProperty("zOrder", &CameraComponent::getZOrder, &CameraComponent::setZOrder)
            .endClass()
            .deriveClass<KeyFrameAnimator, Component>("KeyFrameAnimator")
                .addFunction("play", &KeyFrameAnimator::play)
                .addFunction("enabled", &KeyFrameAnimator::isEnabled, &KeyFrameAnimator::enable)
                .addProperty("looping", &KeyFrameAnimator::isLooping, &KeyFrameAnimator::setLoop)
                .addProperty("ended", &KeyFrameAnimator::hasEnded)
            .endClass()
            .deriveClass<LightDirComp, Component>("LightDirComp")
                .addFunction("setColor", &LightDirComp::setColor)
                .addFunction("setPowerScale", &LightDirComp::setPowerScale)
                .addFunction("setDirection", &LightDirComp::setDirection)
                .addFunction("produceShadows", &LightDirComp::produceShadows)
            .endClass()
            .deriveClass<LightPointComp, Component>("LightPointComp")
                .addFunction("setColor", &LightPointComp::setColor)
                .addFunction("setPowerScale", &LightPointComp::setPowerScale)
                .addFunction("setAttenuation", &LightPointComp::setAttenuation)
                .addProperty("visible", &LightPointComp::isVisible, &LightPointComp::setVisible)
            .endClass()
            .deriveClass<LightRectComp, Component>("LightRectComp")
                .addFunction("setColor", &LightRectComp::setColor)
                .addFunction("setPowerScale", &LightRectComp::setPowerScale)
                .addFunction("setAttenuation", &LightRectComp::setAttenuation)
                .addFunction("setWidth", &LightRectComp::setWidth)
                .addFunction("setHeight", &LightRectComp::setHeight)
                .addFunction("setSize", &LightRectComp::setSize)
                .addProperty("visible", &LightRectComp::isVisible, &LightRectComp::setVisible)
            .endClass()
            .deriveClass<LightSpotComp, Component>("LightSpotComp")
                .addFunction("setColor", &LightSpotComp::setColor)
                .addFunction("setPowerScale", &LightSpotComp::setPowerScale)
                .addFunction("setAttenuation", &LightSpotComp::setAttenuation)
                .addFunction("setDirection", &LightSpotComp::setDirection)
                .addFunction("setNearClipDistance", &LightSpotComp::setNearClipDistance)
                .addFunction("setInnerAngle", &LightSpotComp::setInnerAngle)
                .addFunction("setOuterAngle", &LightSpotComp::setOuterAngle)
                .addFunction("setFalloff", &LightSpotComp::setFalloff)
                .addProperty("visible", &LightSpotComp::isVisible, &LightSpotComp::setVisible)
            .endClass()
            .deriveClass<MeshRenderer, Component>("MeshRenderer")
                .addFunction("setMaterial", &MeshRenderer::setMaterial)
                .addFunction("setMesh", &MeshRenderer::setMesh)
                .addProperty("visible", &MeshRenderer::isVisible, &MeshRenderer::setVisible)
            .endClass()
            .deriveClass<ParticleSystemComponent, Component>("ParticleSystemComponent")
                .addProperty("emitting",
                    [](ParticleSystemComponent* self) -> bool { return self->isEmitting(); },
                    [](ParticleSystemComponent* self, bool emitting) { self->setEmitting(emitting); })
                .addProperty("quota",
                    [](ParticleSystemComponent* self) -> bool { return self->getQuota(); },
                    [](ParticleSystemComponent* self, size_t quota) { self->setQuota(quota); })
                .addProperty("visible", &ParticleSystemComponent::isVisible, &ParticleSystemComponent::setVisible)
                .addFunction("fastForward", &ParticleSystemComponent::fastForward)
            .endClass()
            .deriveClass<PlaneComponent, Component>("PlaneComponent")
                .addProperty("visible", &PlaneComponent::isVisible, &PlaneComponent::setVisible)
            .endClass()
            .deriveClass<SpriteRenderer, Component>("SpriteRenderer")
                .addProperty("visible", &SpriteRenderer::isVisible, &SpriteRenderer::setVisible)
            .endClass()
            .deriveClass<RigidBody, Component>("RigidBody")
                .addProperty("active",
                    [](RigidBody* self) -> bool { return self->isActive(); },
                    [](RigidBody* self, bool active) { self->setActive(active); })
                .addProperty("movementType", &RigidBody::getMovementType, &RigidBody::setMomeventType)
                .addProperty("trigger", &RigidBody::getTrigger, &RigidBody::setTrigger)
                .addProperty("colliderShape", &RigidBody::getColliderShape, &RigidBody::setColliderShape)
                .addProperty("mask", &RigidBody::getMask, &RigidBody::setMask)
                .addProperty("group", &RigidBody::getGroup, &RigidBody::setGroup)
                .addProperty("mass", &RigidBody::getMass, &RigidBody::setMass)
                .addProperty("tensor", &RigidBody::getTensor, &RigidBody::setTensor)
                .addProperty("friction", &RigidBody::getFriction, &RigidBody::setFriction)
                .addProperty("damping", &RigidBody::getDamping, &RigidBody::setDamping)
                .addProperty("bounciness", &RigidBody::getBounciness, &RigidBody::setBounciness)
                .addProperty("velocity", &RigidBody::getVelocity, &RigidBody::setVelocity)
                .addProperty("gravity", &RigidBody::getGravity, &RigidBody::setGravity)
                .addFunction("setTrackScale", &RigidBody::setTrackScale)
                .addFunction("addForce", &RigidBody::addForce)
                .addFunction("addImpulse", &RigidBody::addImpulse)
                .addFunction("clearForces", &RigidBody::clearForces)
            .endClass()
            .deriveClass<AudioListenerComponent, Component>("AudioListenerComponent")
                .addFunction("setVelocity", &AudioListenerComponent::setVelocity)
            .endClass()
            .deriveClass<AudioSourceComponent, Component>("AudioSourceComponent")
                .addFunction("playOnce", &AudioSourceComponent::playOnce)
                .addFunction("playLooped", &AudioSourceComponent::playLooped)
                .addFunction("stop", &AudioSourceComponent::stop)
                .addFunction("isPaused", &AudioSourceComponent::isPaused)
                .addFunction("hasFinished", &AudioSourceComponent::hasFinished)
                .addFunction("hasStopped", &AudioSourceComponent::hasStopped)
                .addFunction("pause", &AudioSourceComponent::pause)
                .addFunction("loop", &AudioSourceComponent::loop)
                .addFunction("setVolume", &AudioSourceComponent::setVolume)
            .endClass()
        .endNamespace()
        // Esto es importante ya que Lua no lo puede hacer por sí mismo
        .beginNamespace("casts")
            .beginNamespace("fromComponent")
                .addFunction("Transform",
                    +[](Component* variable) -> Transform* { return static_cast<Transform*>(variable); })
                .addFunction("Animator",
                    +[](Component* variable) -> Animator* { return static_cast<Animator*>(variable); })
                .addFunction("CameraComponent",
                    +[](Component* variable) -> CameraComponent* { return static_cast<CameraComponent*>(variable); })
                .addFunction("KeyFrameAnimator",
                    +[](Component* variable) -> KeyFrameAnimator* { return static_cast<KeyFrameAnimator*>(variable); })
                .addFunction("LightDirComp",
                    +[](Component* variable) -> LightDirComp* { return static_cast<LightDirComp*>(variable); })
                .addFunction("LightPointComp",
                    +[](Component* variable) -> LightPointComp* { return static_cast<LightPointComp*>(variable); })
                .addFunction("LightRectComp",
                    +[](Component* variable) -> LightRectComp* { return static_cast<LightRectComp*>(variable); })
                .addFunction("LightSpotComp",
                    +[](Component* variable) -> LightSpotComp* { return static_cast<LightSpotComp*>(variable); })
                .addFunction("MeshRenderer",
                    +[](Component* variable) -> MeshRenderer* { return static_cast<MeshRenderer*>(variable); })
                .addFunction("ParticleSystemComponent",
                    +[](Component* variable) -> ParticleSystemComponent* { return static_cast<ParticleSystemComponent*>(variable); })
                .addFunction("PlaneComponent",
                    +[](Component* variable) -> PlaneComponent* { return static_cast<PlaneComponent*>(variable); })
                .addFunction("SpriteRenderer",
                    +[](Component* variable) -> SpriteRenderer* { return static_cast<SpriteRenderer*>(variable); })
                .addFunction("RigidBody",
                    +[](Component* variable) -> RigidBody* { return static_cast<RigidBody*>(variable); })
                .addFunction("AudioListenerComponent",
                    +[](Component* variable) -> AudioListenerComponent* { return static_cast<AudioListenerComponent*>(variable); })
                .addFunction("AudioSourceComponent",
                    +[](Component* variable) -> AudioSourceComponent* { return static_cast<AudioSourceComponent*>(variable); })
            .endNamespace()
        .endNamespace();
}

void mapInput() {
    // Crea una instancia de LUA
    lua_State* luaState = luaL_newstate();

    // Construye la ruta completa al archivo LUA
    const std::string MAP_FILE = "controlsMapping.lua";
    std::string path = "assets\\" + MAP_FILE;

    // Si no puede cargar el archivo, se muestra un mensaje y cierra la instancia de LUA
    if (luaL_dofile(luaState, path.c_str()) != 0) {
        logError(("MapInput: Error al cargar el archivo Lua: " + std::string(lua_tostring(luaState, -1))).c_str());
        lua_close(luaState);
        return;
    }
    else {
        logInfo("MapInput: Mapa de controles cargado correctamente.");
        // Mete en el stack de luaState la variable global con el nombre events
        lua_getglobal(luaState, "events");

        // Si lo que haya en el stack de luaState es una tabla
        // (-1 es el nombre de la variable, en este caso, events)
        if (lua_istable(luaState, -1)) {
            // "Entra" a leer las variables de la tabla
            lua_pushnil(luaState);

            // Itera sobre todos los elementos de la tabla
            // (-2 es el valor de la variable. En este caso, todo lo que
            // haya dentro del {} despues de events =)
            while (lua_next(luaState, -2) != 0) {
                // Lee le nombre del evento
                std::string evt = lua_tostring(luaState, -2);

                // Lo separa por el caracter "_" (por si hay
                // varios controles mapeados al mismo evento)
                std::vector<std::string> tokens;
                std::stringstream ss(evt);
                std::string token = "";
                char delimiter = '_';
                while (std::getline(ss, token, delimiter))
                    tokens.push_back(token);

                // Obtiene el nombre del evento ("ev" + "_" + "nombre")
                evt = tokens[0] + "_" + tokens[1];


                std::string src = "";
                int ctrl = 0;

                // Si lo que haya en el stack de luaState es una tabla
                // (-1 es el nombre de la variable, en este caso, el nombre del evento)
                if (lua_istable(luaState, -1)) {
                    // "Entra" a leer las variables de la tabla
                    lua_pushnil(luaState);

                    // Itera sobre todos los elementos de la tabla
                    // (-2 es el valor de la variable. En este caso, todo lo que
                    // haya dentro del {} despues de ev_EVENTO =)
                    while (lua_next(luaState, -2) != 0) {
                        // Lee el nombre de la variable y su valor
                        std::string key = lua_tostring(luaState, -2);
                        std::string value = lua_tostring(luaState, -1);

                        // Lo saca del stack para leer la siguiente variable
                        lua_pop(luaState, 1);

                        // Si la variable es "src", el valor es la clave
                        // del evento de inputEventTriggered. Si es "control",
                        // el valor es la tecla/boton/eje/etc. mapeado al evento
                        if (key == "src") src = value;
                        else if (key == "control")
                            ctrl = stoi(value);
                    }
                    // Anade el evento al mapa de input

                    input->mapInput(evt, src, ctrl);

                    // Saca el evento del stack para leer el siguiente
                    lua_pop(luaState, 1);
                }
            }
            // Saca events del stack, en caso de que hubiera otra variable global a leer
            lua_pop(luaState, 1);
        }
    }

    lua_close(luaState);
}
}
