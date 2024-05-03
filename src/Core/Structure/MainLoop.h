#pragma once
#include "Utilities/Singleton.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <unordered_set>
#include "defs.h"

namespace Tapioca {
class Scene;
class Module;

/*
* @brief Clase que contiene el bucle principal del motor
*/
class TAPIOCA_API MainLoop : public Singleton<MainLoop> {
private:
    friend Singleton<MainLoop>;

    std::vector<Event> delayedEvents;

    // warning C4251 'Tapioca::MainLoop::modules' :
    // class 'std::vector<Tapioca::Module *,std::allocator<Tapioca::Module *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::MainLoop'
    // warning C4251 'Tapioca::MainLoop::loadedScenes' :
    // class 'std::stack<Tapioca::Scene *,std::deque<Tapioca::Scene *,std::allocator<Tapioca::Scene *>>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::MainLoop'
    // warning C4251 'Tapioca::MainLoop::sceneBuffer' :
    // class 'std::stack<Tapioca::Scene *,std::deque<Tapioca::Scene *,std::allocator<Tapioca::Scene *>>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::MainLoop'
    // warning C4251 'Tapioca::MainLoop::toDelete' :
    // class 'std::vector<Tapioca::Scene *,std::allocator<Tapioca::Scene *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::MainLoop'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::vector<Module*> modules;                           // Modulos del motor
    std::unordered_map<std::string, Scene*> loadedScenes;   // Escenas cargadas
    std::unordered_set<Scene*> sceneBuffer;                 // Escenas que se van a cargar la siguiente iteracion
    std::unordered_set<Scene*> toDelete;                    // Escenas que se van a eliminar
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    uint64_t deltaTime;                                  // Tiempo transcurrido desde el ultimo tick
    static const uint64_t MAX_NUM_FIXED_UDPATES = 150;   // Numero maximo de fixedUpdates
    bool finish;                                         // True si va a terminar la ejecucion, false en caso contrario

    /*
    * @brief Inicializa los modulos
    */
    void start();
    /*
    * @brief Llama al update de los modulos y escenas cargadas
    */
    void update();
    /*
    * @brief Llama al fixedUpdate de los modulos y escenas cargadas si estan activas
    */
    void fixedUpdate();
    /*
    * @brief LLama al render de los modulos
    */
    void render();
    /*
    * @brief Borra los objetos y componentes muertos y borra las escenas q se han marcado como pendientes de eliminar.
    * Comprueba si no hay escenas cargadas y no hay escenas a cargar, si es asi, termina la ejecucion
    */
    void refresh();
    /*
    * @brief Envia los mensajes retrasados
    */
    void handleDelayedEvents();

    /*
    * @brief Constructor de la clase MainLoop
    */
    MainLoop();

public:
    /*
    * @brief Libera las escenas cargadas, los modulos y la biblioteca cargada desde DynamicLibraryLoader
    */
    ~MainLoop();

    static const uint64_t FIXED_DELTA_TIME = 1000 / 60;   // mas de 60 fps (62.5)

    /*
    * @brief Detiene el bucle principal
    */
    inline void exit() { finish = true; }
    /*
    * @brief Inicializa los modulos
    * @return true si se ha inicializado correctamente, false en caso contrario
    */
    bool init();
    /*
    * @brief Inicializa la configuracion del juego
    * @return true si se ha inicializado correctamente, false en caso contrario
    */
    bool initConfig();
    /*
    * @brief Metodo que contiene el bucle principal del juego
    */
    void run();

    /*
    * @brief Carga una escena que se ejecutara a la vez que el resto de escenas cargadas
    * @param sc Puntero a la escena que se quiere cargar
    */
    void loadScene(Scene* const sc);
    /*
    * @brief Elimina la escena indicada para que deje de ejecutarse
    * @param sc Puntero a la escena que se quiere borrar
    */
    void deleteScene(Scene* const sc);
    /*
    * @brief Elimina la escena indicada para que deje de ejecutarse
    * @param sc Nombre de la escena que se quiere borrar
    */
    void deleteScene(std::string const& sc);

    /*
    * @brief Devuelve todas las escenas cargadas
    * @return unordered_map con las escenas cargadas
    */
    std::unordered_map<std::string, Scene*> getLoadedScenes() const;
    /*
    * @brief Devuelve la escena
    * @param sc Nombre de la escena que se quiere obtener
    * @return Puntero a la escena con el nombre indicado, si la escena no existe, devuelve nullptr
    */
    Scene* getScene(std::string sc);

    /*
    * @brief Anade un modulo al vector
    * @param m Modulo que se quiere anadir
    */
    void addModule(Module* const m);
    /*
    * @brief Envia un evento a las escenas
    * @param e Evento que se quiere enviar
    * @param delay True si se quiere enviar el evento en el siguiente tick, false en caso contrario
    */
    void pushEvent(Event const& e, bool const delay = false);
};
}