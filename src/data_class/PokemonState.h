#ifndef GENIITEAMBUILDER_POKEMONSTATE_H
#define GENIITEAMBUILDER_POKEMONSTATE_H


#include <algorithm>
#include "StatModifiers.h"
#include "Stat.h"
#include "PokemonStateSubset.h"

class PokemonState {
public:
    const bool buffed;
    const Pokemon pokemon;
    const double attackStat;
    const double defenseStat;
    const double specialAttack;
    const double specialDefense;
    const double speed;
    const double maxHp;
    const double currentHp;

    PokemonState(
            const PokemonStateSubset &pokemonStateSubset,
            const bool buffed
    ) : pokemon(pokemonStateSubset.pokemon),
        buffed(buffed),
        currentHp(pokemonStateSubset.minStats ? pokemonStateSubset.pokemon.allStats.level_50_min_stats.health
                                              : pokemonStateSubset.pokemon.allStats.level_50_max_stats.health),
        maxHp(pokemonStateSubset.minStats ? pokemonStateSubset.pokemon.allStats.level_50_min_stats.health
                                          : pokemonStateSubset.pokemon.allStats.level_50_max_stats.health),
        attackStat((pokemonStateSubset.minStats ? pokemonStateSubset.pokemon.allStats.level_50_min_stats.attack
                                                : pokemonStateSubset.pokemon.allStats.level_50_max_stats.attack) *
                   pokemonStateSubset.statModifiers.attackModifier),
        defenseStat((pokemonStateSubset.minStats ? pokemonStateSubset.pokemon.allStats.level_50_min_stats.defense
                                                 : pokemonStateSubset.pokemon.allStats.level_50_max_stats.defense) *
                    pokemonStateSubset.statModifiers.defenseModifier),
        specialAttack(
                (pokemonStateSubset.minStats ? pokemonStateSubset.pokemon.allStats.level_50_min_stats.special_attack
                                             : pokemonStateSubset.pokemon.allStats.level_50_max_stats.special_attack) *
                pokemonStateSubset.statModifiers.specialAttackModifier),
        specialDefense(
                (pokemonStateSubset.minStats ? pokemonStateSubset.pokemon.allStats.level_50_min_stats.special_defense
                                             : pokemonStateSubset.pokemon.allStats.level_50_max_stats.special_defense) *
                pokemonStateSubset.statModifiers.specialDefenseModifier),
        speed((pokemonStateSubset.minStats ? pokemonStateSubset.pokemon.allStats.level_50_min_stats.speed
                                           : pokemonStateSubset.pokemon.allStats.level_50_max_stats.speed) *
              pokemonStateSubset.statModifiers.speedModifier) {}

    PokemonState(
            const bool &buffed,
            const Pokemon &pokemon,
            const double &attackStat,
            const double &defenseStat,
            const double &specialAttack,
            const double &specialDefense,
            const double &speed,
            const double &maxHp,
            const double &currentHp
    )
            : buffed(buffed),
              pokemon(pokemon),
              attackStat(attackStat),
              defenseStat(defenseStat),
              specialAttack(specialAttack),
              specialDefense(specialDefense),
              speed(speed),
              maxHp(maxHp),
              currentHp(currentHp) {}

    PokemonState addDamage(const double damage) {
        return {
                buffed,
                pokemon,
                attackStat,
                defenseStat,
                specialAttack,
                specialDefense,
                speed,
                maxHp,
                currentHp - damage
        };
    }

    PokemonState addHealth(const double health) {
        auto hp = currentHp + health;
        hp = std::min(currentHp, maxHp);
        return {
                buffed,
                pokemon,
                attackStat,
                defenseStat,
                specialAttack,
                specialDefense,
                speed,
                maxHp,
                hp
        };
    }

    PokemonState restore() {
        return {
                buffed,
                pokemon,
                attackStat,
                defenseStat,
                specialAttack,
                specialDefense,
                speed,
                maxHp,
                maxHp
        };
    }

};

#endif //GENIITEAMBUILDER_POKEMONSTATE_H
