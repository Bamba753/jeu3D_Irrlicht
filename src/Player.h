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
            ISceneNode* targetGun();
            int getHealth();
            bool isDead();
            void healthDecrease(IrrlichtDevice* device);
            void setHighLight(scene::ISceneNode* value);


    private:
            ISceneManager* smgr;
            IVideoDriver* driver;
            Camera* camera;
            SMaterial material;
            IAnimatedMesh* mesh;
            IAnimatedMeshSceneNode* node;
            int health;
            IBillboardSceneNode * bill;
            ILightSceneNode * light;
            ISceneNode* highlightedSceneNode;
            ISceneCollisionManager* collMan;
            f32 lasttouch = 0.0f;

};
