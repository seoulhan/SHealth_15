#include <gtest/gtest.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace {

// Matches unit-test ratio tolerance; stdout uses printf %f (6 fractional digits).
constexpr double kGoldenRatioTolerance = 0.0001;
constexpr int kExpectedAgeBandLines = 6;

std::string normalizeNewlines(std::string text) {
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\r') {
            if (i + 1 < text.size() && text[i + 1] == '\n') {
                ++i;
            }
            out.push_back('\n');
        } else {
            out.push_back(text[i]);
        }
    }
    while (!out.empty() && out.back() == '\n') {
        out.pop_back();
    }
    return out;
}

std::string trimLineEnd(std::string line) {
    while (!line.empty() && (line.back() == ' ' || line.back() == '\t')) {
        line.pop_back();
    }
    return line;
}

std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line)) {
        line = trimLineEnd(line);
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    return lines;
}

struct AgeBandOutput {
    int ageBand = 0;
    double underweight = 0.0;
    double normal = 0.0;
    double overweight = 0.0;
    double obesity = 0.0;
};

bool parseAgeBandLine(const std::string& line, AgeBandOutput& out) {
    static const std::regex pattern(
        R"(^(\d+)\s-\sunderweight\s=\s([\d.eE+-]+),\snormal\s=\s([\d.eE+-]+),\soverweight\s=\s([\d.eE+-]+),\sobesity\s=\s([\d.eE+-]+)$)");
    std::smatch match;
    if (!std::regex_match(line, match, pattern)) {
        return false;
    }
    out.ageBand = std::stoi(match[1].str());
    out.underweight = std::stod(match[2].str());
    out.normal = std::stod(match[3].str());
    out.overweight = std::stod(match[4].str());
    out.obesity = std::stod(match[5].str());
    return true;
}

std::string readFile(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return {};
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

std::filesystem::path resolveSHealthBmiExecutable() {
#ifdef SHEALTH_GOLDEN_SHEALTH_BMI_EXE
    const std::filesystem::path configured(SHEALTH_GOLDEN_SHEALTH_BMI_EXE);
    if (std::filesystem::exists(configured)) {
        return configured;
    }
#endif
    const std::filesystem::path fallback =
        std::filesystem::path(SHEALTH_GOLDEN_BINARY_DIR) / "SHealthBMI";
#ifdef _WIN32
    const std::filesystem::path withExe = fallback.string() + ".exe";
    if (std::filesystem::exists(withExe)) {
        return withExe;
    }
#endif
    return fallback;
}

std::string captureProcessStdout(const std::filesystem::path& executable,
                                 const std::filesystem::path& workingDirectory) {
    const std::filesystem::path prev = std::filesystem::current_path();
    std::error_code ec;
    std::filesystem::current_path(workingDirectory, ec);

    std::string command = "\"" + executable.string() + "\"";
#ifdef _WIN32
    FILE* pipe = _popen(command.c_str(), "r");
#else
    FILE* pipe = popen(command.c_str(), "r");
#endif
    if (!pipe) {
        std::filesystem::current_path(prev, ec);
        return {};
    }

    std::string captured;
    char buffer[512];
    while (std::fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        captured += buffer;
    }
#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif

    std::filesystem::current_path(prev, ec);
    return captured;
}

void expectRatiosNear(double actual, double expected) {
    EXPECT_NEAR(actual, expected, kGoldenRatioTolerance)
        << "actual=" << actual << " expected=" << expected;
}

void compareGoldenLines(const std::vector<std::string>& actualLines,
                        const std::vector<std::string>& expectedLines) {
    ASSERT_EQ(actualLines.size(), expectedLines.size())
        << "Line count mismatch (expected " << expectedLines.size() << " age-band lines).";

    for (std::size_t i = 0; i < actualLines.size(); ++i) {
        AgeBandOutput actual{};
        AgeBandOutput expected{};
        ASSERT_TRUE(parseAgeBandLine(actualLines[i], actual))
            << "Actual line " << (i + 1) << " does not match output contract:\n"
            << actualLines[i];
        ASSERT_TRUE(parseAgeBandLine(expectedLines[i], expected))
            << "Baseline line " << (i + 1) << " is invalid:\n"
            << expectedLines[i];

        EXPECT_EQ(actual.ageBand, expected.ageBand) << "line " << (i + 1);
        expectRatiosNear(actual.underweight, expected.underweight);
        expectRatiosNear(actual.normal, expected.normal);
        expectRatiosNear(actual.overweight, expected.overweight);
        expectRatiosNear(actual.obesity, expected.obesity);
    }
}

}  // namespace

TEST(SHealthGoldenMaster, TC_GM_01_SHealthBMI_stdout_matches_baseline) {
    const std::filesystem::path baselinePath(SHEALTH_GOLDEN_BASELINE_FILE);
    const std::filesystem::path dataFile(SHEALTH_GOLDEN_DATA_FILE);
    const std::filesystem::path executable = resolveSHealthBmiExecutable();

    ASSERT_TRUE(std::filesystem::exists(baselinePath))
        << "Missing golden baseline: " << baselinePath;
    ASSERT_TRUE(std::filesystem::exists(dataFile))
        << "Missing input data: " << dataFile;
    ASSERT_TRUE(std::filesystem::exists(executable))
        << "Build SHealthBMI first. Not found: " << executable;

    const std::filesystem::path workingDirectory = dataFile.parent_path();
    const std::string rawStdout = captureProcessStdout(executable, workingDirectory);
    ASSERT_FALSE(rawStdout.empty()) << "SHealthBMI produced no stdout.";

    std::vector<std::string> actualLines = splitLines(normalizeNewlines(rawStdout));
    const std::vector<std::string> expectedLines =
        splitLines(normalizeNewlines(readFile(baselinePath)));

    ASSERT_GE(actualLines.size(), kExpectedAgeBandLines)
        << "Expected at least " << kExpectedAgeBandLines
        << " age-band lines (FR-08); FR-C01/C02 demo lines may follow.";
    ASSERT_EQ(expectedLines.size(), kExpectedAgeBandLines);

    actualLines.resize(kExpectedAgeBandLines);
    compareGoldenLines(actualLines, expectedLines);
}
