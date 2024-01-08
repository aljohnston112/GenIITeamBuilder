#ifndef GENIITEAMBUILDER_DEFENSEMULTIPLIERS_H
#define GENIITEAMBUILDER_DEFENSEMULTIPLIERS_H

#include <unordered_map>
#include <vector>
#include <string>
#include "data_class/PokemonType.h"
#include "TypeChartDataSource.h"

using DefenseMultipliers = std::unordered_map<PokemonType, double>;

DefenseMultipliers getDefenseMultipliersForType(
        const PokemonType& pokemonType,
        DefenseMultipliers& currentDefenseMultipliers
) {
    auto typeChartDefend = getDefendTypeDict();

    std::vector<std::vector<PokemonType>> typeChart = {
            typeChartDefend[0].at(pokemonType),
            typeChartDefend[1].at(pokemonType),
            typeChartDefend[2].at(pokemonType),
            typeChartDefend[3].at(pokemonType)
    };

    for (const PokemonType& noEffectType : typeChart[0]) {
        currentDefenseMultipliers.at(noEffectType) *= 0.0;
    }
    for (const PokemonType& notEffectiveType : typeChart[1]) {
        currentDefenseMultipliers.at(notEffectiveType) *= 0.5;
    }
    for (const PokemonType& normalEffectiveType : typeChart[2]) {
        currentDefenseMultipliers.at(normalEffectiveType) *= 1.0;
    }
    for (const PokemonType& superEffectiveType : typeChart[3]) {
        currentDefenseMultipliers.at(superEffectiveType) *= 2.0;
    }

    return currentDefenseMultipliers;
}

DefenseMultipliers getDefenseMultipliersForTypes(const std::vector<PokemonType>& defenderTypes) {

    DefenseMultipliers defenseMultipliers;
    auto defaultMultiplier = [] { return 1.0; };
    for (const auto& defenderType : defenderTypes) {
        defenseMultipliers[defenderType] = defaultMultiplier();
    }

    for (const auto& defenderType : defenderTypes) {
        defenseMultipliers = getDefenseMultipliersForType(defenderType, defenseMultipliers);
    }
    return defenseMultipliers;
}


#endif //GENIITEAMBUILDER_DEFENSEMULTIPLIERS_H
