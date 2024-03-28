#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

class Actor;
class Avatar;
class Wall;
class Pea;
class Pit;
class Crystal;
class Exit;
class Goodie;
class ExtraLife;
class Health;
class Ammo;
class RageBot;
class ThiefBot;
class MeanBot;
class Factory;

#include "GameWorld.h"
#include "Level.h"
#include "GraphObject.h"
#include <string>
using namespace std;

class StudentWorld : public GameWorld {
  public:
    StudentWorld(string assetPath);
    ~StudentWorld();

    // Accessors
    string* getObjects(); // Returns array of the game map
    Avatar* getAvatar(); // Returns player
    int getTicks(); // Returns number of ticks elapsed
    int egrillCount(); // Returns number of egrills left

    // Mutators
    void addActor(Actor* a); // Add new actor
    void removeEgrill(); // Removes an egrill from the count
    void finishLevel(); // Set finishedLevel to true
    bool moveMarble(double x, double y, int dir); // Move a marble when pushed
    bool peaScan(double x, double y); // For a pea to check for collision
    void pitScan(double x, double y); // For a pit to check for marble
    char thiefBotScan(double x, double y); // For a thief bot to check for goodies
    
    // Operations
    virtual int init(); // Initialization
    virtual int move(); // Take action during each tick
    virtual void cleanUp(); // Delete dynamically allocated objects
  private:
    Avatar* m_avatar; // Pointer to player
    vector<Actor*> m_actors; // Vector of actors
    vector<Actor*> m_toAdd; // Vector of actors to add during each tick
    string* m_objects; // Array of the game map, marks all objects that are obstructions to player
    int m_egrillCount; // Records number of egrills left
    int m_ticks; // Number of ticks elapsed
    bool m_finishedLevel; // Tracks player's status completing the level
    int m_bonus; // Bonus points
};

#endif // STUDENTWORLD_H_



