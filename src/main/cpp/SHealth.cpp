#include "SHealth.h"

#include "SHealthCsvLoader.h"
#include "SHealthImputer.h"
#include "SHealthStatistics.h"

namespace {

constexpr int kLegacyTypeUnderweight = 100;
constexpr int kLegacyTypeNormal = 200;
constexpr int kLegacyTypeOverweight = 300;
constexpr int kLegacyTypeObesity = 400;

const AgeBandRatios kEmptyAgeBandRatios{};

}  // namespace

BmiCategory SHealth::testClassifyBmi(double bmi) {
    return SHealthDomain::classifyBmi(bmi);
}

bool SHealth::testIsInAgeBand(int age, int bandStart) {
    return SHealthDomain::isInAgeBand(age, bandStart);
}

void SHealth::resetStatisticsState() {
    statisticsReady = false;
    normalBmiUserIds_.clear();
    globalBmiRatios = AgeBandRatios{};
    for (int i = 0; i < SHealthDomain::AGE_BAND_COUNT; ++i) {
        ageBandRatios[i] = AgeBandRatios{};
    }
}

int SHealth::calculateBmi(const std::string& filename) {
    resetStatisticsState();
    if (SHealthCsvLoader::loadRecordsFromFile(filename, MAX_RECORDS, recordCount, ids, ages,
                                            weights, heights) == 0) {
        return 0;
    }
    SHealthImputer::imputeMissingWeightsByAgeBand(recordCount, ages, weights);
    SHealthImputer::imputeMissingHeightsByAgeBand(recordCount, ages, heights);
    SHealthDomain::computeBmis(recordCount, heights, weights, bmis);
    SHealthStatistics::aggregateAgeBandStatistics(recordCount, ages, bmis, ageBandRatios);
    const SHealthStatistics::GlobalAggregateResult globalResult =
        SHealthStatistics::aggregateGlobalBmiStatistics(recordCount, ids, bmis);
    globalBmiRatios = globalResult.globalRatios;
    normalBmiUserIds_ = globalResult.normalBmiUserIds;
    statisticsReady = true;
    return recordCount;
}

const AgeBandRatios& SHealth::getAgeBandRatios(int ageClass) const {
    if (!SHealthDomain::isValidAgeClass(ageClass)) {
        return kEmptyAgeBandRatios;
    }
    return ageBandRatios[SHealthDomain::ageBandToIndex(ageClass)];
}

double SHealth::getBmiRatio(int ageClass, int type) {
    BmiCategory category;
    switch (type) {
        case kLegacyTypeUnderweight:
            category = BmiCategory::Underweight;
            break;
        case kLegacyTypeNormal:
            category = BmiCategory::Normal;
            break;
        case kLegacyTypeOverweight:
            category = BmiCategory::Overweight;
            break;
        case kLegacyTypeObesity:
            category = BmiCategory::Obesity;
            break;
        default:
            return 0.0;
    }

    return SHealthDomain::ratioForCategory(getAgeBandRatios(ageClass), category);
}

std::vector<int> SHealth::getNormalBmiUserIds() const {
    if (!statisticsReady) {
        return {};
    }
    return normalBmiUserIds_;
}

const AgeBandRatios& SHealth::getGlobalBmiRatios() const {
    if (!statisticsReady) {
        return kEmptyAgeBandRatios;
    }
    return globalBmiRatios;
}
