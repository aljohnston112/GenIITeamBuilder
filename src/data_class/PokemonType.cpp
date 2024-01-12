#include <unordered_map>
#include "PokemonType.h"

PokemonType convertToPokemonType(const std::string& pokemon_type) {
    static std::unordered_map<std::string, PokemonType> typeDict = {
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

    return typeDict[pokemon_type];
}