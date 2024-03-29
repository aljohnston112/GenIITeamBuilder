#ifndef GENIITEAMBUILDER_POKEMON_H
#define GENIITEAMBUILDER_POKEMON_H

#include <unordered_map>
#include "PokemonInformation.h"
#include "AllStats.h"
#include "Attack.h"

class Pokemon {

public:
    const PokemonInformation pokemonInformation;
    const AllStats allStats;
    const std::unordered_map<int, std::vector<Attack>> attacks;

    Pokemon(
            const PokemonInformation &pokemon_info,
            const AllStats &all_stats,
            const std::unordered_map<int, std::vector<Attack>> &attacks
    ) :
            pokemonInformation(pokemon_info),
            allStats(all_stats),
            attacks(attacks) {}
};

#endif //GENIITEAMBUILDER_POKEMON_H
