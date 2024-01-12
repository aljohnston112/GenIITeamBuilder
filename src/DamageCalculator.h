#ifndef GENIITEAMBUILDER_DAMAGECALCULATOR_H
#define GENIITEAMBUILDER_DAMAGECALCULATOR_H

#include <vector>
#include <string>
#include <memory>
#include "data_class/PokemonState.h"

class DamageCalculator {
private:
    static const std::vector<std::pair<std::string, int>> buffedAttackPowers;
    static const std::vector<std::pair<std::string, int>> nonBuffedAttackPowers;

    static double getAttackStat(const std::shared_ptr<PokemonState> &pokemonState, bool isSpecial);

    static double getDefenseStat(const std::shared_ptr<PokemonState> &pokemonState, bool isSpecial);

public:
    static int getAttackPower(
            const Attack &attack,
            const std::shared_ptr<PokemonState> attackerState,
            const std::shared_ptr<PokemonState> defender
    );

    static std::pair<double, std::shared_ptr<Attack>> getMaxDamageAttackerCanDoToDefender(
            std::shared_ptr<PokemonState> attacker_state,
            std::shared_ptr<PokemonState> defender_state,
            bool attacker_buffed,
            int level = 50
    );

};

#endif //GENIITEAMBUILDER_DAMAGECALCULATOR_H
