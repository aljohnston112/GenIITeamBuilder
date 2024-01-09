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

    [[nodiscard]] const std::string& getName() const { return name; }
    [[nodiscard]] const PokemonType& getPokemonType() const { return pokemonType; }
    [[nodiscard]] const Category& getCategory() const { return category; }
    [[nodiscard]] const int& getPower() const { return power; }
    [[nodiscard]] const int& getAccuracy() const { return accuracy; }
    [[nodiscard]] const int& getEffectPercent() const { return effectPercent; }

};

#endif //GENIITEAMBUILDER_ATTACK_H
