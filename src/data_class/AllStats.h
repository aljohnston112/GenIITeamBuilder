#ifndef GENIITEAMBUILDER_ALLSTATS_H
#define GENIITEAMBUILDER_ALLSTATS_H

#include <string>
#include <utility>
#include "BaseStats.h"

class AllStats {

public:
    const BaseStats base_stats;
    const Stats level_50_min_stats;
    const Stats level_50_max_stats;
    const Stats level_100_min_stats;
    const Stats level_100_max_stats;

    AllStats(
            BaseStats base_stats,
            Stats level_50_min_stats,
            Stats level_50_max_stats,
            Stats level_100_min_stats,
            Stats level_100_max_stats
    ) : base_stats(std::move(base_stats)),
        level_50_min_stats(std::move(level_50_min_stats)),
        level_50_max_stats(std::move(level_50_max_stats)),
        level_100_min_stats(std::move(level_100_min_stats)),
        level_100_max_stats(std::move(level_100_max_stats)) {}

};

#endif //GENIITEAMBUILDER_ALLSTATS_H
