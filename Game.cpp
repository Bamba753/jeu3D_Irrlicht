#include "stdafx.h"
#include "Game.h"

Game::Game(IrrlichtDevice* irrDevice, ISceneManager* manager, IVideoDriver* videoDriver,
           SMaterial mymaterial,ITriangleSelector* myselector,Camera* mycamera)
{
        device = irrDevice;
        smgr = manager;
        driver = videoDriver;
        env = device->getGUIEnvironment();
        material = mymaterial;
        selector  = myselector;
        camera = mycamera;



        digits[0] = driver->getTexture("data/0.png");
        digits[1] = driver->getTexture("data/1.png");
        digits[2] = driver->getTexture("data/2.png");
        digits[3] = driver->getTexture("data/3.png");
        digits[4] = driver->getTexture("data/4.png");
        digits[5] = driver->getTexture("data/5.png");
        digits[6] = driver->getTexture("data/6.png");
        digits[7] = driver->getTexture("data/7.png");
        digits[8] = driver->getTexture("data/8.png");
        digits[9] = driver->getTexture("data/9.png");
        digits[10] = driver->getTexture("data/point.png");
}

void Game::initInterface()
{
    IGUISkin* skin = env->getSkin();
    IGUIFont* font = env->getFont("data/fonthaettenschweiler.bmp");
    if (font)
        skin->setFont(font);
    env->clear();

    env->addImage(device->getVideoDriver()->getTexture("data/interface.png"), vector2d<s32>(0, 0), false);
    skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
    env->addButton(rect<s32>(50, 320, 295, 360), 0, GUI_ID_PLAY_BUTTON,
            L"Lancer une partie !", L"T'es chaud. Vas y !!!");
    env->addButton(rect<s32>(345, 320, 590, 360), 0, GUI_ID_INSTRUCTIONS_BUTTON, L"Instructions", L"Si t'es prêt. Check les intructions de la mission");
    env->addButton(rect<s32>(50, 400, 295, 440), 0, GUI_ID_CONTROLS_BUTTON, L"Commandes", L"Tu veux savoir les touches pour jouer. Press!!!");
    env->addButton(rect<s32>(345, 400, 590, 440), 0, GUI_ID_QUIT_BUTTON,
            L"Quiter le jeu", L"Ah noooon. Tu veux quitter le jeu?");

    time = 600;
    fps = 0;
}

void Game::pause(bool isPaused,GameEvent* receiver)
{

    if(isPaused)
    {
        receiver->Context.pause = true;

    }

}

void Game::unpause(bool isUnaused,GameEvent* receiver,Player* player){

    if(isUnaused)
    {
        receiver->Context.pause = false;
        player->healthDecrease();

    }

}


void Game::displayHealthBar(Player* player)
{
        if (player->getHealth() < 0.0)
        {
                return;
        }
        float alpha = ((float) player->getHealth())/10.0;
        SColor r = SColor(255, 255, 0, 0);
        SColor g = SColor(150, 0, 128, 0);
        SColor t = SColor((1-alpha)*r.getAlpha()+alpha*g.getAlpha(),(1-alpha)*r.getRed()+alpha*g.getRed(),
                          (1-alpha)*r.getGreen()+alpha*g.getGreen(),(1-alpha)*r.getBlue()+alpha*g.getBlue());
        driver->draw2DRectangle(rect<s32>(600, 90 + 30*(10-player->getHealth()), 610, 390), t, t, r, r);
}

void Game::loadEnemy()
{

    /*Enemy* ptr = NULL;
    list<Enemy*>::Iterator it = list<Enemy*>::Iterator();
    list<Enemy*>::Iterator tempIterator = list<Enemy*>::Iterator();
    it = updateList.begin();
    for(int i;i<5;i++){

       }
      */
    Enemy* nodeEnemy = new Enemy(core::vector3df(0.231,0.0,207.0),0.3,smgr,camera,material,driver);
    nodeEnemy->collision(smgr,selector);
}

void Game::displayPauseMenu()
{


    env->addImage(device->getVideoDriver()->getTexture("data/interface.png"), vector2d<s32>(0, 0), false);
    env->addButton(rect<s32>(50, 50, 200, 90), 0, GUI_ID_PLAY_BUTTON,
            L"Relancer le jeu", L"T'es chaud. Vas y !!!");
    env->addButton(rect<s32>(50,110, 200, 150), 0, GUI_ID_INSTRUCTIONS_BUTTON, L"Instructions", L"Si t'es prêt. Check les intructions de la mission");
    env->addButton(rect<s32>(50, 170, 200, 210), 0, GUI_ID_CONTROLS_BUTTON, L"Commandes", L"Tu veux savoir les touches pour jouer. Press!!!");
    env->addButton(rect<s32>(50,230, 200, 270), 0, GUI_ID_QUIT_BUTTON,
            L"Quiter le jeu", L"Ah noooon. Tu veux quitter le jeu?");
    env->drawAll();
}


void Game::displayTime()
{
    fps +=1;


    if(fps > 60)
    {
        time -= 1;
        fps = 0;
    }
    env->clear();

    IGUIImage *minutes_d = env->addImage(rect<s32>(525,10,  540,50));
    minutes_d->setScaleImage(true);
    IGUIImage *minutes_u  = env->addImage(rect<s32>(545,10, 560,50));
    minutes_u->setScaleImage(true);

    IGUIImage *point_sup  = env->addImage(rect<s32>(565,20, 570,30));
    point_sup->setScaleImage(true);
    IGUIImage *point_inf  = env->addImage(rect<s32>(565,30, 570,40));
    point_inf->setScaleImage(true);

    IGUIImage *secondes_d   = env->addImage(rect<s32>(575,10,  590,50));
    secondes_d->setScaleImage(true);
    IGUIImage *secondes_u    = env->addImage(rect<s32>(595,10, 610,50));
    secondes_u->setScaleImage(true);


    minutes_d->setImage(digits[(time / 60) /10]);
    minutes_u->setImage(digits[(time / 60) % 10]);
    point_inf->setImage(digits[10]);
    point_sup->setImage(digits[10]);
    secondes_d->setImage(digits[(time % 60)/10]);
    secondes_u->setImage(digits[(time % 60) % 10]);

    env->drawAll();
}
