#ifndef GENIITEAMBUILDER_STAT_H
#define GENIITEAMBUILDER_STAT_H

#include <iostream>
#include <unordered_map>
#include <cmath>

enum class StatEnum {
    HEALTH,
    ATTACK,
    DEFENSE,
    SPECIAL_ATTACK,
    SPECIAL_DEFENSE,
    SPEED,
    NO_STAT
};

struct Stat {
    StatEnum statType;
    int value;
};

std::unordered_map<std::string, StatEnum> STAT_DICT = {
        {"health",          StatEnum::HEALTH},
        {"attack",          StatEnum::ATTACK},
        {"defense",         StatEnum::DEFENSE},
        {"special_attack",  StatEnum::SPECIAL_ATTACK},
        {"special_defense", StatEnum::SPECIAL_DEFENSE},
        {"speed",           StatEnum::SPEED}
};

StatEnum get_stat_enum(const std::string &stat) {
    return STAT_DICT[stat];
}

int get_non_health_stat(int base, int iv, int ev, int level) {
    return floor(
            floor(
                    ((2.0 * (base + iv) + floor(sqrt(ev) / 4.0)) * level) / 100.0
            ) + 5
    );
}

int get_health_stat(int base, int iv, int ev, int level) {
    return floor(
            (((2.0 * (base + iv) + floor(sqrt(ev) / 4.0)) * level) / 100.0) + level + 10
            );
}

#endif //GENIITEAMBUILDER_STAT_H
