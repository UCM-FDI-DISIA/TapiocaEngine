#pragma once

namespace std {
	template<class _Ty>
	class allocator;
	template<class _Ty, class _Alloc = allocator<_Ty>>
	class vector;
}

namespace Tapioca {
class Vector3;
class Transform;

class INode {
private:
	friend Transform;
	Transform* transform;
public:
	virtual void setPosition(Vector3 position) = 0;
	virtual void translate(Vector3 movement) = 0;
	virtual void setRotation(Vector3 rotation) = 0;
	virtual void setScale(Vector3 scale) = 0;
	virtual void scale(Vector3 scale) = 0;

	virtual std::vector<INode*> getAllChildren() = 0;
	virtual void setParent(INode* parent) = 0;

	Transform* getTransform() { return transform; }

	virtual ~INode() { }
};
}
