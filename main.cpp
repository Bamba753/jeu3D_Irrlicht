#include <irrlicht.h>
#include "driverChoice.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


const static f32 MAX_PITCH=88.0f;

inline f64 toRadian(f64 deglee)
{
  return M_PI * deglee / 180.0;
}
inline f64 toDeglee(f64 radian)
{
  return 180.0 * radian / M_PI;
}

enum
{
    // I use this ISceneNode ID to indicate a scene node that is
    // not pickable by getSceneNodeAndCollisionPointFromRay()
    ID_IsNotPickable = 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be picked by ray selection.
    IDFlag_IsPickable = 1 << 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be highlighted.  In this example, the
    // homonids can be highlighted, but the level mesh can't.
    IDFlag_IsHighlightable = 1 << 1
};


class MyEventReceiver : public IEventReceiver
{
public:
    // This is the one method that we have to implement
    virtual bool OnEvent(const SEvent& event)
    {
        // Remember whether each key is down or up
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

        return false;
    }

    // This is used to check whether a key is being held down
    virtual bool IsKeyDown(EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }

    MyEventReceiver()
    {
        for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

private:
    // We use this array to store the current state of each key
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};


int main()
{
    // driver OpenGl
    video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

    // create device
    MyEventReceiver receiver;

    IrrlichtDevice* device = createDevice(driverType,
            core::dimension2d<u32>(640, 480), 16, false, false, false, &receiver);


    if (!device)
        return 1;

    device->setWindowCaption(L"Jeux video");
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();

    ////////////////////////////////////////////////////////
    /// Affichage a l'ecran
    /////////////////////////////////////////////////////////
    //IGUIEnvironment* guienv = device->getGUIEnvironment();
    //guienv->addStaticText(L"Are you ready for the Game!",
    //        rect<s32>(10,10,260,22), true);

    ////////////////////////////////////////////////////////
    /// Initialisation
    /////////////////////////////////////////////////////////
    device->getFileSystem()->addFileArchive("irrlicht-1.8.4/media/map-20kdm2.pk3");
    scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
    scene::IMeshSceneNode* node = 0;
    scene::ITriangleSelector* selector = 0;

    // The mesh is pickable, but doesn't get highlighted.
    if (mesh)
        node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, IDFlag_IsPickable);


    if (node)
    {
        node->setPosition(core::vector3df(-1350,-130,-1400));

        selector = smgr->createOctreeTriangleSelector(
                node->getMesh(), node, 128);
        node->setTriangleSelector(selector);
        // We're not done with this selector yet, so don't drop it.
    }

    ////////////////////////////////////////////////////////
    /// Creation et Initialisation de la camera
    /////////////////////////////////////////////////////////
    // Set a jump speed of 3 units per second, which gives a fairly realistic jump
    // when used with the gravity of (0, -10, 0) in the collision response animator.
    scene::ICameraSceneNode* camera =
        smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
    camera->setPosition(core::vector3df(50,50,-60));
    camera->setTarget(core::vector3df(-70,30,-60));

    ////////////////////////////////////////////////////////
    /// Pointeur de l'arme
    /////////////////////////////////////////////////////////
    // Add the billboard.
    scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
    bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
    bill->setMaterialTexture(0, driver->getTexture("irrlicht-1.8.4/media/particle.bmp"));
    bill->setMaterialFlag(video::EMF_LIGHTING, false);
    bill->setMaterialFlag(video::EMF_ZBUFFER, false);
    bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
    bill->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it


    //scene::IAnimatedMeshSceneNode* anms =
    //    smgr->addAnimatedMeshSceneNode(smgr->getMesh("irrlicht-1.8.4/media/ninja.b3d"));

    ////////////////////////////////////////////////////////
    /// Gun'zer
    /////////////////////////////////////////////////////////

    const f32 scalingFactor=1.2f;

    scene::IAnimatedMeshSceneNode* anms =
           smgr->addAnimatedMeshSceneNode(smgr->getMesh("Modelpack/Shotgun/shotgun.md2"));
    if (anms)
    {
        anms->setPosition(core::vector3df(5,-15,15));

        anms->setMaterialFlag(video::EMF_LIGHTING, false);

        anms->setFrameLoop(0, 13);
        anms->setAnimationSpeed(15);
        anms->setMD2Animation(scene::EMAT_STAND);

        anms->setScale(core::vector3df(0.5f,0.5f,0.5f));
        anms->setRotation(core::vector3df(0,0,0));
        anms->setMaterialTexture(0, driver->getTexture("Modelpack/Shotgun/shotgun.jpg"));

    }

    {
      // collision avec l'environnement
      const core::aabbox3df& box=anms->getBoundingBox();
      scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
          selector, anms, scalingFactor*(box.MaxEdge-box.getCenter()),
          core::vector3df(0,0,0), -scalingFactor*(box.getCenter()));
      anms->addAnimator(anim);
      anim->drop();
    }


    ////////////////////////////////////////////////////////
    /// Enemy
    /////////////////////////////////////////////////////////
    scene::IAnimatedMeshSceneNode* enemynode = 0;

    video::SMaterial material;

    // Add an MD2 node, which uses vertex-based animation.
    enemynode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("modelpack20/modelpack20/Archvile/Archvile.md2"),
                        0, IDFlag_IsPickable | IDFlag_IsHighlightable);
    enemynode->setPosition(core::vector3df(-90,-10,-100)); // Put its feet on the floor.
    enemynode->setScale(core::vector3df(1.6f)); // Make it appear realistically scaled
    enemynode->setMD2Animation(scene::EMAT_STAND);
    enemynode->setAnimationSpeed(20.f);
    material.setTexture(0, driver->getTexture("modelpack20/modelpack20/Archvile/archvile.png"));
    material.Lighting = true;
    material.NormalizeNormals = true;
    enemynode->getMaterial(0) = material;

    // Now create a triangle selector for it.  The selector will know that it
    // is associated with an animated node, and will update itself as necessary.
    selector = smgr->createTriangleSelector(enemynode);
    enemynode->setTriangleSelector(selector);
    selector->drop(); // We're done with this selector, so drop it now.


    ////////////////////////////////////////////////////////
    /// Configuration
    /////////////////////////////////////////////////////////

    material.setTexture(0, 0);
    material.Lighting = false;

    // Add a light, so that the unselected nodes aren't completely dark.
    scene::ILightSceneNode * light = smgr->addLightSceneNode(0, core::vector3df(-60,100,400),
                                                             video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);
    light->setID(ID_IsNotPickable); // Make it an invalid target for selection.

    // Remember which scene node is highlighted
    scene::ISceneNode* highlightedSceneNode = 0;
    scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();
    int lastFPS = -1;

    // draw the selection triangle only as wireframe
    material.Wireframe = true;
    device->getCursorControl()->setVisible(false);

    // In order to do framerate independent movement, we have to know
    // how long it was since the last frame
    u32 then = device->getTimer()->getTime();

    // This is the movemen speed in units per second.
    const f32 MOVEMENT_SPEED = 50.f;

    while(device->run())
    {
        // Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
        then = now;

        ///////////////////////////////////////////////////////////////////////////
        /// Configuration collusion
        /////////////////////////////////////////////////////////////////////////////

        // Unlight any currently highlighted scene node
        if (highlightedSceneNode)
        {
            highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, true);
            highlightedSceneNode = 0;
        }

        // All intersections in this example are done with a ray cast out from the camera to
        // a distance of 1000.  You can easily modify this to check (e.g.) a bullet
        // trajectory or a sword's position, or create a ray from a mouse click position using
        // ISceneCollisionManager::getRayFromScreenCoordinates()
        core::line3d<f32> ray;
        ray.start = camera->getPosition();
        ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;

        // Tracks the current intersection point with the level or a mesh
        core::vector3df intersection;
        // Used to show with triangle has been hit
        core::triangle3df hitTriangle;

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
            //driver->draw3DTriangle(hitTriangle, video::SColor(0,255,0,0));

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



        ///////////////////////////////////////////////////////////////////////////////
        /// Keyboard Move
        ///////////////////////////////////////////////////////////////////////////////

            core::vector3df nodePosition = anms->getPosition();
        if(receiver.IsKeyDown(irr::KEY_KEY_F))
            nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime;
        else if(receiver.IsKeyDown(irr::KEY_KEY_G))
            nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;

        if(receiver.IsKeyDown(irr::KEY_KEY_V))
            nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
        else if(receiver.IsKeyDown(irr::KEY_KEY_T)){
            nodePosition.X += MOVEMENT_SPEED  * frameDeltaTime;
        }

        anms->setPosition(nodePosition);
        camera->setPosition(nodePosition);

        ///////////////////////////////////////////////////////////////////////////////
        /// ENEMY MOVING
        ///////////////////////////////////////////////////////////////////////////////

        core::vector3df enemyPosition = enemynode->getPosition();
        enemyPosition.X += 0.001 * ( nodePosition.X - enemyPosition.X) ;
        enemyPosition.Y += 0.001 * ( nodePosition.Y - enemyPosition.Y) ;
        enemyPosition.Z += 0.001 * ( nodePosition.Z - enemyPosition.Z);
        enemynode->setPosition(enemyPosition);

        if(receiver.IsKeyDown(irr::KEY_KEY_O))
        {   //smgr->addToDeletionQueue(enemynode);
            enemynode->remove();

            enemynode->drop();
            enemynode = 0;
        }
        ////////////////////////////////////////////////////////////////
        /// Draw scene
        ////////////////////////////////////////////////////////////////
        driver->beginScene(true, true, video::SColor(255,113,113,133));
        smgr->drawAll(); // draw the 3d scene
        device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)
        driver->endScene();
        //std::cout<<ok<<std::endl;
        int fps = driver->getFPS();

        if (lastFPS != fps)
        {
            core::stringw tmp(L"Movement Example - Irrlicht Engine [");
            tmp += driver->getName();
            tmp += L"] fps: ";
            tmp += fps;

            device->setWindowCaption(tmp.c_str());
            lastFPS = fps;
        }
    }
    device->drop();

    return 0;
}
