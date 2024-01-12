#ifndef GENIITEAMBUILDER_POKEMONINFORMATION_H
#define GENIITEAMBUILDER_POKEMONINFORMATION_H

#include <vector>
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
            const int id,
            const float pounds
    ) : name(name), pokemonTypes(types), id(id), pounds(pounds) {}
};

#endif //GENIITEAMBUILDER_POKEMONINFORMATION_H