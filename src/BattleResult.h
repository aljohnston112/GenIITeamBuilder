#ifndef GENIITEAMBUILDER_BATTLERESULT_H
#define GENIITEAMBUILDER_BATTLERESULT_H

#include <string>
#include <utility>

class BattleResult {
public:
    BattleResult() : attackerWon(false) {}

    BattleResult(
            std::string attackerName,
            std::string attackName,
            std::string defenderName
    ) : attackerWon(true),
        attackerName(std::move(attackerName)),
        attackName(std::move(attackName)),
        defenderName(std::move(defenderName)) {}

    [[nodiscard]] bool isAttackerWon() const {
        return attackerWon;
    }

    [[nodiscard]] const std::string &getAttackerName() const {
        return attackerName;
    }

    [[nodiscard]] const std::string &getAttackName() const {
        return attackName;
    }

    [[nodiscard]] const std::string &getDefenderName() const {
        return defenderName;
    }

private:
    bool attackerWon;
    std::string attackerName;
    std::string attackName;
    std::string defenderName;
};

#endif //GENIITEAMBUILDER_BATTLERESULT_H
