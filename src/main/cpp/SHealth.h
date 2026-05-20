#pragma once

#include <string>
#include <vector>

#include "SHealthDomain.h"
#include "SHealthTypes.h"

class SHealth {
public:
    int calculateBmi(const std::string& filename);
    double getBmiRatio(int ageClass, int type);
    const AgeBandRatios& getAgeBandRatios(int ageClass) const;
    std::vector<int> getNormalBmiUserIds() const;
    const AgeBandRatios& getGlobalBmiRatios() const;

    static BmiCategory testClassifyBmi(double bmi);
    static bool testIsInAgeBand(int age, int bandStart);

private:
    static constexpr int MAX_RECORDS = 10000;

    int recordCount = 0;
    int ids[MAX_RECORDS];
    int ages[MAX_RECORDS];
    double heights[MAX_RECORDS];
    double weights[MAX_RECORDS];
    double bmis[MAX_RECORDS];

    bool statisticsReady = false;
    AgeBandRatios ageBandRatios[SHealthDomain::AGE_BAND_COUNT];
    AgeBandRatios globalBmiRatios{};
    std::vector<int> normalBmiUserIds_;

    void resetStatisticsState();
};
