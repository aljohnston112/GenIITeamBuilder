#ifndef GENIITEAMBUILDER_ATTACK_H
#define GENIITEAMBUILDER_ATTACK_H

#include <string>
#include <utility>
#include "PokemonType.h"
#include "Category.h"

class Attack {

public:
    const std::string attackName;
    const PokemonType pokemonType;
    const Category category;
    const int power;
    const int accuracy;
    const int effectPercent;

    Attack(
            std::string attackName,
            PokemonType pokemonType,
            Category category,
            int power,
            int accuracy,
            int effectPercent
    ) : attackName(std::move(attackName)),
        pokemonType(pokemonType),
        category(category),
        power(power),
        accuracy(accuracy),
        effectPercent(effectPercent) {}

};

#endif //GENIITEAMBUILDER_ATTACK_H
