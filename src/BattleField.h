#ifndef GENIITEAMBUILDER_BATTLEFIELD_H
#define GENIITEAMBUILDER_BATTLEFIELD_H

#include <string>
#include <future>
#include <vector>
#include "PokemonDataSource.h"
#include "ThreadPool.h"
#include "data_class/PokemonState.h"

namespace battle_field {
    const std::vector<double> MULTIPLIERS = {
            0.25, 0.28, 0.33, 0.40, 0.50, 0.66, 1.00, 1.50, 2.00, 2.50, 3.00, 3.50, 4.00
    };

    const int NUMBER_OF_POKEMON = 251;
    const size_t NUMBER_OF_BATTLES = NUMBER_OF_POKEMON * NUMBER_OF_POKEMON * MULTIPLIERS.size();

}

class BattleField {

private:
    ThreadPool threadPool;
    std::vector<BattleFunction> functions;
    std::vector<Pokemon> ALL_POKEMON = PokemonDataSource::getAllPokemon();
    std::vector<std::future<std::string>> battleFutures;
    std::vector<std::shared_ptr<std::promise<std::string>>> battlePromises;
    std::unordered_map<std::string, shared_ptr<PokemonState>> defenderPokemonStates;
    std::unordered_map<std::string, shared_ptr<PokemonState>> attackerPokemonStates;

    static void battle(
            const Pokemon &attacker,
            const Pokemon &defender,
            std::promise<std::string> &promise
    ) {
        promise.set_value(defender.pokemonInformation.name);
        // TODO Battle logic
    }

    shared_ptr<PokemonState> getDefenderPokemonState(Pokemon &pokemon, StatModifiers statModifiers) {
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

    std::shared_ptr<PokemonState> getAttackerPokemonState(Pokemon &pokemon, StatModifiers statModifiers) {
        if (attackerPokemonStates.find(pokemon.pokemonInformation.name) == attackerPokemonStates.end()) {
            attackerPokemonStates.insert(
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
                            auto attackerState = getAttackerPokemonState(attackingPokemon,
                                                                                 attackerStatModifiers);
                            double attackerSpeedStat = attackerState->getSpeed();

                            for (Pokemon &defendingPokemon: ALL_POKEMON) {
                                auto defenderState = getDefenderPokemonState(defendingPokemon,
                                                                                     defenderStatModifiers);
                                double defenderSpeedStat = defenderState->getSpeed();

                                auto battlePromise = std::make_shared<std::promise<std::string>>();
                                battleFutures.emplace_back(battlePromise->get_future());
                                battlePromises.emplace_back(battlePromise);
                                functions.emplace_back(
                                        std::move(
                                                [&attackingPokemon, &defendingPokemon, battlePromise] {
                                                    battle(
                                                            attackingPokemon,
                                                            defendingPokemon,
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
            battleFuture.get();
        }

        return 0;
    }

};

#endif //GENIITEAMBUILDER_BATTLEFIELD_H
