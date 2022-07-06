#include "NPCEnemy.hpp"

int NPCEnemy::getBattleTimeout() {
    return battleTimeout;
}

float NPCEnemy::getWanderAreaRadius() {
    return wanderAreaRadius;
}

float NPCEnemy::getBattleAreaRadius() {
    return battleAreaRadius;
}

EnemyType NPCEnemy::getType() {
    return type;
}
