#ifndef GENIITEAMBUILDER_DAMAGECALCULATOR_H
#define GENIITEAMBUILDER_DAMAGECALCULATOR_H

#include <utility>
#include "data_class/Attack.h"
#include "data_class/PokemonState.h"
#include "data_class/Pokemon.h"
#include "AttackUtil.h"
#include "DefenseMultipliers.h"

class DamageCalculator {

    const std::vector<std::pair<std::string, int>> buffedAttackPowers = {
            {"Present",                120},
            {"Reversal",               0},
            {"Magnitude",              150},
            {"Hidden Power",           70},
            {"Explosion",              0},
            {"Selfdestruct",           0},
            {"Sky AttackCrystal Only", 70},
            {"Sky Attack",             70},
            {"Hyper Beam",             75},
            {"Solarbeam",              120},
            {"Skull Bash",             65}
    };

    const std::vector<std::pair<std::string, int>> nonBuffedAttackPowers = {
            {"Present",                0},
            {"Reversal",               0},
            {"Magnitude",              10},
            {"Hidden Power",           31},
            {"Explosion",              0},
            {"Selfdestruct",           0},
            {"Sky AttackCrystal Only", 70},
            {"Sky Attack",             70},
            {"Hyper Beam",             75},
            {"Solarbeam",              60},
            {"Double-Edge",            0},
            {"Thrash",                 0},
            {"Vital Throw",            0},
            {"Skull Bash",             65}
    };

    int getAttackPower(
            const Attack &attack,
            const std::shared_ptr<PokemonState> &attackerState,
            const std::shared_ptr<PokemonState> &defender
    ) {
        const bool buff = attackerState->buffed;
        const std::string &attackName = attack.name;
        int movePower = attack.power;

        // movePower = getReversalPower(attackerState.currentHp, attackerState.getMaxHp());
        if (attackName == "Low Kick") {
            movePower = getLowKickPower(defender->pokemon.pokemonInformation.pounds);
        } else if (buff) {
            for (const auto& pair : buffedAttackPowers) {
                if (pair.first == attackName) {
                    movePower = pair.second;
                }
            }
        } else if (!buff) {
            for (const auto& pair : nonBuffedAttackPowers) {
                if (pair.first == attackName) {
                    movePower = pair.second;
                }
            }
        } else if (movePower == -1) {
            movePower = 0;
        }
        return movePower;
    }

    static double getAttackStat(const std::shared_ptr<PokemonState> &pokemonState, bool isSpecial) {
        return isSpecial ? pokemonState->specialAttack : pokemonState->attackStat;
    }

    static double getDefenseStat(const std::shared_ptr<PokemonState> &pokemonState, bool isSpecial) {
        return isSpecial ? pokemonState->specialDefense : pokemonState->defenseStat;
    }

public:
    std::pair<double, std::shared_ptr<Attack>> getMaxDamageAttackerCanDoToDefender(
            std::shared_ptr<PokemonState> attacker_state,
            std::shared_ptr<PokemonState> defender_state,
            bool attacker_buffed,
            int level = 50
    ) {
        const Pokemon &attacker = attacker_state->pokemon;
        const Pokemon &defender = defender_state->pokemon;

        const std::vector<PokemonType> &defender_types = defender.pokemonInformation.pokemonTypes;
        DefenseMultipliers defender_defense_multipliers = getDefenseMultipliersForTypes(
                defender_types
        );
        double max_damage = 0.0f;
        std::shared_ptr<Attack> bestPokemonMove;

        const std::shared_ptr<std::unordered_map<int, std::vector<Attack>>> &attackers_attacks = attacker.attacks;

        for (const auto &pair: *attackers_attacks) {
            const auto &attacks = pair.second;
            for (auto &attack: attacks) {
                if (attacker_buffed || attack.accuracy == 100) {
                    bool is_special = attack.category == Category::SPECIAL;
                    double attack_stat = getAttackStat(attacker_state, is_special);
                    double defense_stat = getDefenseStat(attacker_state, is_special);

                    double attack_power = getAttackPower(attack, attacker_state, defender_state);
                    double damage =
                            (((((2.0 * level) / 5.0) + 2.0) * attack_power * (attack_stat / defense_stat)) / 50.0 +
                             2.0) *
                            defender_defense_multipliers.at(attack.pokemonType);

                    bool zero = true;
                    if (attack.name == "Night Shade" || attack.name == "Seismic Toss") {
                        damage = level;
                        zero = false;
                    }

                    auto attackerTypes = attacker.pokemonInformation.pokemonTypes;
                    if ((std::find(attackerTypes.begin(), attackerTypes.end(), attack.pokemonType) !=
                         attackerTypes.end()) || (attack.name == "Hidden Power" && attacker_state->buffed)) {
                        damage *= 1.5f;
                    }

                    if (attack.name == "Hidden Power" && !attacker_state->buffed) {
                        auto it = std::min_element(
                                defender_defense_multipliers.begin(),
                                defender_defense_multipliers.end(),
                                [](const auto &p1, const auto &p2) { return p1.second < p2.second; }
                        );
                        damage *= it->second;
                    }

                    if (attack.name == "Present" && !attacker_state->buffed) {
                        damage = -(defender_state->maxHp / 4);
                    }

                    if (zero && (attack_power == 0 || attack_power == -1)) {
                        damage = 0.0f;
                    }

                    if (damage > max_damage) {
                        bestPokemonMove = std::make_shared<Attack>(attack);
                    }
                    max_damage = std::max(damage, max_damage);
                }
            }
        }

        return {
                max_damage,
                bestPokemonMove
        };
    }

};

#endif //GENIITEAMBUILDER_DAMAGECALCULATOR_H
