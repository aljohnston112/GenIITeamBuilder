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
    SPEED
};

extern std::unordered_map<std::string, StatEnum> STAT_DICT;

StatEnum get_stat_enum(const std::string &stat);

int get_non_health_stat(int base, int iv, int ev, int level);

int get_health_stat(int base, int iv, int ev, int level);

#endif //GENIITEAMBUILDER_STAT_H
