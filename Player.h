#include <irrlicht.h>
#include "Camera.h"
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;


class Player
{
    public:
            Player(ISceneManager* mysmgr, IVideoDriver* mydriver, Camera* mycamera,SMaterial mymaterial);
            IAnimatedMeshSceneNode* getNode();
            IBillboardSceneNode* getBill();
            vector3df getPosition();
            void updatePosition(Camera* camera);
            void targetGun();
            int getHealth();
            void shoot();
            void isDead();
            void healthDecrease();


    private:
            ISceneManager* smgr;
            IVideoDriver* driver;
            Camera* camera;
            SMaterial material;
            IAnimatedMesh* mesh;
            IAnimatedMeshSceneNode* node;
            int health;
            core::vector3df dist_cameraplayer = core::vector3df(0.0,-20.0,10.0);
            scene::IBillboardSceneNode * bill;
            scene::ILightSceneNode * light;
            scene::ISceneNode* highlightedSceneNode;
            scene::ISceneCollisionManager* collMan;

};
