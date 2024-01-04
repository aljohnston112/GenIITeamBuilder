#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

#include "rapidjson/document.h"
#include "MyHandler.h"
#include "Timer.h"
#include "PokemonType.h"
#include "PokemonInformation.h"


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

PokemonInformation extractPokemonInformation(const auto& pokemonInformation) {
    auto id = pokemonInformation["id"].GetInt();
    auto name = pokemonInformation["name"].GetString();
    auto pounds = pokemonInformation["pounds"].GetDouble();
    auto typeListJSON = pokemonInformation["pokemon_types"].GetArray();
    std::vector<PokemonType> types;
    for (auto& typeValue : typeListJSON){
        types.emplace_back(convert_to_pokemon_type(typeValue.GetString()));
    }
    return PokemonInformation(name, types, id, pounds);

}

void extractAllStats(const auto& allStats) {}

void extractGenIIAttacks(const auto& genIIAttacks) {}

void extractGenIAttacks(const auto& genIAttacks) {}

void extractTmHmAttacks(const auto& tmHmAttacks) {}

void extractMoveTutorAttacks(const auto& moveTutorAttacks) {}

void extractEggMoves(const auto& eggMoves) {}

void extractPreEvolutionAttacks(const auto& preEvolutionAttacks) {}

void extractSpecialAttacks(const auto& specialAttacks) {}

void extractPokemon(const auto& pokemon) {
    extractPokemonInformation(pokemon["pokemon_information"]);
    extractAllStats(pokemon["all_stats"]);
    extractGenIIAttacks(pokemon["genII_level_to_attacks"]);
    extractGenIAttacks(pokemon["genI_attacks"]);
    extractTmHmAttacks(pokemon["tm_or_hm_to_attack"]);
    extractMoveTutorAttacks(pokemon["move_tutor_attacks"]);
    extractEggMoves(pokemon["egg_moves"]);
    extractPreEvolutionAttacks(pokemon["pre_evolution_attacks"]);
    extractSpecialAttacks(pokemon["special_attacks"]);
}

int main() {
    logFunctionTime<int>(
            [] {
                auto allPokemonJSON = read_all_pokemon().str();
                StringStream buffer(allPokemonJSON.c_str());

                Document document;
                document.ParseStream(buffer);
                for (auto& m : document.GetObject()) {
                    GenericObject pokemonInformation = m.value.GetObject();
                    extractPokemon(pokemonInformation);
                }

                return 0;
            },
            "Program run time: "
    );

    return 0;
}
