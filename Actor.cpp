#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <cstdlib> 
using namespace std;

// __________________________
// Actor Base Class
// __________________________

Actor::Actor(int imageID, double startX, double startY, int dir, StudentWorld* world) : GraphObject(imageID, startX, startY, dir) {
    m_world = world; m_state = true; m_hp = -69; m_tickFreq = -420; m_blockPeas = false; m_damageable = false; m_isMarble = false; m_isGoodie = false;
}

Actor::~Actor() {}

void Actor::doSomething() {return;} // Do something does nothing by default

void Actor::takeDmg(int dmg) {return;} // Take damage does nothing by default

bool Actor::pushed(int dir) {return false;} // Useless by default

bool Actor::botShoot() {
    int currentX = GraphObject::getX();
    int currentY = GraphObject::getY();
    int playerX = getWorld()->getAvatar()->GraphObject::getX();
    int playerY = getWorld()->getAvatar()->GraphObject::getY();
    int dir = GraphObject::getDirection();
    if (dir == up || dir == down) { // Vertical
        if (playerX == currentX) { // Player in same column, different row
            bool check = true;
            if (currentY < playerY) { // Check for obstructions
                for (int i = currentY+1; i < playerY; i++) {
                    char object = getWorld()->getObjects()[currentX][i];
                    if (object != Level::empty && object != Level::pit) check = false;
                }
            } else if (currentY > playerY) { // Check for obstructions
                for (int i = playerY+1; i < currentY; i++) {
                    char object = getWorld()->getObjects()[currentX][i];
                    if (object != Level::empty && object != Level::pit) check = false;
                }
            }
            if (dir == up && playerY < currentY) check = false; // Check if facing player
            if (dir == down && playerY > currentY) check = false; // Check if facing player
            if (check) { 
                Actor* newPea = new Pea(currentX, currentY, dir, getWorld());
                newPea->move();
                getWorld()->addActor(newPea);
                getWorld()->GameWorld::playSound(SOUND_ENEMY_FIRE); // Shoot pea
                return true;
            }
        }
    } else if (dir == left || dir == right) { // Horizontal
        if (playerY == currentY) { // Player in same row, different column
            bool check = true;
            if (currentX < playerX) {
                for (int i = currentX+1; i < playerX; i++) { // Check for obstructions
                    char object = getWorld()->getObjects()[i][currentY];
                    if (object != Level::empty && object != Level::pit) check = false;
                }
            } else if (currentX > playerX) {
                for (int i = playerX+1; i < currentX; i++) { // Check for obstructions
                    char object = getWorld()->getObjects()[i][currentY];
                    if (object != Level::empty && object != Level::pit) check = false;
                }
            }
            if (dir == left && playerX > currentX) check = false; // Check if facing player
            if (dir == right && playerX < currentX) check = false; // Check if facing player
            if (check) { // Check if no obstructions
                Actor* newPea = new Pea(currentX, currentY, dir, getWorld());
                newPea->move();
                getWorld()->addActor(newPea);
                getWorld()->GameWorld::playSound(SOUND_ENEMY_FIRE); // Shoot pea
                return true;
            }
        }
    }
    return false;
}

bool Actor::botPathfind(int dir) {
    int currentX = GraphObject::getX();
    int currentY = GraphObject::getY();
    int playerX = getWorld()->getAvatar()->GraphObject::getX();
    int playerY = getWorld()->getAvatar()->GraphObject::getY();
    if (dir == -69) dir = GraphObject::getDirection();
    bool check = false;
    switch(dir) { // Check for obstructions and players
        case up:
        {
            char item = getWorld()->getObjects()[currentX][currentY+1];
            if (item == Level::empty && !(currentX == playerX && currentY+1 == playerY)) check = true;
            break;
        }
        case down:
        {
            char item = getWorld()->getObjects()[currentX][currentY-1];
            if (item == Level::empty && !(currentX == playerX && currentY-1 == playerY)) check = true;
            break;
        }
        case left:
        {
            char item = getWorld()->getObjects()[currentX-1][currentY];
            if (item == Level::empty && !(currentX-1 == playerX && currentY == playerY)) check = true;
            break;
        }
        case right:
        {
            char item = getWorld()->getObjects()[currentX+1][currentY];
            if (item == Level::empty && !(currentX+1 == playerX && currentY == playerY)) check = true;
            break;
        }
    }
    return check;
}

void Actor::setHP(double n) {m_hp = n;} 

void Actor::addHP(double n) {m_hp += n;}

void Actor::setState(bool s) {m_state = s;}

void Actor::setBlockPeas(bool b) {m_blockPeas = b;}

void Actor::setDamageable(bool b) {m_damageable = b;}

void Actor::setIsMarble(bool b) {m_isMarble = b;}

void Actor::setIsGoodie(bool b) {m_isGoodie = b;}

void Actor::setTickFreq(int n) {m_tickFreq = n;}

int Actor::getTickFreq() {return m_tickFreq;}

bool Actor::getState() {return m_state;}

bool Actor::getIsMarble() {return m_isMarble;}

bool Actor::getBlockPeas() {return m_blockPeas;}

bool Actor::getDamageable() {return m_damageable;}

bool Actor::getIsGoodie() {return m_isGoodie;}

char Actor::getVarient() {return ' ';} // Does nothing by default

bool Actor::onPlayer() {
    int currentX = GraphObject::getX();
    int currentY = GraphObject::getY();
    int playerX = getWorld()->getAvatar()->GraphObject::getX();
    int playerY = getWorld()->getAvatar()->GraphObject::getY();
    if (currentX == playerX) {
        if (currentY == playerY) { // If on same location as player
            return true;
        }
    }
    return false;
}

StudentWorld* Actor::getWorld() {return m_world;}

double Actor::getHP() {return m_hp;}

void Actor::move(int dir) {
    if (dir == -69) dir = GraphObject::getDirection(); // Use GraphObject::getDirection() by default
    int currentX = GraphObject::getX();
    int currentY = GraphObject::getY();
    switch(dir) {
        case up:
        {
            GraphObject::setDirection(up);
            int newY = currentY+1;
            if (newY < VIEW_HEIGHT) GraphObject::moveTo(currentX, newY);
            break;
        }
        case down:
        {
            GraphObject::setDirection(down);
            int newY = currentY-1;
            if (newY >= 0) GraphObject::moveTo(currentX, newY);
            break;
        }
        case left:
        {
            GraphObject::setDirection(left);
            int newX = currentX-1;
            if (newX >= 0) GraphObject::moveTo(newX, currentY);
            break;
        }
        case right:
        {
            GraphObject::setDirection(right);
            int newX = currentX+1;
            if (newX < VIEW_WIDTH) GraphObject::moveTo(newX, currentY);
            break;
        }
    }
}

// __________________________
// Derived Class Constructors
// __________________________

Wall::Wall(double startX, double startY) : Actor(IID_WALL, startX, startY) {
    Actor::GraphObject::setVisible(true);
    Actor::setBlockPeas(true);
}

Avatar::Avatar(double startX, double startY, StudentWorld* world) : Actor(IID_PLAYER, startX, startY, right, world) {
    Actor::GraphObject::setVisible(true);
    Actor::setHP(20);
    Actor::setBlockPeas(true);
    Actor::setDamageable(true);
    m_ammo = 20;
}

Pea::Pea(double startX, double startY, int dir, StudentWorld* world) : Actor(IID_PEA, startX, startY, dir, world) {Actor::GraphObject::setVisible(true); m_fresh = true;}

Marble::Marble(double startX, double startY, StudentWorld* world) : Actor(IID_MARBLE, startX, startY, none, world) {
    Actor::GraphObject::setVisible(true);
    Actor::setBlockPeas(true);
    Actor::setDamageable(true);
    Actor::setIsMarble(true);
    Actor::setHP(10);
}

Pit::Pit(double startX, double startY, StudentWorld* world) : Actor(IID_PIT, startX, startY, none, world) {Actor::GraphObject::setVisible(true);}

Crystal::Crystal(double startX, double startY, StudentWorld* world) : Actor(IID_CRYSTAL, startX, startY, none, world) {Actor::GraphObject::setVisible(true);}

Exit::Exit(double startX, double startY, StudentWorld* world) : Actor(IID_EXIT, startX, startY, none, world) {Actor::GraphObject::setVisible(false);}

Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* world) : Actor(imageID, startX, startY, none, world) {
    Actor::GraphObject::setVisible(true);
    Actor::setIsGoodie(true);
    m_varient = ' ';
}

Ammo::Ammo(double startX, double startY, StudentWorld* world) : Goodie(IID_AMMO, startX, startY, world) {
    Actor::GraphObject::setVisible(true);
    Goodie::setVarient(Level::ammo);
}

Health::Health(double startX, double startY, StudentWorld* world) : Goodie(IID_RESTORE_HEALTH, startX, startY, world) {
    Actor::GraphObject::setVisible(true);
    Goodie::setVarient(Level::restore_health);
}

ExtraLife::ExtraLife(double startX, double startY, StudentWorld* world) : Goodie(IID_EXTRA_LIFE, startX, startY, world) {
    Actor::GraphObject::setVisible(true);
    Goodie::setVarient(Level::extra_life);
}

RageBot::RageBot(double startX, double startY, int dir, StudentWorld* world) : Actor(IID_RAGEBOT, startX, startY, dir, world) {
    Actor::GraphObject::setVisible(true);
    Actor::setHP(10);
    Actor::setBlockPeas(true);
    Actor::setDamageable(true);
    Actor::setTickFreq((28 - world->GameWorld::getLevel()) / 4);
    if (Actor::getTickFreq() < 3) Actor::setTickFreq(3);
}

ThiefBot::ThiefBot(int imageID, double startX, double startY, StudentWorld* world, int HP) : Actor(imageID, startX, startY, right, world) {
    Actor::GraphObject::setVisible(true);
    Actor::setHP(HP);
    Actor::setBlockPeas(true);
    Actor::setDamageable(true);
    Actor::setTickFreq((28 - world->GameWorld::getLevel()) / 4);
    if (Actor::getTickFreq() < 3) Actor::setTickFreq(3);
    m_distBeforeTurn = rand() % 6 + 1;
    m_goodie = ' ';
    m_dist = 0;
}

MeanBot::MeanBot(double startX, double startY, StudentWorld* world) : ThiefBot(IID_MEAN_THIEFBOT, startX, startY, world, 8) {
    Actor::GraphObject::setVisible(true);
    Actor::setBlockPeas(true);
    Actor::setDamageable(true);
}

Factory::Factory(double startX, double startY, StudentWorld* world, bool mean) : Actor(IID_ROBOT_FACTORY, startX, startY, none, world) {
    Actor::GraphObject::setVisible(true);
    Actor::setBlockPeas(true);
    m_mean = mean;
}

// __________________________
// Derived Class Do Somethings
// __________________________

void Avatar::doSomething() {
    if (Actor::getHP() <= 0 || !Actor::getState()) return; // If player is dead
    int ch;
    if (Actor::getWorld()->GameWorld::getKey(ch)) {
        int currentX = Actor::GraphObject::getX();
        int currentY = Actor::GraphObject::getY();
        switch (ch) {
            case KEY_PRESS_ESCAPE: // Escape
            {
                Actor::setHP(0);
                Actor::setState(false);
                break;
            }
            case KEY_PRESS_SPACE:
            {
                if (m_ammo > 0) {
                    Actor* newPea = new Pea(currentX, currentY, Actor::GraphObject::getDirection(), Actor::getWorld());
                    newPea->move();
                    Actor::getWorld()->addActor(newPea);
                    m_ammo--;
                    Actor::getWorld()->GameWorld::playSound(SOUND_PLAYER_FIRE);
                }
                break;
            }
            case KEY_PRESS_LEFT:
            {
                Actor::GraphObject::setDirection(left);
                int newX = currentX-1;
                if (Actor::getWorld()->getObjects()[newX][currentY] == Level::empty) Actor::move(left); // If adjacent spot is empty
                if (Actor::getWorld()->getObjects()[newX][currentY] == Level::marble) // If adjacent spot is marble
                    if (Actor::getWorld()->StudentWorld::moveMarble(newX, currentY, left)) Actor::move(left);
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                Actor::GraphObject::setDirection(right);
                int newX = currentX+1;
                if (Actor::getWorld()->getObjects()[newX][currentY] == Level::empty) Actor::move(right); // If adjacent spot is empty
                if (Actor::getWorld()->getObjects()[newX][currentY] == Level::marble) // If adjacent spot is marble
                    if (Actor::getWorld()->StudentWorld::moveMarble(newX, currentY, right)) Actor::move(right);
                break;
            }
            case KEY_PRESS_UP:
            {
                Actor::GraphObject::setDirection(up);
                int newY = currentY+1;
                if (Actor::getWorld()->getObjects()[currentX][newY] == Level::empty) Actor::move(up); // If adjacent spot is empty
                if (Actor::getWorld()->getObjects()[currentX][newY] == Level::marble) // If adjacent spot is marble
                    if (Actor::getWorld()->StudentWorld::moveMarble(currentX, newY, up)) Actor::move(up);
                break;
            }
            case KEY_PRESS_DOWN:
            {
                Actor::GraphObject::setDirection(down);
                int newY = currentY-1;
                if (Actor::getWorld()->getObjects()[currentX][newY] == Level::empty) Actor::move(down); // If adjacent spot is empty
                if (Actor::getWorld()->getObjects()[currentX][newY] == Level::marble) // If adjacent spot is marble
                    if (Actor::getWorld()->StudentWorld::moveMarble(currentX, newY, down)) Actor::move(down);
                break;
            }
        }
    }
}

void Pea::doSomething() {
    if (m_fresh) {m_fresh = false;} // Not used
    doSomethingHelper();
    Actor::move();
    doSomethingHelper();
}

void Pea::doSomethingHelper() { // Damage objects on the same location
    if (!Actor::getState()) return; // If pea is dead
    int currentX = Actor::GraphObject::getX();
    int currentY = Actor::GraphObject::getY();
    int playerX = Actor::getWorld()->getAvatar()->Actor::GraphObject::getX();
    int playerY = Actor::getWorld()->getAvatar()->Actor::GraphObject::getY();
    if (currentX == playerX && currentY == playerY) { // If hit player
        Actor::getWorld()->getAvatar()->takeDmg(2);
        Actor::setState(false);
        return;
    }
    if (Actor::getWorld()->StudentWorld::peaScan(currentX, currentY)) Actor::setState(false);
}

void Pit::doSomething() {
    if (!Actor::getState()) return; // If pit is dead
    int currentX = Actor::GraphObject::getX();
    int currentY = Actor::GraphObject::getY();
    if (Actor::getWorld()->getObjects()[currentX][currentY] == Level::marble) { // If pit on same location as marble
        Actor::setState(false);
        Actor::getWorld()->getObjects()[currentX][currentY] = Level::empty;
        Actor::getWorld()->pitScan(currentX, currentY);
        Actor::getWorld()->getObjects()[currentX][currentY] = Level::empty;
    }
}

void Crystal::doSomething() {
    if (!Actor::getState()) return; // If crystal is dead
    if (Actor::onPlayer()) { // If at same location as player
        Actor::getWorld()->GameWorld::increaseScore(50);
        Actor::setState(false);
        Actor::getWorld()->GameWorld::playSound(SOUND_GOT_GOODIE);
        Actor::getWorld()->removeCrystal();
    }
}

void Exit::doSomething() {
    if (Actor::getWorld()->crystalCount() <= 0) { // If no more crystals
        if (!Actor::GraphObject::isVisible()) { // If not visible yet
            Actor::GraphObject::setVisible(true); // Make visible
            Actor::getWorld()->GameWorld::playSound(SOUND_REVEAL_EXIT);
        }
        if (Actor::onPlayer()) { // If on same location as player
            Actor::getWorld()->GameWorld::playSound(SOUND_FINISHED_LEVEL);
            Actor::getWorld()->GameWorld::increaseScore(2000);
            Actor::getWorld()->finishLevel();
        }
    }
}

void Goodie::doSomething() {
    if (!Actor::getState()) return; // If goodie is dead
    if (Actor::onPlayer()) { // If on same location as player
        Actor::getWorld()->GameWorld::playSound(SOUND_GOT_GOODIE);
        additional();
        Actor::setState(false);
    }
}

void RageBot::doSomething() {
    if (!Actor::getState()) return; // If bot is dead
    if (Actor::getWorld()->getTicks() % Actor::getTickFreq() == 0) { // If not rest tick
        int currentX = Actor::GraphObject::getX();
        int currentY = Actor::GraphObject::getY();
        int playerX = Actor::getWorld()->getAvatar()->GraphObject::getX();
        int playerY = Actor::getWorld()->getAvatar()->GraphObject::getY();
        int dir = Actor::GraphObject::getDirection();
        if (!Actor::botShoot())  { // If did not fire pea, move
            bool check = Actor::botPathfind(); // Check for obstructions and player
            if (check) { // If no obstructions
                Actor::getWorld()->getObjects()[currentX][currentY] = Level::empty;
                Actor::move();
                int newX = Actor::GraphObject::getX();
                int newY = Actor::GraphObject::getY();
                Actor::getWorld()->getObjects()[newX][newY] = Level::horiz_ragebot;
            } else { // Turn around if obstructions
                switch(dir) {
                    case up:
                        Actor::GraphObject::setDirection(down);
                        break;
                    case down:
                        Actor::GraphObject::setDirection(up);
                        break;
                    case left:
                        Actor::GraphObject::setDirection(right);
                        break;
                    case right:
                        Actor::GraphObject::setDirection(left);
                        break;
                }
            }
        }
    }
}

void ThiefBot::doSomething() {
    if (!Actor::getState()) return; // If bot is dead
    if (Actor::getWorld()->getTicks() % Actor::getTickFreq() == 0) { // If not rest tick
        additional();
        int currentX = Actor::GraphObject::getX();
        int currentY = Actor::GraphObject::getY();
        bool flag = false;
        if (m_goodie == ' ') { // If no goodie yet
            char v = Actor::getWorld()->StudentWorld::thiefBotScan(currentX, currentY);
            if (v != ' ') {
                m_goodie = v;
                Actor::getWorld()->GameWorld::playSound(SOUND_ROBOT_MUNCH);
                flag = true;
            }
        } 
        if (!flag) { // If did not take goodie
            bool flag2 = false;
            int dir = Actor::GraphObject::getDirection();
            int playerX = Actor::getWorld()->getAvatar()->GraphObject::getX();
            int playerY = Actor::getWorld()->getAvatar()->GraphObject::getY();
            if (m_dist < m_distBeforeTurn) { // If has not moved distance before turning times
                bool check = Actor::botPathfind(); // Check for obstructions and player
                if (check) { // If no obstructions or player
                    Actor::getWorld()->getObjects()[currentX][currentY] = Level::empty;
                    Actor::move();
                    int newX = Actor::GraphObject::getX();
                    int newY = Actor::GraphObject::getY();
                    Actor::getWorld()->getObjects()[newX][newY] = thiefbot;
                    m_dist++;
                    flag2 = true;
                }
            }
            if (!flag2) { // Did not got to move
                m_dist = 0;
                m_distBeforeTurn = rand() % 6 + 1;
                int maxD = 4;
                bool moved = false;
                
                int dTemp = rand() % maxD; maxD--; // Initial direction roll
                vector<int> directions;
                directions.push_back(90);
                directions.push_back(270);
                directions.push_back(180);
                directions.push_back(0);
                int d = directions[dTemp]; // Record initial direction roll into d

                while (!moved && maxD >= 1) { // While bot has not moved yet, and there is direction left
                    bool check = Actor::botPathfind(directions[dTemp]); // Check for obstructions and player
                    if (check) { // If no obstructions or player
                        Actor::getWorld()->getObjects()[currentX][currentY] = Level::empty;
                        Actor::GraphObject::setDirection(directions[dTemp]);
                        Actor::move();
                        int newX = Actor::GraphObject::getX();
                        int newY = Actor::GraphObject::getY();
                        Actor::getWorld()->getObjects()[newX][newY] = thiefbot;
                        m_dist++;
                        moved = true;
                    } else { // If cannot move
                        vector<int>::iterator it = directions.begin() + dTemp;
                        directions.erase(it); // Remove direction
                        dTemp = rand() % maxD; maxD--; // Redo direction roll
                    }
                }
                if (!moved) Actor::GraphObject::setDirection(d); // If obstructions on all four sides
            }
        }
    }
}

void Factory::doSomething() {
    int currentX = Actor::GraphObject::getX();
    int currentY = Actor::GraphObject::getY();
    if (Actor::getWorld()->getObjects()[currentX][currentY] == Level::empty) Actor::getWorld()->getObjects()[currentX][currentY] = Level::thiefbot_factory; // Refresh recordings of the factory's location

    int xLow = currentX - 3;
    if (xLow < 0) xLow = 0;
    int xHigh = currentX + 3;
    if (xHigh >= VIEW_WIDTH) xHigh = VIEW_WIDTH-1;
    int yLow = currentY - 3;
    if (yLow < 0) yLow = 0;
    int yHigh = currentY + 3;
    if (yHigh >= VIEW_HEIGHT) yHigh = VIEW_HEIGHT-1;
    int count = 0;
    for (int i = xLow; i <= xHigh; i++) {
        for (int j = yLow; j <= yHigh; j++) { // Count the number of thiefbots within square region
            char object = Actor::getWorld()->getObjects()[i][j];
            if (object == thiefbot || object == mean_thiefbot) count++;
        }
    }

    if (count < 3) {
        char object = Actor::getWorld()->getObjects()[currentX][currentY];
        if (object != thiefbot && object != mean_thiefbot) { // No bot on factory
            if (rand() % 50 == 0) { // 1 in 50 chance
                if (m_mean) {
                    Actor* bot = new MeanBot(currentX, currentY, Actor::getWorld());
                    Actor::getWorld()->addActor(bot);
                }
                else {
                    Actor* bot = new ThiefBot(IID_THIEFBOT, currentX, currentY, Actor::getWorld(), 5);
                    Actor::getWorld()->addActor(bot);
                }
                Actor::getWorld()->GameWorld::playSound(SOUND_ROBOT_BORN);
                Actor::getWorld()->getObjects()[currentX][currentY] = thiefbot;
            }
        }
    }
}

// __________________________
// Derived Class Take Damages
// __________________________

void Avatar::takeDmg(int dmg) {
    Actor::addHP(-1 * dmg); // Take damage
    if (Actor::getHP() > 0) Actor::getWorld()->GameWorld::playSound(SOUND_PLAYER_IMPACT);
    else if (Actor::getHP() <= 0) {
        Actor::setState(false);
        Actor::getWorld()->GameWorld::playSound(SOUND_PLAYER_DIE);
    }
}

void Marble::takeDmg(int dmg) {
    Actor::addHP(-1 * dmg); // Take damage
    if (Actor::getHP() <= 0) {
        Actor::setState(false); // Die
        int currentX = Actor::GraphObject::getX();
        int currentY = Actor::GraphObject::getY();
        Actor::getWorld()->getObjects()[currentX][currentY] = Level::empty;
    }
}

void RageBot::takeDmg(int dmg) {
    Actor::addHP(-1 * dmg); // Take damage
    if (Actor::getHP() > 0) Actor::getWorld()->GameWorld::playSound(SOUND_ROBOT_IMPACT);
    else if (Actor::getHP() <= 0) {
        Actor::setState(false); // Die
        int currentX = Actor::GraphObject::getX();
        int currentY = Actor::GraphObject::getY();
        Actor::getWorld()->getObjects()[currentX][currentY] = Level::empty;
        Actor::getWorld()->GameWorld::playSound(SOUND_ROBOT_DIE);
        Actor::getWorld()->GameWorld::increaseScore(100);
    }
}

void ThiefBot::takeDmg(int dmg) {
    Actor::addHP(-1 * dmg); // Take damage
    if (Actor::getHP() > 0) Actor::getWorld()->GameWorld::playSound(SOUND_ROBOT_IMPACT);
    else if (Actor::getHP() <= 0) {
        Actor::setState(false); // Die
        int currentX = Actor::GraphObject::getX();
        int currentY = Actor::GraphObject::getY();
        Actor::getWorld()->getObjects()[currentX][currentY] = Level::empty;
        Actor::getWorld()->GameWorld::playSound(SOUND_ROBOT_DIE);
        Actor::getWorld()->GameWorld::increaseScore(10);
        if (m_goodie != ' ') { // If the bot has a goodie
            if (m_goodie == Level::ammo) Actor::getWorld()->addActor(new Ammo(currentX, currentY, Actor::getWorld()));
            else if (m_goodie == Level::extra_life) Actor::getWorld()->addActor(new ExtraLife(currentX, currentY, Actor::getWorld()));
            else if (m_goodie == Level::restore_health) Actor::getWorld()->addActor(new Health(currentX, currentY, Actor::getWorld()));
        }
    }
}

// __________________________
// Derived Class Misc Functions
// __________________________

int Avatar::getAmmo() {return m_ammo;}

bool Marble::pushed(int dir) {
    if (!Actor::getState()) return false;
    int currentX = Actor::GraphObject::getX();
    int currentY = Actor::GraphObject::getY();
    bool check = false;
    switch(dir) {
        case up:
        {
            char item = Actor::getWorld()->getObjects()[currentX][currentY+1];
            if (item == Level::empty || item == Level::pit) check = true;
            break;
        }
        case down:
        {
            char item = Actor::getWorld()->getObjects()[currentX][currentY-1];
            if (item == Level::empty || item == Level::pit) check = true;
            break;
        }
        case left:
        {
            char item = Actor::getWorld()->getObjects()[currentX-1][currentY];
            if (item == Level::empty || item == Level::pit) check = true;
            break;
        }
        case right:
        {
            char item = Actor::getWorld()->getObjects()[currentX+1][currentY];
            if (item == Level::empty || item == Level::pit) check = true;
            break;
        }
    }
    if (check) { 
        Actor::getWorld()->getObjects()[currentX][currentY] = Level::empty;
        Actor::move(dir);
        int newX = Actor::GraphObject::getX();
        int newY = Actor::GraphObject::getY();
        Actor::getWorld()->getObjects()[newX][newY] = Level::marble;
    }
    return check;
}

void Avatar::addAmmo(int n) {
    m_ammo += n;
}

void Goodie::additional() {return;}

char Goodie::getVarient() {return m_varient;}

void Goodie::setVarient(char v) {m_varient = v;}

void ExtraLife::additional() {
    Actor::getWorld()->GameWorld::increaseScore(1000);
    Actor::getWorld()->GameWorld::incLives();
}

void Health::additional() {
    Actor::getWorld()->GameWorld::increaseScore(500);
    Actor::getWorld()->getAvatar()->setHP(20);
}

void Ammo::additional() {
    Actor::getWorld()->GameWorld::increaseScore(100);
    Actor::getWorld()->getAvatar()->addAmmo(20);
}

void ThiefBot::additional() {return;}

void MeanBot::additional() {Actor::botShoot();}









