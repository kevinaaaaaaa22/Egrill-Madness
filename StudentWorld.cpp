#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
using namespace std;

string setDisplayText(int score, int level, int lives, int health, int ammo, int bonus);

GameWorld* createStudentWorld(string assetPath) {
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath) {
    m_objects = new string[VIEW_HEIGHT];
    m_ticks = 0;
    m_egrillCount = 0;
    m_finishedLevel = false;
    m_bonus = 1000;
}

StudentWorld::~StudentWorld() { // Destructor is identical to cleanup
    cleanUp();
}

string* StudentWorld::getObjects() {return m_objects;}

Avatar* StudentWorld::getAvatar() {return m_avatar;}

int StudentWorld::egrillCount() {return m_egrillCount;}

int StudentWorld::getTicks() {return m_ticks;}

void StudentWorld::removeEgrill() {m_egrillCount--;}

void StudentWorld::finishLevel() {m_finishedLevel = true;}

void StudentWorld::addActor(Actor* a) {
    m_toAdd.push_back(a);
}

bool StudentWorld::moveMarble(double x, double y, int dir) {
    for (Actor* each : m_actors) // Iterate through all actors
        if (each->Actor::GraphObject::getX() == x) 
            if (each->Actor::GraphObject::getY() == y) // Check if actor on right location
                if (each->getIsMarble()) // Check if actor is marble
                    {if (each->pushed(dir)) return true; break;}
    return false;
}

bool StudentWorld::peaScan(double x, double y) {
    for (Actor* each : m_actors) // Iterate through all actors
        if (each->Actor::GraphObject::getX() == x) 
            if (each->Actor::GraphObject::getY() == y) // Check if actor on same location
                if (each->getBlockPeas()) {
                    if (each->getDamageable()) each->takeDmg(2); // Do damage
                    return true;
                }
    return false;
}

void StudentWorld::pitScan(double x, double y) {
    for (Actor* each : m_actors) // Iterate through all actors
        if (each->Actor::GraphObject::getX() == x)
            if (each->Actor::GraphObject::getY() == y) // Check if actor on same location
                if (each->getIsMarble()) {
                    each->Actor::setHP(0);
                    each->Actor::setState(false);
                }
}

char StudentWorld::thiefBotScan(double x, double y) {
    for (Actor* each : m_actors) // Check all actors
        if (each->Actor::GraphObject::getX() == x)
            if (each->Actor::GraphObject::getY() == y) // Check if actor on same location
                if (each->getIsGoodie()) { // If goodie
                    int temp = rand() % 10;
                    if (temp == 0) { // If 1 in 10 chance of picking up
                        each->setState(false);
                        return each->getVarient();
                    }
                }
    return ' ';
}

int StudentWorld::init() {
    m_objects = new string[VIEW_HEIGHT];
    m_ticks = 0;
    m_egrillCount = 0;
    m_finishedLevel = false;
    m_bonus = 1000;

    int currentLevel = GameWorld::getLevel();
    if (currentLevel > 99) return GWSTATUS_PLAYER_WON; // End game if finished level 99

    string levelPath;
    if (currentLevel < 10) levelPath = "level0" + to_string(currentLevel) + ".txt"; // Read in single digit levels
    else levelPath = "level" + to_string(currentLevel) + ".txt"; // Read in double digit levels
    Level lev(assetPath());

    Level::LoadResult result = lev.loadLevel(levelPath); 
    if (result == Level::load_fail_file_not_found) return GWSTATUS_PLAYER_WON; // End game if no more levels
    else if (result == Level::load_fail_bad_format) return GWSTATUS_LEVEL_ERROR; // Error if file has wrong format
    else if (result == Level::load_success) {
        for (int x = 0; x < VIEW_WIDTH; x++) {
            for (int y = 0; y < VIEW_HEIGHT; y++) {
                Level::MazeEntry current = lev.getContentsOf(x, y);
                switch(current) { // Record everything in level file into m_objects, create objects into m_actors
                    case Level::empty:
                        m_objects[x] += Level::empty;
                        break;
                    case Level::exit:
                        m_actors.push_back(new Exit(x, y, this));
                        m_objects[x] += Level::empty;
                        break;
                    case Level::player:
                        m_objects[x] += Level::empty;
                        m_avatar = new Avatar(x, y, this);
                        break;
                    case Level::horiz_ragebot:
                        m_actors.push_back(new RageBot(x, y, 0, this));
                        m_objects[x] += Level::horiz_ragebot;
                        break;
                    case Level::vert_ragebot:
                        m_actors.push_back(new RageBot(x, y, 270, this));
                        m_objects[x] += Level::vert_ragebot;
                        break;
                    case Level::thiefbot_factory:
                        m_actors.push_back(new Factory(x, y, this, false));
                        m_objects[x] += Level::thiefbot_factory;
                        break;
                    case Level::mean_thiefbot_factory:
                        m_actors.push_back(new Factory(x, y, this, true));
                        m_objects[x] += Level::mean_thiefbot_factory;
                        break;
                    case Level::ammo:
                        m_actors.push_back(new Ammo(x, y, this));
                        m_objects[x] += Level::empty;
                        break;
                    case Level::extra_life:
                        m_actors.push_back(new ExtraLife(x, y, this));
                        m_objects[x] += Level::empty;
                        break;
                    case Level::restore_health:
                        m_actors.push_back(new Health(x, y, this));
                        m_objects[x] += Level::empty;
                        break;
                    case Level::marble:
                        m_actors.push_back(new Marble(x, y, this));
                        m_objects[x] += Level::marble;
                        break;
                    case Level::pit:
                        m_actors.push_back(new Pit(x, y, this));
                        m_objects[x] += Level::pit;
                        break;
                    case Level::wall:
                        m_actors.push_back(new Wall(x, y));
                        m_objects[x] += Level::wall;
                        break;
                    case Level::thiefbot:
                        break;
                    case Level::mean_thiefbot:
                        break;
                    case Level::pea:
                        break;
                    case Level::regular_egrill:
                        m_actors.push_back(new Egrill(x, y, this, REGULAR_EGRILL, IID_REGULAR_EGRILL));
                        m_objects[x] += Level::empty;
                        m_egrillCount++;
                        break;
                    case Level::pokylane:
                        m_actors.push_back(new Egrill(x, y, this, POKYLANE, IID_POKYLANE));
                        m_objects[x] += Level::empty;
                        m_egrillCount++;
                        break;
                }
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    GameWorld::setGameStatText(setDisplayText(GameWorld::getScore(), GameWorld::getLevel(), GameWorld::getLives(), m_avatar->getHP(), m_avatar->getAmmo(), m_bonus)); // Update display
    
    m_avatar->doSomething(); // Avatar does something

    for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) { // Iterate through every actor
        Actor* each = *it;
        each->doSomething(); // Each actor does something
        if (!m_avatar->getState()) { // If player died
            GameWorld::decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (m_finishedLevel) { // If level finished
            GameWorld::increaseScore(m_bonus);
            return GWSTATUS_FINISHED_LEVEL;
        }
        if (!each->getState()) { // If actor died
            delete each;
            it = m_actors.erase(it);
            it--;
        }
    }

    while (!m_toAdd.empty()) {
        m_actors.push_back(m_toAdd.back());
        m_toAdd.pop_back();
    }

    m_ticks++; // Increase tick
    if (m_bonus > 0) m_bonus--; // Decrease bonus if bonus not 0
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    if (m_avatar != nullptr) { // Delete player object
        delete m_avatar;
        m_avatar = nullptr;
    }
    while (!m_actors.empty()) { // Delete every actor object
        if (m_actors.back() != nullptr) delete m_actors.back();
        m_actors.pop_back();
    }
    if (m_objects != nullptr) { // Delete array of game map
        delete [] m_objects;
        m_objects = nullptr;
    }
}

string setDisplayText(int score, int level, int lives, int health, int ammo, int bonus) { // Returns text to display
    string s = to_string(score);
    int sSize = s.size();
    for (int i = 0; i < 7 - sSize; i++) s = "0" + s;
    string l1 = to_string(level);
    if (l1.size() == 1) l1 = "0" + l1;
    string l2 = to_string(lives);
    if (l2.size() == 1) l2 = " " + l2;
    int healthP = health/20.0*100;
    string h = to_string(healthP);
    if (h.size() == 1) h = "  " + h;
    else if (h.size() == 2) h = " " + h;
    h += "%";
    string a = to_string(ammo);
    if (a.size() == 1) a = "  " + a;
    else if (a.size() == 2) a = " " + a;
    string b = to_string(bonus);
    int bSize = b.size();
    for (int i = 0; i < 4 - bSize; i++) b = " " + b;
    return "Score: " + s + "  Level: " + l1 + "  Lives: " + l2 + "  Health: " + h + "  Ammo: " + a + "  Bonus: " + b;
}







