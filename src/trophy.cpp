#include "trophy.h"
#include <string>

Trophy::Trophy(const string& name)
    : name(name), quantity(0) {}
    
int Trophy::getQuantity() {
    return this->quantity;
}

void Trophy::increaseQuantity(int amount) {
    this->quantity += amount;
}

void Trophy::decreaseQuantity(int amount) {
    this->quantity -= amount;
}