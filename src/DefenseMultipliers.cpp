#include "DefenseMultipliers.h"
#include "TypeChartDataSource.h"

DefenseMultiplierMap& DefenseMultipliers::getDefenseMultipliersForType(
        const PokemonType &pokemonType,
        DefenseMultiplierMap &currentDefenseMultipliers
) {
    static auto typeChartDefend = TypeChartDataSource::getDefendTypeDict();

    auto typesImmuneToPokemon = typeChartDefend[0].at(pokemonType);
    for (const PokemonType &typeImmuneToPokemon: (typesImmuneToPokemon)) {
        currentDefenseMultipliers.at(typeImmuneToPokemon) *= 0.0;
    }
    auto typesResistantToPokemon = typeChartDefend[1].at(pokemonType);
    for (const PokemonType &typeResistantToPokemon: typesResistantToPokemon) {
        currentDefenseMultipliers.at(typeResistantToPokemon) *= 0.5;
    }
    auto typesWeakToPokemon = typeChartDefend[3].at(pokemonType);
    for (const PokemonType &typeWeakToPokemon: typesWeakToPokemon) {
        currentDefenseMultipliers.at(typeWeakToPokemon) *= 2.0;
    }

    return currentDefenseMultipliers;
}

DefenseMultiplierMap DefenseMultipliers::getDefenseMultipliersForTypes(const std::vector<PokemonType> &defenderTypes) {

    DefenseMultiplierMap defenseMultipliers;
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