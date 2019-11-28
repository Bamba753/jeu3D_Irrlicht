#include <irrlicht.h>
#include "parameters.h"

using namespace irr;
using namespace core;
using namespace scene;


class Camera
{
    public:
        Camera(ISceneManager* smgr);
        ICameraSceneNode* getNode();
        vector3df getPosition();
        void collision(ISceneManager* smgr,ITriangleSelector* selector);


    private:
        ICameraSceneNode* node;
};
