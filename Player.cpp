#include "stdafx.h"
#include "Player.h"


Player::Player(ISceneManager* mysmgr, IVideoDriver* mydriver, Camera* mycamera,SMaterial mymaterial):smgr(mysmgr),driver(mydriver),camera(mycamera),material(mymaterial)
{
	mesh = smgr->getMesh("data/AK/AK.obj");//data/AK_47/Adding/AK.obj");
    node = smgr->addAnimatedMeshSceneNode(mesh, smgr->getActiveCamera(), 10, core::vector3df(35, -35, 75));
	node->setMaterialTexture(0, driver->getTexture("data/AK/Textures/AK_diffuse.jpg"));//AK_47/T2.bmp"));
	/*node->setMaterialTexture(1, driver->getTexture("data/AK/Textures/AK_normal.jpg"));//AK_47/T2.bmp"));
	node->setMaterialTexture(2, driver->getTexture("data/AK/Textures/AK_specular.jpg"));//AK_47/T2.bmp"));
	*/
	
	node->setMaterialFlag(EMF_LIGHTING, false);
    node->setScale(core::vector3df(0.4f));
	node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	node->setMaterialFlag(video::EMF_ZBUFFER, false);


    //node->setPosition(camera->getNode()->getAbsolutePosition()+dist_cameraplayer);
   // node->setRotation(camera->getNode()->getRotation());
    health = 10;

    // Viseur.
    bill = smgr->addBillboardSceneNode();
    bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
    bill->setMaterialTexture(0, driver->getTexture("media/particle.bmp"));
    bill->setMaterialFlag(video::EMF_LIGHTING, false);
    bill->setMaterialFlag(video::EMF_ZBUFFER, false);
    bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));

    bill->setID(0); // This ensures that we don't accidentally ray-pick it

    // Add a light, so that the unselected nodes aren't completely dark.
    light = smgr->addLightSceneNode(0, core::vector3df(-60,100,400),video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);
    light->setID(ID_IsNotPickable); // Make it an invalid target for selection.

    // Remember which scene node is highlighted
     highlightedSceneNode = 0;
     collMan = smgr->getSceneCollisionManager();

}

int Player::getHealth(){
    return health;
}

void Player::healthDecrease()
{
    health -= 1;
    //std::cout<< health<<std::endl;
}


IBillboardSceneNode * Player::getBill(){
    return bill;
}

void Player::updatePosition(Camera* camera)
{
    //std::cout<<camera->getNode()->getPosition().X<<" "<<camera->getNode()->getPosition().Y<<" "<<camera->getNode()->getPosition().Z<<std::endl;
    node->setPosition(camera->getNode()->getAbsolutePosition()+dist_cameraplayer);
    node->setRotation(camera->getNode()->getRotation());

}

IAnimatedMeshSceneNode* Player::getNode()
{
    return node;
}


void Player::shoot()
{


}

void Player::isDead()
{

}

void Player::targetGun()
{
    if (highlightedSceneNode)
    {
        highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, true);
        highlightedSceneNode = 0;
    }

    // ray
    core::line3d<f32> ray;
    ray.start = camera->getNode()->getPosition();
    ray.end = ray.start + (camera->getNode()->getTarget() - ray.start).normalize() * 10000.0f;

    // Tracks the current intersection point with the level or a mesh
    core::vector3df intersection;
    // Used to show with triangle has been hit
    core::triangle3df hitTriangle;

    // Intersection
    scene::ISceneNode * selectedSceneNode =
            collMan->getSceneNodeAndCollisionPointFromRay(
                ray,
                intersection, // This will be the position of the collision
                hitTriangle, // This will be the triangle hit in the collision
                IDFlag_IsPickable, // This ensures that only nodes that we have
                // set up to be pickable are considered
                0); // Check the entire scene (this is actually the implicit default)

    // If the ray hit anything, move the billboard to the collision position
    // and draw the triangle that was hit.
    if(selectedSceneNode)
    {
        bill->setPosition(intersection);

        // We need to reset the transform before doing our own rendering.
        driver->setTransform(video::ETS_WORLD, core::matrix4());
        driver->setMaterial(material);

        // We can check the flags for the scene node that was hit to see if it should be
        // highlighted. The animated nodes can be highlighted, but not the Quake level mesh
        if((selectedSceneNode->getID() & IDFlag_IsHighlightable) == IDFlag_IsHighlightable)
        {
            highlightedSceneNode = selectedSceneNode;

            // Highlighting in this case means turning lighting OFF for this node,
            // which means that it will be drawn with full brightness.
            highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
        }
    }
}



