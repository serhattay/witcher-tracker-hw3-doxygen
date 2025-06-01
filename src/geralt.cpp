/**
 * @file geralt.cpp
 * @brief Implementation of Geralt’s inventory, bestiary and alchemy‑tracking logic for
 *        CMPE 230 Assignment 3 “Witcher Tracker”.
 *
 * This file contains all method definitions that mutate or query global game state:
 * static accessors for shared maps (`ingredients`, `potions`, `monsters`, `trophies`)
 * inventory actions — loot, trade, brew
 * knowledge acquisition — learnSign, learnPotion, learnFormula
 * encounter resolution
 * query helpers for inventory, bestiary and alchemy
 *
 * Each public method directly corresponds to a grammar rule in the assignment
 * specification and produces exactly the console output prescribed by the spec.
 *
 * @author  Akin Tuna Sakalli
 * @date    June 1, 2025
 */

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <memory>
#include <algorithm>
#include <functional>

#include "geralt.h"
#include "tokenizer.h"
#include "token.h"

using namespace std;

map<string, shared_ptr<Ingredient>> Geralt::ingredients;
map<string, shared_ptr<Potion>> Geralt::potions;
map<string, shared_ptr<Monster>> Geralt::monsters;
map<string, shared_ptr<Trophy>> Geralt::trophies;

/**
 * @brief Returns a modifiable reference to the global ingredients map.
 *
 * The returned reference allows callers to read or mutate the shared
 * container that represents Geralt’s current ingredient inventory.
 *
 * @return Reference to the underlying `std::map`.
 */
map<string, shared_ptr<Ingredient>>& Geralt::getIngredients() {
    return ingredients;
}

/**
 * @brief Returns a modifiable reference to the global potions map.
 *
 * The returned reference allows callers to read or mutate the shared
 * container that represents Geralt’s current potion inventory.
 *
 * @return Reference to the underlying `std::map`.
 */
map<string, shared_ptr<Potion>>& Geralt::getPotions() {
    return potions;
}

/**
 * @brief Returns a modifiable reference to the global monsters map.
 *
 * The returned reference allows callers to read or mutate the shared
 * container that represents Geralt’s current bestiary knowledge.
 *
 * @return Reference to the underlying `std::map`.
 */
map<string, shared_ptr<Monster>>& Geralt::getMonsters() {
    return monsters;
}

/**
 * @brief Returns a modifiable reference to the global trophies map.
 *
 * The returned reference allows callers to read or mutate the shared
 * container that represents Geralt’s current trophy inventory.
 *
 * @return Reference to the underlying `std::map`.
 */
map<string, shared_ptr<Trophy>>& Geralt::getTrophies() {
    return trophies;
}

/**
 * @brief Handles the loot action.
 * 
 * Increases the quantity of each looted ingredient by the specified amount.
 *
 * Outputs: “Alchemy ingredients obtained”
 *
 * @param tokenList Tokenized user input line.
 */
void Geralt::loot(const vector<Token>& tokenList) {
    int i = 2;
    auto& ingredients = Geralt::getIngredients();

    while (i+1 < tokenList.size()) {
        int ingredientQuantity = stoi(tokenList[i].getContent());
        string ingredientName = tokenList[i+1].getContent();
        
        // The corresponding ingredient is already present in the map, increase its quantity
        if (ingredients.count(ingredientName) > 0) {
            ingredients[ingredientName]->increaseQuantity(ingredientQuantity);
        }
        // It is the first time that ingredient is encountered, add it to the map
        else {
            shared_ptr<Ingredient> newIngredient = make_shared<Ingredient>(ingredientName, ingredientQuantity);
            ingredients.emplace(ingredientName, newIngredient);
        }
    // If this ingredient is the last ingredient in the input sentence, break the iteration
    if (i+2 >= tokenList.size()) {
        break;
    }
    // If there are still ingredients remaining in the input sentence, keep iterating to loot those as well
    i += 3;
    }
    // Print the output
    cout << "Alchemy ingredients obtained" << endl;
}

/**
 * @brief Processes a **trade** action.
 *
 * Verifies that Geralt owns every trophy requested in sufficient quantity.
 * If so, decrements trophies and increases ingredients; otherwise leaves state unchanged.
 *
 * Outputs:  
 * “Trade successful” on success  
 * “Not enough trophies” on failure
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::trade(const vector<Token>& tokenList) {
    int i = 2;
    bool neededTrophiesExist = true;
    auto& trophies = Geralt::getTrophies();
    auto& ingredients = Geralt::getIngredients();

    // Iterate through the input sentence if there are still quantities and trophies that need to be processed
    while (tokenList[i].getType() == TOKEN_QUANTITY && tokenList[i+1].getType() == TOKEN_WORD) {
        int neededQuantity = stoi(tokenList[i].getContent());
        string trophyName = tokenList[i+1].getContent();

        bool enoughTrophies = true;

        // Trophy is in the trophy list
        if (trophies.count(trophyName) > 0) {
            // If the trophy quantity is insufficient, there are not enough trophies
            if (trophies[trophyName]->getQuantity() < neededQuantity) {
                enoughTrophies = false;
            }
        }
        // If the trophy is not in the trophy list, there are not any trophies
        else {
            enoughTrophies = false;
        }

        if (!enoughTrophies) {
            neededTrophiesExist = false;
            break;
        }
        
        // If there are not any trophies that need to be processed, get out of the loop
        if (tokenList[i+2].getType() == TOKEN_TROPHY) {
            break;
        }
        // If there are still more trophies, keep iterating through the input sentence
        else {
            i += 3;
        }
    }
    
    // There are enough trophies
    if (neededTrophiesExist) {
        i = 2;
        cout << "Trade successful" << endl;

        // The trophy quantities are decreased by an amount equal to the quantity that is needed
        while (tokenList[i].getType() == TOKEN_QUANTITY && tokenList[i+1].getType() == TOKEN_WORD) {
            int neededQuantity = stoi(tokenList[i].getContent());
            string trophyName = tokenList[i+1].getContent();
    
            trophies[trophyName]->decreaseQuantity(neededQuantity);

            // If there are not any trophies that need to be processed, get out of the loop
            if (tokenList[i+2].getType() == TOKEN_TROPHY) {
                break;
            }
            // If there are still more trophies, keep iterating through the input sentence
            else {
                i += 3;
            }
        }

        // Ingredients are processed and incremented by amount that is equal to the given quantity
        i += 4;
        while (i + 1 < tokenList.size()) {
            int quantity = stoi(tokenList[i].getContent());
            string ingredientName = tokenList[i+1].getContent();
            
            // If that ingredient already exists, increase its quantity
            if (ingredients.count(ingredientName) > 0) {
                ingredients[ingredientName]->increaseQuantity(quantity);
            }
            // If the ingredient does not exist, it is added to the list
            else {
                shared_ptr<Ingredient> newIngredient = make_shared<Ingredient>(ingredientName, quantity);
                ingredients.emplace(ingredientName, newIngredient);
            }


            // If there are not any ingredients that need to be processed, get out of the loop
            if (i+2 >= tokenList.size()) {
                break;
            }
            // If there are still more ingredients, keep iterating through the input sentence
            else {
                i += 3;
            }
        }

    }
    // There are not enough trophies
    else {
        cout << "Not enough trophies" << endl;
    }
}

/**
 * @brief Handles the **brew** action.
 *
 * Checks if the formula for the potion is known. If not, prints “No formula for <potion>”.
 * If the formula is known, checks if all required ingredients are present in sufficient quantity.
 * On success, decrements ingredient quantities and increases potion count, printing
 * “Alchemy item created: <potion>”. If ingredients are insufficient, prints “Not enough ingredients”.
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::brew(const vector<Token>& tokenList){
    string potionName = tokenList[2].getContent();
    auto& potions = Geralt::getPotions();
    auto& ingredients = Geralt::getIngredients();

    // Potion is in the potions list
    if (potions.count(potionName) > 0) {

        // If the formula is defined, then check if there are enough ingredients
        if (potions[potionName]->isFormulaDefined()) {
            bool enoughIngredients = true;

            // Traverse the formulae list in order to check if all ingredients are present with enough quantity
            for (const pair<string, int>& formulaIngredient : potions[potionName]->getFormula()) {
                // Check if the needed ingredient exists in the ingredients list, if it does not exist, mark there are not enough ingredients
                if (ingredients.count(formulaIngredient.first) == 0) {
                    enoughIngredients = false;
                    break;
                }
                // If the ingredient exists, check if its quantity is sufficient, if it is insufficient, mark there are not enough ingredients
                if (ingredients[formulaIngredient.first]->getQuantity() < formulaIngredient.second) {
                    enoughIngredients = false;
                    break;
                }
            }

            // If there are enough ingredients, decrease each of their quantity by the specified amount, and increase the potion's quantity
            if (enoughIngredients) {
                for (const pair<string, int>& formulaIngredient : potions[potionName]->getFormula())  {
                    ingredients[formulaIngredient.first]->decreaseQuantity(formulaIngredient.second);
                }
                potions[potionName]->increaseQuantity(1);

                cout << "Alchemy item created: " <<  potionName << endl;
            }
            else {
            cout << "Not enough ingredients" << endl;
            }
        }
        // Potion formula is not known
        else {
            cout << "No formula for " << potionName << endl;
        }
    }
    // Potion formula is not known
    else {
        cout << "No formula for " << potionName << endl;
    }
}

/**
 * @brief Registers a new sign as effective against a monster in the bestiary.
 *
 * Updates the bestiary knowledge by associating the specified sign with the monster.
 * If the monster is new, adds the monster to the bestiary; otherwise, updates its effectiveness list.
 * Prints appropriate output depending on whether knowledge is new or already known.
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::learnSign(const vector<Token>& tokenList) {
    string signName = tokenList[2].getContent();
    string monsterName = tokenList[7].getContent();
    auto& monsters = Geralt::getMonsters();

    // If it is the first time monster is mentioned, it is added to the list and effective sign is added
    if (monsters.count(monsterName) == 0) {
        shared_ptr<Monster> newMonster = make_shared<Monster>(monsterName);
        monsters.emplace(monsterName, newMonster);

        vector<string> effectiveSigns = monsters[monsterName]->getEffectiveSigns();

        // Add the sign if it is not already in the list
        if (std::find(effectiveSigns.begin(), effectiveSigns.end(), signName) == effectiveSigns.end()) {
            monsters[monsterName]->addEffectiveSign(signName);
        }

        cout << "New bestiary entry added: " << monsterName << endl;
    }
    // If the monster is already in the list, add the effective sign
    else {
        vector<string> effectiveSigns = monsters[monsterName]->getEffectiveSigns();

        // Add the sign if it is not already in the list
        if (std::find(effectiveSigns.begin(), effectiveSigns.end(), signName) == effectiveSigns.end()) {
            monsters[monsterName]->addEffectiveSign(signName);
            cout << "Bestiary entry updated: " << monsterName << endl;
        }
        // Sign is already in the list
        else {
            cout << "Already known effectiveness" << endl;
        }
    }
}

/**
 * @brief Registers a new potion as effective against a monster in the bestiary and potion inventory.
 *
 * Updates bestiary and potion knowledge. Adds the monster or potion if new, and associates
 * the potion as effective against the monster. Prints output according to whether knowledge is new or already known.
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::learnPotion(const vector<Token>& tokenList) {
    string potionName = tokenList[2].getContent();
    string monsterName = tokenList[7].getContent();
    auto& monsters = Geralt::getMonsters();
    auto& potions = Geralt::getPotions();

    // If it is the first time monster is mentioned, it is added to the list and effective potion is added
    if (monsters.count(monsterName) == 0) {
        shared_ptr<Monster> newMonster = make_shared<Monster>(monsterName);
        monsters.emplace(monsterName, newMonster);

        vector<string> effectivePotions = monsters[monsterName]->getEffectivePotions();

        // Add the potion if it is not already in the list
        if (std::find(effectivePotions.begin(), effectivePotions.end(), potionName) == effectivePotions.end()) {
            monsters[monsterName]->addEffectivePotion(potionName);
        }
        // If this is the first time potion is encountered, it is added to the potions list
        if (potions.count(potionName) == 0) {
            shared_ptr<Potion> newPotion = make_shared<Potion>(potionName);
            potions.emplace(potionName, newPotion);
        }

        cout << "New bestiary entry added: " << monsterName << endl;
    }
    // If the monster is already in the list, effective potion is added
    else {
        vector<string> effectivePotions = monsters[monsterName]->getEffectivePotions();

        // If this is the first time potion is encountered, it is added to the potion list
        if (potions.count(potionName) == 0) {
            shared_ptr<Potion> newPotion = make_shared<Potion>(potionName);
            potions.emplace(potionName, newPotion);
        }

        // Add the potion if it is not already in the effective potions list
        if (std::find(effectivePotions.begin(), effectivePotions.end(), potionName) == effectivePotions.end()) {
            monsters[monsterName]->addEffectivePotion(potionName);
            cout << "Bestiary entry updated: " << monsterName << endl;
        }
        // Potion is already in the list
        else {
            cout << "Already known effectiveness" << endl;
        }
    }
}

/**
 * @brief Learns and stores a new alchemy formula for a potion.
 *
 * Parses `<quantity> <ingredient>` pairs and defines the potion's formula if not already known.
 * Adds new ingredients to inventory if needed. Prints output indicating new formula or if already known.
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::learnFormula(const vector<Token>& tokenList) {
    string potionName = tokenList[2].getContent();
    auto& potions = Geralt::getPotions();
    auto& ingredients = Geralt::getIngredients();

    // If this is the first time potion is encountered, it is added to the potion list
    if (potions.count(potionName) == 0) {
        shared_ptr<Potion> newPotion = make_shared<Potion>(potionName);
        potions.emplace(potionName, newPotion);
    }
        
    // If the formula is already defined, do not update the formula
    if (potions[potionName]->isFormulaDefined()) {
        cout << "Already known formula" << endl; 
    }
    // If the formula is not already known, the formula is added to the potion
    else {
        int i = 6;
        while (i+1 < tokenList.size()) {
            int quantity = stoi(tokenList[i].getContent());
            string ingredientName = tokenList[i+1].getContent();

            // If this is the first time that ingredient is encountered, it is added to the ingredient list
            if (ingredients.count(ingredientName) == 0) {
                shared_ptr<Ingredient> newIngredient = make_shared<Ingredient>(ingredientName, 0);
                ingredients.emplace(ingredientName, newIngredient);
            }

            potions[potionName]->addToFormula(quantity, ingredientName);

            // If there are not any ingredients that need to be added to the formula, get out of the loop
            if (i+2 >= tokenList.size()) {
                break;
            }
            // If there are still more ingredients, keep iterating through the input sentence
            i += 3;
        }
        potions[potionName]->defineFormula();

        cout << "New alchemy formula obtained: " << potionName << endl;
    }
}

/**
 * @brief Resolves a monster encounter.
 *
 * Determines whether Geralt can defeat the monster based on known effective
 * signs/potions and available potion quantities, updates trophies and potion
 * counts accordingly, and prints the outcome line defined by the spec.
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::encounter(const vector<Token>& tokenList) {
    string monsterName = tokenList[3].getContent();
    auto& monsters = Geralt::getMonsters();
    auto& potions = Geralt::getPotions();
    auto& trophies = Geralt::getTrophies();
    
    // If this is the first time this monster's name is encountered, 
    // there are not any effective signs or potions, so Geralt is defeated
    if (monsters.count(monsterName) == 0) {
        cout << "Geralt is unprepared and barely escapes with his life" << endl;
    }
    // Monster is encountered before
    else {
        bool can_defeat = false;
        // If there is an effective sign, Geralt can defeat the monster
        if (monsters[monsterName]->getEffectiveSigns().size() > 0) {
            can_defeat = true;
        }
        // If there is an effective potion, and its quantity is greater than 0, Geralt can defeat the monster
        if (monsters[monsterName]->getEffectivePotions().size() > 0) {
            for (string potionName : monsters[monsterName]->getEffectivePotions()) {
                if (potions[potionName]->getQuantity() > 0) {
                    can_defeat = true;
                    break;
                }
            }
        }

        if (can_defeat) {
            cout << "Geralt defeats " << monsterName << endl;

            // Geralt consumes each potion he has against the monster
            for (string potionName : monsters[monsterName]->getEffectivePotions()) {
                // If that potion is present, consume it
                if (potions.count(potionName) > 0) {
                    if (potions[potionName]->getQuantity() >= 1) {
                        potions[potionName]->decreaseQuantity(1);
                    }
                }
            }
            // Geralt earns a trophy
            // If the trophy is earned before, its quantity is incremented
            if (trophies.count(monsterName) > 0) {
                trophies[monsterName]->increaseQuantity(1);
            }
            // If it is the first time that Geralt earns this trophy, it is added to the trophy list, and its quantity is incremented
            else {
                shared_ptr<Trophy> newTrophy = make_shared<Trophy>(monsterName);
                trophies.emplace(monsterName, newTrophy);
                trophies[monsterName]->increaseQuantity(1);
            }
        }
        // If Geralt does not have enough knowledge or resources, he is defeated
        else {
            cout << "Geralt is unprepared and barely escapes with his life" << endl;
        }
    }
}

/**
 * @brief Prints the quantity of the given ingredient.
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::querySpecificIngredient(const vector<Token>& tokenList) {
    string ingredientName = tokenList[2].getContent();
    auto& ingredients = Geralt::getIngredients();
    
    // Ingredient is not in the inventory
    if (ingredients.count(ingredientName) == 0) {
        cout << 0 << endl;
    }
    // Ingredient is in the inventory, print its quantity
    else {
        int quantity = ingredients[ingredientName]->getQuantity();
        cout << quantity << endl;
    }
}

/**
 * @brief Prints the quantity of the given potion.
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::querySpecificPotion(const vector<Token>& tokenList) {
    string potionName = tokenList[2].getContent();
    auto& potions = Geralt::getPotions();

    // Potion is not in the inventory
    if (potions.count(potionName) == 0) {
        cout << 0 << endl;
    }
    // Potion is in the inventory, print its quantity
    else {
        int quantity = potions[potionName]->getQuantity();
        cout << quantity << endl;
    }
}

/**
 * @brief Prints the quantity of the given trophy.
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::querySpecificTrophy(const vector<Token>& tokenList) {
    string trophyName = tokenList[2].getContent();
    auto& trophies = Geralt::getTrophies();

    // Trophy is not in the inventory
    if (trophies.count(trophyName) == 0) {
        cout << 0 << endl;
    }
    // Trophy is in the inventory, print its quantity
    else {
        int quantity = trophies[trophyName]->getQuantity();
        cout << quantity << endl;
    }
}

/**
 * @brief Prints all the ingredients and their quantities.
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::queryAllIngredients(const vector<Token>& tokenList) {
    auto& ingredients = Geralt::getIngredients();

    // There are no ingredients in the inventory
    if (ingredients.size() == 0) {
        cout << "None" << endl;
    }
    else {
        bool first = true;
        bool ingredientExists = false;
        // For each element in the ingredients map, print out the ingredient name with its quantity
        for (const auto& ingredientPair : ingredients) {
            string ingredientName = ingredientPair.first;
            int quantity = ingredientPair.second->getQuantity();
            // Ingredient is printed with its quantity if its quantity is greater than 0
            if (quantity > 0) {
                if (!first) {
                    cout << ", ";
                }
                first = false;
                ingredientExists = true;
                cout << quantity << " " << ingredientName;
            }
        }
        // Go to new line
        if (ingredientExists) {
            cout << endl;
        }
        // If none of the ingredients have a quantity greater than 0, print none
        else {
            cout << "None" << endl;
        }
    }
}

/**
 * @brief Prints all the potions and their quantities.
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::queryAllPotions(const vector<Token>& tokenList) {
    auto& potions = Geralt::getPotions();

    // There are no potions in the inventory
    if (potions.size() == 0) {
        cout << "None" << endl;
    }
    else {
        bool first = true;
        bool potionExists = false;
        // For each element in the potions map, print out the potion name with its quantity
        for (const auto& potionPair : potions) {
            string potionName = potionPair.first;
            int quantity = potionPair.second->getQuantity();
            // Ingredient is printed with its quantity if its quantity is greater than 0
            if (quantity > 0) {
                if (!first) {
                    cout << ", ";
                }
                first = false;
                potionExists = true;
                cout << quantity << " " << potionName;
            }
        }
        // Go to new line
        if (potionExists) {
            cout << endl;
        }
        // If none of the potions have a quantity greater than 0, print none
        else {
            cout << "None" << endl;
        }
    }
}

/**
 * @brief Prints all the trophies and their quantities.
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::queryAllTrophies(const vector<Token>& tokenList) {
    auto& trophies = Geralt::getTrophies();

    // There are no trophies in the inventory
    if (trophies.size() == 0) {
        cout << "None" << endl;
    }
    else {
        bool first = true;
        bool trophyExists = false;
        // For each element in the trophies map, print out the trophy name with its quantity
        for (const auto& trophyPair : trophies) {
            string trophyName = trophyPair.first;
            int quantity = trophyPair.second->getQuantity();
            // Ingredient is printed with its quantity if its quantity is greater than 0
            if (quantity > 0) {
                if (!first) {
                    cout << ", ";
                }
                first = false;
                trophyExists = true;
                cout << quantity << " " << trophyName;
            }
        }
        // Go to new line
        if (trophyExists) {
            cout << endl;
        }
        // If none of the trophies have a quantity greater than 0, print none
        else {
            cout << "None" << endl;
        }
    }
}

/**
 * @brief Lists all known effective signs and potions against a monster.
 *
 * Merges two vectors, sorts, and prints them comma‑separated,
 * or prints “No knowledge of <monster>” if none exist.
 *
 * @param tokenList Tokenized input line.
 */
void Geralt::queryEffectiveness(const vector<Token>& tokenList) {
    auto& monsters = Geralt::getMonsters();

    string monsterName = tokenList[4].getContent();

    // If the monster is present in the monsters map, print its effective signs and potions
    if (monsters.count(monsterName) > 0) {
        vector<string> effectiveSigns = monsters[monsterName]->getEffectiveSigns();
        vector<string> effectivePotions = monsters[monsterName]->getEffectivePotions();

        // Merge signs and potions in order to sort and print them in descending order
        vector<string> mergedVector;

        // Add the sign names
        for (string signName : effectiveSigns) {
            mergedVector.push_back(signName);
        }
        // Add the potion names
        for (string potionName : effectivePotions) {
            mergedVector.push_back(potionName);
        }
        // Sort the merged vector
        sort(mergedVector.begin(), mergedVector.end());
        
        // Effective signs and potions are printed
        if (mergedVector.size() > 0) {
            bool first = true;
            for (string elem : mergedVector) {
                if (!first) {
                    cout << ", ";
                }
                cout << elem;
                first = false;
            }
            cout << endl;
        }
        // If the total size is zero, then there is no knowledge of signs or potions
        else {
            cout << "No knowledge of " << monsterName << endl;
        }
    }
    // If the monster is not present in the monsters map, there is no knowledge about effective signs or potions
    else {
        cout << "No knowledge of " << monsterName << endl;
    }
}

/**
 * @brief Prints the sorted formula for a potion.
 *
 * Sorts potions by descending quantity, secondary ascending by name.
 *
 * @param tokenList Tokenized query line.
 */
void Geralt::queryFormula(const vector<Token>& tokenList) {
    auto& potions = Geralt::getPotions();

    string potionName = tokenList[3].getContent();
    // If the potion does not exist, there is no formula for that
    if (potions.count(potionName) == 0) {
        cout << "No formula for " << potionName << endl;
    }
    else {
        // If there is a formula that is defined, print it
        if (potions[potionName]->isFormulaDefined()) {
            bool first = true;
            for (pair<string, int> formulaPair : potions[potionName]->getSortedFormula()) {
                if (!first) {
                    cout << ", ";
                }
                first = false;
                cout << formulaPair.second << " " << formulaPair.first;
            }
            cout << endl;
        }
        // If there is not a formula that is defined, print no formula
        else {
            cout << "No formula for " << potionName << endl;
        }
    }
} 
