#ifndef GENIITEAMBUILDER_POKEMONSTATE_H
#define GENIITEAMBUILDER_POKEMONSTATE_H


#include <algorithm>
#include "StatModifiers.h"

class PokemonState {
private:
    const bool buffed;
    double attackStat;
    double defenseStat;
    double specialAttack;
    double specialDefense;
    double speed;
    double maxHp;
    double currentHp;

public:
    PokemonState(
            const Pokemon& pokemon,
            const StatModifiers &stat_modifiers,
            bool buffed,
            bool minStats
    ) : buffed(buffed) {
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

    double get_max_hp() const {
        return maxHp;
    }
    void add_damage(double damage) {
        currentHp -= damage;
    }

    void add_health(double health) {
        currentHp += health;
        currentHp = std::min(currentHp, maxHp);
    }

    void restore() {
        currentHp = maxHp;
    }

    bool isBuffed() const {
        return buffed;
    }

    double getAttackStat() const {
        return attackStat;
    }

    double getDefenseStat() const {
        return defenseStat;
    }

    double getSpecialAttack() const {
        return specialAttack;
    }

    double getSpecialDefense() const {
        return specialDefense;
    }

    double getSpeed() const {
        return speed;
    }

};

#endif //GENIITEAMBUILDER_POKEMONSTATE_H
