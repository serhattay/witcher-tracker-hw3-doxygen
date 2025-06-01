#ifndef POTION_H
#define POTION_H


/**
 * @file potion.h
 * @brief Declaration of the @c Potion class.
 */

#include <string>
#include <vector>

using namespace std;

/**
 * @struct Comparator
 * @brief Custom comparator for quantity-ingredient pairs in the formula
 *
 * Orders two quantity, ingredient pairs:
 *   1. Higher @c quantity comes first.
 *   2. For equal quantities, lower (alphabetical) @c ingredient name comes first.
 */
struct Comparator {
    bool operator()(const pair<string, int>& a, const pair<string, int>& b) const {
        // If their quantities are different, they are sorted by their quantities
        if (a.second != b.second) {
            return a.second > b.second;
        }
        // If they have the same quantity, then they are sorted by their names
        else {
            return a.first < b.first;
        }
    }
};

/**
 * @class Potion
 * @brief This class acts as a blueprint for the Potion object 
 * which stores the potion's name, quantity and the formula for that potion.
*/
class Potion {
private:
    /// Data fields are declared private in order to encapsulate the data.
    string name;
    int quantity;
    vector<pair<string, int>> formula;
    bool formulaDefined;

    /// This private method sorts the formula before printing.
    void sortFormula();

public:
    /**
     * @brief Construct a Potion.
     * @param name Potion name.
     */
    Potion(const string& name);

    /**
     * @brief Get current quantity.
     */
    int getQuantity();

    /**
     * @brief Increase quantity.
     * @param amount Amount to add.
     */
    void increaseQuantity(int amount);

    /**
     * @brief Decrease quantity.
     * @param amount Amount to subtract.
     */
    void decreaseQuantity(int amount);

    /**
     * @brief Check if formula is defined.
     */
    bool isFormulaDefined();

    /**
     * @brief Mark formula as defined.
     */
    void defineFormula();

    /**
     * @brief Add ingredient to formula.
     * @param quantity Amount required.
     * @param ingredientName Ingredient name.
     */
    void addToFormula(int quantity, const string& ingredientName);

    /**
     * @brief Get formula.
     */
    const vector<pair<string, int>>& getFormula();

    /**
     * @brief Get sorted formula.
     */
    const vector<pair<string, int>>&  getSortedFormula();
};

#endif