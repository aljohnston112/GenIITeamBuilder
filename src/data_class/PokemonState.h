#ifndef GENIITEAMBUILDER_POKEMONSTATE_H
#define GENIITEAMBUILDER_POKEMONSTATE_H


#include <algorithm>
#include "StatModifiers.h"

class PokemonState {
private:
    const bool buffed;
    int attack_stat;
    int defense_stat;
    int special_attack;
    int special_defense;
    int speed;
    int max_hp;
    int current_hp;

public:
    PokemonState(
            auto& pokemonJSON,
            StatModifiers &stat_modifiers,
            bool buffed,
            bool min_stats
    ) : buffed(buffed) {
        if (min_stats) {
            current_hp = pokemonJSON["all_stats"]["level_50_min_stats"]["health"].GetInt();
            max_hp = pokemonJSON["all_stats"]["level_50_min_stats"]["health"].GetInt();
            attack_stat = pokemonJSON["all_stats"]["level_50_min_stats"]["attack"].GetInt() * stat_modifiers.attackModifier;
            defense_stat = pokemonJSON["all_stats"]["level_50_min_stats"]["defense"].GetInt() * stat_modifiers.defenseModifier;
            special_attack = pokemonJSON["all_stats"]["level_50_min_stats"]["special_attack"].GetInt() * stat_modifiers.specialAttackModifier;
            special_defense = pokemonJSON["all_stats"]["level_50_min_stats"]["special_defense"].GetInt() * stat_modifiers.specialDefenseModifier;
            speed = pokemonJSON["all_stats"]["level_50_min_stats"]["speed"].GetInt() * stat_modifiers.speedModifier;
        } else {
            current_hp = pokemonJSON["all_stats"]["level_50_max_stats"]["health"].GetInt();
            max_hp = pokemonJSON["all_stats"]["level_50_max_stats"]["health"].GetInt();
            attack_stat = pokemonJSON["all_stats"]["level_50_max_stats"]["attack"].GetInt() * stat_modifiers.attackModifier;
            defense_stat = pokemonJSON["all_stats"]["level_50_max_stats"]["defense"].GetInt() * stat_modifiers.defenseModifier;
            special_attack = pokemonJSON["all_stats"]["level_50_max_stats"]["special_attack"].GetInt() * stat_modifiers.specialAttackModifier;
            special_defense = pokemonJSON["all_stats"]["level_50_max_stats"]["special_defense"].GetInt() * stat_modifiers.specialDefenseModifier;
            speed = pokemonJSON["all_stats"]["level_50_max_stats"]["speed"].GetInt() * stat_modifiers.speedModifier;
        }
    }

    int get_max_hp() const {
        return max_hp;
    }
    void add_damage(int damage) {
        current_hp -= damage;
    }

    void add_health(int health) {
        current_hp += health;
        current_hp = std::min(current_hp, max_hp);
    }

    void restore() {
        current_hp = max_hp;
    }

    bool isBuffed() const {
        return buffed;
    }

    int getAttackStat() const {
        return attack_stat;
    }

    int getDefenseStat() const {
        return defense_stat;
    }

    int getSpecialAttack() const {
        return special_attack;
    }

    int getSpecialDefense() const {
        return special_defense;
    }

    int getSpeed() const {
        return speed;
    }

};

#endif //GENIITEAMBUILDER_POKEMONSTATE_H
