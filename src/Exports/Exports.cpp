#include <lua.hpp>
#include <sstream>
#include "Exports.h"
#include "Structure/Game.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "Structure/FactoryManager.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "UIManager.h"
#include "SoundManager.h"

// Componentes
#include "Structure/BasicBuilder.h"

// Core
#include "Components/Transform.h"
// Physics
#include "Components/RigidBody.h"
// Graphics
#include "Components/MeshRenderer.h"
#include "Components/CameraComponent.h"
#include "Components/Animator.h"
#include "Components/ParticleSystemComponent.h"
#include "Components/PlaneComponent.h"
#include "Components/SpriteRenderer.h"
// UI
#include "Components/Button.h"
#include "Components/InputText.h"
#include "Components/Text.h"
#include "Components/ImageButton.h"

//Sound
#include "Components/AudioListenerComponent.h"
#include "Components/AudioSourceComponent.h"

// PRUEBAS
#include "RenderNode.h"
#include "Mesh.h"
#include "LightDirectional.h"
#include "Utilities/Vector3.h"
#include "Utilities/Quaternion.h"

namespace Tapioca {
void initEngine() {
    game = Tapioca::Game::create();
    createModules(game);
}

void deleteEngine() { delete game; }

Vector3 right(Quaternion q) {

    Vector3 r(-1, 0, 0);   //X

    /*
    // Crear cuaterniones para las rotaciones en cada eje
    Quaternion q_x(rotation.x, Vector3(1, 0, 0));   // Rotación en el eje X
    Quaternion q_y(rotation.y, Vector3(0, 1, 0));   // Rotación en el eje Y
    Quaternion q_z(rotation.z, Vector3(0, 0, 1));   // Rotación en el eje Z

    // Multiplicar los cuaterniones en orden (Z * Y * X)
    Quaternion combinedRotation = q_z * q_y * q_x;
    */


    // Rotar el vector forward utilizando el cuaternion combinado
    Vector3 v = q.rotatePoint(r);

    v.normalize();

    return v;
}
Vector3 up(Quaternion q) {

    Vector3 u(0, 1, 0);   //Y

    /*
    // Crear cuaterniones para las rotaciones en cada eje
    Quaternion q_x(rotation.x, Vector3(1, 0, 0));   // Rotación en el eje X
    Quaternion q_y(rotation.y, Vector3(0, 1, 0));   // Rotación en el eje Y
    Quaternion q_z(rotation.z, Vector3(0, 0, 1));   // Rotación en el eje Z

    // Multiplicar los cuaterniones en orden (Z * Y * X)
    Quaternion combinedRotation = q_z * q_y * q_x;
    */


    // Rotar el vector forward utilizando el cuaternion combinado
    Vector3 v = q.rotatePoint(u);

    v.normalize();

    return v;
}
Vector3 forward(Quaternion q) {

    Vector3 r = right(q);
    Vector3 u = up(q);
    Vector3 v = u.cross(r);

    v.normalize();

    return v;
}

void runEngine() {
    if (game->init()) {
        createEngineBuilders();

        /*
        Vector3 fatherPos = Vector3(0.0f, 0.0f, 0.0f);
        Vector3 childPos = Vector3(40.0f, 20.0f, 0.0f);
        Vector3 childPos2 = Vector3(80.0f, 20.0f, 0.0f);

        // PRUEBAS DE GRAPHICS
        auto node1 = graphics->createNode(fatherPos, Vector3(3.1f, 3.1f, 3.1f));
        //auto sphere = graphics->createMesh(node1, "models/Sinbad.mesh");
        //sphere->castShadows(true);

        auto node2 = graphics->createNode(childPos, Vector3(3.1f, 3.1f, 3.1f));
        //auto sphere2 = graphics->createMesh(node2, "models/Sinbad.mesh");
        //sphere2->castShadows(true);
        ////sphere2->setMaterial("juan");

        auto node3 = graphics->createNode(childPos2, Vector3(3.1f, 3.1f, 3.1f));
        //auto sphere3 = graphics->createMesh(node3, "models/Sinbad.mesh");
        //sphere3->castShadows(true);

        auto node4 = graphics->createNode(Vector3(0.0f, 0.0f, 0.0f));
        auto light = graphics->createLightDirectional(node4, Tapioca::Vector3(0.0f, 0.0f, -1.0f));
        //light->produceShadows(true);

        Vector3 childRot2 = Vector3(90.0f, 0.0f, 0.0f);
        Quaternion childQ2 = Quaternion(childRot2);

        Vector3 childRot = Vector3(0.0f, 0.0f, 90.0f);
        Quaternion childQ = Quaternion(childRot);

        // PRIMERA VEZ

        // padre
        Vector3 fatherRot = Vector3(0.0f, 180.0f, 0.0f);
        Quaternion fatherQ = Quaternion(fatherRot);

        // roll, yaw y por ultimo, pitch
        node1->setRotation(fatherQ);

        Vector3 xAxis = -right(fatherRot);
        Vector3 yAxis = up(fatherRot);
        Vector3 zAxis = forward(fatherRot);

        // hijo
        // se supone que es la posicion local del hijo
        Vector3 point = childPos - fatherPos;

        Vector3 pos;
        pos.x = point.x * xAxis.x + point.y * yAxis.x + point.z * zAxis.x;
        pos.y = point.x * xAxis.y + point.y * yAxis.y + point.z * zAxis.y;
        pos.z = point.x * xAxis.z + point.y * yAxis.z + point.z * zAxis.z;

        pos = pos + fatherPos;
        node2->setPosition(pos);

        node2->setRotation(fatherQ * childQ);

        // hijo2

        Vector3 xAxis2 = -right(childQ);
        Vector3 yAxis2 = up(childQ);
        Vector3 zAxis2 = forward(childQ);

        // se supone que es la posicion local del hijo
        Vector3 point2 = childPos2 - childPos;

        Vector3 pos2;
        pos2.x = point2.x * xAxis2.x + point2.y * yAxis2.x + point2.z * zAxis2.x;
        pos2.y = point2.x * xAxis2.y + point2.y * yAxis2.y + point2.z * zAxis2.y;
        pos2.z = point2.x * xAxis2.z + point2.y * yAxis2.z + point2.z * zAxis2.z;

        pos2 = pos2 + childPos;

        point2 = pos2 - fatherPos;
        pos2.x = point2.x * xAxis.x + point2.y * yAxis.x + point2.z * zAxis.x;
        pos2.y = point2.x * xAxis.y + point2.y * yAxis.y + point2.z * zAxis.y;
        pos2.z = point2.x * xAxis.z + point2.y * yAxis.z + point2.z * zAxis.z;

        pos2 = pos2 + fatherPos;
        node3->setPosition(pos2);

        node3->setRotation(fatherQ * childQ * childQ2);

        Vector3 xAxis3 = -right(fatherQ * childQ);
        Vector3 yAxis3 = up(fatherQ * childQ);
        Vector3 zAxis3 = forward(fatherQ * childQ);

        // SEGUNDA VEZ
        //fatherRot = Vector3(0.0f, 0.0f, 180.0f);
        //fatherQ = Quaternion(fatherRot);

        //// roll, yaw y por ultimo, pitch
        //node1->setRotation(fatherQ);

        //xAxis = -right(fatherRot);
        //yAxis = up(fatherRot);
        //zAxis = forward(fatherRot);

        //point = childPos;

        //pos;
        //pos.x = point.x * xAxis.x + point.y * yAxis.x + point.z * zAxis.x;
        //pos.y = point.x * xAxis.y + point.y * yAxis.y + point.z * zAxis.y;
        //pos.z = point.x * xAxis.z + point.y * yAxis.z + point.z * zAxis.z;

        //pos = pos + fatherPos;
        //node2->setPosition(pos);

        //node2->setRotation(fatherQ * childQ);

        //// hijo 2

        //xAxis2 = -right(childQ);
        //yAxis2 = up(childQ);
        //zAxis2 = forward(childQ);

        //// se supone que es la posicion local del hijo
        //point2 = childPos2 - childPos;

        //pos2;
        //pos2.x = point2.x * xAxis2.x + point2.y * yAxis2.x + point2.z * zAxis2.x;
        //pos2.y = point2.x * xAxis2.y + point2.y * yAxis2.y + point2.z * zAxis2.y;
        //pos2.z = point2.x * xAxis2.z + point2.y * yAxis2.z + point2.z * zAxis2.z;

        //point2 = pos2 + childPos - fatherPos;
        //pos2.x = point2.x * xAxis.x + point2.y * yAxis.x + point2.z * zAxis.x;
        //pos2.y = point2.x * xAxis.y + point2.y * yAxis.y + point2.z * zAxis.y;
        //pos2.z = point2.x * xAxis.z + point2.y * yAxis.z + point2.z * zAxis.z;

        //pos2 = pos2 + fatherPos;
        //node3->setPosition(pos2);

        //node3->setRotation(childQ * fatherQ * childQ2);

        // cambiar la rotacion del hijo en el transform? NO
        // los ejes se calcular a partir de la rotacion global
        /*node2->rotate(fatherRot);
        node2->rotate(childRot);*/

        /*Quaternion fatherQ = Quaternion(fatherRot);
        Vector3 fatherE = fatherQ.euler();
        Quaternion childQ = Quaternion(childRot);
        Vector3 childE = childQ.euler();
        Quaternion q = fatherQ * childQ;
        node2->setRotation(q);*/
        // node2->setRotation(q.euler());

        //scenes->loadScene("archivo2.lua");

        // PRUEBAS DE CORE
        /*Quaternion q = Tapioca::Quaternion(0, Tapioca::Vector3(0, 1, 0));
        Quaternion q2 = Tapioca::Quaternion(0, Tapioca::Vector3(0, 0, 1));
        Quaternion q3 = Tapioca::Quaternion(1, 0, 0, 0.0000000017);
        Quaternion qc = q * q2 * q3;
        Vector3 p = qc.rotatePoint(Tapioca::Vector3(0, 0, 1));*/

        // PRUEBAS DE SOUND
        SoundManager::instance()->testsample();

       // scenes->loadScene("archivo2.lua");
        scenes->loadScene("startScene.lua");

        game->run();

        /*delete node1;
        delete node2;
        delete node3;
        delete node4;*/
    }
#ifdef _DEBUG
    else
        std::cerr << "Error al inicializar un modulo\n";
#endif
}

static void createModules(Tapioca::Game* game) {
    window = Tapioca::WindowManager::create();
    game->addModule(window);

    graphics = Tapioca::GraphicsManager::create();
    game->addModule(graphics);

    ui = Tapioca::UIManager::create();
    game->addModule(ui);

    input = Tapioca::InputManager::create();
    game->addModule(input);
    mapInput();

    factories = Tapioca::FactoryManager::create();
    game->addModule(factories);

    physics = Tapioca::PhysicsManager::create();
    game->addModule(physics);

    scenes = Tapioca::SceneManager::create();
    game->addModule(scenes);

    sound = Tapioca::SoundManager::create();
    game->addModule(sound);

    // Importante: UI tiene que suscribirse antes que input
    // para ignorar eventos de input que interactuen con la UI
    // y ambos deberian ser los ultimos para que el resto de
    // eventos de ventana no se ignoren por los eventos de UI
    if (graphics != nullptr) window->subscribeModule(graphics);
    if (ui != nullptr) window->subscribeModule(ui);
    if (window != nullptr) window->subscribeModule(input);
}

static void createEngineBuilders() {
#ifdef _DEBUG
    std::cout << "Anadiendo factorias del motor\n";
#endif
    FactoryManager* manager = FactoryManager::instance();
    // Core
    manager->addFactory(new BasicBuilder<Transform>());
    // Physiscs
    manager->addFactory(new BasicBuilder<RigidBody>());
    // Graphics
    manager->addFactory(new BasicBuilder<MeshRenderer>());
    manager->addFactory(new BasicBuilder<CameraComponent>());
    manager->addFactory(new BasicBuilder<Animator>());
    manager->addFactory(new BasicBuilder<ParticleSystemComponent>());
    manager->addFactory(new BasicBuilder<SpriteRenderer>());
    manager->addFactory(new BasicBuilder<PlaneComponent>());
    // UI
    manager->addFactory(new BasicBuilder<Button>());
    manager->addFactory(new BasicBuilder<InputText>());
    manager->addFactory(new BasicBuilder<Text>());
    manager->addFactory(new BasicBuilder<ImageButton>());

    //Sound
    manager->addFactory(new BasicBuilder<AudioListenerComponent>());
    manager->addFactory(new BasicBuilder<AudioSourceComponent>());
}

void mapInput() {
    // Crea una instancia de LUA
    lua_State* luaState = luaL_newstate();

    // Construye la ruta completa al archivo LUA
    const std::string MAP_FILE = "controlsMapping.lua";
    std::string path = "assets\\controls\\" + MAP_FILE;

    // Si no puede cargar el archivo, se muestra un mensaje y cierra la instancia de LUA
    if (luaL_dofile(luaState, path.c_str()) != 0) {
#ifdef _DEBUG
        std::cerr << "Error al cargar archivo LUA: " << lua_tostring(luaState, -1) << '\n';
#endif
        lua_close(luaState);
    }
    else {
#ifdef _DEBUG
        std::cout << "Mapa de controles cargado correctamente" << '\n';
#endif
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
                    // A�ade el evento al mapa de input

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