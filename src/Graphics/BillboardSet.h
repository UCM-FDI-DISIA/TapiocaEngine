#pragma once
#include "RenderObject.h"
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

namespace Ogre {
class SceneManager;
class BillboardSet;
}

namespace Tapioca {
class Node;
class GraphicsEngine;
class Billboard;

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

    //Crea un BillboardSet con nombre a partir de un n�mero de elementos
    BillboardSet(Ogre::SceneManager* scnMgr, Node* node, std::string const& name, unsigned int poolSize);

public:
    //Destructora de la clase
    virtual ~BillboardSet();

    //Devuelve el nombre del Billboard
    const std::string getName() const;
    //Ajusta el tama�o del BillboardSet al indicado
    void setPoolSize(size_t size);
    //Devuelve el tama�o del BillboardSet
    int getPoolSize() const;
    //Devuelve el n�mero de Billboards del BillboardSet
    int getNumBillboards() const;
    //Cambia la visibilidad del BillboardSet
    //void setVisible(bool v);
    ////Comprueba si el Billboard es visible o no
    //bool getVisible() const;
    //Vac�a el BillboardSet
    void clear();
    //Elimina el Billboard seg�n su �ndice
    void removeBillboard(int index);
    //Elimina el Billboard proporcionado
    void removeBillboard(Billboard* bb);
    //A�ade un Billboard al BillboardSet con la posici�n y color proporcionados (blanco por defecto)
    Tapioca::Billboard* addBillboard(const Vector3& position,
                                        const Vector4& colour = Vector4(255.0f, 255.0f, 255.0f, 255.0f));
    //Devuelve un puntero al Billboard que corresponde con el �ndice indicado
    Tapioca::Billboard* getBillboard(int index) const;
};
}