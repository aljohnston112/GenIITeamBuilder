#ifndef GENIITEAMBUILDER_BASESTATS_H
#define GENIITEAMBUILDER_BASESTATS_H

#include "Stats.h"

struct BaseStats {

    const Stats stats;

    explicit BaseStats(const Stats& stats) : stats(stats) {}

};

#endif //GENIITEAMBUILDER_BASESTATS_H