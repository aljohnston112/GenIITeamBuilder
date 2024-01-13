#ifndef GENIITEAMBUILDER_POKEMONSTATECACHE_H
#define GENIITEAMBUILDER_POKEMONSTATECACHE_H

#include <memory>
#include "data_class/PokemonState.h"
#include "ThreadSafeCache.h"

class PokemonStateCache {
private:
    // Caching the PokemonStates only saved about 20ms.
    ThreadSafeCache<std::string, std::shared_ptr<PokemonState>> defenderPokemonStates;
    ThreadSafeCache<std::string, std::shared_ptr<PokemonState>> attackerPokemonStates;

public:
    std::shared_ptr<PokemonState> getAttackerPokemonState(Pokemon &pokemon, StatModifiers &statModifiers);

    std::shared_ptr<PokemonState> getDefenderPokemonState(Pokemon &pokemon, StatModifiers &statModifiers);

    void invalidate();
};

#endif //GENIITEAMBUILDER_POKEMONSTATECACHE_H
