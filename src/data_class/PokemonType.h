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
    DARK
};

std::unordered_map<std::string, PokemonType> TYPE_DICT = {
        {"normal", PokemonType::NORMAL},
        {"fighting", PokemonType::FIGHTING},
        {"flying", PokemonType::FLYING},
        {"poison", PokemonType::POISON},
        {"ground", PokemonType::GROUND},
        {"rock", PokemonType::ROCK},
        {"bug", PokemonType::BUG},
        {"ghost", PokemonType::GHOST},
        {"steel", PokemonType::STEEL},
        {"fire", PokemonType::FIRE},
        {"water", PokemonType::WATER},
        {"grass", PokemonType::GRASS},
        {"electric", PokemonType::ELECTRIC},
        {"psychic", PokemonType::PSYCHIC},
        {"ice", PokemonType::ICE},
        {"dragon", PokemonType::DRAGON},
        {"dark", PokemonType::DARK}
};

std::vector<PokemonType> pokemon_types = {
        PokemonType::NORMAL,
        PokemonType::FIGHTING,
        PokemonType::FLYING,
        PokemonType::POISON,
        PokemonType::GROUND,
        PokemonType::ROCK,
        PokemonType::BUG,
        PokemonType::GHOST,
        PokemonType::STEEL,
        PokemonType::FIRE,
        PokemonType::WATER,
        PokemonType::GRASS,
        PokemonType::ELECTRIC,
        PokemonType::PSYCHIC,
        PokemonType::ICE,
        PokemonType::DRAGON,
        PokemonType::DARK
};

PokemonType convert_to_pokemon_type(const std::string& pokemon_type) {
    return TYPE_DICT[pokemon_type];
}

#endif //GENIITEAMBUILDER_POKEMONTYPE_H
