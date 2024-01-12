#ifndef GENIITEAMBUILDER_BATTLERESULT_H
#define GENIITEAMBUILDER_BATTLERESULT_H

#include <string>

class BattleResult {
public:
    const bool attackerWon;
    const std::string attackerName;
    const std::string attackName;
    const std::string defenderName;

    BattleResult(
            const std::string& attackerName,
            const std::string& attackName,
            const std::string& defenderName,
            const bool attackerWon
    ) : attackerWon(attackerWon),
        attackerName(attackerName),
        attackName(attackName),
        defenderName(defenderName) {}

};

#endif //GENIITEAMBUILDER_BATTLERESULT_H
