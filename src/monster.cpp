#include "monster.h"
#include <string>

Monster::Monster(const string& name) 
    : name(name), effectiveSigns(), effectivePotions() {}

const vector<string>& Monster::getEffectiveSigns() {
    return this->effectiveSigns;
}

const vector<string>& Monster::getEffectivePotions() {
    return this->effectivePotions;
}

void Monster::addEffectiveSign(const string& name) {
    this->effectiveSigns.push_back(name);
}

void Monster::addEffectivePotion(const string& name) {
    this->effectivePotions.push_back(name);
}