#ifndef GENIITEAMBUILDER_POKEMONTYPE_H
#define GENIITEAMBUILDER_POKEMONTYPE_H

#include <string>

enum class PokemonType {
    NORMAL,
    FIGHTING,
    FLYING,
    POISON,
    GROUND,
    ROCK,
    BUG,
    GHOST,
    STEEL,
    FIRE,
    WATER,
    GRASS,
    ELECTRIC,
    PSYCHIC,
    ICE,
    DRAGON,
    DARK,
    Count
};

PokemonType convertToPokemonType(const std::string &pokemon_type);

#endif //GENIITEAMBUILDER_POKEMONTYPE_H
