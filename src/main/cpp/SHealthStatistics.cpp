#include "SHealthStatistics.h"

#include <algorithm>
#include <cmath>

#include "SHealthDomain.h"

namespace SHealthStatistics {

void aggregateAgeBandStatistics(int recordCount, const int* ages, const double* bmis,
                                AgeBandRatios* ageBandRatios) {
    using namespace SHealthDomain;
    for (int bandStart = MIN_AGE_BAND; bandStart <= MAX_AGE_BAND; bandStart += AGE_BAND_WIDTH) {
        int underweightCount = 0;
        int normalCount = 0;
        int overweightCount = 0;
        int obesityCount = 0;
        int bandMemberCount = 0;

        for (int i = 0; i < recordCount; i++) {
            if (!isInAgeBand(ages[i], bandStart)) {
                continue;
            }
            bandMemberCount++;
            if (!std::isfinite(bmis[i])) {
                continue;
            }
            switch (classifyBmi(bmis[i])) {
                case BmiCategory::Underweight:
                    underweightCount++;
                    break;
                case BmiCategory::Normal:
                    normalCount++;
                    break;
                case BmiCategory::Overweight:
                    overweightCount++;
                    break;
                case BmiCategory::Obesity:
                    obesityCount++;
                    break;
            }
        }

        AgeBandRatios& ratios = ageBandRatios[ageBandToIndex(bandStart)];
        if (bandMemberCount == 0) {
            ratios = AgeBandRatios{};
            continue;
        }

        const double scale = PERCENT_SCALE / bandMemberCount;
        ratios.underweight = underweightCount * scale;
        ratios.normal = normalCount * scale;
        ratios.overweight = overweightCount * scale;
        ratios.obesity = obesityCount * scale;
    }
}

GlobalAggregateResult aggregateGlobalBmiStatistics(int recordCount, const int* ids,
                                                   const double* bmis) {
    using namespace SHealthDomain;
    GlobalAggregateResult result;

    if (recordCount == 0) {
        return result;
    }

    int underweightCount = 0;
    int normalCount = 0;
    int overweightCount = 0;
    int obesityCount = 0;

    for (int i = 0; i < recordCount; i++) {
        if (!std::isfinite(bmis[i])) {
            continue;
        }
        switch (classifyBmi(bmis[i])) {
            case BmiCategory::Underweight:
                underweightCount++;
                break;
            case BmiCategory::Normal:
                normalCount++;
                result.normalBmiUserIds.push_back(ids[i]);
                break;
            case BmiCategory::Overweight:
                overweightCount++;
                break;
            case BmiCategory::Obesity:
                obesityCount++;
                break;
        }
    }

    std::sort(result.normalBmiUserIds.begin(), result.normalBmiUserIds.end());

    const double scale = PERCENT_SCALE / recordCount;
    result.globalRatios.underweight = underweightCount * scale;
    result.globalRatios.normal = normalCount * scale;
    result.globalRatios.overweight = overweightCount * scale;
    result.globalRatios.obesity = obesityCount * scale;
    return result;
}

}  // namespace SHealthStatistics
