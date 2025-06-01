#include "potion.h"
#include <string>
#include <algorithm>

Potion::Potion(const string& name) 
    : name(name), quantity(0), formulaDefined(false) {}

void Potion::sortFormula() {
    sort(formula.begin(), formula.end(), Comparator());
}

int Potion::getQuantity() {
    return this->quantity;
}

void Potion::increaseQuantity(int amount) {
    this->quantity += amount;
}

void Potion::decreaseQuantity(int amount) {
    this->quantity -= amount;
}

bool Potion::isFormulaDefined() {
    return this->formulaDefined;
}

void Potion::defineFormula() {
    this->formulaDefined = true;
}

void Potion::addToFormula(int quantity, const string& ingredientName) {
    this->formula.push_back(make_pair(ingredientName, quantity));
}

const vector<pair<string,int>>& Potion::getFormula() {
    return this->formula;
}

const vector<pair<string, int>>& Potion::getSortedFormula() {
    sortFormula();
    return this->formula;
}