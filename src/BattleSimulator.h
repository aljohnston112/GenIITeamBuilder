#ifndef GENIITEAMBUILDER_BATTLESIMULATOR_H
#define GENIITEAMBUILDER_BATTLESIMULATOR_H

#include <vector>
#include <cstddef>
#include <fstream>
#include <unordered_set>
#include <set>
#include "PokemonStateCache.h"
#include "ThreadPool.h"
#include "PokemonDataSource.h"
#include "data_class/BattleResult.h"
#include "BattleField.h"
#include "Timer.h"

namespace battle_field {
    const std::vector<double> MULTIPLIERS = {
            0.25
            // , 0.28, 0.33, 0.40, 0.50, 0.66, 1.00, 1.50, 2.00, 2.50, 3.00, 3.50, 4.00
    };
    const int NUMBER_OF_POKEMON = 251;
    const size_t NUMBER_OF_BATTLES = NUMBER_OF_POKEMON * NUMBER_OF_POKEMON * MULTIPLIERS.size();
}

class BattleSimulator {

private:
    ThreadPool threadPool;
    PokemonStateCache pokemonStateCache;

public:


    static std::set<std::vector<std::string>> generateCombinations(
            std::vector<std::string> &elements,
            long length
    ) {

        std::sort(elements.begin(), elements.end());

        std::set<std::vector<std::string>> result;
        do {
            std::vector<std::string> subset(elements.begin(), elements.begin() + length);
            result.insert(subset);
        } while (std::next_permutation(elements.begin(), elements.end()));

        for (const auto &c: result) {
            for (const auto &e: c) std::cout << e;
            std::cout << '\n';
        }

        return result;
    }

    static void analyzeDefeats(
            const std::unordered_map<std::string, std::unordered_set<std::string>> &pokemon_to_pokemon_lost_to
    ) {
        int min_overlaps = battle_field::NUMBER_OF_POKEMON;
        std::unordered_set<std::string> top_threats;

        std::vector<std::string> keys;
        for (const auto &entry: pokemon_to_pokemon_lost_to) {
            keys.push_back(entry.first);
        }

        long combinationLength = 3;
        std::set<std::vector<std::string>> combinations = generateCombinations(
                keys, combinationLength
        );

        for (const auto &pokemon_combo: combinations) {
            std::unordered_set<std::string> seen;
            int overlaps = 0;
            for (const auto &pokemon: pokemon_combo) {
                for (const std::string &threat: pokemon_to_pokemon_lost_to.at(pokemon)) {
                    if (seen.find(threat) == seen.end()) {
                        seen.insert(threat);
                    } else {
                        overlaps += 1;
                    }
                }
            }
            if (overlaps < min_overlaps) {
                min_overlaps = overlaps;

                std::cout << "Team: ";
                for (const auto &elem: pokemon_combo) {
                    std::cout << elem << " ";
                }
                std::cout << "\nWith threats:\n";
                top_threats = seen;
                for (const auto &threat: top_threats) {
                    std::cout << "    " << threat << std::endl;
                }

            }
        }

        std::unordered_map<std::string, int> pokemon_to_threat_count;

        for (const auto &defeated_pokemon: pokemon_to_pokemon_lost_to) {
            for (const auto &threat: top_threats) {
                if (defeated_pokemon.second.find(threat) != defeated_pokemon.second.end()) {
                    pokemon_to_threat_count[threat] += 1;
                }
            }
        }

        std::vector<std::pair<std::string, int>> pokemon_vector(pokemon_to_threat_count.begin(), pokemon_to_threat_count.end());
        std::sort(
                pokemon_vector.begin(),
                pokemon_vector.end(),
                [](const auto &lhs, const auto &rhs) {
                    return lhs.second < rhs.second;
                }
        );

        std::cout << "Top threat to the threat ranks:" << std::endl;
        for (const auto &threat_count: pokemon_to_threat_count) {
            std::cout << "    " << threat_count.first << ": " << threat_count.second << std::endl;
        }
    }


    static void saveThreatLists(
            const std::unordered_map<std::string, std::unordered_set<std::string>> &pokemon_to_pokemon_lost_to,
            const std::string &pokemonThreatFileName,
            const std::string &leastThreatsFileName
    ) {
        std::ofstream outputFile(pokemonThreatFileName);

        for (const auto &entry: pokemon_to_pokemon_lost_to) {
            outputFile << entry.first << " lost to: ";
            for (const auto &lost_to: entry.second) {
                outputFile << "    " << lost_to << "\n";
            }
            outputFile << "\n";
        }
        outputFile.close();
    }

    int battleTime() {
        std::vector<Pokemon> allPokemon = PokemonDataSource::getAllPokemon();

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

            std::vector<std::shared_ptr<std::promise<BattleResult>>> battlePromises;
            std::vector<std::future<BattleResult>> battleFutures;
            std::vector<BattleFunction> functions;

            Timer::logFunctionTime(
                    [&attackerStatModifiers, &defenderStatModifiers, &allPokemon, &battlePromises, &battleFutures, &functions, this] {
                        for (Pokemon &attackingPokemon: allPokemon) {
                            std::shared_ptr<PokemonState> attackerState = pokemonStateCache.getAttackerPokemonState(
                                    attackingPokemon,
                                    attackerStatModifiers
                            );

                            for (Pokemon &defendingPokemon: allPokemon) {
                                std::shared_ptr<PokemonState> defenderState = pokemonStateCache.getDefenderPokemonState(
                                        defendingPokemon,
                                        defenderStatModifiers
                                );

                                auto battlePromise = std::make_shared<std::promise<BattleResult>>(
                                        std::promise<BattleResult>()
                                );
                                battlePromises.emplace_back(battlePromise);
                                battleFutures.emplace_back(battlePromise->get_future());
                                functions.emplace_back(
                                        std::move(
                                                [attackerState, defenderState, battlePromise] {
                                                    BattleField::battle(
                                                            attackerState,
                                                            defenderState,
                                                            battlePromise
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

            std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> attackerToMoveToPokemonDefeated =
                    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>();
            std::unordered_map<std::string, int> pokemonToTimesDefeated = std::unordered_map<std::string, int>();
            std::unordered_map<std::string, std::unordered_set<std::string>> pokemon_to_pokemon_lost_to;
            for (auto &battleFuture: battleFutures) {
                BattleResult battleResult = battleFuture.get();
                if (battleResult.attackerWon) {
                    attackerToMoveToPokemonDefeated[
                            battleResult.attackerName
                    ][
                            battleResult.attackName
                    ].push_back(battleResult.defenderName);
                    pokemonToTimesDefeated[battleResult.defenderName]++;
                } else {
                    pokemon_to_pokemon_lost_to[battleResult.attackerName].insert(
                            battleResult.defenderName
                    );
                }
            }

            std::string rankFileName =
                    "data/generated/pokemon_ranks_" + std::to_string(attackerStatMultiplier) +
                    "_" + std::to_string(defenderStatMultiplier);
            std::string moveRankFileName =
                    "data/generated/move_ranks_" + std::to_string(attackerStatMultiplier) +
                    "_" + std::to_string(defenderStatMultiplier);
            calculatePokemonStatistics(
                    attackerToMoveToPokemonDefeated,
                    pokemonToTimesDefeated,
                    rankFileName,
                    moveRankFileName
            );

            std::string pokemonThreatFileName =
                    "data/generated/pokemon_threats_" + std::to_string(attackerStatMultiplier) +
                    "_" + std::to_string(defenderStatMultiplier);

            std::string leastThreatsFileName =
                    "data/generated/pokemon_threats_" + std::to_string(attackerStatMultiplier) +
                    "_" + std::to_string(defenderStatMultiplier);

            saveThreatLists(
                    pokemon_to_pokemon_lost_to,
                    pokemonThreatFileName,
                    leastThreatsFileName
            );

            analyzeDefeats(pokemon_to_pokemon_lost_to);

            std::string threatsThreatFilename =
                    "data/generated/threats_threats" + std::to_string(attackerStatMultiplier) +
                    "_" + std::to_string(defenderStatMultiplier);

        }

        return 0;
    }

    static void saveStats(
            std::unordered_map<double, std::vector<std::pair<std::string, std::unordered_map<std::string, double>>>> attackerRankToPokemon,
            std::unordered_map<std::string, double> bestMoves,
            std::string &rankFileName,
            std::string &moveRankFilename
    ) {
        std::vector<std::pair<double, std::vector<std::pair<std::string, std::unordered_map<std::string, double>>>>> attackerVector(
                attackerRankToPokemon.begin(), attackerRankToPokemon.end());
        std::vector<std::pair<std::string, double>> bestMovesVector(bestMoves.begin(), bestMoves.end());

        std::sort(attackerVector.begin(), attackerVector.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.first < rhs.first;
        });

        std::sort(bestMovesVector.begin(), bestMovesVector.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.second > rhs.second;
        });

        std::ofstream rankFile(rankFileName);

        for (const auto &entry: attackerVector) {
            rankFile << "Rank: " << entry.first << "\n";
            for (const auto &pokemonEntry: entry.second) {
                rankFile << "    Pokemon: " << pokemonEntry.first << "\n";
                for (const auto &moveEntry: pokemonEntry.second) {
                    rankFile << "        Move: " << moveEntry.first << ", Count: " << moveEntry.second << "\n";
                }
            }
        }
        rankFile.close();

        std::ofstream moveFile(moveRankFilename);

        for (const auto &entry: bestMovesVector) {
            moveFile << "Move: " << entry.first << ", Value: " << entry.second << "\n";
        }

        moveFile.close();
    }

    static void calculatePokemonStatistics(
            const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> &attackerToMoveToPokemonDefeated,
            const std::unordered_map<std::string, int> &pokemonToTimesDefeated,
            std::string &rankFileName,
            std::string &moveRankFilename
    ) {
        std::unordered_map<double, std::vector<std::pair<std::string, std::unordered_map<std::string, double>>>> attackerRankToPokemon;
        std::unordered_map<std::string, double> bestMoves;

        for (auto &[attackerName, moveToPokemonDefeated]: attackerToMoveToPokemonDefeated) {

            std::unordered_map<std::string, double> bestAttackerMoves;
            size_t numberOfPokemonDefeatedByAttacker = 0;
            for (auto &[move, pokemonDefeated]: moveToPokemonDefeated) {
                bestAttackerMoves[move] += static_cast<double>(pokemonDefeated.size());
                numberOfPokemonDefeatedByAttacker += pokemonDefeated.size();
            }

            for (auto &[move, pokemonDefeated]: moveToPokemonDefeated) {
                for (auto &pokemon: pokemonDefeated) {
                    bestMoves[move] += 1.0 / pokemonToTimesDefeated.at(pokemon);
                }
                bestAttackerMoves[move] /= static_cast<double>(numberOfPokemonDefeatedByAttacker);
            }
            double rank = static_cast<double>(numberOfPokemonDefeatedByAttacker) / battle_field::NUMBER_OF_POKEMON;
            attackerRankToPokemon[rank].emplace_back(attackerName, bestAttackerMoves);
        }
        saveStats(attackerRankToPokemon, bestMoves, rankFileName, moveRankFilename);
    }

};

#endif //GENIITEAMBUILDER_BATTLESIMULATOR_H
