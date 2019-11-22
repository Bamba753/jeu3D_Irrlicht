#include <irrlicht.h>
#include "driverChoice.h"
#include "GameEvent.h"   //Gestion Interface
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif


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

void udPutGravityToObjects(scene::IAnimatedMesh *mesh, scene::IAnimatedMeshSceneNode *node, scene::IMeshSceneNode *meshObject, scene::ISceneManager *smgr)
{
   scene::ITriangleSelector *selector = 0;

   if(node)
   {
      selector = smgr->createOctTreeTriangleSelector(mesh, node, 1024);
      node->setTriangleSelector(selector);
   }

   if(selector)
   {

      scene::ISceneNodeAnimator *anim = smgr->createCollisionResponseAnimator(selector, meshObject, core::vector3df(50, 100, 50),core::vector3df(0,0,0),core::vector3df(50,100,50), 0.005);

      meshObject->addAnimator(anim);
      anim->drop();
   }
}

int main()
{
    // driver OpenGl
    video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
    if (driverType==video::EDT_COUNT)
        return 1;

    // create device and exit if creation failed

    IrrlichtDevice * device = createDevice(driverType, core::dimension2d<u32>(640, 480),16,false);

    if (device == 0)
        return 1; // could not create selected driver.

    device->setWindowCaption(L"Irrlicht Engine - User Interface Demo");
    device->setResizable(true);

    device->getFileSystem()->addFileArchive("data/egy2.pk3"); //crewctf.pk3");

    video::IVideoDriver* driver = device->getVideoDriver();
    IGUIEnvironment* env = device->getGUIEnvironment();
    scene::ISceneManager* smgr = device->getSceneManager();

    IGUISkin* skin = env->getSkin();
    IGUIFont* font = env->getFont("data/fonthaettenschweiler.bmp");
    if (font)
        skin->setFont(font);

    skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);

    env->addButton(rect<s32>(10,240,110,240 + 32), 0, GUI_ID_QUIT_BUTTON,
            L"Quit", L"Exits Program");
    env->addButton(rect<s32>(10,280,110,280 + 32), 0, GUI_ID_NEW_WINDOW_BUTTON,
            L"Launch Game", L"Launches a new Window");

    /////////////////////////////////////////////////////////////////////////
    /// Interface
    ////////////////////////////////////////////////////////////////////////
    // Store the appropriate data in a context structure.
    SAppContext context;
    SAppContext game;

    context.device = device;
    context.launch = false;

    // Then create the event receiver, giving it that context structure.
    GameEvent receiver(context);;

    // And tell the device to use our custom event receiver.
    device->setEventReceiver(&receiver);



    ///////////////////////////////////////////////////////////////////////////
    /// Scene
    //////////////////////////////////////////////////////////////////////////
    scene::IAnimatedMesh* mesh = smgr->getMesh("egyziana2.bsp");
    scene::IMeshSceneNode* node = 0;
    scene::ITriangleSelector* selector = 0;

    if (mesh)
        //node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
        node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, IDFlag_IsPickable);

    if (node)
    {
        node->setPosition(core::vector3df(100,-15.0,250));

        selector = smgr->createOctreeTriangleSelector(
                node->getMesh(), node, 128);
        node->setTriangleSelector(selector);
    }
    ///////////////////////////////////////////////////////////////////////////
    /// camera
    //////////////////////////////////////////////////////////////////////////
    // Set a jump speed of 3 units per second, which gives a fairly realistic jump
    // when used with the gravity of (0, -10, 0) in the collision response animator.

    SKeyMap keyMap[9];
            keyMap[0].Action = EKA_MOVE_FORWARD;
            keyMap[0].KeyCode = KEY_UP;

            keyMap[2].Action = EKA_MOVE_BACKWARD;
            keyMap[2].KeyCode = KEY_DOWN;


            keyMap[4].Action = EKA_STRAFE_LEFT;
            keyMap[4].KeyCode = KEY_LEFT;


            keyMap[6].Action = EKA_STRAFE_RIGHT;
            keyMap[6].KeyCode = KEY_RIGHT;


            keyMap[8].Action = EKA_JUMP_UP;
            keyMap[8].KeyCode = KEY_KEY_S;

    scene::ICameraSceneNode* camera =
                    smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, keyMap, 9, true, 10.f);
    camera->setPosition(core::vector3df(50,100,-60));
    camera->setTarget(core::vector3df(-70,30,-60));

    ///////////////////////////////////////////////////////////////////////////
    /// Collision
    //////////////////////////////////////////////////////////////////////////

    if (selector)
    {

        core::vector3df radius = core::vector3df(30,40,30);
        scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
                    selector, camera,radius ,
                    core::vector3df(0,-10,0), core::vector3df(0,60,0));
        selector->drop(); // As soon as we're done with the selector, drop it.
        camera->addAnimator(anim);
        anim->drop();  // And likewise, drop the animator when we're done referring to it.
    }

    // Add the billboard.
    scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
    bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
    bill->setMaterialTexture(0, driver->getTexture("media/particle.bmp"));
    bill->setMaterialFlag(video::EMF_LIGHTING, false);
    bill->setMaterialFlag(video::EMF_ZBUFFER, false);
    bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
    bill->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it

    ///////////////////////////////////////////////////////////////////////////
    /// Enemy
    //////////////////////////////////////////////////////////////////////////
    scene::IAnimatedMeshSceneNode* nodeEnemy = 0;

    video::SMaterial material;

    // Add an MD2 node, which uses vertex-based animation.
    nodeEnemy = smgr->addAnimatedMeshSceneNode(smgr->getMesh("data/Archvile/Archvile.md2"),
                        0, IDFlag_IsPickable | IDFlag_IsHighlightable);
    nodeEnemy->setPosition(core::vector3df(0.231,0.0,207.0)); // Put its feet on the floor.
    nodeEnemy->setScale(core::vector3df(1.6f)); // Make it appear realistically scaled
    nodeEnemy->setMD2Animation(scene::EMAT_STAND);
    nodeEnemy->setAnimationSpeed(20.f);
    material.setTexture(0, driver->getTexture("data/Archvile/archvile.png"));
    material.Lighting = true;
    material.NormalizeNormals = true;
    nodeEnemy->getMaterial(0) = material;
    nodeEnemy->setDebugDataVisible(irr::scene::EDS_BBOX);

    if (nodeEnemy)
    {
        scene::ITriangleSelector *selectorenemy = 0;
        {
           selectorenemy = smgr->createOctreeTriangleSelector(
                   node->getMesh(), node, 128);
           node->setTriangleSelector(selectorenemy);
        }

        {

            const core::aabbox3d<f32>& box = nodeEnemy->getBoundingBox();
            core::vector3df radius = box.MaxEdge - box.getCenter();

            scene::ISceneNodeAnimator* animenemy = smgr->createCollisionResponseAnimator(
                        selectorenemy, nodeEnemy,radius,
                        core::vector3df(0,-10,0), core::vector3df(0,10,0));
            nodeEnemy->addAnimator(animenemy);
           // animenemy->drop();
        }
    }



    selector = smgr->createTriangleSelector(nodeEnemy);
    nodeEnemy->setTriangleSelector(selector);
    //udPutGravityToObjects(mesh,nodeEnemy, node,smgr);
    selector->drop();

    material.setTexture(0, 0);
    material.Lighting = false;

    // Add a light, so that the unselected nodes aren't completely dark.
    scene::ILightSceneNode * light = smgr->addLightSceneNode(0, core::vector3df(-60,100,400),
        video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);
    light->setID(ID_IsNotPickable); // Make it an invalid target for selection.

    // Remember which scene node is highlighted
    scene::ISceneNode* highlightedSceneNode = 0;
    scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();

    // draw the selection triangle only as wireframe
    material.Wireframe=true;
    device->getCursorControl()->setVisible(true);
    int lastFPS = -1;

    while(device->run())
     {
        game = receiver.Context;
        std::cout<<camera->getPosition().X<<"  "<<camera->getPosition().Y<<"  "<<camera->getPosition().Z<<std::endl;
        if (device->isWindowActive())
        {
            driver->beginScene(true, true, video::SColor(255,200,200,200));
            ////////////////////////////////////////////////////////////////
            /// Game
            ////////////////////////////////////////////////////////////////
            if(game.launch){
                device->getCursorControl()->setVisible(false);
                smgr->drawAll();

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
                ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 10000.0f;
                // std::cout<<camera->getPosition().X<<","<<camera->getPosition().Y<<","<<camera->getPosition().Z<<std::endl;

                // Tracks the current intersection point with the level or a mesh
                core::vector3df intersection;
                // Used to show with triangle has been hit
                core::triangle3df hitTriangle;

                // This call is all you need to perform ray/triangle collision on every scene node
                // that has a triangle selector, including the Quake level mesh.  It finds the nearest
                // collision point/triangle, and returns the scene node containing that point.
                // Irrlicht provides other types of selection, including ray/triangle selector,
                // ray/box and ellipse/triangle selector, plus associated helpers.
                // See the methods of ISceneCollisionManager
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
                core::vector3df pos = camera->getPosition();
                core::vector3df posEnemy = nodeEnemy->getPosition();
                core::vector3df diff = pos- posEnemy;

                scene::ISceneNodeAnimator* attack =
                    smgr->createFlyStraightAnimator(posEnemy,
                    pos, 3500, false);
                if (attack)
                {
                    nodeEnemy->addAnimator(attack);
                    attack->drop();
                }

                nodeEnemy->setRotation(diff);

                int fps = driver->getFPS();

                if (lastFPS != fps)
                {
                    core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
                    str += driver->getName();
                    str += "] FPS:";
                    str += fps;

                    device->setWindowCaption(str.c_str());
                    lastFPS = fps;
                }
            }
            else
            {
                env->drawAll();
            }

            driver->endScene();

        }
        else
           device->yield();
        }

    device->drop();

    return 0;
}

