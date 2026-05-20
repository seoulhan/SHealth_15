#include "SHealthPresenter.h"

#include <cstdio>
#include <vector>

#include "SHealth.h"
#include "SHealthTypes.h"

namespace SHealthPresenter {

void printAgeBandReport(const SHealth& shealth) {
    const int ageBands[] = {20, 30, 40, 50, 60, 70};
    for (int ageBand : ageBands) {
        const AgeBandRatios& ratios = shealth.getAgeBandRatios(ageBand);
        printf("%d - underweight = %f, normal = %f, overweight = %f, obesity = %f\n",
               ageBand, ratios.underweight, ratios.normal, ratios.overweight, ratios.obesity);
    }
}

void printNormalBmiUsers(const SHealth& shealth) {
    const std::vector<int> normalIds = shealth.getNormalBmiUserIds();
    printf("Normal BMI users (count=%zu):", normalIds.size());
    for (size_t i = 0; i < normalIds.size(); ++i) {
        printf("%s%d", (i == 0 ? " " : ", "), normalIds[i]);
    }
    printf("\n");
}

void printGlobalBmiRatios(const SHealth& shealth) {
    const AgeBandRatios& globalRatios = shealth.getGlobalBmiRatios();
    printf("Global - underweight = %f, normal = %f, overweight = %f, obesity = %f\n",
           globalRatios.underweight, globalRatios.normal, globalRatios.overweight,
           globalRatios.obesity);
}

}  // namespace SHealthPresenter
