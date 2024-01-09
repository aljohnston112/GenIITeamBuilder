#ifndef GENIITEAMBUILDER_POKEMONSTATESUBSET_H
#define GENIITEAMBUILDER_POKEMONSTATESUBSET_H

#include "Pokemon.h"
#include "StatModifiers.h"

class PokemonStateSubset {
public:
    const Pokemon pokemon;
    const StatModifiers statModifiers;
    const bool minStats;

    PokemonStateSubset(
            const Pokemon &pokemon,
            const StatModifiers &stat_modifiers,
            const bool minStats
    ) : pokemon(pokemon),
        statModifiers(stat_modifiers),
        minStats(minStats) {}
};

#endif //GENIITEAMBUILDER_POKEMONSTATESUBSET_H
