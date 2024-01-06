
#include "data_class/PokemonType.h"

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

std::vector<PokemonType> pokemonTypes = {
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