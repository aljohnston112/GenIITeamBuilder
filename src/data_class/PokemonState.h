#ifndef GENIITEAMBUILDER_POKEMONSTATE_H
#define GENIITEAMBUILDER_POKEMONSTATE_H


#include <algorithm>
#include "StatModifiers.h"

class PokemonState {
private:
    const Pokemon pokemon;
    double attackStat;
    double defenseStat;
    double specialAttack;
    double specialDefense;
    double speed;
    double maxHp;
    double currentHp;

public:
    const bool buffed;

    PokemonState(
            const Pokemon& pokemon,
            const StatModifiers &stat_modifiers,
            const bool buffed,
            const bool minStats
    ) : pokemon(pokemon), buffed(buffed) {
        if (minStats) {
            currentHp = pokemon.allStats.level_50_min_stats.health;
            maxHp = pokemon.allStats.level_50_min_stats.health;
            attackStat = pokemon.allStats.level_50_min_stats.attack * stat_modifiers.attackModifier;
            defenseStat = pokemon.allStats.level_50_min_stats.defense * stat_modifiers.defenseModifier;
            specialAttack = pokemon.allStats.level_50_min_stats.special_attack * stat_modifiers.specialAttackModifier;
            specialDefense = pokemon.allStats.level_50_min_stats.special_defense * stat_modifiers.specialDefenseModifier;
            speed = pokemon.allStats.level_50_min_stats.speed * stat_modifiers.speedModifier;
        } else {
            currentHp = pokemon.allStats.level_50_max_stats.health;
            maxHp = pokemon.allStats.level_50_max_stats.health;
            attackStat = pokemon.allStats.level_50_max_stats.attack * stat_modifiers.attackModifier;
            defenseStat = pokemon.allStats.level_50_max_stats.defense * stat_modifiers.defenseModifier;
            specialAttack = pokemon.allStats.level_50_max_stats.special_attack * stat_modifiers.specialAttackModifier;
            specialDefense = pokemon.allStats.level_50_max_stats.special_defense * stat_modifiers.specialDefenseModifier;
            speed = pokemon.allStats.level_50_max_stats.speed * stat_modifiers.speedModifier;
        }
    }

    [[nodiscard]] double getMaxHp() const {
        return maxHp;
    }
    void addDamage(double damage) {
        currentHp -= damage;
    }

    void addHealth(double health) {
        currentHp += health;
        currentHp = std::min(currentHp, maxHp);
    }

    void restore() {
        currentHp = maxHp;
    }

    [[nodiscard]] bool isBuffed() const {
        return buffed;
    }

    [[nodiscard]] double getAttackStat() const {
        return attackStat;
    }

    [[nodiscard]] double getDefenseStat() const {
        return defenseStat;
    }

    [[nodiscard]] double getSpecialAttack() const {
        return specialAttack;
    }

    [[nodiscard]] double getSpecialDefense() const {
        return specialDefense;
    }

    [[nodiscard]] double getSpeed() const {
        return speed;
    }

    [[nodiscard]] const Pokemon& getPokemon() const {
        return pokemon;
    }

    [[nodiscard]] double getCurrentHP() const{
        return currentHp;
    }

};

#endif //GENIITEAMBUILDER_POKEMONSTATE_H
