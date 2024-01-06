#ifndef GENIITEAMBUILDER_BATTLEFIELD_H
#define GENIITEAMBUILDER_BATTLEFIELD_H

#include <string>
#include <future>
#include <vector>
#include "PokemonDataSource.h"
#include "ThreadPool.h"
#include "data_class/PokemonState.h"

class BattleField {

private:
    std::vector<std::future<std::string>> battleFutures;
    std::vector<std::shared_ptr<std::promise<std::string>>> battlePromises;
    ThreadPool threadPool;
    std::vector<BattleFunction> functions;
    std::vector<std::string> pokemonNames = PokemonDataSource::getAllPokemonNames();

    static void battle(
            const std::string &attacker,
            const std::string &defender,
            std::promise<std::string> &promise
    ) {
        promise.set_value(defender);
        // TODO
    }

public:
    int battleTime() {
        // Modifiers
        // Stats: .25, .28, .33, .40, .50, .66, 1.00, 1.50, 2.00, 2.50, 3.00, 3.50, 4.00
        // Accuracy and evasion: .33, .36, .43, .50, .66, .75, 1.00, 1.33, 1.66, 2.00, 2.33, 2.66, 3.00
        std::vector<double> multipliers = {0.25, 0.28, 0.33, 0.40, 0.50, 0.66, 1.00, 1.50, 2.00, 2.50, 3.00, 3.50,
                                           4.00};

        double defenderStatMultiplier = 4.00;
        for (double attackerStatMultiplier: multipliers) {
            // TODO cache StatModifiers if speed is slow
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
            functions.reserve(pokemonNames.size() * pokemonNames.size());
            logFunctionTime(
                    [this, &attackerStatModifiers, &defenderStatModifiers] {
                        for (const std::string &attackingPokemonIndex: pokemonNames) {
                            PokemonState attackerState(
                                    PokemonDataSource::getPokemonObjectByIndex(attackingPokemonIndex),
                                    attackerStatModifiers,
                                    false,
                                    true
                            );
                            double attackerSpeedStat = attackerState.getSpeed();

                            for (const std::string &defendingPokemonIndex: pokemonNames) {
                                PokemonState defenderState(
                                        PokemonDataSource::getPokemonObjectByIndex(defendingPokemonIndex),
                                        defenderStatModifiers,
                                        true,
                                        false
                                );
                                double defenderSpeedStat = defenderState.getSpeed();

                                auto battlePromise = std::make_shared<std::promise<std::string>>();
                                battleFutures.emplace_back(battlePromise->get_future());
                                battlePromises.emplace_back(battlePromise);
                                functions.emplace_back(
                                        std::move(
                                                [&attackingPokemonIndex, &defendingPokemonIndex, battlePromise] {
                                                    battle(
                                                            attackingPokemonIndex,
                                                            defendingPokemonIndex,
                                                            *battlePromise);
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
            battleFuture.get();
        }

        return 0;
    }

};

#endif //GENIITEAMBUILDER_BATTLEFIELD_H
