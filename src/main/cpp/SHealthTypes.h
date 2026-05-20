#pragma once

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
