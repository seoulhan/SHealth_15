#include "SHealthDomain.h"

namespace SHealthDomain {

bool isInAgeBand(int age, int bandStart) {
    return age >= bandStart && age < bandStart + AGE_BAND_WIDTH;
}

int ageBandToIndex(int bandStart) {
    return (bandStart - MIN_AGE_BAND) / AGE_BAND_WIDTH;
}

bool isValidAgeClass(int ageClass) {
    return ageClass >= MIN_AGE_BAND && ageClass <= MAX_AGE_BAND &&
           (ageClass - MIN_AGE_BAND) % AGE_BAND_WIDTH == 0;
}

BmiCategory classifyBmi(double bmi) {
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

double ratioForCategory(const AgeBandRatios& ratios, BmiCategory category) {
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

void computeBmis(int recordCount, const double* heights, const double* weights, double* bmis) {
    for (int i = 0; i < recordCount; i++) {
        const double heightMeters = heights[i] / CM_PER_METER;
        bmis[i] = weights[i] / (heightMeters * heightMeters);
    }
}

}  // namespace SHealthDomain
