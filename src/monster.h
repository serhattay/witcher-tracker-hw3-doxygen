#ifndef MONSTER_H
#define MONSTER_H

/**
 * @file monster.h
 * @brief Declaration of the @c Monster class.
 */

#include <string>
#include <vector>

using namespace std;

/**
 * @class Monster
 * @brief This class acts as a blueprint for the Monster object that stores its name 
 * and the lists of signs and potions that are effective against it.
 */
class Monster {
private:
    /// Data fields are declared private in order to encapsulate the data.
    string name;
    vector<string> effectiveSigns;
    vector<string> effectivePotions;
public:
    /**
     * @brief Construct a new Monster.
     * @param name Monster's name.
     */
    Monster(const string& name);

    /**
     * @brief Getter function for the vector that stores effective signs against the monster
    */
    const vector<string>& getEffectiveSigns();

    /**
     * @brief Getter function for the vector that stores effective potinos against the monster
    */  
    const vector<string>& getEffectivePotions();

    /**
     * @brief Adds a sign name to the effective signs list
     * @param name Name of the effective sign
    */
    void addEffectiveSign(const string& name);

    /**
     * @brief Adds a potion name to the effective potions list
     * @param name Name of the effective potion
    */ 
    void addEffectivePotion(const string& name);
};

#endif