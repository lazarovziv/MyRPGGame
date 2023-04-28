#include "../include/Weapon.hpp"

Weapon::Weapon(WeaponType type) {
    switch (type) {
        case WeaponType::SWORD:
            name = "Sword";
            attackPoints = 10;
            hitRadius = 5 * 6.5f;
            numHitsPerSecond = 3;
            break;
        case WeaponType::DAGGER:
            name = "Dagger";
            attackPoints = 5;
            hitRadius = 2 * 4.5f;
            numHitsPerSecond = 7;
            break;
        case WeaponType::AXE:
            name = "Axe";
            attackPoints = 12;
            hitRadius = 6.5f * 6.5f;
            numHitsPerSecond = 2;
            break;
        case WeaponType::MACE:
            name = "Mace";
            attackPoints = 12;
            hitRadius = 6.5f * 6.5f;
            numHitsPerSecond = 2;
            break;
        case WeaponType::BARE_HANDED:
            name = "Bare Handed";
            attackPoints = 1;
            hitRadius = 1 * 3.5f;
            numHitsPerSecond = 12;
            break;
        case WeaponType::STAFF:
            name = "Staff";
            attackPoints = 2;
            hitRadius = 3 * 4.5f;
            numHitsPerSecond = 3;
            break;
    }
}

void Weapon::increaseAttackPoints(int amount) {
    attackPoints += amount;
}

void Weapon::increaseHitRadius(float amount) {
    hitRadius += amount;
}

void Weapon::increaseNumHitsPerSecond(int amount) {
    numHitsPerSecond += amount;
}

void Weapon::decreaseAttackPoints(int amount) {
    // don't let any attack points lower than 1
    if (attackPoints - amount <= 1) return;
    attackPoints -= amount;
}

void Weapon::decreaseHitRadius(float amount) {
    // don't let any hit radius lower than 1
    if (hitRadius - amount <= 1) return;
    hitRadius -= amount;
}

void Weapon::decreaseNumHitsPerSecond(int amount) {
    // don't let any hit radius lower than 1
    if (numHitsPerSecond - amount <= 1) return;
    numHitsPerSecond -= amount;
}

int Weapon::getAttackPoints() {
    return attackPoints;
}

float Weapon::getHitRadius() {
    return hitRadius;
}

int Weapon::getNumHitsPerSecond() {
    return numHitsPerSecond;
}
