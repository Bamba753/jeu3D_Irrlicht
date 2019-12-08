#include "GameEvent.h"

GameEvent::GameEvent(SAppContext & context) : Context(context)
{
        for (int i = 0; i < KEY_KEY_CODES_COUNT; ++i)
        {
                keyDown[i] = false;
        }
}


bool GameEvent::OnEvent(const irr::SEvent& event)
{
    switch (event.EventType)
    {
        case EET_KEY_INPUT_EVENT:
        {
                keyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
                break;
        }

        case EET_GUI_EVENT:
        {
            s32 id = event.GUIEvent.Caller->getID();
            IGUIEnvironment* env = Context.device->getGUIEnvironment();
            LeftButtonDown = false;

            switch(event.GUIEvent.EventType)
            {


                case EGET_BUTTON_CLICKED:
                    switch(id)
                    {
                    case GUI_ID_QUIT_BUTTON:
                        env->clear();
                        Context.device->closeDevice();
                        return true;

                    case GUI_ID_PLAY_BUTTON:
                        env->clear();
                        Context.launch = true;
                        return true;

                    case GUI_ID_INSTRUCTIONS_BUTTON:
                        env->clear();
                        env->addImage(Context.device->getVideoDriver()->getTexture("data/interface.png"), vector2d<s32>(0, 0), false);
                        env->addButton(rect<s32>(270, 380, 370, 420), NULL, GUI_ID_INSTRUCTIONS_BACK_BUTTON, L"Retour");
                        break;

                    case GUI_ID_INSTRUCTIONS_BACK_BUTTON:
                        env->clear();
                        env->addImage(Context.device->getVideoDriver()->getTexture("data/interface.png"), vector2d<s32>(0, 0), false);
                        env->addButton(rect<s32>(50, 320, 295, 360), 0, GUI_ID_PLAY_BUTTON,
                                L"Lancer une partie !", L"T'es chaud. Vas y !!!");
                        env->addButton(rect<s32>(345, 320, 590, 360), 0, GUI_ID_INSTRUCTIONS_BUTTON, L"Instructions", L"Si t'es prêt. Check les intructions de la mission");
                        env->addButton(rect<s32>(50, 400, 295, 440), 0, GUI_ID_CONTROLS_BUTTON, L"Commandes", L"Tu veux savoir les touches pour jouer. Press!!!");
                        env->addButton(rect<s32>(345, 400, 590, 440), 0, GUI_ID_QUIT_BUTTON,
                                L"Quiter le jeu", L"Ah noooon. Tu veux quitter le jeu?");

                    case GUI_ID_CONTROLS_BUTTON:
                        env->clear();
                        env->addImage(Context.device->getVideoDriver()->getTexture("data/interface.png"), vector2d<s32>(0, 0), false);
                        env->addButton(rect<s32>(270, 380, 370, 420), NULL, GUI_ID_CONTROLS_BACK_BUTTON, L"Retour");
                        break;

                    case GUI_ID_CONTROLS_BACK_BUTTON:
                        env->clear();
                        env->addImage(Context.device->getVideoDriver()->getTexture("data/interface.png"), vector2d<s32>(0, 0), false);
                        env->addButton(rect<s32>(50, 320, 295, 360), 0, GUI_ID_PLAY_BUTTON,
                                L"Lancer une partie !", L"T'es chaud. Vas y !!!");
                        env->addButton(rect<s32>(345, 320, 590, 360), 0, GUI_ID_INSTRUCTIONS_BUTTON, L"Instructions", L"Si t'es prêt. Check les intructions de la mission");
                        env->addButton(rect<s32>(50, 400, 295, 440), 0, GUI_ID_CONTROLS_BUTTON, L"Commandes", L"Tu veux savoir les touches pour jouer. Press!!!");
                        env->addButton(rect<s32>(345, 400, 590, 440), 0, GUI_ID_QUIT_BUTTON,
                                L"Quiter le jeu", L"Ah noooon. Tu veux quitter le jeu?");
                        break;

                    case GUI_ID_PLAY_CONTINUE_BUTTON:
                        env->clear();
                        Context.launch = true;
                        Context.pause = false;

                    case GUI_ID_RESTART_BUTTON:
                        env->clear();
                        Context.launch = true;
                        Context.pause = false;
                        Context.restart = true;

                    default:
                        return false;
                    }
                break;
            }
        }

        case  EET_MOUSE_INPUT_EVENT:
        {
            switch(event.MouseInput.Event)
            {
                case EMIE_LMOUSE_PRESSED_DOWN:
                    LeftButtonDown = true;
                    break;

                case EMIE_LMOUSE_LEFT_UP:
                    LeftButtonDown = false;
                    break;

                default:
                    break;
            }
        }

        default:
            break;

    }
    return false;
}

bool GameEvent::getLeftButton(void) const
{
    return LeftButtonDown;
}


bool GameEvent::isKeyDown(EKEY_CODE keyCode) const
{
        return keyDown[keyCode];
}

bool GameEvent::isKeyUp(EKEY_CODE keyCode) const
{
        return keyDown[keyCode];
}

