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

    static std::stringstream readAllPokemonFile();

    static PokemonInformation extractPokemonInformation(const auto &pokemonInformation);

    static Stats extractStats(const auto &stats);


    static BaseStats extractBaseStats(const auto &baseStats);

    static AllStats extractAllStats(const auto &allStats);

    static Attack getAttack(
            const auto &attack
    );

    static void addAttacks(
            int level,
            const auto &attackArray,
            const std::shared_ptr<std::unordered_map<int, std::vector<Attack>>> &attacks
    );

    static void extractGenIIAttacks(
            const auto &genIIAttacks,
            const std::shared_ptr<std::unordered_map<int, std::vector<Attack>>> &attacks
    );

    static void extractAttacksFromList(
            const auto &attackList,
            const std::shared_ptr<std::unordered_map<int, std::vector<Attack>>> &attacks
    );

    static void extractTmHmAttacks(
            const auto &tmHmAttacks,
            const std::shared_ptr<std::unordered_map<int, std::vector<Attack>>> &attacks
    );

    static Pokemon extractPokemon(const auto &pokemon);

public:

    static std::vector<Pokemon> getAllPokemon();

    static std::vector<std::string> getAllPokemonNames();

};

#endif //GENIITEAMBUILDER_POKEMONDATASOURCE_H
