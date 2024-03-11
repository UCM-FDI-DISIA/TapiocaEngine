#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Utilities/defs.h"

namespace Tapioca {
class GameObject;

class TAPIOCA_API Scene {
private:
    friend class GameObject;
    std::vector<GameObject*> objects;                           // Objetos que tiene la escena
    std::unordered_map<std::string, GameObject*> handlers;      // Relaciones entre handlers y objetos

public:
    /*
    * @brief Constructora de la clase Scene
    */
    Scene();
    /*
    * @brief Destructora de la clase Scene
    */
    ~Scene();
    /*
    * @brief Devuelve los objetos de esta escena
    * @return Vector que contiene punteros a los objetos de esta escena
    */
    std::vector<GameObject*> getObjects() const;
    /*
    * @brief Devuelve el objeto al que esta asociado un handler
    * @param handler Handler del objeto que se quiere obtener
    * @return Objeto asociado al handler
    */
    GameObject* getHandler(std::string const& handler) const;

    /*
    * @brief Aniade un objeto a la escena
    * @param object Objeto que se quiere aniadir a la escena
    * @param handler Handler que se quiere asociar al objeto que se quiere aniadir. Los handlers deben ser unicos
    * @return True se se ha aniadido correctamente a la escena, false en caso contrario
    */
    bool addObject(GameObject* const object, std::string const& handler = "");
    /*
    * @brief Inicializa los objetos de la escena
    */
    void start();
    /*
    * @brief Actualiza los objetos activos
    * @param deltaTime Tiempo que ha pasado desde el ultimo update
=   */
    void update(const uint64_t deltaTime);
    //void handleEvents();
    /*
    * @brief Actualiza las componentes activas del objeto.
    * Se llama cada cierto tiempo fijo (Game::FIXED_DELTA_TIME),
    */
    void fixedUpdate();
    /*
    * @brief Elimina los objetos muertos de la escena ademas de todas sus componentes
=   */
    void refresh();
    /*
    * @brief Procesa un evento recibido
    * @param id String que indica el tipo de evento
    * @param info Parametros del mensaje, cada evento gestiona sus propios parametros
    */
    void handleEvent(std::string const& id, void* info);
    /*
    * @brief Metodo que se usa para enviar un evento
    * @param id Indica el tipo de mensaje
    * @param info Puntero a void para pasar parametros
    * @param global Indica si el evento debe ser enviado glabalmente
    */
    void pushEvent(std::string const& id, void* info);
};
}
