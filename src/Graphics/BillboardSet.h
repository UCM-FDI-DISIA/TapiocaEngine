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

class TAPIOCA_API BillboardSet : public RenderObject {
private:
    friend GraphicsEngine;

protected:
    //BillboardSet de Ogre
    Ogre::BillboardSet* mBillboardSet;
    //UnorderedSet de Billboards de Tapioca
    std::vector<Tapioca::Billboard*> billboards;
    //Nombre identificador
    std::string mName;

    //Crea un BillboardSet con nombre a partir de un número de elementos
    BillboardSet(Ogre::SceneManager* const scnMgr, RenderNode* const node, std::string const& name,
                 const unsigned int poolSize);

public:
    //Destructora de la clase
    virtual ~BillboardSet();

    //Devuelve el nombre del Billboard
    const std::string getName() const;
    //Ajusta el tamaño del BillboardSet al indicado
    void setPoolSize(const size_t size);
    //Devuelve el tamaño del BillboardSet
    int getPoolSize() const;
    //Devuelve el n�mero de Billboards del BillboardSet
    int getNumBillboards() const;
    //Vacía el BillboardSet
    void clear();
    //Elimina el Billboard según su índice
    void removeBillboard(const int index);
    //Elimina el Billboard proporcionado
    void removeBillboard(Billboard* const bb);
    //Añade un Billboard al BillboardSet con la posici�n y color proporcionados (blanco por defecto)
    Tapioca::Billboard* addBillboard(const Vector3& position, const Vector4& colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    //Devuelve un puntero al Billboard que corresponde con el índice indicado
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