#include "SHealth.h"

#include <cstdio>

int main() {
    SHealth shealth;
    shealth.calculateBmi("shealth.dat");

    const int ageBands[] = {20, 30, 40, 50, 60, 70};
    for (int ageBand : ageBands) {
        const AgeBandRatios& ratios = shealth.getAgeBandRatios(ageBand);
        printf("%d - underweight = %f, normal = %f, overweight = %f, obesity = %f\n",
               ageBand, ratios.underweight, ratios.normal, ratios.overweight, ratios.obesity);
    }

    return 0;
}
