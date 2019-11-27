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
            void setNode(IAnimatedMeshSceneNode* value);
            IBillboardSceneNode* getBill();
            vector3df getPosition();
            aabbox3d<f32> getBox();
            void updateBox();
            scene::ISceneNode* targetGun();
            int getHealth();
            void shoot();
            void isDead();
            void healthDecrease();
            void setHighLight(scene::ISceneNode* value);


    private:
            ISceneManager* smgr;
            IVideoDriver* driver;
            Camera* camera;
            SMaterial material;
            IAnimatedMesh* mesh;
            IAnimatedMeshSceneNode* node;
            int health;
            scene::IBillboardSceneNode * bill;
            scene::ILightSceneNode * light;
            scene::ISceneNode* highlightedSceneNode;
            scene::ISceneCollisionManager* collMan;

};
