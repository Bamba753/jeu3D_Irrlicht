#include <irrlicht.h>
#include "Player.h"
#include <math.h>

using namespace irr;
using namespace core;
using namespace scene;


class Enemy
{

    public:
        Enemy(vector3df newPosition,float newSpeed,ISceneManager* smgr,Camera* playerTarget,video::SMaterial material, IVideoDriver* driver);
        IAnimatedMeshSceneNode* getNode();
        virtual vector3df getPosition();
        vector3df getDirection(vector3df v, vector3df r);
        vector3df getTargetAngle(vector3df v, vector3df r);
        float getDistanceEnemeyPlayer(vector3df v, vector3df r);
        void collision(ISceneManager* smgr, ITriangleSelector* selector);
        void attack();
        void healthDecrease();


    private:
        IAnimatedMeshSceneNode* node;
        Camera* target;
        vector3df facingDirection;
        IrrlichtDevice* device;
        video::SMaterial material;
        float speed;
        int health;


};
