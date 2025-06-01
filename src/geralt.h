#ifndef GERALT_H
#define GERALT_H

/**
 * @file geralt.h
 * @brief Defines the methods and the data fields of the inventory.
 *
 * The class stores four static maps that model Geralt’s inventory, bestiary
 * and trophies, and offers high-level actions that correspond to the grammar rules.
 *
 * Every method accepts a tokenized command line and either mutates the
 * shared state or prints the answer required by the specification.
 */

#include <map>
#include <vector>
#include <string>
#include <memory>

#include "ingredient.h"
#include "potion.h"
#include "sign.h"
#include "monster.h"
#include "trophy.h"
#include "token.h"

/**
 * @class Geralt
 * @brief Class that represents the Witcher’s knowledge and inventory.
 *
 * All members are static; the class is never instantiated.  
 * Users interact with the state via the high-level action/query functions.
 */
class Geralt {
private:
    /// These are the data fields that stores the ingredient, potion, monster and trophy data.
    static std::map<std::string, std::shared_ptr<Ingredient>> ingredients;
    static std::map<string, shared_ptr<Potion>> potions;
    static std::map<string, shared_ptr<Monster>> monsters;
    static std::map<string, shared_ptr<Trophy>> trophies;
public:
    /// Getter functions return the private data fields which are encapsulated and declared private.
    static std::map<string, shared_ptr<Ingredient>>& getIngredients();
    static std::map<string, shared_ptr<Potion>>& getPotions();
    static std::map<string, shared_ptr<Monster>>& getMonsters();
    static std::map<string, shared_ptr<Trophy>>& getTrophies();
    
    /// Functions that execute the corresponding action
    static void loot(const std::vector<Token>& tokenList);
    static void trade(const std::vector<Token>& tokenList);
    static void brew(const std::vector<Token>& tokenList);
    static void learnSign(const std::vector<Token>& tokenList);
    static void learnPotion(const std::vector<Token>& tokenList);
    static void learnFormula(const std::vector<Token>& tokenList);
    static void encounter(const std::vector<Token>& tokenList);
    static void querySpecificIngredient(const std::vector<Token>& tokenList);
    static void querySpecificPotion(const std::vector<Token>& tokenList);
    static void querySpecificTrophy(const std::vector<Token>& tokenList);
    static void queryAllIngredients(const std::vector<Token>& tokenList);
    static void queryAllPotions(const std::vector<Token>& tokenList);
    static void queryAllTrophies(const std::vector<Token>& tokenList);
    static void queryEffectiveness(const std::vector<Token>& tokenList);
    static void queryFormula(const std::vector<Token>& tokenList);
};

#endif