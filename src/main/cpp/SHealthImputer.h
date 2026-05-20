#pragma once

namespace SHealthImputer {

void imputeMissingWeightsByAgeBand(int recordCount, const int* ages, double* weights);
void imputeMissingHeightsByAgeBand(int recordCount, const int* ages, double* heights);

}  // namespace SHealthImputer
