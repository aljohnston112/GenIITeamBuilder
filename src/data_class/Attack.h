#ifndef GENIITEAMBUILDER_ATTACK_H
#define GENIITEAMBUILDER_ATTACK_H

#include <string>
#include <utility>
#include "PokemonType.h"
#include "Category.h"

class Attack {
private:
    std::string name;
    PokemonType pokemonType;
    Category category;
    int power;
    int accuracy;
    int effectPercent;

public:

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

    std::string getName() const { return name; }
    PokemonType getPokemonType() const { return pokemonType; }
    Category getCategory() const { return category; }
    int getPower() const { return power; }
    int getAccuracy() const { return accuracy; }
    int getEffectPercent() const { return effectPercent; }

};

#endif //GENIITEAMBUILDER_ATTACK_H
