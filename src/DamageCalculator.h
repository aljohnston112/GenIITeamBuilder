#ifndef GENIITEAMBUILDER_DAMAGECALCULATOR_H
#define GENIITEAMBUILDER_DAMAGECALCULATOR_H

#include <utility>
#include "data_class/Attack.h"
#include "data_class/PokemonState.h"
#include "data_class/Pokemon.h"

double getAttackStat(const PokemonState &pokemonState, bool isSpecial) {
    return isSpecial ? pokemonState.getSpecialAttack() : pokemonState.getAttackStat();
}

double getDefenseStat(const PokemonState &pokemonState, bool isSpecial) {
    return isSpecial ? pokemonState.getSpecialDefense() : pokemonState.getDefenseStat();
}

std::pair<float, Attack> getMaxDamageAttackerCanDoToDefender(
        const PokemonState &attacker_state,
        const PokemonState &defender_state,
        bool attacker_buffed,
        int level = 50
) {
    const Pokemon &attacker = attacker_state.getPokemon();
    const Pokemon &defender = defender_state.getPokemon();

    const std::vector<PokemonType> &defender_types = defender.pokemonInformation.pokemonTypes;
    const std::unordered_map<PokemonType, float> &defender_defense_multipliers = get_defense_multipliers_for_types(
            defender_types);
    float max_damage = 0.0f;
    std::optional<Attack> best_pokemon_move;

    const std::unordered_map<int, std::vector<Attack>> &attackers_attacks = attacker.attacks;

    for (const Attack &attack: attackers_attacks) {
        if (attacker_buffed || attack.accuracy == 100) {

            bool is_special = attack.category == Category::SPECIAL;
            float attack_stat = getAttackStat(attacker_state, is_special);
            float defense_stat = getDefenseStat(attacker_state, is_special);

            float attack_power = get_attack_power(attack, attacker_state, defender);
            float damage =
                    (((((2.0f * level) / 5.0f) + 2.0f) * attack_power * (attack_stat / defense_stat)) / 50.0f + 2.0f) *
                    defender_defense_multipliers.at(attack.pokemonType);

            bool zero = true;
            if (attack.name == "Night Shade" || attack.name == "Seismic Toss") {
                damage = level;
                zero = false;
            }

            auto attackerTypes = attacker.pokemonInformation.pokemonTypes;
            if ((std::find(attackerTypes.begin(), attackerTypes.end(), attack.pokemonType) !=
                 attackerTypes.end()) || (attack.name == "Hidden Power" && attacker_state.isBuffed())) {
                damage *= 1.5f;
            }

            if (attack.name == "Hidden Power" && !attacker_state.isBuffed()) {
                auto it = std::min_element(
                        defender_defense_multipliers.begin(),
                        defender_defense_multipliers.end(),
                        [](const auto &p1, const auto &p2) { return p1.second < p2.second; }
                );
                damage *= it->second;
            }

            if (attack.name == "Present" && !attacker_state.isBuffed()) {
                damage = -(defender_state.get_max_hp() / 4);
            }

            if (zero && attack_power == 0) {
                damage = 0.0f;
            }

            if (damage > max_damage) {
                best_pokemon_move = attack;
            }
            max_damage = std::max(damage, max_damage);
        }
    }

    return {max_damage, best_pokemon_move.value()};
}

#endif //GENIITEAMBUILDER_DAMAGECALCULATOR_H
