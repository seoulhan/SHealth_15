#pragma once

#include <string>
#include <vector>

namespace SHealthCsvLoader {

int loadRecordsFromFile(const std::string& filename, int maxRecords, int& recordCount, int* ids,
                        int* ages, double* weights, double* heights);

std::vector<std::string> split(const std::string& line, char delimiter);

}  // namespace SHealthCsvLoader
