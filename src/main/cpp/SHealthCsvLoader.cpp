#include "SHealthCsvLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace SHealthCsvLoader {

std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(line);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int loadRecordsFromFile(const std::string& filename, int maxRecords, int& recordCount, int* ids,
                        int* ages, double* weights, double* heights) {
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
        if (recordCount >= maxRecords) {
            std::cerr << "Maximum record count exceeded: " << maxRecords << std::endl;
            break;
        }
        if (tokens[0].empty()) {
            std::cerr << "Skipping line with empty id: " << line << std::endl;
            continue;
        }
        try {
            ids[recordCount] = std::stoi(tokens[0]);
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

}  // namespace SHealthCsvLoader
