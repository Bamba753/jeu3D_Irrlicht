#include "Camera.h"


Camera::Camera(ISceneManager* smgr)
{
    SKeyMap keyMap[9];
         /*   keyMap[0].Action = EKA_MOVE_FORWARD;
            keyMap[0].KeyCode = KEY_UP;

            keyMap[2].Action = EKA_MOVE_BACKWARD;
            keyMap[2].KeyCode = KEY_DOWN;


            keyMap[4].Action = EKA_STRAFE_LEFT;
            keyMap[4].KeyCode = KEY_LEFT;


            keyMap[6].Action = EKA_STRAFE_RIGHT;
            keyMap[6].KeyCode = KEY_RIGHT;


            keyMap[8].Action = EKA_JUMP_UP;
            keyMap[8].KeyCode = KEY_KEY_S;*/

    keyMap[0].Action = EKA_MOVE_FORWARD;
        keyMap[0].KeyCode = KEY_UP;
        keyMap[1].Action = EKA_MOVE_FORWARD;
        keyMap[1].KeyCode = KEY_KEY_Z;

        keyMap[2].Action = EKA_MOVE_BACKWARD;
        keyMap[2].KeyCode = KEY_DOWN;
        keyMap[3].Action = EKA_MOVE_BACKWARD;
        keyMap[3].KeyCode = KEY_KEY_S;

        keyMap[4].Action = EKA_STRAFE_LEFT;
        keyMap[4].KeyCode = KEY_LEFT;
        keyMap[5].Action = EKA_STRAFE_LEFT;
        keyMap[5].KeyCode = KEY_KEY_Q;

        keyMap[6].Action = EKA_STRAFE_RIGHT;
        keyMap[6].KeyCode = KEY_RIGHT;
        keyMap[7].Action = EKA_STRAFE_RIGHT;
        keyMap[7].KeyCode = KEY_KEY_D;

        keyMap[8].Action = EKA_JUMP_UP;
        keyMap[8].KeyCode = KEY_SPACE;



    node = smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, 0, keyMap, 9, true, 10.f);

    node->setDebugDataVisible(irr::scene::EDS_BBOX);
    node->setPosition(core::vector3df(50,100,-60));
    node->setTarget(core::vector3df(-70,30,-60));
}

ICameraSceneNode* Camera::getNode()
{
        return node;
}


vector3df Camera::getPosition()
{
        node->updateAbsolutePosition();
        return node->getAbsolutePosition();
}


// Colision du mesh avec l'environnement (collision du joueur (joueur rattaché au camera

void Camera::collision(ISceneManager* smgr, ITriangleSelector* selector) {

    core::vector3df radius = core::vector3df(30,40,30);

    scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
                selector, node,radius ,
                core::vector3df(0,-10,0), core::vector3df(0,60,0));
    node->addAnimator(anim);
    anim->drop();  // And likewise, drop the animator when we're done referring to it.
}
