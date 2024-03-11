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
class GraphicsEngine;
class Billboard;

enum BillboardType {
    BBT_POINT,
    BBT_ORIENTED_COMMON,
    BBT_ORIENTED_SELF,
    BBT_PERPENDICULAR_COMMON,
    BBT_PERPENDICULAR_SELF
};

/*
* @brief Wrapper de la clase BillboardSet de Ogre.
*/
class TAPIOCA_API BillboardSet : public RenderObject {
private:
    friend GraphicsEngine;

protected:
    Ogre::BillboardSet* mBillboardSet;                  // BillboardSet de Ogre
    std::vector<Tapioca::Billboard*> billboards;        // UnorderedSet de Billboards de Tapioca
    std::string mName;                                  // Nombre identificador

    /*
    * @brief Constructora de la clase BillboardSet.
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param name Nombre del billboardSet
    * @param poolSize Numero maximo de billboards que puede tener
    */
    BillboardSet(Ogre::SceneManager* const scnMgr, RenderNode* const node, std::string const& name,
                 const unsigned int poolSize);

public:
    /*
    * @brief Desstructora de la clase BillboardSet.
    */
    virtual ~BillboardSet();

    /*
    * @brief Devuelve el nombre del billboardSet
    * @return Nombre del billboardSet
    */
    const std::string getName() const;
    /*
    * @brief Ajusta el tamanio del BillboardSet al indicado
    * @param size Tamanio al que se quiere cambiar
    */
    void setPoolSize(const size_t size);
    /*
    * @brief Devuelve el tamanio del BillboardSet
    * @return Tamanio del BillboardSet
    */
    int getPoolSize() const;
    /*
    * @brief Devuelve el numero de billboards en el BillboardSet
    * @return Numero de billboards en el BillboardSet
    */
    int getNumBillboards() const;
    //Cambia la visibilidad del BillboardSet
    //void setVisible(bool v);
    ////Comprueba si el Billboard es visible o no
    //bool getVisible() const;
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
    * @brief Aniade un Billboard al BillboardSet con la posici�n y color proporcionados (blanco por defecto)
    * @param position Posicion del billboard
    * @param colour Color del billboard
    */
    Tapioca::Billboard* addBillboard(const Vector3& position,
                                        const Vector4& colour = Vector4(255.0f, 255.0f, 255.0f, 255.0f));
    /*
    * @brief Devuelve el billboard con el indice proporcionado
    * @param index Indice del billboard que se quiere devover
    * @return Puntero al billboard con indice index
    */
    Tapioca::Billboard* getBillboard(const int index) const;
    //Cambia el tipo de Billboard
    void setBillboardType(BillboardType billboardType);
    //Devuelve el tipo de Billboard
    BillboardType getBillboardType();
    //Establece el nombre del material del BillboardSet
    void setMaterialName(const std::string& name);
    //Devuelve el nombre del material del BillboardSet
    const std::string& getMaterialName(void) const;
};
}