#pragma once
#include "RenderObject.h"
#include <string>
#include <unordered_set>

namespace Ogre {
class SceneManager;
class BillboardSet;
class Billboard;
}

namespace Tapioca {
class Node;
class GraphicsEngine;
class Vector3;
class Vector4;
class Billboard;

class BillboardSet : public RenderObject {
private:
    friend GraphicsEngine;

protected:
    //BillboardSet de Ogre
    Ogre::BillboardSet* mBillboardSet;
    //UnorderedSet de Billboards de Tapioca
    std::unordered_set<Billboard*> mBillboardUnorderedSet;
    //Nombre identificador
    std::string mName;

    //Crea un BillboardSet con nombre a partir de un número de elementos
    BillboardSet(Ogre::SceneManager* scnMgr, Node* node, std::string name, unsigned int poolSize);

public:
    //Destructora de la clase
    virtual ~BillboardSet() {
        if (mBillboardSet != nullptr) delete mBillboardSet;
    }

    //Cambia la visibilidad del BillboardSet
    void setVisible(bool v);
    //Comprueba si el Billboard es visible o no
    bool getVisible() const;
    //Vacía el BillboardSet
    void clear();
    //Devuelve el nombre del Billboard
    const std::string getName() const;
    //Ajusta el tamaño del BillboardSet al indicado
    void setPoolSize(size_t size);
    //Devuelve el tamaño del BillboardSet
    int getPoolSize() const;
    //Devuelve el número de Billboards del BillboardSet
    int getNumBillboards() const;
    //Elimina el Billboard según su índice
    void removeBillboard(unsigned int index);
    //Añade un Billboard al BillboardSet con la posición y color proporcionados (blanco por defecto)
    Tapioca::Billboard* createBillboard(const Vector3& position, const Vector4& colour);
};
}