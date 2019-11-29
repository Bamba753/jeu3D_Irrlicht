#include <irrlicht.h>
#include "driverChoice.h"
#include "Game.h"
#include <irrKlang.h>
#include "../common/conio.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace irrklang;

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
    device->getFileSystem()->addFileArchive("data/egy2.pk3");

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* env = device->getGUIEnvironment();
    video::SMaterial material;

    ISoundEngine* engine = createIrrKlangDevice();
    ISoundSource* backgroundSound = engine->addSoundSourceFromFile("data/audio.wav");
    ISoundSource* shootSound = engine->addSoundSourceFromFile("data/AK.wav");

    engine->play2D(backgroundSound,true); // Music background

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
    /// Initialisation
    //////////////////////////////////////////////////////////////////////////
    Camera* camera = new Camera(smgr);                          // Creation  de la camera
    camera->collision(smgr,selector);                           // Initialisation de la collision camera(player) et environnement
    Game* game = new Game(device,smgr,driver);                  // camera
    game->initInterface();                                      // Initialisation de l'interface
    Player* player = new Player(smgr,driver,camera,material);   // Chargement de l'arme


    device->getCursorControl()->setVisible(true);               //Curseur visible au debut du jeu
    material.setTexture(0, 0);
    material.Lighting = false;

    int lastFPS = -1;
    material.Wireframe=false;

    f32 deltaTime = 0.0f;
    f32 presentTime = 0.0f;
    f32 previousTime = 0.0f;
    f32 lastTime = 0.0f;
    f32 lastShoot = 0.0f;
    u32 width;
    u32 heigth;
    bool begin = true;
    bool recul=false;
    while(device->run())
    {
        if (!device->isWindowActive())
        {
            device->yield();
            continue;
        }
        //std::cout<<player->getNode()->getPosition().X<<player->getNode()->getPosition().Y<<player->getNode()->getPosition().Z<<std::endl;


        presentTime = device->getTimer()->getTime();
        deltaTime = presentTime - previousTime;

        if((deltaTime > 15) && (device->isWindowActive()))
        {
            driver->beginScene(true, true, 0);

            // Taille de la fenetre du jeu
            const core::dimension2d<u32>& sizeWindow = driver->getScreenSize();
            width = sizeWindow.Width;
            heigth = sizeWindow.Height;

            // pause or unpause the game
            if(!begin && (presentTime - lastTime > 300) &&receiver->isKeyDown(KEY_ESCAPE))
            {
                if (!receiver->Context.pause)
                {
                    std::cout<<"pause"<<std::endl;
                    game->pause(receiver->isKeyDown(KEY_ESCAPE) ,receiver);
                    receiver->Context.pause=true;
                }
                else
                {
                    std::cout<<"Unpause"<<std::endl;

                    game->unpause(receiver->isKeyDown(KEY_ESCAPE) ,receiver,player);
                    receiver->Context.pause = false;
                }
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
                if(receiver->Context.launch && !game->gameOver(player) && !game->gameComplete())
                {
                    // Arrete la musique au debut du jeu
                    if(engine)
                    {
                        //engine->drop();
                        //engine = createIrrKlangDevice();
                    }


                    device->getCursorControl()->setVisible(false);
                    begin =  false;
                    smgr->drawAll();
                    player->targetGun();
                    game->jeu(player,camera,receiver,
                              material,selector,
                              width,heigth);


                    if(receiver->getLeftButton() && (device->getTimer()->getTime() - lastShoot > 200))
                    {
                        engine->play2D(shootSound); // Music background

                        player->getNode()->setPosition(player->getNode()->getPosition()+core::vector3df(0,0,-25));
                        recul=true;

                        lastShoot = device->getTimer()->getTime();
                    }
                    if (recul && (device->getTimer()->getTime() - lastShoot > 50))
                    {

                        player->getNode()->setPosition(player->getNode()->getPosition()+core::vector3df(0,0,25));
                        recul=false;

                    }



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
                else if(game->gameOver(player))
                {
                    device->getCursorControl()->setVisible(true);
                    game->displayGameOverMenu();
                }
                else if(game->gameComplete())
                {
                    device->getCursorControl()->setVisible(true);
                    game->displayGameCompleteMenu();
                }
                else
                {
                    env->drawAll();
                }

            }
            driver->endScene();
        }


    }
    engine->drop();
    device->drop();

    return 0;
}


