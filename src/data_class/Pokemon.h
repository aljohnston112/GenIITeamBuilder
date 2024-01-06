#ifndef GENIITEAMBUILDER_POKEMON_H
#define GENIITEAMBUILDER_POKEMON_H

#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>
#include "PokemonInformation.h"
#include "AllStats.h"
#include "Attack.h"

class Pokemon {
public:
    const PokemonInformation pokemonInformation;
    const AllStats allStats;
    const std::unordered_map<int, std::vector<Attack>> attacks;

    Pokemon(
            PokemonInformation  pokemon_info,
            AllStats all_stats,
            std::unordered_map<int, std::vector<Attack>> &attacks
    ) :
            pokemonInformation(std::move(pokemon_info)),
            allStats(std::move(all_stats)),
            attacks(attacks){}
};

#endif //GENIITEAMBUILDER_POKEMON_H
