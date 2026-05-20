#include "SHealth.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {

constexpr int kLegacyTypeUnderweight = 100;
constexpr int kLegacyTypeNormal = 200;
constexpr int kLegacyTypeOverweight = 300;
constexpr int kLegacyTypeObesity = 400;

}  // namespace

bool SHealth::isInAgeBand(int age, int bandStart) {
    return age >= bandStart && age < bandStart + AGE_BAND_WIDTH;
}

int SHealth::ageBandToIndex(int bandStart) {
    return (bandStart - MIN_AGE_BAND) / AGE_BAND_WIDTH;
}

BmiCategory SHealth::classifyBmi(double bmi) {
    if (bmi <= BMI_UNDERWEIGHT_MAX) {
        return BmiCategory::Underweight;
    }
    if (bmi < BMI_NORMAL_MAX) {
        return BmiCategory::Normal;
    }
    if (bmi < BMI_OVERWEIGHT_MAX) {
        return BmiCategory::Overweight;
    }
    return BmiCategory::Obesity;
}

BmiCategory SHealth::testClassifyBmi(double bmi) {
    return classifyBmi(bmi);
}

bool SHealth::testIsInAgeBand(int age, int bandStart) {
    return isInAgeBand(age, bandStart);
}

double SHealth::ratioForCategory(const AgeBandRatios& ratios, BmiCategory category) {
    switch (category) {
        case BmiCategory::Underweight:
            return ratios.underweight;
        case BmiCategory::Normal:
            return ratios.normal;
        case BmiCategory::Overweight:
            return ratios.overweight;
        case BmiCategory::Obesity:
            return ratios.obesity;
    }
    return 0.0;
}

int SHealth::loadRecordsFromFile(const std::string& filename) {
    recordCount = 0;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return 0;
    }

    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::vector<std::string> tokens = split(line, ',');
        if (tokens.empty()) {
            break;
        }
        if (tokens.size() < 4) {
            std::cerr << "Skipping malformed line (too few columns): " << line << std::endl;
            continue;
        }
        if (recordCount >= MAX_RECORDS) {
            std::cerr << "Maximum record count exceeded: " << MAX_RECORDS << std::endl;
            break;
        }
        try {
            ages[recordCount] = std::stoi(tokens[1]);
            weights[recordCount] = std::stod(tokens[2]);
            heights[recordCount] = std::stod(tokens[3]);
        } catch (const std::exception& e) {
            std::cerr << "Skipping line with invalid numeric field: " << line << " ("
                      << e.what() << ")" << std::endl;
            continue;
        }
        recordCount++;
    }
    file.close();
    return recordCount;
}

void SHealth::imputeMissingWeightsByAgeBand() {
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

void SHealth::computeBmis() {
    for (int i = 0; i < recordCount; i++) {
        const double heightMeters = heights[i] / CM_PER_METER;
        bmis[i] = weights[i] / (heightMeters * heightMeters);
    }
}

void SHealth::aggregateAgeBandStatistics() {
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

int SHealth::calculateBmi(const std::string& filename) {
    if (loadRecordsFromFile(filename) == 0) {
        return 0;
    }
    imputeMissingWeightsByAgeBand();
    computeBmis();
    aggregateAgeBandStatistics();
    return recordCount;
}

double SHealth::getBmiRatio(int ageClass, int type) {
    if (ageClass < MIN_AGE_BAND || ageClass > MAX_AGE_BAND ||
        (ageClass - MIN_AGE_BAND) % AGE_BAND_WIDTH != 0) {
        return 0.0;
    }

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

    return ratioForCategory(ageBandRatios[ageBandToIndex(ageClass)], category);
}

std::vector<std::string> SHealth::split(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(line);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
