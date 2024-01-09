#ifndef GENIITEAMBUILDER_BATTLESIMULATOR_H
#define GENIITEAMBUILDER_BATTLESIMULATOR_H

#include <string>
#include <future>
#include <vector>
#include <cmath>
#include "PokemonDataSource.h"
#include "ThreadPool.h"
#include "data_class/PokemonState.h"
#include "DamageCalculator.h"
#include "BattleResult.h"

namespace battle_field {
    const std::vector<double> MULTIPLIERS = {
            0.25
            // , 0.28, 0.33, 0.40, 0.50, 0.66, 1.00, 1.50, 2.00, 2.50, 3.00, 3.50, 4.00
    };

    const int NUMBER_OF_POKEMON = 251;
    const size_t NUMBER_OF_BATTLES = NUMBER_OF_POKEMON * NUMBER_OF_POKEMON * MULTIPLIERS.size();
    const int LEVEL = 50;

}

class BattleSimulator {

private:
    ThreadPool threadPool;
    std::vector<BattleFunction> functions;
    std::vector<Pokemon> ALL_POKEMON = PokemonDataSource::getAllPokemon();
    std::vector<std::future<BattleResult>> battleFutures;
    std::vector<std::shared_ptr<std::promise<BattleResult>>> battlePromises;

    // Caching the PokemonStates only saved about 20ms.
    std::unordered_map<std::string, std::shared_ptr<PokemonState>> defenderPokemonStates;
    std::unordered_map<std::string, std::shared_ptr<PokemonState>> attackerPokemonStates;
    DamageCalculator damageCalculator;
    std::shared_ptr<std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>> attackerToMoveToPokemonDefeated =
            std::make_shared<std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>>();
    std::shared_ptr<std::unordered_map<std::string, int>> pokemonToTimesDefeated = std::make_shared<std::unordered_map<std::string, int>>();

    void battle(
            const std::shared_ptr<PokemonState> &attacker,
            const std::shared_ptr<PokemonState> &defender,
            std::promise<BattleResult> &promise
    ) {
        bool attackerBuffed = false;
        bool defenderBuffed = true;
        auto [damageToDefender, defenderAttack] = damageCalculator.getMaxDamageAttackerCanDoToDefender(
                attacker,
                defender,
                attackerBuffed,
                battle_field::LEVEL
        );
        auto [damageToAttacker, attackerAttack] = damageCalculator.getMaxDamageAttackerCanDoToDefender(
                attacker,
                defender,
                defenderBuffed,
                battle_field::LEVEL
        );
        double defenderSpeedStat = defender->getSpeed();
        double attackerSpeedStat = attacker->getSpeed();
        bool attackerFirst = attackerSpeedStat > defenderSpeedStat;

        BattleResult battleResult;
        if (damageToAttacker == 0 && damageToDefender == 0) {
            attacker->addDamage(attacker->getMaxHp());
        } else {
            while (attacker->getCurrentHP() > 0 && defender->getCurrentHP() > 0) {
                attacker->addDamage(damageToAttacker);
                defender->addDamage(damageToDefender);

                if (attackerAttack->getName() == "Giga Drain" &&
                    ((attackerFirst && defender->getCurrentHP() > 0) || !attackerFirst)) {
                    double gainedHealth = static_cast<double>(damageToDefender) / 2.0;
                    if (attacker->getCurrentHP() < 0) {
                        gainedHealth += damageToAttacker + attacker->getCurrentHP();
                    }
                    attacker->addHealth(gainedHealth);
                }

                if (attackerAttack->getName() == "Double-Edge") {
                    defender->addDamage(ceil(static_cast<double>(damageToDefender) / 4.0));
                }

                if (defenderAttack->getName() == "Giga Drain" &&
                    ((!attackerFirst && attacker->getCurrentHP() > 0) || attackerFirst)) {
                    double gainedHealth = static_cast<double>(damageToAttacker) / 2.0;
                    if (defender->getCurrentHP() < 0) {
                        gainedHealth += damageToDefender + defender->getCurrentHP();
                    }
                    defender->addHealth(gainedHealth);

                    if (defenderAttack->getName() == "Double-Edge") {
                        defender->addDamage(ceil(static_cast<double>(damageToDefender) / 4.0));
                    }
                }

                std::tie(damageToDefender, defenderAttack) = damageCalculator.getMaxDamageAttackerCanDoToDefender(
                        attacker,
                        defender,
                        battle_field::LEVEL
                );

                std::tie(damageToAttacker, attackerAttack) = damageCalculator.getMaxDamageAttackerCanDoToDefender(
                        defender,
                        attacker,
                        battle_field::LEVEL
                );
            }

            if (attacker->getCurrentHP() > 0 ||
                (attacker->getCurrentHP() <= 0 && defender->getCurrentHP() <= 0 && attackerFirst)
                    ) {
                battleResult = BattleResult(
                        attacker->getPokemon().pokemonInformation.name,
                        attackerAttack->getName(),
                        defender->getPokemon().pokemonInformation.name
                );
            }
        }
        // TODO Battle logic
        promise.set_value(battleResult);

    }

    std::shared_ptr<PokemonState> getDefenderPokemonState(Pokemon &pokemon, StatModifiers &statModifiers) {
        if (defenderPokemonStates.find(pokemon.pokemonInformation.name) == defenderPokemonStates.end()) {
            defenderPokemonStates.insert(
                    {
                            pokemon.pokemonInformation.name,
                            std::make_shared<PokemonState>(
                                    PokemonState(
                                            pokemon,
                                            statModifiers,
                                            true,
                                            false
                                    )
                            )
                    }
            );
        }
        auto state = defenderPokemonStates.at(pokemon.pokemonInformation.name);
        state->restore();
        return state;
    }

    std::shared_ptr<PokemonState> getAttackerPokemonState(Pokemon &pokemon, StatModifiers &statModifiers) {
        if (attackerPokemonStates.find(pokemon.pokemonInformation.name) == attackerPokemonStates.end()) {
            attackerPokemonStates.insert(
                    {
                            pokemon.pokemonInformation.name,
                            std::make_shared<PokemonState>(
                                    PokemonState(
                                            pokemon,
                                            statModifiers,
                                            false,
                                            true
                                    )
                            )
                    }
            );
        }
        auto state = attackerPokemonStates.at(pokemon.pokemonInformation.name);
        state->restore();
        return state;
    }

public:
    int battleTime() {
        // Modifiers
        // Stats: .25, .28, .33, .40, .50, .66, 1.00, 1.50, 2.00, 2.50, 3.00, 3.50, 4.00
        // Accuracy and evasion: .33, .36, .43, .50, .66, .75, 1.00, 1.33, 1.66, 2.00, 2.33, 2.66, 3.00
        double defenderStatMultiplier = 4.00;

        for (double attackerStatMultiplier: battle_field::MULTIPLIERS) {
            StatModifiers attackerStatModifiers(
                    attackerStatMultiplier,
                    attackerStatMultiplier,
                    attackerStatMultiplier,
                    attackerStatMultiplier,
                    attackerStatMultiplier,
                    attackerStatMultiplier,
                    attackerStatMultiplier
            );
            StatModifiers defenderStatModifiers(
                    defenderStatMultiplier,
                    defenderStatMultiplier,
                    defenderStatMultiplier,
                    defenderStatMultiplier,
                    defenderStatMultiplier,
                    defenderStatMultiplier,
                    defenderStatMultiplier
            );

            functions.clear();
            functions.reserve(ALL_POKEMON.size() * ALL_POKEMON.size());
            logFunctionTime(
                    [this, &attackerStatModifiers, &defenderStatModifiers] {
                        for (Pokemon &attackingPokemon: ALL_POKEMON) {
                            auto attackerState = getAttackerPokemonState(
                                    attackingPokemon,
                                    attackerStatModifiers
                            );

                            for (Pokemon &defendingPokemon: ALL_POKEMON) {
                                auto defenderState = getDefenderPokemonState(
                                        defendingPokemon,
                                        defenderStatModifiers
                                );
                                auto battlePromise = std::make_shared<std::promise<BattleResult>>();
                                battleFutures.emplace_back(battlePromise->get_future());
                                battlePromises.emplace_back(battlePromise);
                                functions.emplace_back(
                                        std::move(
                                                [attackerState, defenderState, battlePromise, this] {
                                                    battle(
                                                            attackerState,
                                                            defenderState,
                                                            *battlePromise
                                                    );
                                                }
                                        )
                                );
                            }

                        }
                        threadPool.addTasks(functions);
                    },
                    "Loop time: "
            );

        }

        for (auto &battleFuture: battleFutures) {
            BattleResult battleResult = battleFuture.get();
            if (battleResult.isAttackerWon()) {
                (*attackerToMoveToPokemonDefeated)[
                        battleResult.getAttackerName()
                ][
                        battleResult.getAttackName()
                ].push_back(battleResult.getDefenderName());
                (*pokemonToTimesDefeated)[battleResult.getDefenderName()]++;
            }
        }


        return 0;
    }

    void calculatePokemonStatistics() {
        std::unordered_map<double, std::vector<std::pair<std::string, std::unordered_map<std::string, size_t>>>> attackerRankToPokemon;
        std::unordered_map<std::string, double> bestMoves;

        for (auto &[attackerName, moveToPokemonDefeated]: *attackerToMoveToPokemonDefeated) {
            std::unordered_map<std::string, size_t> bestAttackerMoves;
            size_t numberOfPokemonDefeatedByAttacker = 0;
            for (auto &[move, pokemonDefeated]: moveToPokemonDefeated) {
                bestAttackerMoves[move] += pokemonDefeated.size();
                numberOfPokemonDefeatedByAttacker += pokemonDefeated.size();
            }

            for (auto &[move, pokemonDefeated]: moveToPokemonDefeated) {
                for (auto &pokemon: pokemonDefeated) {
                    bestMoves[move] += 1.0 / (*pokemonToTimesDefeated)[pokemon];
                }
                bestAttackerMoves[move] /= numberOfPokemonDefeatedByAttacker;
            }
            double rank = numberOfPokemonDefeatedByAttacker / battle_field::NUMBER_OF_POKEMON;
            attackerRankToPokemon[rank].emplace_back(attackerName, bestAttackerMoves);
        }
    }

};

#endif //GENIITEAMBUILDER_BATTLESIMULATOR_H
