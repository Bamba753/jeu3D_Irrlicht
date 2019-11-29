#include "Game.h"

Game::Game(IrrlichtDevice* irrDevice, ISceneManager* manager, IVideoDriver* videoDriver)
{
        device = irrDevice;
        smgr = manager;
        driver = videoDriver;
        env = device->getGUIEnvironment();


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
    env->clear();
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

void Game::jeu(Player* player,Camera* camera,GameEvent* receiver,SMaterial material,ITriangleSelector* selector,u32 w,u32 h)
{

     displayHealthBar(player);
     displayTime();
     runEnemy();
     collisionBetweenEnemy();
     collisionPlayerAllEnemy(camera,player);
     killEnemy(player,receiver);


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

void Game::loadEnemy(Camera* camera,SMaterial material,ITriangleSelector* selector)
{

    for(int i;i<5;i++)
    {
        Enemy* enemy = new Enemy(core::vector3df(i*10+0.231,0.0,i*207.0),1,smgr,camera,material,driver);
        enemy->collision(smgr,selector);
        enemyList.push_back(enemy);
    }
}

void Game::runEnemy()
{
    Enemy* ptr = NULL;
    list<Enemy*>::Iterator it = list<Enemy*>::Iterator();
    list<Enemy*>::Iterator tempIterator = list<Enemy*>::Iterator();
    it = enemyList.begin();

    if(enemyList.getSize() != 0)
    {
        //  parcourir la liste des enemy
        for (int i = 0; i <enemyList.getSize() ; ++i)
        {
                // Check for it.current = NULL.
                if (it.operator==(tempIterator))
                {
                        return;
                }

                // Check for Enemy.
                ptr = dynamic_cast<Enemy*>(it.operator*());
                if (ptr != NULL)
                {
                        // Enemy instance.
                        Enemy* enemy = dynamic_cast<Enemy*>(ptr);
                        enemy->attack();
                }

                // Check for it.current->next = NULL.
                if (tempIterator.operator==(it.operator++()))
                {
                        return;
                }
        }
    }
}

void Game::displayPauseMenu()
{

    env->addImage(device->getVideoDriver()->getTexture("data/interface.png"), vector2d<s32>(0, 0), false);
    env->addButton(rect<s32>(50, 50, 200, 90), 0, GUI_ID_PLAY_CONTINUE_BUTTON,
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


    if(fps > driver->getFPS())
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

void Game::killEnemy(Player* player,GameEvent* receiver)

{
    if (enemy_dammage && device->getTimer()->getTime()-last2>200.0f)
    {
        enemy_hurt->enemyHurt();
        enemy_dammage=false;
        last2=device->getTimer()->getTime();
        enemy_hurt=NULL;



    }

    if ((player->targetGun() && player->targetGun()->getID()!=-1))
    {
        if (receiver->getLeftButton() && ((device->getTimer()->getTime() - last) >600))
        {
            scene::ISceneNode* ptr = NULL;
            Enemy* ptr_bis = NULL;
            list<Enemy*>::Iterator it = list<Enemy*>::Iterator();
            list<Enemy*>::Iterator tempIterator = list<Enemy*>::Iterator();
            it=enemyList.begin();

            if(enemyList.getSize() != 0)
            {
                    //  parcourir la liste des
                    for (int i = 0; i <enemyList.getSize() ; ++i)
                    {
                            // Check for it.current = NULL.
                            if (it.operator==(tempIterator))
                            {
                                    return;
                            }

                            // Check for Enemy.
                            ptr = dynamic_cast<Enemy*>(it.operator*())->getNode();
                            ptr_bis = dynamic_cast<Enemy*>(it.operator*());
                            if ((ptr != NULL) && (player->targetGun() == ptr))
                            {

                                Enemy* enemy = dynamic_cast<Enemy*>(ptr_bis);
                                enemy_hurt=dynamic_cast<Enemy*>(ptr_bis);
                                enemy->healthDecrease();
                                enemy_dammage=true;
                                last2=device->getTimer()->getTime();
                                std::cout<<enemy->getHealth()<<std::endl;
                                if(enemy->getHealth() <= 0 )
                                {
                                    enemy_dammage=false;
                                    player->targetGun()->remove();
                                    player->setHighLight(NULL);
                                    enemyList.erase(it);
                                }
                                break;
                            }

                            // Check for it.current->next = NULL.
                            if (tempIterator.operator==(it.operator++()))
                            {
                                    return;
                            }
                    }
            }
            last = device->getTimer()->getTime();
        }
    }

}


void Game::collisionBetweenEnemy()
{
    Enemy* ptr = NULL;
    list<Enemy*>::Iterator it = list<Enemy*>::Iterator();
    list<Enemy*>::Iterator tempIterator = list<Enemy*>::Iterator();
    it = enemyList.begin();

    if(enemyList.getSize() != 0)
    {
        //  parcourir la liste des enemy
        for (int i = 0; i <enemyList.getSize() ; ++i)
        {
                // Check for it.current = NULL.
                if (it.operator==(tempIterator))
                {
                        return;
                }

                // Check for Enemy.
                ptr = dynamic_cast<Enemy*>(it.operator*());
                if (ptr != NULL)
                {
                        // Enemy instance.
                        Enemy* enemy = dynamic_cast<Enemy*>(ptr);
                        collisionEnemy(enemy);

                }

                // Check for it.current->next = NULL.
                if (tempIterator.operator==(it.operator++()))
                {
                        return;
                }
        }
    }


}

void Game::collisionEnemy(Enemy* enemy1)
{   
    Enemy* ptr = NULL;
    list<Enemy*>::Iterator it = list<Enemy*>::Iterator();
    list<Enemy*>::Iterator tempIterator = list<Enemy*>::Iterator();
    it = enemyList.begin();

     if(enemyList.getSize() != 0)
     {
         //  parcourir la liste des enemy
         for (int i = 0; i <enemyList.getSize() ; ++i)
         {
                 // Check for it.current = NULL.
                 if (it.operator==(tempIterator))
                 {
                         return;
                 }

                 // Check for Enemy.
                 ptr = dynamic_cast<Enemy*>(it.operator*());
                 if (ptr != NULL)
                 {
                         // Enemy instance.
                         Enemy* enemy2 = dynamic_cast<Enemy*>(ptr);

                         // Definition des deux box player - enemy (camera -> player)
                         aabbox3df boxenemy1 = enemy1->getNode()->getTransformedBoundingBox();
                         aabbox3df boxenemy2 = enemy2->getNode()->getTransformedBoundingBox();

                         // Direction enemy -> player
                         vector3df diff = boxenemy1.getCenter() - boxenemy2.getCenter();
                         diff.Y = 0;0;
                         diff.normalize();

                        if (boxenemy1.intersectsWithBox(boxenemy2))
                        {
                            //  recul enemy et player | healthdecrease
                                enemy1->getNode()->setPosition(enemy1->getPosition() + 3*diff) ;
                                enemy2->getNode()->setPosition(enemy2->getPosition() - 3*diff);
                        }

                 }

                 // Check for it.current->next = NULL.
                 if (tempIterator.operator==(it.operator++()))
                 {
                         return;
                 }
         }
     }

}

void Game::collisionPlayerAllEnemy(Camera* camera,Player* player)
{
    Enemy* ptr = NULL;
    list<Enemy*>::Iterator it = list<Enemy*>::Iterator();
    list<Enemy*>::Iterator tempIterator = list<Enemy*>::Iterator();
    it = enemyList.begin();

    if(enemyList.getSize() != 0)
    {
        //  parcourir la liste des enemy
        for (int i = 0; i <enemyList.getSize() ; ++i)
        {
                // Check for it.current = NULL.
                if (it.operator==(tempIterator))
                {
                        return;
                }

                // Check for Enemy.
                ptr = dynamic_cast<Enemy*>(it.operator*());
                if (ptr != NULL)
                {
                        // Enemy instance.
                        Enemy* enemy = dynamic_cast<Enemy*>(ptr);
                        collisionPlayerEnemy(camera,player,enemy);
                }

                // Check for it.current->next = NULL.
                if (tempIterator.operator==(it.operator++()))
                {
                        return;
                }
        }
    }


}


void Game::collisionPlayerEnemy(Camera* camera,Player* player ,Enemy* enemy)
{
        // Definition des deux box player - enemy (camera -> player)
      aabbox3d<f32> boxcamera =  aabbox3d<f32>(camera->getNode()->getPosition().X - 10.0,
                                         camera->getNode()->getPosition().Y -10.0,
                   camera->getNode()->getPosition().Z - 10.0, camera->getNode()->getPosition().X + 10.0,
                   camera->getNode()->getPosition().Y + 10.0, camera->getNode()->getPosition().Z + 10.0);
      aabbox3df boxenemy = enemy->getNode()->getTransformedBoundingBox();

      // Direction enemy -> player
      vector3df diff = boxcamera.getCenter() - boxenemy.getCenter();
      diff.Y = 0;0;
      diff.normalize();

     if (boxcamera.intersectsWithBox(boxenemy))
     {
         //  recul enemy et player | healthdecrease
             camera->getNode()->setPosition(camera->getPosition() + 3*diff) ;
             enemy->getNode()->setPosition(enemy->getPosition() - 3*diff);
             player->healthDecrease(device);
             displayRedScreen();
     }
}

void Game::displayRedScreen()
{
    SColor r = SColor(50, 255, 0, 0);
    driver->draw2DRectangle(rect<s32>(0, 0, 640, 480), r, r, r, r);
}

void Game::displayGameOverMenu()
{

    env->addImage(device->getVideoDriver()->getTexture("data/interface.png"), vector2d<s32>(0, 0), false);
    env->addButton(rect<s32>(50, 50, 200, 90), 0, GUI_ID_PLAY_CONTINUE_BUTTON,
            L"Recommencer", L"T'es chaud. Vas y !!!");
    env->addButton(rect<s32>(50,230, 200, 270), 0, GUI_ID_QUIT_BUTTON,
            L"Quiter le jeu", L"Ah noooon. Tu veux quitter le jeu?");
    env->drawAll();

}

bool Game::gameOver(Player* player)
{
    if (player->isDead())
        return true;
    false;
}
