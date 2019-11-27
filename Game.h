#include <irrlicht.h>
#include "driverChoice.h"
#include "GameEvent.h"
#include "Enemy.h"


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Game
{
    public:
        Game(IrrlichtDevice* irrDevice, ISceneManager* manager, IVideoDriver* videoDriver,SMaterial mymaterial,ITriangleSelector* myselector,Camera* mycamera);
        void initInterface();
        void displayHealthBar(Player* player);
        void displayTime();
        void displayPauseMenu();
        void pause(bool isPaused,GameEvent* receiver);
        void unpause(bool isUnpaused,GameEvent* receiver,Player* player);
        void loadEnemy();
        void killEnemy(Player* player,GameEvent* receiver);
        void collisionPlayerEnemy(Camera* camera, Player* player ,Enemy* enemy);
        void runEnemy();
        void collisionEnemy(Enemy* enemy1,Enemy* enemy2);
        void collisionEntreEnemy();


    private:
        IrrlichtDevice* device;
        ISceneManager* smgr;
        IVideoDriver* driver;
        IGUIEnvironment* env;
        SMaterial material;
        ITriangleSelector* selector;
        Camera* camera;
        list<Enemy*> enemyList;
        int NumberEnemy;
        ITexture *digits[11];
        int time;
        int fps;
        //oldtime;
};
