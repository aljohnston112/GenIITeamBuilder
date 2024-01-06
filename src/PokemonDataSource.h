#ifndef GENIITEAMBUILDER_POKEMONDATASOURCE_H
#define GENIITEAMBUILDER_POKEMONDATASOURCE_H

#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include "rapidjson/document.h"
#include "data_class/PokemonType.h"
#include "data_class/PokemonInformation.h"
#include "data_class/AllStats.h"

using namespace rapidjson;

class PokemonDataSource {

private:
    static constexpr char ALL_POKEMON_FILE[] = "data/all_pokemon.json";
    static Document document;

    static std::stringstream readAllPokemonFile() {
        std::stringstream buffer;
        auto allPokemonFile = ALL_POKEMON_FILE;
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

    static PokemonInformation extractPokemonInformation(const auto &pokemonInformation) {
        auto id = pokemonInformation["id"].GetInt();
        auto name = pokemonInformation["name"].GetString();
        auto pounds = pokemonInformation["pounds"].GetDouble();
        auto typeListJSON = pokemonInformation["pokemon_types"].GetArray();
        std::vector<PokemonType> types;
        for (auto &typeValue: typeListJSON) {
            types.emplace_back(convert_to_pokemon_type(typeValue.GetString()));
        }
        return PokemonInformation(name, types, id, pounds);

    }

    static Stats extractStats(const auto &stats) {
        return Stats(
        stats["health"].GetInt(),
        stats["attack"].GetInt(),
        stats["defense"].GetInt(),
        stats["special_attack"].GetInt(),
        stats["special_defense"].GetInt(),
        stats["speed"].GetInt()
        );
    }


    static BaseStats extractBaseStats(const auto &baseStats) {
        return BaseStats(extractStats(baseStats["stats"].GetObject()));
    }

    static AllStats extractAllStats(const auto &allStats) {
        auto baseStats = extractBaseStats(allStats["base_stats"].GetObject());
        auto level50MinStats = extractStats(allStats["level_50_min_stats"].GetObject());
        auto level50MaxStats = extractStats(allStats["level_50_max_stats"].GetObject());
        auto level100MinStats = extractStats(allStats["level_100_min_stats"].GetObject());
        auto level100MaxStats = extractStats(allStats["level_100_max_stats"].GetObject());
        return AllStats(
                baseStats,
                level50MinStats,
                level50MaxStats,
                level100MinStats,
                level100MaxStats
        );
    }

    static void extractGenIIAttacks(const auto &genIIAttacks) {}

    static void extractGenIAttacks(const auto &genIAttacks) {}

    static void extractTmHmAttacks(const auto &tmHmAttacks) {}

    static void extractMoveTutorAttacks(const auto &moveTutorAttacks) {}

    static void extractEggMoves(const auto &eggMoves) {}

    static void extractPreEvolutionAttacks(const auto &preEvolutionAttacks) {}

    static void extractSpecialAttacks(const auto &specialAttacks) {}

    static void extractPokemon(const auto &pokemon) {
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

    static void parseData() {
        for (auto &m: document.GetObject()) {
            GenericObject pokemonInformation = m.value.GetObject();
            extractPokemon(pokemonInformation);
        }
    }

public:

    static void initialize() {
        std::string allPokemonJSON = readAllPokemonFile().str();
        StringStream buffer(allPokemonJSON.c_str());
        document.ParseStream(buffer);
        parseData();
    }

    static std::vector<std::string> getAllPokemonNames() {
        std::vector<std::string> pokemonNames{};
        pokemonNames.reserve(251);
        for (auto &m: document.GetObject()) {
            pokemonNames.emplace_back(m.name.GetString());
        }
        return pokemonNames;
    }

    static auto &getPokemonObjectByIndex(const std::string &id) {
        auto pokemon = document.FindMember(id.c_str());
        return pokemon->value;
    }

};

#endif //GENIITEAMBUILDER_POKEMONDATASOURCE_H
