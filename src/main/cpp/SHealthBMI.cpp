#include "SHealth.h"

#include <cstdio>
#include <vector>

int main() {
    SHealth shealth;
    shealth.calculateBmi("shealth.dat");

    const int ageBands[] = {20, 30, 40, 50, 60, 70};
    for (int ageBand : ageBands) {
        const AgeBandRatios& ratios = shealth.getAgeBandRatios(ageBand);
        printf("%d - underweight = %f, normal = %f, overweight = %f, obesity = %f\n",
               ageBand, ratios.underweight, ratios.normal, ratios.overweight, ratios.obesity);
    }

    const std::vector<int> normalIds = shealth.getNormalBmiUserIds();
    printf("Normal BMI users (count=%zu):", normalIds.size());
    for (size_t i = 0; i < normalIds.size(); ++i) {
        printf("%s%d", (i == 0 ? " " : ", "), normalIds[i]);
    }
    printf("\n");

    const AgeBandRatios& globalRatios = shealth.getGlobalBmiRatios();
    printf("Global - underweight = %f, normal = %f, overweight = %f, obesity = %f\n",
           globalRatios.underweight, globalRatios.normal, globalRatios.overweight,
           globalRatios.obesity);

    return 0;
}
