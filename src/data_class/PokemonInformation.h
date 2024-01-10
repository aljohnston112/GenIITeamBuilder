#ifndef GENIITEAMBUILDER_POKEMONINFORMATION_H
#define GENIITEAMBUILDER_POKEMONINFORMATION_H

#include "PokemonType.h"

class PokemonInformation {

public:
    const std::string name;
    const std::vector<PokemonType> pokemonTypes;
    const int id;
    const float pounds;

    PokemonInformation(
            const std::string &name,
            const std::vector<PokemonType> &types,
            const int i,
            const float p
    ) : name(name), pokemonTypes(types), id(i), pounds(p) {}
};

#endif //GENIITEAMBUILDER_POKEMONINFORMATION_H