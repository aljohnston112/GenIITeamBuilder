#ifndef GENIITEAMBUILDER_POKEMONDATASOURCE_H
#define GENIITEAMBUILDER_POKEMONDATASOURCE_H

#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <iostream>
#include "rapidjson/document.h"
#include "data_class/PokemonType.h"
#include "data_class/PokemonInformation.h"
#include "data_class/AllStats.h"
#include "data_class/Attack.h"
#include "data_class/Pokemon.h"

using namespace rapidjson;

class PokemonDataSource {

private:
    static constexpr char ALL_POKEMON_FILE[] = "data/all_pokemon.json";
    static Document document;
    static std::vector<Pokemon> ALL_POKEMON;

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

    static Attack getAttack (
            const auto &attack
    ) {
        return Attack(
                attack["name"].GetString(),
                convert_to_pokemon_type(attack["pokemon_type"].GetString()),
                convertToAttackCategory(attack["category"].GetString()),
                attack["power"].GetInt(),
                attack["accuracy"].GetInt(),
                attack["effect_percent"].GetInt()
        );
    }

    static void addAttacks(
            int level,
            const auto &attackArray,
            const std::shared_ptr<std::unordered_map<int, std::vector<Attack>>> &attacks
    ) {
        for (auto &attack: attackArray) {
            (*attacks)[level].emplace_back(getAttack(attack));
        }
    }

    static void extractGenIIAttacks(
            const auto &genIIAttacks,
            const std::shared_ptr<std::unordered_map<int, std::vector<Attack>>> &attacks
    ) {
        for (auto &m: genIIAttacks.GetObject()) {
            int level = std::stoi(m.name.GetString());
            auto attackArray = m.value.GetArray();
            addAttacks(level, attackArray, attacks);
        }
    }

    static void extractAttacksFromList(
            const auto &attackList,
            const std::shared_ptr<std::unordered_map<int, std::vector<Attack>>> &attacks
    ) {
        if (attackList.IsArray()) {
            for (auto &m: attackList.GetArray()) {
                auto attackObject = m.GetObject();
                (*attacks)[-2].emplace_back(getAttack(attackObject));
            }
        } else {
            if (attackList.GetType() != 0) {
                throw std::runtime_error("");
            }
        }
        printf("");
    }

    static void extractTmHmAttacks(
            const auto &tmHmAttacks,
            const std::shared_ptr<std::unordered_map<int, std::vector<Attack>>> &attacks
    ) {
        if (tmHmAttacks.IsObject()) {
            for (auto &m: tmHmAttacks.GetObject()) {
                auto attackObject = m.value.GetObject();
                (*attacks)[-1].emplace_back(getAttack(attackObject));
            }
        } else {
            if (tmHmAttacks.GetType() != 0) {
                throw std::runtime_error("");
            }
        }
    }

    static Pokemon extractPokemon(const auto &pokemon) {
        const PokemonInformation pokemonInformation = extractPokemonInformation(pokemon["pokemon_information"]);
        const AllStats allStats = extractAllStats(pokemon["all_stats"]);

        const auto attacks = std::make_shared<std::unordered_map<int, std::vector<Attack>>>();
        extractGenIIAttacks(pokemon["genII_level_to_attacks"], attacks);
        extractAttacksFromList(pokemon["genI_attacks"], attacks);
        extractTmHmAttacks(pokemon["tm_or_hm_to_attack"], attacks);
        extractAttacksFromList(pokemon["move_tutor_attacks"], attacks);
        extractAttacksFromList(pokemon["egg_moves"], attacks);
        extractAttacksFromList(pokemon["pre_evolution_attacks"], attacks);
        extractAttacksFromList(pokemon["special_attacks"], attacks);

        for (auto& pair : *attacks) {
            pair.second = std::move(std::as_const(pair.second));
        }

        return {
                pokemonInformation,
                allStats,
                static_cast<const std::shared_ptr<std::unordered_map<const int, const std::vector<Attack>>>>(attacks)
        };
    }

public:

    static std::vector<Pokemon> getAllPokemon() {
        if (ALL_POKEMON.empty()) {
            std::string allPokemonJSON = readAllPokemonFile().str();
            StringStream buffer(allPokemonJSON.c_str());
            document.ParseStream(buffer);
            for (auto &m: document.GetObject()) {
                ALL_POKEMON.emplace_back(
                        extractPokemon(m.value.GetObject())
                );
            }
        }
        return ALL_POKEMON;
    }

    static std::vector<std::string> getAllPokemonNames() {
        std::vector<std::string> pokemonNames{};
        pokemonNames.reserve(251);
        for (auto &m: document.GetObject()) {
            pokemonNames.emplace_back(m.name.GetString());
        }
        return pokemonNames;
    }

};

#endif //GENIITEAMBUILDER_POKEMONDATASOURCE_H
