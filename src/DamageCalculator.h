#ifndef GENIITEAMBUILDER_DAMAGECALCULATOR_H
#define GENIITEAMBUILDER_DAMAGECALCULATOR_H

#include <utility>
#include "data_class/Attack.h"
#include "data_class/PokemonState.h"
#include "data_class/Pokemon.h"
#include "AttackUtil.h"
#include "DefenseMultipliers.h"


int getAttackPower(
        const Attack &attack,
        const std::shared_ptr<PokemonState> &attackerState,
        const std::shared_ptr<PokemonState> &defender
) {
    bool buff = attackerState->buffed;
    int movePower = attack.getPower();

    if (attack.getName() == "Low Kick") {
        movePower = getLowKickPower(defender->pokemon.pokemonInformation.pounds);
    } else if (attack.getName() == "Present") {
        if (buff) {
            movePower = 120;
        } else {
            movePower = 0;
        }
    } else if (attack.getName() == "Reversal") {
        // movePower = getReversalPower(attackerState.currentHp, attackerState.getMaxHp());
        movePower = 0;
    } else if (attack.getName() == "Magnitude") {
        if (buff) {
            movePower = 150;
        } else {
            movePower = 10;
        }
    } else if (attack.getName() == "Hidden Power") {
        if (buff) {
            movePower = 70;
        } else {
            movePower = 31;
        }
    } else if (attack.getName() == "Explosion" || attack.getName() == "Selfdestruct") {
        movePower = 0;
    } else if (attack.getName() == "Sky AttackCrystal Only" || attack.getName() == "Sky Attack") {
        movePower = 70;
    } else if (attack.getName() == "Hyper Beam") {
        movePower = 75;
    } else if (attack.getName() == "Solarbeam") {
        if (buff) {
            movePower = 120;
        } else {
            movePower = 60;
        }
    } else if (
            (attack.getName() == "Double-Edge" || attack.getName() == "Thrash" || attack.getName() == "Vital Throw") &&
            !buff) {
        movePower = 0;
    } else if (attack.getName() == "Skull Bash") {
        movePower = 65;
    } else {
        if (movePower == -1) {
            movePower = 0;
        }
    }

    return movePower;
}

double getAttackStat(const std::shared_ptr<PokemonState> &pokemonState, bool isSpecial) {
    return isSpecial ? pokemonState->getSpecialAttack() : pokemonState->getAttackStat();
}

double getDefenseStat(const std::shared_ptr<PokemonState> &pokemonState, bool isSpecial) {
    return isSpecial ? pokemonState->getSpecialDefense() : pokemonState->getDefenseStat();
}

std::pair<double, Attack> getMaxDamageAttackerCanDoToDefender(
        const std::shared_ptr<PokemonState> &attacker_state,
        const std::shared_ptr<PokemonState> &defender_state,
        bool attacker_buffed,
        int level = 50
) {
    const Pokemon &attacker = attacker_state->getPokemon();
    const Pokemon &defender = defender_state->getPokemon();

    const std::vector<PokemonType> &defender_types = defender.pokemonInformation.pokemonTypes;
    DefenseMultipliers defender_defense_multipliers = getDefenseMultipliersForTypes(
            defender_types
    );
    double max_damage = 0.0f;
    std::optional<Attack> best_pokemon_move;

    const std::shared_ptr<std::unordered_map<int, std::vector<Attack>>> &attackers_attacks = attacker.attacks;

    for (const auto &pair: *attackers_attacks) {
        const auto attacks = pair.second;
        for (auto &attack: attacks) {
            if (attacker_buffed || attack.getAccuracy() == 100) {
                bool is_special = attack.getCategory() == Category::SPECIAL;
                double attack_stat = getAttackStat(attacker_state, is_special);
                double defense_stat = getDefenseStat(attacker_state, is_special);

                double attack_power = getAttackPower(attack, attacker_state, defender_state);
                double damage =
                        (((((2.0f * level) / 5.0f) + 2.0f) * attack_power * (attack_stat / defense_stat)) / 50.0f +
                         2.0f) *
                        defender_defense_multipliers.at(attack.getPokemonType());

                bool zero = true;
                if (attack.getName() == "Night Shade" || attack.getName() == "Seismic Toss") {
                    damage = level;
                    zero = false;
                }

                auto attackerTypes = attacker.pokemonInformation.pokemonTypes;
                if ((std::find(attackerTypes.begin(), attackerTypes.end(), attack.getPokemonType()) !=
                     attackerTypes.end()) || (attack.getName() == "Hidden Power" && attacker_state->isBuffed())) {
                    damage *= 1.5f;
                }

                if (attack.getName() == "Hidden Power" && !attacker_state->isBuffed()) {
                    auto it = std::min_element(
                            defender_defense_multipliers.begin(),
                            defender_defense_multipliers.end(),
                            [](const auto &p1, const auto &p2) { return p1.second < p2.second; }
                    );
                    damage *= it->second;
                }

                if (attack.getName() == "Present" && !attacker_state->isBuffed()) {
                    damage = -(defender_state->get_max_hp() / 4);
                }

                if (zero && attack_power == 0) {
                    damage = 0.0f;
                }

                if (damage > max_damage) {
                    best_pokemon_move.value() = attack;
                }
                max_damage = std::max(damage, max_damage);
            }
        }
    }

    return {
            max_damage, best_pokemon_move.

                    value()

    };
}

#endif //GENIITEAMBUILDER_DAMAGECALCULATOR_H
