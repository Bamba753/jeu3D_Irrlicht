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
        Game(IrrlichtDevice* irrDevice, ISceneManager* manager, IVideoDriver* videoDriver);
        void initInterface();
        void jeu(Player* player,Camera* camera,GameEvent* receiver,SMaterial material,ITriangleSelector* selector,u32 w,u32 h);
        void displayHealthBar(Player* player);
        void displayTime();
        void displayPauseMenu();
        void pause(bool isPaused,GameEvent* receiver);
        void unpause(bool isUnpaused,GameEvent* receiver,Player* player);
        void loadEnemy(Camera* camera,SMaterial material,ITriangleSelector* selector);
        void killEnemy(Player* player,GameEvent* receiver);
        void collisionPlayerEnemy(Camera* camera, Player* player ,Enemy* enemy);
        void runEnemy();
        void collisionEnemy(Enemy* enemy1);
        void collisionBetweenEnemy();
        void collisionPlayerAllEnemy(Camera* camera,Player* player);
        void displayRedScreen();
        void displayGameOverMenu();
        bool gameOver(Player* player);


    private:
        IrrlichtDevice* device;
        ISceneManager* smgr;
        IVideoDriver* driver;
        IGUIEnvironment* env;
        Camera* camera;
        list<Enemy*> enemyList;
        int NumberEnemy;
        ITexture *digits[11];
        int time;
        int fps;
        f32 last = 0.0f;
};
