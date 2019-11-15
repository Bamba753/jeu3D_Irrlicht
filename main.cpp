#include <irrlicht.h>
#include "driverChoice.h"
#include "GameEvent.h"   //Gestion Interface

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

    device->setWindowCaption(L"Irrlicht Engine - User Interface Demo");
    device->setResizable(true);
    device->getFileSystem()->addFileArchive("data/map-20kdm2.pk3");

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
    scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
    scene::ISceneNode* node = 0;

    if (mesh)
        node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);

    if (node)
        node->setPosition(core::vector3df(-1300,-144,-1249));

    smgr->addCameraSceneNodeFPS();
    device->getCursorControl()->setVisible(false);
    int lastFPS = -1;

    while(device->run())
     {
        game = receiver.Context;
        if (device->isWindowActive())
        {
            driver->beginScene(true, true, video::SColor(255,200,200,200));
            ////////////////////////////////////////////////////////////////
            /// Game
            ////////////////////////////////////////////////////////////////
            if(game.launch){
                smgr->drawAll();

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

