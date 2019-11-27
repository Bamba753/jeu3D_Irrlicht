#include <irrlicht.h>
#include "driverChoice.h"
#include "Game.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

int main()
{
    // driver OpenGl
    video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
    if (driverType==video::EDT_COUNT)
        return 1;

    // create device and exit if creation failed
    IrrlichtDevice * device = createDevice(driverType, core::dimension2d<u32>(640, 480));

    if (device == 0)
        return 1; // could not create selected driver.

    device->setWindowCaption(L"Survive ");
  //  device->setResizable(true);
    device->getFileSystem()->addFileArchive("data/egy2.pk3");

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* env = device->getGUIEnvironment();
    video::SMaterial material;

    /////////////////////////////////////////////////////////////////////////
    /// Interface
    ////////////////////////////////////////////////////////////////////////
    // Store the appropriate data in a context structure.
    SAppContext context;
    context.device = device;

    // Then create the event receiver, giving it that context structure.
    GameEvent* receiver = new GameEvent(context);;

    // And tell the device to use our custom event receiver.
    device->setEventReceiver(receiver);
    receiver->Context.launch = false;
    receiver->Context.pause = false;

    ///////////////////////////////////////////////////////////////////////////
    /// Scene
    //////////////////////////////////////////////////////////////////////////
    scene::IAnimatedMesh* mesh = smgr->getMesh("egyziana2.bsp");
    scene::IMeshSceneNode* node = 0;
    scene::ITriangleSelector* selector = 0;

    if (mesh)
        //node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
        node = smgr->addOctreeSceneNode(mesh->getMesh(0));

    if (node){
            node->setPosition(core::vector3df(100,-15.0,250));
            selector = smgr->createOctreeTriangleSelector(node->getMesh(), node, 128);
            node->setTriangleSelector(selector);
    }


    ///////////////////////////////////////////////////////////////////////////
    /// Camera and player
    //////////////////////////////////////////////////////////////////////////

    Camera* camera = new Camera(smgr);
    camera->collision(smgr,selector);

    ///////////////////////////////////////////////////////////////////////////
    /// Initialisation
    //////////////////////////////////////////////////////////////////////////

    Game* game = new Game(device,smgr,driver,material,selector,camera);
    game->initInterface();
    Player* player = new Player(smgr,driver,camera,material);

    //Enemy* nodeEnemy = new Enemy(core::vector3df(0.231,0.0,207.0),0.3,smgr,camera,material,driver);
    //nodeEnemy->collision(smgr,selector);
    game->loadEnemy();
    device->getCursorControl()->setVisible(true);

    material.setTexture(0, 0);
    material.Lighting = false;

    int lastFPS = -1;
    material.Wireframe=true;

    f32 deltaTime = 0.0f;
    f32 presentTime = 0.0f;
    f32 previousTime = 0.0f;
    f32 lastTime = 0.0f;
    bool begin = true;

    while(device->run())
    {
        if (!device->isWindowActive())
        {
            device->yield();
            continue;
        }

        presentTime = device->getTimer()->getTime();
        deltaTime = presentTime - previousTime;

        if((deltaTime > 15) && (device->isWindowActive()))
        {
            driver->beginScene(true, true, 0);

            // pause or unpause the game
            if(!begin && (presentTime - lastTime > 100)){
                game->pause(receiver->isKeyDown(KEY_SPACE) ,receiver);
                game->unpause(receiver->isKeyDown(KEY_KEY_B) ,receiver,player);
                lastTime = presentTime;
            }

            if((deltaTime > 15) && (receiver->Context.pause))
            {
                device->getCursorControl()->setVisible(true);
                game->displayPauseMenu();
            }

            if((deltaTime > 15) && (!receiver->Context.pause))
            {
                previousTime = presentTime;
                if(receiver->Context.launch)
                {
                    device->getCursorControl()->setVisible(false);
                    begin =  false;
                    smgr->drawAll();
                    /*scene::ISceneNode* highlightedSceneNode=player->targetGun();
                    if (highlightedSceneNode->getID()!=-1);*/
                    game->killEnemy(player,receiver);

                    //player->updatePosition(camera);
                    game->displayHealthBar(player);
                    game->displayTime();
                    game->runEnemy();
                    game->collisionEntreEnemy();


                    //game
                    //nodeEnemy->attack();

                    //game->collisionPlayerEnemy(camera,player,nodeEnemy);



                   // if (receiver.GetMouseState())


                    int fps = driver->getFPS();

                    if (lastFPS != fps)
                    {
                        core::stringw str = L"Survive [";
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

            }
            driver->endScene();
        }


    }

    device->drop();

    return 0;
}


