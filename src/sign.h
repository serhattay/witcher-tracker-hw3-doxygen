#ifndef SIGN_H
#define SIGN_H

/**
 * @file sign.h
 * @brief Declaration of the @c Sign class used by Geralt’s inventory.
 */

#include <string>

using namespace std;

/**
 * @class Sign
 * @brief This class acts as a blueprint for the Sign object that stores the name of the sign.
 */
class Sign {
private:
    /// Name data field is declared private for data encapsulation
    string name;

public:
    /**
     * @brief Construct a Sign.
     * @param name Sign name.
     */
    Sign(const string& name);
};

#endif