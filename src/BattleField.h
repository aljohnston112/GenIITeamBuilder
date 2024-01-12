#ifndef GENIITEAMBUILDER_BATTLEFIELD_H
#define GENIITEAMBUILDER_BATTLEFIELD_H

#include <memory>
#include <future>
#include "data_class/PokemonState.h"
#include "data_class/BattleResult.h"

class BattleField {

public:
    static void battle(
            std::shared_ptr<PokemonState> attacker,
            std::shared_ptr<PokemonState> defender,
            std::shared_ptr<std::promise<BattleResult>> promise
    );

};

#endif //GENIITEAMBUILDER_BATTLEFIELD_H
