#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <future>
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
    std::vector<std::string> pokemonNames{};
    pokemonNames.reserve(251);
    for (auto &m: document.GetObject()) {
        pokemonNames.emplace_back(m.value["pokemon_information"]["name"].GetString());
    }
    return pokemonNames;
}

void battle(
        std::string attacker,
        std::string defender,
        std::promise<std::string> &promise
) {
    promise.set_value(defender);
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

    ThreadPool threadPool;
    std::vector<BattleFunction> functions;
    functions.reserve(pokemonNames.size() * pokemonNames.size());

    logFunctionTime(
            [&pokemonNames, &battleFutures, &battlePromises, &functions, &threadPool] {
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
    return 0;
}

int main() {
    logFunctionTime(
            [] {
                battleTime();
                return "";
            },
            "Program run time: "
    );

    return 0;
}
