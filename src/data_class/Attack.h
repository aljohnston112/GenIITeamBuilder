#ifndef GENIITEAMBUILDER_ATTACK_H
#define GENIITEAMBUILDER_ATTACK_H

#include <string>
#include <utility>
#include "PokemonType.h"
#include "Category.h"

class Attack {

public:
    std::string name;
    PokemonType pokemonType;
    Category category;
    int power;
    int accuracy;
    int effectPercent;

    Attack(
            std::string attackName,
            PokemonType pokemonType,
            Category category,
            int power,
            int accuracy,
            int effectPercent
    ) : name(std::move(attackName)),
        pokemonType(pokemonType),
        category(category),
        power(power),
        accuracy(accuracy),
        effectPercent(effectPercent) {}

    Attack(const Attack& attack) = default;

};

#endif //GENIITEAMBUILDER_ATTACK_H
