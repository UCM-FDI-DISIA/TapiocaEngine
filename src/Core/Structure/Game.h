#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "Utilities/Singleton.h"
#include "defs.h"

namespace Tapioca {
class Scene;
class Module;

class TAPIOCA_API Game : public Singleton<Game> {
private:
    friend Singleton<Game>;
    // warning C4251 'Tapioca::Game::modules' :
    // class 'std::vector<Tapioca::Module *,std::allocator<Tapioca::Module *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Game'
    // warning C4251 'Tapioca::Game::scenes' :
    // class 'std::stack<Tapioca::Scene *,std::deque<Tapioca::Scene *,std::allocator<Tapioca::Scene *>>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Game'
    // warning C4251 'Tapioca::Game::toDelete' :
    // class 'std::vector<Tapioca::Scene *,std::allocator<Tapioca::Scene *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Game'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::vector<Module*> modules;                           // Modulos del motor
    std::unordered_map<std::string, Scene*> loadedScenes;   // Escenas cargadas
    std::vector<Scene*> toDelete;                           // Escenas que se van a eliminar
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    uint64_t deltaTime;                                     // Tiempo transcurrido desde el ultimo tick
    static const uint64_t MAX_NUM_FIXED_UDPATES = 150;      // Numero maximo de fixedUpdates
    bool finish;                                            // True si va a terminar la ejecucion, false en caso contrario
    bool gameInitialized;                                   // True si el juego ha sido inicializado, false en caso contrario
    static const uint64_t TIME_TO_INITIALIZE_GAME = 2000;   // Tiempo que se espera para inicializar el juego

    const char* MAIN_SCENE_NAME = "startScene";             // Nombre de la escena principal

    /*
    * @brief Comienza los modulos
    */
    void start();
    /*
    * @brief Llama al update de los modulos y escenas
    */
    void update();
    /*
    * @brief Llama al fixedUpdate de los modulos y escenas
    */
    void fixedUpdate();
    /*
    * @brief Renderiza los objetos que contienen 
    */
    void render();
    /*
    * @brief Borra los objetos y componentes muertos y borra las escenas q se han marcado como pendientes de eliminar
    */
    void refresh();
    /*
    * @brief Constructora de la clase Game
    */
    Game();

public:
    /*
    * @brief Destructora de la clase Game
    */
    ~Game();

    static const uint64_t FIXED_DELTA_TIME = 1000 / 60;   // mas de 60 fps (62.5)
    /*
    * @brief Detiene el bucle principal
    */
    inline void exit() { finish = true; }
    /*
    * @brief Inicializa los modulos
    * @return True si se ha inicializado correctamente, false en caso contrario
    */
    bool init();
    /*
    * @brief Inicializa la configuracion del juego
    * @return True si se ha inicializado correctamente, false en caso contrario
    */
    bool initConfig();
    /*
    * @brief Metodo que contiene el bucle principal del juego
    */
    void run();

    /*
    * @brief Devuelve el nombre de la escena principal
    * @return Nombre de la escena principal
    */
    std::string getMainScene() const { return MAIN_SCENE_NAME; }

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
    * @brief Devuelve la esce
    * @param sc Nombre de la escena que se quiere obtener
    * @return Puntero a la escena con el nombre indicado
    */
    Scene* getScene(std::string sc);

    /*
    * @brief Aniade un modulo al vector
    * @param m Modulo que se quiere aniadir
    */
    void addModule(Module* const m);
    /*
    * @brief Envia un evento a las escenas
    */
    void pushEvent(std::string const& id, void* info);
};
}