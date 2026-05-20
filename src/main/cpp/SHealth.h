#pragma once

#include <string>
#include <vector>

enum class BmiCategory : int {
    Underweight = 100,
    Normal = 200,
    Overweight = 300,
    Obesity = 400
};

struct AgeBandRatios {
    double underweight = 0.0;
    double normal = 0.0;
    double overweight = 0.0;
    double obesity = 0.0;
};

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
    static constexpr int MIN_AGE_BAND = 20;
    static constexpr int MAX_AGE_BAND = 70;
    static constexpr int AGE_BAND_WIDTH = 10;
    static constexpr int AGE_BAND_COUNT =
        (MAX_AGE_BAND - MIN_AGE_BAND) / AGE_BAND_WIDTH + 1;

    static constexpr double CM_PER_METER = 100.0;
    static constexpr double PERCENT_SCALE = 100.0;

    static constexpr double BMI_UNDERWEIGHT_MAX = 18.5;
    static constexpr double BMI_NORMAL_MAX = 23.0;
    static constexpr double BMI_OVERWEIGHT_MAX = 25.0;

    int recordCount = 0;
    int ids[MAX_RECORDS];
    int ages[MAX_RECORDS];
    double heights[MAX_RECORDS];
    double weights[MAX_RECORDS];
    double bmis[MAX_RECORDS];

    bool statisticsReady = false;
    AgeBandRatios ageBandRatios[AGE_BAND_COUNT];
    AgeBandRatios globalBmiRatios{};
    std::vector<int> normalBmiUserIds_;

    static bool isInAgeBand(int age, int bandStart);
    static bool isValidAgeClass(int ageClass);
    static int ageBandToIndex(int bandStart);
    static BmiCategory classifyBmi(double bmi);
    static double ratioForCategory(const AgeBandRatios& ratios, BmiCategory category);

    int loadRecordsFromFile(const std::string& filename);
    void imputeMissingWeightsByAgeBand();
    void imputeMissingHeightsByAgeBand();
    void computeBmis();
    void aggregateAgeBandStatistics();
    void aggregateGlobalBmiStatistics();

    std::vector<std::string> split(const std::string& line, char delimiter);
};
