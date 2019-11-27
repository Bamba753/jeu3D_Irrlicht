#include "Enemy.h"

Enemy::Enemy(vector3df newPosition,float newSpeed,ISceneManager* smgr,Camera* playerTarget ,video::SMaterial material,IVideoDriver* driver)
{
    // Add an MD2 node, which uses vertex-based animation.
    node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("data/Archvile/Archvile.md2"),
                        0, IDFlag_IsPickable | IDFlag_IsHighlightable);
    node->setPosition(newPosition); // Put its feet on the floor.
    node->setScale(core::vector3df(1.6f)); // Make it appear realistically scaled
    node->setMD2Animation(scene::EMAT_STAND);
    node->setAnimationSpeed(20.f);
    material.setTexture(0, driver->getTexture("data/Archvile/archvile.png"));
    material.Lighting = true;
    material.NormalizeNormals = true;
    node->getMaterial(0) = material;

    node->setDebugDataVisible(irr::scene::EDS_BBOX);
   // node->setDebugDataVisible(irr::scene::EDS_HALF_TRANSPARENCY); // Idee deuxieme vage
    target = playerTarget;
    speed  = newSpeed;

    health = 5;
}


IAnimatedMeshSceneNode* Enemy::getNode()
{
    return node;
}

vector3df Enemy::getPosition()
{
    node->updateAbsolutePosition();
    return node->getAbsolutePosition();
}


vector3df Enemy::getTargetAngle(vector3df v, vector3df r)
{
    //v - Position de l'enemi
    //r - Position du joueur

    vector3df angle;
    float x, y, z;
    x = r.X - v.X;
    y = r.Y - v.Y;
    z = r.Z - v.Z;

    angle.Y = atan2(x, z);
    angle.Y *= (180 / PI);
    angle.Y -= 90; // bonne direction

    // Angle entre 0-360 degrees
    if (angle.Y < 0) angle.Y += 360;
    if (angle.Y >= 360) angle.Y -= 360;

    return angle;
}

vector3df Enemy::getDirection(vector3df v, vector3df r)
{
    //v - Position de l'enemi
    //r - Position du joueur

    vector3df direction = r - v;
    direction.Y = 0;
    direction.normalize();
    return direction;

}

float Enemy::getDistanceEnemeyPlayer(vector3df v, vector3df r)
{
    //v - Position de l'enemi
    //r - Position du joueur

    vector3df direction = r - v;

    float distance = sqrt(pow(direction.X ,2)+ pow(direction.Z,2));
    return distance;

}




void Enemy::attack()
{
     if(getDistanceEnemeyPlayer(node->getAbsolutePosition(), target->getPosition()) < 700)
     {
            vector3df pos = node->getPosition() + speed*getDirection(node->getAbsolutePosition(), target->getPosition());
            node->setPosition(pos);
            node->setRotation(getTargetAngle(node->getAbsolutePosition(), target->getPosition()));
     }
}

void Enemy::collision(ISceneManager* smgr, ITriangleSelector* selector) {

    const core::aabbox3d<f32>& box = node->getBoundingBox();
    core::vector3df radius = box.MaxEdge - box.getCenter() + core::vector3df(0,-15.0,0);

    scene::ISceneNodeAnimator* anim =  smgr->createCollisionResponseAnimator(
                selector, node,radius,
                core::vector3df(0,-10,0));
    node->addAnimator(anim);
    anim->drop();

    selector = smgr->createTriangleSelector(node);
    node->setTriangleSelector(selector);
}

void Enemy::healthDecrease()
{
    health-= 1;
}


