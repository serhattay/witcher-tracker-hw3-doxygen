#ifndef TROPHY_H
#define TROPHY_H

/**
 * @file trophy.h
 * @brief Declaration of the @c Trophy class used by Geralt’s inventory.
 */

#include <string>

using namespace std;

class Trophy {
private:
    /// Data fields are declared private in order to encapsulate the data.
    string name;
    int quantity;
public:
    /**
     * @brief Construct a Trophy.
     * @param name Monster name.
     */
    Trophy(const string& name);

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
};

#endif