#ifndef GENIITEAMBUILDER_ATTACK_H
#define GENIITEAMBUILDER_ATTACK_H

#include "PokemonType.h"
#include "Category.h"

class Attack {
public:

    Attack(
            const std::string &attackName,
            const PokemonType &pokemonType,
            const Category &category,
            const int &power,
            const int &accuracy,
            const int &effectPercent
    ) : name(attackName),
        pokemonType(pokemonType),
        category(category),
        power(power),
        accuracy(accuracy),
        effectPercent(effectPercent) {}

    const std::string name;
    const PokemonType pokemonType;
    const Category category;
    const int power;
    const int accuracy;
    const int effectPercent;

};

#endif //GENIITEAMBUILDER_ATTACK_H
