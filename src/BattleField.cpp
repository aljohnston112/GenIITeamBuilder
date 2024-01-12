#include <cmath>
#include "BattleField.h"
#include "DamageCalculator.h"

void BattleField::battle(
        std::shared_ptr<PokemonState> attacker,
        std::shared_ptr<PokemonState> defender,
        std::shared_ptr<std::promise<BattleResult>> promise
) {
    static const int level = 50;


    bool attackerBuffed = false;
    bool defenderBuffed = true;

    auto [damageToDefender, attackerAttack] = DamageCalculator::getMaxDamageAttackerCanDoToDefender(
            attacker,
            defender,
            attackerBuffed,
            level
    );
    auto [damageToAttacker, defenderAttack] = DamageCalculator::getMaxDamageAttackerCanDoToDefender(
            defender,
            attacker,
            defenderBuffed,
            level
    );
    double defenderSpeedStat = defender->speed;
    double attackerSpeedStat = attacker->speed;
    bool attackerFirst = attackerSpeedStat > defenderSpeedStat;

    std::string attackerAttackName;
    if (attackerAttack != nullptr){
        attackerAttackName = attackerAttack->name;
    } else {
        attackerAttackName = "";
    }

    std::shared_ptr<BattleResult> battleResult = std::make_shared<BattleResult>(
            attacker->pokemon.pokemonInformation.name,
            attackerAttackName,
            defender->pokemon.pokemonInformation.name,
            false
    );
    if (damageToAttacker != 0 || damageToDefender != 0) {
        int i = 0;
        while (attacker->currentHp > 0 && defender->currentHp > 0) {
            attacker = std::make_shared<PokemonState>(attacker->afterAddingDamage(damageToAttacker));
            defender = std::make_shared<PokemonState>(defender->afterAddingDamage(damageToDefender));

            if (attackerAttack) {
                if (attackerAttack->name == "Giga Drain" &&
                    ((attackerFirst && defender->currentHp > 0) || !attackerFirst)) {
                    double gainedHealth = static_cast<double>(damageToDefender) / 2.0;
                    attacker = std::make_shared<PokemonState>(attacker->afterAddingHealth(gainedHealth));
                }

                if (attackerAttack->name == "Double-Edge") {
                    defender = std::make_shared<PokemonState>(
                            defender->afterAddingDamage(std::ceil(static_cast<double>(damageToDefender) / 4.0))
                    );
                }
            }

            if (defenderAttack) {
                if (defenderAttack->name == "Giga Drain" &&
                    ((!attackerFirst && attacker->currentHp > 0) || attackerFirst)) {
                    double gainedHealth = static_cast<double>(damageToAttacker) / 2.0;
                    defender = std::make_shared<PokemonState>(defender->afterAddingHealth(gainedHealth));

                    if (defenderAttack->name == "Double-Edge") {
                        defender = std::make_shared<PokemonState>(
                                defender->afterAddingDamage(std::ceil(static_cast<double>(damageToDefender) / 4.0))
                        );
                    }
                }
            }

            std::tie(damageToDefender, attackerAttack) = DamageCalculator::getMaxDamageAttackerCanDoToDefender(
                    attacker,
                    defender,
                    attackerBuffed,
                    level
            );

            std::tie(damageToAttacker, defenderAttack) = DamageCalculator::getMaxDamageAttackerCanDoToDefender(
                    defender,
                    attacker,
                    defenderBuffed,
                    level
            );

            // TODO for debugging
            if (i > 100) {
                printf("");
            }
            i++;
        }

        if (attacker->currentHp > 0 ||
            (attacker->currentHp <= 0 && defender->currentHp <= 0 && attackerFirst)
                ) {
            battleResult = std::make_shared<BattleResult>(
                    attacker->pokemon.pokemonInformation.name,
                    attackerAttack->name,
                    defender->pokemon.pokemonInformation.name,
                    true
            );
        }
    }
    // TODO Battle logic
    promise->set_value(*battleResult);

}