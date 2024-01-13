#include <sstream>
#include <fstream>
#include <filesystem>
#include <optional>
#include "rapidjson/document.h"
#include "PokemonDataSource.h"
#include <algorithm>

rapidjson::Document PokemonDataSource::document;
std::vector<Pokemon> PokemonDataSource::allPokemon;

std::stringstream PokemonDataSource::readAllPokemonFile() {
    std::stringstream buffer;
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

PokemonInformation PokemonDataSource::extractPokemonInformation(const auto &pokemonInformation) {
    auto id = pokemonInformation["id"].GetInt();
    auto name = pokemonInformation["name"].GetString();
    auto pounds = pokemonInformation["pounds"].GetDouble();
    auto typeListJSON = pokemonInformation["pokemon_types"].GetArray();
    std::vector<PokemonType> types;
    for (auto &typeValue: typeListJSON) {
        types.emplace_back(convertToPokemonType(typeValue.GetString()));
    }
    return PokemonInformation(name, types, id, pounds);

}

Stats PokemonDataSource::extractStats(const auto &stats) {
    return Stats(
            stats["health"].GetInt(),
            stats["attack"].GetInt(),
            stats["defense"].GetInt(),
            stats["special_attack"].GetInt(),
            stats["special_defense"].GetInt(),
            stats["speed"].GetInt()
    );
}


BaseStats PokemonDataSource::extractBaseStats(const auto &baseStats) {
    return BaseStats(extractStats(baseStats["stats"].GetObject()));
}

AllStats PokemonDataSource::extractAllStats(const auto &allStats) {
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

Attack PokemonDataSource::getAttack(
        const auto &attack
) {
    return Attack(
            attack["name"].GetString(),
            convertToPokemonType(attack["pokemon_type"].GetString()),
            convertToAttackCategory(attack["category"].GetString()),
            attack["power"].GetInt(),
            attack["accuracy"].GetInt(),
            attack["effect_percent"].GetInt()
    );
}

void PokemonDataSource::addAttacks(
        int level,
        const auto &attackArray,
        std::unordered_map<int, std::vector<Attack>> &attacks
) {
    for (auto &attack: attackArray) {
        (attacks)[level].emplace_back(getAttack(attack));
    }
}

void PokemonDataSource::extractGenIIAttacks(
        const auto &genIIAttacks,
        std::unordered_map<int, std::vector<Attack>> &attacks
) {
    for (auto &m: genIIAttacks.GetObject()) {
        int level = std::stoi(m.name.GetString());
        auto attackArray = m.value.GetArray();
        addAttacks(level, attackArray, attacks);
    }
}

void PokemonDataSource::extractAttacksFromList(
        const auto &attackList,
        std::unordered_map<int, std::vector<Attack>> &attacks
) {
    if (attackList.IsArray()) {
        for (auto &m: attackList.GetArray()) {
            auto attackObject = m.GetObject();
            (attacks)[-2].emplace_back(getAttack(attackObject));
        }
    } else {
        if (attackList.GetType() != 0) {
            throw std::runtime_error("");
        }
    }
    printf("");
}

void PokemonDataSource::extractTmHmAttacks(
        const auto &tmHmAttacks,
        std::unordered_map<int, std::vector<Attack>> &attacks
) {
    if (tmHmAttacks.IsObject()) {
        for (auto &m: tmHmAttacks.GetObject()) {
            auto attackObject = m.value.GetObject();
            (attacks)[-1].emplace_back(getAttack(attackObject));
        }
    } else {
        if (tmHmAttacks.GetType() != 0) {
            throw std::runtime_error("");
        }
    }
}

std::optional<Pokemon> PokemonDataSource::extractPokemon(const auto &pokemon) {
    const PokemonInformation pokemonInformation = extractPokemonInformation(pokemon["pokemon_information"]);
    const std::vector<std::string> doNotInclude{
            "Articuno",
            "Zapdos",
            "Moltres",
            "Raikou",
            "Entei",
            "Suicune",
            "Mewtwo",
            "Lugia",
            "Ho-Oh",
            "Mew",
            "Celebi",
            "Dragonite",
            "Tyranitar"
    };
    std::optional<Pokemon> returnMe{};
    if (std::find(doNotInclude.begin(), doNotInclude.end(), pokemonInformation.name) == doNotInclude.end()) {
        const AllStats allStats = extractAllStats(pokemon["all_stats"]);

        auto attacks = std::unordered_map<int, std::vector<Attack>>();
        extractGenIIAttacks(pokemon["genII_level_to_attacks"], attacks);
        extractAttacksFromList(pokemon["genI_attacks"], attacks);
        extractTmHmAttacks(pokemon["tm_or_hm_to_attack"], attacks);
        extractAttacksFromList(pokemon["move_tutor_attacks"], attacks);
        extractAttacksFromList(pokemon["egg_moves"], attacks);
        extractAttacksFromList(pokemon["pre_evolution_attacks"], attacks);
        extractAttacksFromList(pokemon["special_attacks"], attacks);
        returnMe.emplace(
                Pokemon{
                        pokemonInformation,
                        allStats,
                        attacks
                }
        );
    }
    return returnMe;
}

std::vector<Pokemon> PokemonDataSource::getAllPokemon() {
    if (allPokemon.empty()) {
        std::string allPokemonJSON = readAllPokemonFile().str();
        rapidjson::StringStream buffer(allPokemonJSON.c_str());
        document.ParseStream(buffer);
        for (auto &m: document.GetObject()) {
            auto pokemon = extractPokemon(m.value.GetObject());
            if(pokemon.has_value()) {
                allPokemon.emplace_back(
                    pokemon.value()
                );
            }
        }
    }
    return allPokemon;
}
