#include "SHealth.h"

#include <cstdio>

int main() {
    SHealth shealth;
    shealth.calculateBmi("shealth.dat");

    const int ageBands[] = {20, 30, 40, 50, 60, 70};
    for (int ageBand : ageBands) {
        printf("%d - underweight = %f, normal = %f, overweight = %f, obesity = %f\n",
               ageBand,
               shealth.getBmiRatio(ageBand, 100), shealth.getBmiRatio(ageBand, 200),
               shealth.getBmiRatio(ageBand, 300), shealth.getBmiRatio(ageBand, 400));
    }

    return 0;
}
