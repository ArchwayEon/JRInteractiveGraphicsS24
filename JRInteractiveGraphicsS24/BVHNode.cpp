#include "BVHNode.h"
#include "BoundingSphere.h"
#include "GraphicsObject.h"

BVHNode::BVHNode(std::shared_ptr<BVHNode> parent, 
	std::shared_ptr<BoundingSphere> volume, 
	std::shared_ptr<GraphicsObject> object) :
	parent(parent), volume(volume), object(object)
{
	child[0] = child[1] = nullptr;
}

BVHNode::~BVHNode()
{
	Remove();
}

void BVHNode::Remove()
{
    // If there is a parent then process the sibling.
    if (parent != nullptr) {
        // Find our sibling
        std::shared_ptr<BVHNode> sibling;
        if (parent->child[0].get() == this) sibling = parent->child[1];
        else sibling = parent->child[0];

        // Write its data to our parent
        parent->volume = sibling->volume;
        parent->object = sibling->object;
        parent->child[0] = sibling->child[0];
        parent->child[1] = sibling->child[1];

        // Remove the sibling (we blank its parent and children to avoid 
        // processing/deleting them)
        sibling->parent = nullptr;
        sibling->object = nullptr;
        sibling->child[0] = nullptr;
        sibling->child[1] = nullptr;
        sibling->Remove();

        // Recalculate the parent's bounding volume
        parent->RecalculateBoundingVolume();
    }
    if (child[0] != nullptr) {
        child[0]->parent = nullptr;
        child[0]->Remove();
    }
    if (child[1] != nullptr) {
        child[1]->parent = nullptr;
        child[1]->Remove();
    }
}



void BVHNode::Insert(std::shared_ptr<GraphicsObject> newObject, 
    BoundingSphere sphere)
{
    sphere.SetPosition(newObject->GetPosition());
    auto newVolume = std::make_shared<BoundingSphere>(sphere);
    // If this hierarchy is new then add the object and return.
    if (volume == nullptr && object == nullptr) {
        volume = newVolume;
        object = newObject;
        return;
    }
    // If this is a leaf, then the only option is to spawn two
    // new children and place the new body in one.
    if (IsLeaf()) {
        // Child one is a copy of this.
        child[0] = std::make_shared<BVHNode>(
            shared_from_this(), volume, object);

        // Child two holds the new body
        child[1] = std::make_shared<BVHNode>(
            shared_from_this(), newVolume, newObject);

        // And now loose the body (it's no longer a leaf)
        this->object = nullptr;

        // Recalculate the bounding volume
        RecalculateBoundingVolume(true);
    }
    // Otherwise work out which child gets to keep the inserted body. We give 
    // it to whoever would grow the least to incorporate it.
    else {
        auto volumeGrowth1 = child[0]->volume->GetGrowth(*newVolume);
        auto volumeGrowth2 = child[1]->volume->GetGrowth(*newVolume);
        if (volumeGrowth1 < volumeGrowth2) {
            child[0]->Insert(newObject, sphere);
        }
        else {
            child[1]->Insert(newObject, sphere);
        }
    }
}

void BVHNode::RecalculateBoundingVolume(bool recurse)
{
    if (IsLeaf()) return;

    // Use the bounding volume combining constructor.
    volume = std::make_shared<BoundingSphere>(
        *child[0]->volume, *child[1]->volume);

    // Recurse up the tree
    if (recurse == true) {
        if (parent != nullptr) parent->RecalculateBoundingVolume(true);
    }
}

unsigned BVHNode::GetPotentialCollisions(
    std::vector<std::shared_ptr<PotentialCollision>>& collisions) const
{
    // Early out if we're a leaf node.
    if (IsLeaf()) return 0;

    // Get the potential contacts of one of our children with the other
    unsigned count = child[0]->GetPotentialCollisionsWith(child[1], collisions);
    count += child[0]->GetPotentialCollisions(collisions);
    count += child[1]->GetPotentialCollisions(collisions);
    return count;
}

unsigned BVHNode::GetPotentialCollisionsWith(
    std::shared_ptr<BVHNode> other, 
    std::vector<std::shared_ptr<PotentialCollision>>& collisions)
{
    // Early out if we don't overlap
    if (IsOverlapping(other) == false) return 0;

    // If we're both at leaf nodes, then we have a potential contact
    if (IsLeaf() && other->IsLeaf()) {
        object->SetInPotentialCollision(true);
        other->object->SetInPotentialCollision(true);
        auto collision = std::make_shared<PotentialCollision>();
        collision->objects[0] = object;
        collision->objects[1] = other->object;
        collisions.push_back(collision);
        return 1;
    }

    // Determine which node to descend into. If either is a leaf, then we 
    // descend the other. If both are branches, then we use the one with the 
    // largest size.
    unsigned count;
    if (IsLeaf() == false && other->IsLeaf() == false) {
        auto size1 = volume->GetVolume();
        auto size2 = other->volume->GetVolume();
        if (size1 > size2) {
            // Recurse into ourself
            count = child[0]->GetPotentialCollisionsWith(other, collisions);
            return count + child[1]->GetPotentialCollisionsWith(other, collisions);
        }
        else {
            // Recurse into the other node
            count = GetPotentialCollisionsWith(other->child[0], collisions);
            return count + GetPotentialCollisionsWith(other->child[1], collisions);
        }
    }
    else if (other->IsLeaf()) {
        // Recurse into ourself
        count = child[0]->GetPotentialCollisionsWith(other, collisions);
        return count + child[1]->GetPotentialCollisionsWith(other, collisions);
    }
    else {
        // Recurse into the other node
        count = GetPotentialCollisionsWith(other->child[0], collisions);
        return count + GetPotentialCollisionsWith(other->child[1], collisions);
    }
}

bool BVHNode::IsOverlapping(std::shared_ptr<BVHNode> other)
{
    return volume->OverlapsWith(*other->volume);
}