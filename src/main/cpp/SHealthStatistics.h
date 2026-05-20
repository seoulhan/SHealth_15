#pragma once

#include <vector>

#include "SHealthTypes.h"

namespace SHealthStatistics {

void aggregateAgeBandStatistics(int recordCount, const int* ages, const double* bmis,
                                AgeBandRatios* ageBandRatios);

struct GlobalAggregateResult {
    AgeBandRatios globalRatios{};
    std::vector<int> normalBmiUserIds;
};

GlobalAggregateResult aggregateGlobalBmiStatistics(int recordCount, const int* ids,
                                                   const double* bmis);

}  // namespace SHealthStatistics
