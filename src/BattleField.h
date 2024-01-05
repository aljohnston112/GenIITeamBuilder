#ifndef GENIITEAMBUILDER_BATTLEFIELD_H
#define GENIITEAMBUILDER_BATTLEFIELD_H

#include <string>
#include <future>
#include <vector>
#include "PokemonDataSource.h"
#include "ThreadPool.h"

class BattleField {

private:
    std::vector<std::future<std::string>> battleFutures;
    std::vector<std::shared_ptr<std::promise<std::string>>> battlePromises;
    ThreadPool threadPool;
    std::vector<BattleFunction> functions;
    std::vector<std::string> pokemonNames = PokemonDataSource::getAllPokemonNames();

    static void battle(
            const std::string& attacker,
            const std::string& defender,
            std::promise<std::string> &promise
    ) {
        promise.set_value(defender);
        // TODO
    }

public:
    int battleTime() {
        functions.reserve(pokemonNames.size() * pokemonNames.size());
        logFunctionTime(
                [this] {
                    for (const std::string &attackingPokemon: pokemonNames) {
                        for (const std::string &defendingPokemon: pokemonNames) {
                            auto battlePromise = std::make_shared<std::promise<std::string>>();
                            battleFutures.emplace_back(battlePromise->get_future());
                            battlePromises.emplace_back(battlePromise);
                            functions.emplace_back(
                                    std::move(
                                            [&attackingPokemon, &defendingPokemon, battlePromise] {
                                                battle(
                                                        attackingPokemon,
                                                        defendingPokemon,
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

        for(auto& battleFuture: battleFutures){
            battleFuture.get();
        }

        return 0;
    }

};

#endif //GENIITEAMBUILDER_BATTLEFIELD_H
