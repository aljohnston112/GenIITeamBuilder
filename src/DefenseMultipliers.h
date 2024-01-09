#ifndef GENIITEAMBUILDER_DEFENSEMULTIPLIERS_H
#define GENIITEAMBUILDER_DEFENSEMULTIPLIERS_H

#include <unordered_map>
#include <vector>
#include <string>
#include "data_class/PokemonType.h"
#include "TypeChartDataSource.h"

using DefenseMultipliers = std::unordered_map<PokemonType, double>;

namespace defense_multipliers {
    static TypeChartDataSource TYPE_CHART_DATA_SOURCE;
}

DefenseMultipliers getDefenseMultipliersForType(
        const PokemonType &pokemonType,
        DefenseMultipliers &currentDefenseMultipliers
) {
    auto typeChartDefend = defense_multipliers::TYPE_CHART_DATA_SOURCE.getDefendTypeDict();

    auto typesImmuneToPokemon = (*typeChartDefend)[0].at(pokemonType);
    for (const PokemonType &typeImmuneToPokemon: (typesImmuneToPokemon)) {
        currentDefenseMultipliers.at(typeImmuneToPokemon) *= 0.0;
    }
    auto typesResistantToPokemon = (*typeChartDefend)[1].at(pokemonType);
    for (const PokemonType &typeResistantToPokemon: typesResistantToPokemon) {
        currentDefenseMultipliers.at(typeResistantToPokemon) *= 0.5;
    }
    auto typesWeakToPokemon = (*typeChartDefend)[3].at(pokemonType);
    for (const PokemonType &typeWeakToPokemon: typesWeakToPokemon) {
        currentDefenseMultipliers.at(typeWeakToPokemon) *= 2.0;
    }

    return currentDefenseMultipliers;
}

DefenseMultipliers getDefenseMultipliersForTypes(const std::vector<PokemonType> &defenderTypes) {

    DefenseMultipliers defenseMultipliers;
    auto defaultMultiplier = [] { return 1.0; };


    for (int i = 0; i < static_cast<int>(PokemonType::Count); ++i) {
        auto enumValue = static_cast<PokemonType>(i);
        defenseMultipliers[enumValue] = defaultMultiplier();
    }

    for (const auto &defenderType: defenderTypes) {
        defenseMultipliers = getDefenseMultipliersForType(defenderType, defenseMultipliers);
    }
    return defenseMultipliers;
}


#endif //GENIITEAMBUILDER_DEFENSEMULTIPLIERS_H
