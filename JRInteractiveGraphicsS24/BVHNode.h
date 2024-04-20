#pragma once
#include <memory>
#include <vector>

class GraphicsObject;
class BoundingSphere;

struct PotentialCollision {
	std::shared_ptr<GraphicsObject> objects[2];
};

class BVHNode : public std::enable_shared_from_this<BVHNode>
{
protected:
	std::shared_ptr<BVHNode> child[2];
	// Holds a single bounding volume encompassing all the
    // descendents of this node. 
	std::shared_ptr<BoundingSphere> volume;
	// The object at this node of the hierarchy.
    // Only leaf nodes can have an object defined (see IsLeaf).
	std::shared_ptr<GraphicsObject> object;
	// Holds the node immediately above us in the tree.
	std::shared_ptr<BVHNode> parent;

public:
	// Creates a new node in the hierarchy.
	BVHNode(std::shared_ptr<BVHNode> parent = nullptr, 
		std::shared_ptr<BoundingSphere> volume = nullptr, 
		std::shared_ptr<GraphicsObject> object = nullptr);
	virtual ~BVHNode();
	// Removes this node, removing it first from the hierarchy, along with its
    // associated object and child nodes. This method deletes the node 
    // and all its children. This also has the effect of deleting the sibling 
	// of this node, and changing the parent node so that it contains the data 
	// currently in that sibling. Finally it forces the hierarchy above the 
	// current node to reconsider its bounding volume.
	void Remove();
	// Checks if this node is at the bottom of the hierarchy.
	bool IsLeaf() const {
		return (object != nullptr);
	}
	// Checks the potential collisions from this node downwards in the hierarchy,
	// writing them to the given vector. Returns the number of potential contacts
	// it found.
	unsigned GetPotentialCollisions(
		std::vector<std::shared_ptr<PotentialCollision>>& collisions) const;
	// Inserts the given object, with the given bounding sphere radius,
	// into the hierarchy. This may involve the creation of
	// further bounding volume nodes.
	void Insert(std::shared_ptr<GraphicsObject> newObject, 
		BoundingSphere sphere);
protected:
	// For non-leaf nodes, this method recalculates the bounding volume
    // based on the bounding volumes of its children.
	void RecalculateBoundingVolume(bool recurse = true);
	// Checks the potential collisions between this node and the given other node,
	// writing them to the given vector. Returns the number of potential contacts it found.
	unsigned GetPotentialCollisionsWith(
		std::shared_ptr<BVHNode> other, 
		std::vector<std::shared_ptr<PotentialCollision>>& collisions);
	// Checks for overlapping between nodes in the hierarchy. Note
	// that any bounding volume should have an overlaps method implemented
	// that checks for overlapping with another object of its own type.
	bool IsOverlapping(std::shared_ptr<BVHNode> other);
};

