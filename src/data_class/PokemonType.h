#ifndef GENIITEAMBUILDER_POKEMONTYPE_H
#define GENIITEAMBUILDER_POKEMONTYPE_H

#include <unordered_map>
#include <string>
#include <vector>

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

PokemonType convert_to_pokemon_type(const std::string& pokemon_type);

#endif //GENIITEAMBUILDER_POKEMONTYPE_H
