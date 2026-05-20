#include "SHealthImputer.h"

#include "SHealthDomain.h"

namespace SHealthImputer {

void imputeMissingWeightsByAgeBand(int recordCount, const int* ages, double* weights) {
    using namespace SHealthDomain;
    for (int bandStart = MIN_AGE_BAND; bandStart <= MAX_AGE_BAND; bandStart += AGE_BAND_WIDTH) {
        double sum = 0.0;
        int validWeightCount = 0;
        for (int i = 0; i < recordCount; i++) {
            if (!isInAgeBand(ages[i], bandStart)) {
                continue;
            }
            if (weights[i] == 0.0) {
                continue;
            }
            sum += weights[i];
            validWeightCount++;
        }
        if (validWeightCount == 0) {
            continue;
        }
        const double averageWeight = sum / validWeightCount;
        for (int i = 0; i < recordCount; i++) {
            if (isInAgeBand(ages[i], bandStart) && weights[i] == 0.0) {
                weights[i] = averageWeight;
            }
        }
    }
}

void imputeMissingHeightsByAgeBand(int recordCount, const int* ages, double* heights) {
    using namespace SHealthDomain;
    for (int bandStart = MIN_AGE_BAND; bandStart <= MAX_AGE_BAND; bandStart += AGE_BAND_WIDTH) {
        double sum = 0.0;
        int validHeightCount = 0;
        for (int i = 0; i < recordCount; i++) {
            if (!isInAgeBand(ages[i], bandStart)) {
                continue;
            }
            if (heights[i] == 0.0) {
                continue;
            }
            sum += heights[i];
            validHeightCount++;
        }
        if (validHeightCount == 0) {
            continue;
        }
        const double averageHeight = sum / validHeightCount;
        for (int i = 0; i < recordCount; i++) {
            if (isInAgeBand(ages[i], bandStart) && heights[i] == 0.0) {
                heights[i] = averageHeight;
            }
        }
    }
}

}  // namespace SHealthImputer
