#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <future>
#include <thread>
#include <queue>

#include "rapidjson/document.h"
#include "MyHandler.h"
#include "Timer.h"
#include "ThreadPool.h"

using namespace rapidjson;

namespace FileNames {
    const std::string ALL_POKEMON_FILE = "data/all_pokemon.json";
}

auto read_all_pokemon() {
    std::stringstream buffer;
    auto allPokemonFile = FileNames::ALL_POKEMON_FILE;
    std::ifstream file(allPokemonFile);
    if (!file.is_open()) {
        throw std::filesystem::filesystem_error(
                "Unable to open file.",
                std::filesystem::path(allPokemonFile),
                std::make_error_code(std::errc::no_such_file_or_directory));
    }
    buffer << file.rdbuf();
    return buffer;
}

std::vector<std::string> extractAllPokemonNames(const auto &document) {
    std::vector<std::string> pokemonNames{251};
    for (auto &m: document.GetObject()) {
        pokemonNames.emplace_back(m.name.GetString());
    }
    return pokemonNames;
}

void battle(
        std::string attacker,
        std::string defender,
        std::promise<std::string> &promise
) {
    promise.set_value(attacker);
    // TODO
}

int battleTime() {
    auto allPokemonJSON = read_all_pokemon().str();
    StringStream buffer(allPokemonJSON.c_str());
    Document document;
    document.ParseStream(buffer);
    auto pokemonNames = extractAllPokemonNames(document);

    std::vector<std::future<std::string>> battleFutures;
    std::vector<std::shared_ptr<std::promise<std::string>>> battlePromises;

    ThreadPool<std::function<void(void)>> threadPool;

    logFunctionTime(
            [&pokemonNames, &battleFutures, &battlePromises, &threadPool] {
                for (const std::string &attackingPokemon: pokemonNames) {
                    for (const std::string &defendingPokemon: pokemonNames) {
                        auto battlePromise = std::make_shared<std::promise<std::string>>();
                        battleFutures.emplace_back(battlePromise->get_future());
                        battlePromises.emplace_back(battlePromise);
                        threadPool.addTask(
                                [attackingPokemon, defendingPokemon, battlePromise] {
                                    battle(
                                            attackingPokemon,
                                            defendingPokemon,
                                            *battlePromise);
                                }
                        );
                    }
                }
                std::stringstream stringstream;
                for (auto &future: battleFutures) {
                    stringstream << (future.get().c_str()) << std::endl;
                }
                return stringstream.str();
            },
            "Loop time: "
    );
    return 0;
}

int main() {
//    auto num_threads = 12;
//    std::vector<std::jthread> threads{12};
//    for (int i = 0; i < num_threads; ++i) {
//        threads.emplace_back(std::jthread());
//    }


    logFunctionTime (
            [] {
                battleTime();
                return "";
            },
                    "Program run time: "
    );

    return 0;
}
