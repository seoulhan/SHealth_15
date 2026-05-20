#pragma once

#include "SHealthTypes.h"

namespace SHealthDomain {

constexpr int MIN_AGE_BAND = 20;
constexpr int MAX_AGE_BAND = 70;
constexpr int AGE_BAND_WIDTH = 10;
constexpr int AGE_BAND_COUNT = (MAX_AGE_BAND - MIN_AGE_BAND) / AGE_BAND_WIDTH + 1;

constexpr double CM_PER_METER = 100.0;
constexpr double PERCENT_SCALE = 100.0;

constexpr double BMI_UNDERWEIGHT_MAX = 18.5;
constexpr double BMI_NORMAL_MAX = 23.0;
constexpr double BMI_OVERWEIGHT_MAX = 25.0;

bool isInAgeBand(int age, int bandStart);
bool isValidAgeClass(int ageClass);
int ageBandToIndex(int bandStart);
BmiCategory classifyBmi(double bmi);
double ratioForCategory(const AgeBandRatios& ratios, BmiCategory category);

void computeBmis(int recordCount, const double* heights, const double* weights, double* bmis);

}  // namespace SHealthDomain
