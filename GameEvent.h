#include <irrlicht.h>
#include "driverChoice.h"

// Declare a structure to hold some context for the event receiver so that it
// has it available inside its OnEvent() method.
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

struct SAppContext
{
    IrrlichtDevice *device;
    bool            launch;
};

enum
{
    GUI_ID_QUIT_BUTTON = 101,
    GUI_ID_NEW_WINDOW_BUTTON,
};

class GameEvent : public irr::IEventReceiver
{
    public:

        GameEvent(SAppContext& context) : Context(context){};

        virtual bool OnEvent(const irr::SEvent& event);

        SAppContext& Context;
};



