#include <algorithm>
#include "PokemonState.h"

const PokemonState PokemonState::afterAddingDamage(const double damage) {
    return {
            buffed,
            pokemon,
            attackStat,
            defenseStat,
            specialAttack,
            specialDefense,
            speed,
            maxHp,
            currentHp - damage
    };
}

const PokemonState PokemonState::afterAddingHealth(const double health) {
    auto hp = currentHp + health;
    hp = std::min(hp, maxHp);
    return {
            buffed,
            pokemon,
            attackStat,
            defenseStat,
            specialAttack,
            specialDefense,
            speed,
            maxHp,
            hp
    };
}

const PokemonState PokemonState::afterRestoring() {
    return {
            buffed,
            pokemon,
            attackStat,
            defenseStat,
            specialAttack,
            specialDefense,
            speed,
            maxHp,
            maxHp
    };

}