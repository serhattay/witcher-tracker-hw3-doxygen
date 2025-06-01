#include "ingredient.h"
#include <string>

Ingredient::Ingredient(const string& name, int quantity) 
    : name(name), quantity(quantity) {}

int Ingredient::getQuantity() {
    return this->quantity;
}

void Ingredient::increaseQuantity(int amount) {
    this->quantity += amount;
}

void Ingredient::decreaseQuantity(int amount) {
    this->quantity -= amount;
}