#pragma once
#include <vector>
#include <string>
#include "RenderObject.h"
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

namespace Ogre {
class SceneManager;
class BillboardSet;
}

namespace Tapioca {
class RenderNode;
class GraphicsManager;
class Billboard;

enum BillboardType {
    BBT_POINT,
    BBT_ORIENTED_COMMON,
    BBT_ORIENTED_SELF,
    BBT_PERPENDICULAR_COMMON,
    BBT_PERPENDICULAR_SELF
};

/*
* @brief Wrapper de la clase BillboardSet de Ogre
*/
class TAPIOCA_API BillboardSet : public RenderObject {
private:
    friend GraphicsManager;

protected:
    Ogre::BillboardSet* mBillboardSet;   // BillboardSet de Ogre

    // warning C4251 'Tapioca::BillboardSet::billboards' :
    // class 'std::vector<Tapioca::Billboard *,std::allocator<Tapioca::Billboard *>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::BillboardSet'
    // warning C4251 'Tapioca::BillboardSet::mName' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::BillboardSet'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::vector<Tapioca::Billboard*> billboards;   // UnorderedSet de Billboards de Tapioca
    std::string mName;                             // Nombre identificador
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    /*
    * @brief Constructor de la clase BillboardSet
    * @param scnMngr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param name Nombre del billboardSet
    * @param poolSize Numero maximo de billboards que puede tener
    */
    BillboardSet(Ogre::SceneManager* const scnMngr, RenderNode* const node, std::string const& name,
                 const unsigned int poolSize);

public:
    /*
    * @brief Elimina los billboards
    */
    virtual ~BillboardSet();

    /*
    * @brief Devuelve el nombre del billboardSet
    * @return Nombre del billboardSet
    */
    const std::string getName() const;
    /*
    * @brief Ajusta el tamano del BillboardSet al indicado
    * @param size Tamano al que se quiere cambiar
    */
    void setPoolSize(const size_t size);
    /*
    * @brief Devuelve el tamano del BillboardSet
    * @return Tamano del BillboardSet
    */
    int getPoolSize() const;
    /*
    * @brief Devuelve el numero de billboards en el BillboardSet
    * @return Numero de billboards en el BillboardSet
    */
    int getNumBillboards() const;

    /*
    * @brief Vacia el BillboardSet
    */
    void clear();
    /*
    * @brief Elimina un billboard segun su indice
    * @param index Indice del billboard que se quiere eliminar
    */
    void removeBillboard(const int index);
    /*
    * @brief Elimina un billboard segun su puntero
    * @param bb Puntero del billboard que se quiere eliminar
    */
    void removeBillboard(Billboard* const bb);
    /*
    * @brief Anade un Billboard al BillboardSet con la posici�n y color proporcionados (blanco por defecto)
    * @param position Posicion del billboard
    * @param colour Color del billboard
    */
    Billboard* addBillboard(const Vector3& position, const Vector4& colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    /*
    * @brief Devuelve el billboard con el indice proporcionado
    * @param index Indice del billboard que se quiere devover
    * @return Puntero al billboard con indice index
    */
    Billboard* getBillboard(const int index) const;
    /*
    * @brief Establece el tipo de Billboard
    * @param billboardType Tipo de Billboard
    */
    void setBillboardType(BillboardType billboardType);
    /*
    * @brief Devuelve el tipo de Billboard
    * @return Tipo de Billboard
    */
    BillboardType getBillboardType() const;
    /*
    * @brief Establece el nombre del material del BillboardSet
    * @param name Nombre del material
    */
    void setMaterialName(const std::string& name);
    /*
    * @brief Devuelve el nombre del material del BillboardSet
    * @return Nombre del material
    */
    const std::string& getMaterialName(void) const;
};
}
