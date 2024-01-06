#ifndef GENIITEAMBUILDER_POKEMONINFORMATION_H
#define GENIITEAMBUILDER_POKEMONINFORMATION_H

#include <string>
#include <utility>
#include "PokemonType.h"

class PokemonInformation {
public:
    const std::string name;
    const std::vector<PokemonType> pokemonTypes;
    const int id;
    const float pounds;

    PokemonInformation(
            std::string name,
            const std::vector<PokemonType>& types,
            int i,
            float p
            ): name(std::move(name)), pokemonTypes(types), id(i), pounds(p) {}
};

#endif //GENIITEAMBUILDER_POKEMONINFORMATION_H