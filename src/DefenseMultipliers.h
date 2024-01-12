#ifndef GENIITEAMBUILDER_DEFENSEMULTIPLIERS_H
#define GENIITEAMBUILDER_DEFENSEMULTIPLIERS_H

#include <unordered_map>
#include <vector>
#include "data_class/PokemonType.h"

using DefenseMultiplierMap = std::unordered_map<PokemonType, double>;

class DefenseMultipliers {
public:
    static DefenseMultiplierMap& getDefenseMultipliersForType(
            const PokemonType &pokemonType,
            DefenseMultiplierMap &currentDefenseMultipliers
    );

    static DefenseMultiplierMap getDefenseMultipliersForTypes(const std::vector<PokemonType> &defenderTypes);

};


#endif //GENIITEAMBUILDER_DEFENSEMULTIPLIERS_H
