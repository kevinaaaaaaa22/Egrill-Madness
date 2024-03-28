#ifndef ACTOR_H_
#define ACTOR_H_

class StudentWorld;
const char thiefbot = 's';
const char mean_thiefbot = 'm';
const char pea = 'p';

#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include "Level.h"

class Actor: public GraphObject {
    public:
        Actor(int imageID, double startX, double startY, int dir = none, StudentWorld* world = nullptr);
        virtual ~Actor();

        // Accessors
        int getTickFreq(); // Get tick frequency
        bool getState(); // Get state
        bool getIsMarble(); // Get whether object is marble
        bool getBlockPeas(); // Get ability to block pea
        bool getDamageable(); // Get ability to take damage from pea
        bool getIsGoodie(); // Get if object is goodie
        StudentWorld* getWorld(); // Get pointer to game world
        double getHP(); // Get hitpoint

        // Mutators
        void setHP(double n); // Set hitpoint
        void addHP(double n); // Add or subtract hitpoint
        void setState(bool s); // Set state
        void setBlockPeas(bool b); // Set ability to block pea
        void setDamageable(bool b); // Set ability to take damage from pea
        void setIsMarble(bool b); // Set if object is marble
        void setIsGoodie(bool b); // Set if object is goodie
        void setTickFreq(int n); // Set tick frequency
        
        // Utilities
        bool onPlayer(); // Check if object is at same location as player
        void move(int dir = -69); // Move in specified direction by 1 block, if no direction given, use GraphObject::getDirection()
        bool botShoot(); // Fires peas, only used for bots
        bool botPathfind(int dir = -69); // Checks for obstructions and players, only used for bot movements
        
        // Virtual
        virtual void doSomething(); // Do something
        virtual void takeDmg(int dmg); // Take damage
        virtual bool pushed(int dir); // Be pushed, only used for marble
        virtual char getVarient(); // Get varient of goodie, only used for goodie objects
    private:
        double m_hp; // Hitpoint, -1 by default
        bool m_state; // State, true is alive, all objects alive by default
        bool m_blockPeas; // Whether object can stop pea or not
        bool m_damageable; // Whether object can take damage from pea or not
        bool m_isMarble; // Is object marble?
        bool m_isGoodie; // Is object goodie?
        int m_tickFreq; // How often to do thing
        StudentWorld* m_world; // Pointer to the game world
};

class Wall: public Actor {
    public:
        Wall(double startX, double startY);
};

class Avatar: public Actor {
    public:
        Avatar(double startX, double startY, StudentWorld* world);
        void doSomething();
        void takeDmg(int dmg);
        void addAmmo(int n); // Add ammo
        int getAmmo(); // Returns ammo count
    private:
        int m_ammo; // Ammo count for player
};

class Pea: public Actor {
    public:
        Pea(double startX, double startY, int dir, StudentWorld* world);
        void doSomething();
        void doSomethingHelper();
    private:
        bool m_fresh; // Records if pea was created on this tick
};

class Marble: public Actor {
    public:
        Marble(double startX, double startY, StudentWorld* world);
        void takeDmg(int dmg);
        bool pushed(int dir);
};

class Pit: public Actor {
    public:
        Pit(double startX, double startY, StudentWorld* world);
        void doSomething();
};

class Crystal: public Actor {
    public:
        Crystal(double startX, double startY, StudentWorld* world);
        void doSomething();
};

class Exit: public Actor {
    public:
        Exit(double startX, double startY, StudentWorld* world);
        void doSomething();
};

class Goodie: public Actor {
    public:
        Goodie(int imageID, double startX, double startY, StudentWorld* world);
        void doSomething();
        virtual void additional(); // Extra functionalities, does nothing by default
        char getVarient();
        void setVarient(char v);
    private:
        char m_varient; // Records type of goodie
};

class ExtraLife: public Goodie {
    public:
        ExtraLife(double startX, double startY, StudentWorld* world);
        void additional(); // Extra functionalities
};

class Health: public Goodie {
    public:
        Health(double startX, double startY, StudentWorld* world);
        void additional(); // Extra functionalities
};

class Ammo: public Goodie {
    public:
        Ammo(double startX, double startY, StudentWorld* world);
        void additional(); // Extra functionalities
};

class RageBot: public Actor {
    public:
        RageBot(double startX, double startY, int dir, StudentWorld* world);
        void doSomething();
        void takeDmg(int dmg);
};

class ThiefBot: public Actor {
    public:
        ThiefBot(int imageID, double startX, double startY, StudentWorld* world, int HP);
        void doSomething();
        virtual void additional(); // Extra functionalities, does nothing by default
        void takeDmg(int dmg);
    private:
        int m_distBeforeTurn; // Distance before turning
        int m_dist; // Current distance moved
        char m_goodie; // The goodie it is carrying
};

class MeanBot: public ThiefBot {
    public:
        MeanBot(double startX, double startY, StudentWorld* world);
        void additional(); // Extra functionalities
};

class Factory: public Actor {
    public:
        Factory(double startX, double startY, StudentWorld* world, bool mean);
        void doSomething();
    private:
        bool m_mean; // Produce mean bots or not
};

#endif // ACTOR_H_











