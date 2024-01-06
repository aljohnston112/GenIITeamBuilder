#ifndef GENIITEAMBUILDER_STATMODIFIERS_H
#define GENIITEAMBUILDER_STATMODIFIERS_H

class StatModifiers {
public:
    const double attackModifier;
    const double specialAttackModifier;
    const double defenseModifier;
    const double specialDefenseModifier;
    const double speedModifier;
    const double accuracyModifier;
    const double evasionModifier;

    StatModifiers(
            double attackMod,
            double specialAttackMod,
            double defenseMod,
            double specialDefenseMod,
            double speedMod,
            double accuracyMod,
            double evasionMod
    ) :
            attackModifier(attackMod),
            specialAttackModifier(specialAttackMod),
            defenseModifier(defenseMod),
            specialDefenseModifier(specialDefenseMod),
            speedModifier(speedMod),
            accuracyModifier(accuracyMod),
            evasionModifier(evasionMod) {}
};


#endif //GENIITEAMBUILDER_STATMODIFIERS_H
