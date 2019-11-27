#include "stdafx.h"
#include "irrlicht.h"
#include "driverChoice.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


struct SAppContext
{
    irr::IrrlichtDevice *device;
    bool            launch ;
    bool            pause ;

};

enum
{
    GUI_ID_QUIT_BUTTON = 101,
    GUI_ID_PLAY_BUTTON,
    GUI_ID_CONTROLS_BUTTON,
    GUI_ID_INSTRUCTIONS_BUTTON,
    GUI_ID_INSTRUCTIONS_BACK_BUTTON,
    GUI_ID_CONTROLS_BACK_BUTTON
};


class GameEvent : public irr::IEventReceiver
{
    public:

        GameEvent(SAppContext& context);

        virtual bool OnEvent(const irr::SEvent& event);
        virtual bool isKeyDown(EKEY_CODE keyCode) const;
        virtual bool isKeyUp(EKEY_CODE keyCode) const;

        SAppContext& Context;
    private :
        bool keyDown[KEY_KEY_CODES_COUNT];
};



