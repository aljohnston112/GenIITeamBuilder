#include <memory>
#include "PokemonStateCache.h"

std::shared_ptr<PokemonState> PokemonStateCache::getAttackerPokemonState(Pokemon &pokemon, StatModifiers &statModifiers) {
    if (!attackerPokemonStates.find(pokemon.pokemonInformation.name)) {
        attackerPokemonStates.insert(
                pokemon.pokemonInformation.name,
                std::make_shared<PokemonState>(
                        PokemonState(
                                PokemonStateSubset(
                                        pokemon,
                                        statModifiers,
                                        true
                                ),
                                false
                        )
                )
        );
    }
    auto state = attackerPokemonStates.get(pokemon.pokemonInformation.name).value();
    return std::make_shared<PokemonState>(state->afterRestoring());
}

std::shared_ptr<PokemonState> PokemonStateCache::getDefenderPokemonState(Pokemon &pokemon, StatModifiers &statModifiers) {
    if (!defenderPokemonStates.find(pokemon.pokemonInformation.name)) {
        defenderPokemonStates.insert(
                pokemon.pokemonInformation.name,
                std::make_shared<PokemonState>(
                        PokemonState(
                                PokemonStateSubset(
                                        pokemon,
                                        statModifiers,
                                        false
                                ),
                                true
                        )
                )
        );
    }
    auto state = defenderPokemonStates.get(pokemon.pokemonInformation.name).value();
    state = std::make_shared<PokemonState>(state->afterRestoring());
    return state;
}