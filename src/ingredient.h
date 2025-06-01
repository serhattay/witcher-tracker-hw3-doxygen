#ifndef INGREDIENT_H
#define INGREDIENT_H

/**
 * @file ingredient.h
 * @brief Declaration of the @c Ingredient class used by Geralt’s inventory.
 */

#include <string>

using namespace std;

/**
 * @class Ingredient
 * @brief This class acts as a blueprint for the Ingredient object that stores the name 
 * of the ingredient and the current quantity owned by Geralt. Simple helpers allow increasing
 * or decreasing that quantity while keeping the data encapsulated.
 */
class Ingredient {
private:
    /// Data fields are declared private in order to encapsulate the data.
    string name;
    int quantity;

public:
    /**
     * @brief Construct a new Ingredient.
     * @param name      Ingredient identifier (e.g., "Rebis").
     * @param quantity  Initial amount to store (must be non‑negative).
     */
    Ingredient(const string& name, int quantity);

    /**
     * @brief Retrieve current quantity of the ingredient owned by Geralt.
     * @return Quantity of the ingredient.
     */
    int getQuantity();

    /**
     * @brief Increase quantity by a positive @p amount
     * @param amount Amount to add.
     */
    void increaseQuantity(int amount);

    /**
     * @brief Decrease quantity by a positive @p amount.
     * @param amount Amount to subtract.
     */
    void decreaseQuantity(int amount);
};

#endif