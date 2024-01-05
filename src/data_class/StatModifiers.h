#ifndef GENIITEAMBUILDER_STATMODIFIERS_H
#define GENIITEAMBUILDER_STATMODIFIERS_H

class StatModifiers {
public:
    const float attackModifier;
    const float specialAttackModifier;
    const float defenseModifier;
    const float specialDefenseModifier;
    const float speedModifier;
    const float accuracyModifier;
    const float evasionModifier;

    StatModifiers(float attackMod, float specialAttackMod, float defenseMod,
                  float specialDefenseMod, float speedMod, float accuracyMod,
                  float evasionMod) :
            attackModifier(attackMod), specialAttackModifier(specialAttackMod),
            defenseModifier(defenseMod), specialDefenseModifier(specialDefenseMod),
            speedModifier(speedMod), accuracyModifier(accuracyMod), evasionModifier(evasionMod) {}
};


#endif //GENIITEAMBUILDER_STATMODIFIERS_H
