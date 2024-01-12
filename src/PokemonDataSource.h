#ifndef GENIITEAMBUILDER_POKEMONDATASOURCE_H
#define GENIITEAMBUILDER_POKEMONDATASOURCE_H

#include <vector>
#include <unordered_map>
#include "data_class/Pokemon.h"
#include "rapidjson/document.h"

class PokemonDataSource {

private:
    static constexpr char allPokemonFile[] = "data/all_pokemon.json";
    static rapidjson::Document document;
    static std::vector<Pokemon> allPokemon;

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
            std::unordered_map<int, std::vector<Attack>> &attacks
    );

    static void extractGenIIAttacks(
            const auto &genIIAttacks,
            std::unordered_map<int, std::vector<Attack>> &attacks
    );

    static void extractAttacksFromList(
            const auto &attackList,
            std::unordered_map<int, std::vector<Attack>> &attacks
    );

    static void extractTmHmAttacks(
            const auto &tmHmAttacks,
            std::unordered_map<int, std::vector<Attack>> &attacks
    );

    static Pokemon extractPokemon(const auto &pokemon);

public:

    static std::vector<Pokemon> getAllPokemon();

};

#endif //GENIITEAMBUILDER_POKEMONDATASOURCE_H
