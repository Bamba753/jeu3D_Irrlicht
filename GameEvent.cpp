#include "GameEvent.h"



bool GameEvent::OnEvent(const irr::SEvent& event)
{
    if (event.EventType == EET_GUI_EVENT)
    {
        s32 id = event.GUIEvent.Caller->getID();
        IGUIEnvironment* env = Context.device->getGUIEnvironment();

        switch(event.GUIEvent.EventType)
        {

        case EGET_BUTTON_CLICKED:
            switch(id)
            {
            case GUI_ID_QUIT_BUTTON:
                Context.device->closeDevice();
                return true;

            case GUI_ID_NEW_WINDOW_BUTTON:
                Context.launch = true;
                return true;

            default:
                return false;
            }
            break;

        default:
            break;
        }
    }

    return false;
}
