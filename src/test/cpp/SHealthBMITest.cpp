#include <gtest/gtest.h>

#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "SHealth.h"

namespace {

constexpr int kTypeUnderweight = 100;
constexpr int kTypeNormal = 200;
constexpr int kTypeOverweight = 300;
constexpr int kTypeObesity = 400;

constexpr double kRatioTolerance = 0.01;

double expectedBmi(double weightKg, double heightCm) {
    const double heightMeters = heightCm / 100.0;
    return weightKg / (heightMeters * heightMeters);
}

void expectRatioNear(double actual, double expected) {
    EXPECT_NEAR(actual, expected, kRatioTolerance);
}

}  // namespace

class SHealthBMITestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        tempDir_ = std::filesystem::temp_directory_path() / "shealth_bmi_test";
        std::filesystem::create_directories(tempDir_);
        counter_ = 0;
    }

    void TearDown() override {
        for (const auto& path : createdFiles_) {
            std::error_code ec;
            std::filesystem::remove(path, ec);
        }
        createdFiles_.clear();
    }

    std::string writeTempCsv(const std::string& body) {
        const std::string filename =
            (tempDir_ / ("shealth_test_" + std::to_string(++counter_) + ".dat")).string();
        std::ofstream out(filename);
        out << "id,age,weight,height\n";
        out << body;
        out.close();
        createdFiles_.push_back(filename);
        return filename;
    }

    SHealth health;
    std::filesystem::path tempDir_;
    int counter_ = 0;
    std::vector<std::filesystem::path> createdFiles_;
};

// --- TC-CLS-01~07: classifyBmi boundaries (direct) ---

TEST(SHealthBmiClassification, TC_CLS_01_UnderweightAt185) {
    EXPECT_EQ(SHealth::testClassifyBmi(18.5), BmiCategory::Underweight);
}

TEST(SHealthBmiClassification, TC_CLS_02_NormalAbove185) {
    EXPECT_EQ(SHealth::testClassifyBmi(18.5001), BmiCategory::Normal);
}

TEST(SHealthBmiClassification, TC_CLS_03_NormalBelow23) {
    EXPECT_EQ(SHealth::testClassifyBmi(22.999), BmiCategory::Normal);
}

TEST(SHealthBmiClassification, TC_CLS_04_OverweightAt23) {
    EXPECT_EQ(SHealth::testClassifyBmi(23.0), BmiCategory::Overweight);
}

TEST(SHealthBmiClassification, TC_CLS_05_OverweightBelow25) {
    EXPECT_EQ(SHealth::testClassifyBmi(24.999), BmiCategory::Overweight);
}

TEST(SHealthBmiClassification, TC_CLS_06_ObesityAt25) {
    EXPECT_EQ(SHealth::testClassifyBmi(25.0), BmiCategory::Obesity);
}

TEST(SHealthBmiClassification, TC_CLS_07_ObesityAbove25) {
    EXPECT_EQ(SHealth::testClassifyBmi(25.001), BmiCategory::Obesity);
}

// --- TC-AGE: isInAgeBand (direct) ---

TEST(SHealthAgeBand, TC_AGE_IsInAgeBandBoundaries) {
    EXPECT_FALSE(SHealth::testIsInAgeBand(19, 20));
    EXPECT_TRUE(SHealth::testIsInAgeBand(20, 20));
    EXPECT_TRUE(SHealth::testIsInAgeBand(29, 20));
    EXPECT_FALSE(SHealth::testIsInAgeBand(30, 20));
    EXPECT_TRUE(SHealth::testIsInAgeBand(30, 30));
}

// --- TC-BMI: BMI formula (integration) ---

TEST_F(SHealthBMITestFixture, TC_BMI_01_StandardFormulaOverweight) {
    const std::string path = writeTempCsv("1,25,70,170\n");
    EXPECT_EQ(health.calculateBmi(path), 1);
    const double bmi = expectedBmi(70.0, 170.0);
    ASSERT_NEAR(bmi, 24.2215, 0.01);
    expectRatioNear(health.getBmiRatio(20, kTypeOverweight), 100.0);
    expectRatioNear(health.getBmiRatio(20, kTypeObesity), 0.0);
}

TEST_F(SHealthBMITestFixture, TC_BMI_02_CmToMetersObesityAt25) {
    const std::string path = writeTempCsv("1,25,100,200\n");
    EXPECT_EQ(health.calculateBmi(path), 1);
    EXPECT_DOUBLE_EQ(expectedBmi(100.0, 200.0), 25.0);
    expectRatioNear(health.getBmiRatio(20, kTypeObesity), 100.0);
}

TEST_F(SHealthBMITestFixture, TC_BMI_03_FractionalWeightHeight) {
    const std::string path = writeTempCsv("1,25,63.5,175.5\n");
    EXPECT_EQ(health.calculateBmi(path), 1);
    const double bmi = expectedBmi(63.5, 175.5);
    EXPECT_TRUE(std::isfinite(bmi));
    EXPECT_GT(bmi, 0.0);
    const double sum = health.getBmiRatio(20, kTypeUnderweight) +
                       health.getBmiRatio(20, kTypeNormal) +
                       health.getBmiRatio(20, kTypeOverweight) +
                       health.getBmiRatio(20, kTypeObesity);
    expectRatioNear(sum, 100.0);
}

// --- TC-IMP: weight=0 imputation ---

TEST_F(SHealthBMITestFixture, TC_IMP_01_BasicImputationSameBmi) {
    const std::string path = writeTempCsv(
        "1,25,60,170\n"
        "2,27,0,170\n");
    EXPECT_EQ(health.calculateBmi(path), 2);
    const double bmi = expectedBmi(60.0, 170.0);
    EXPECT_NEAR(bmi, 20.761, 0.01);
    expectRatioNear(health.getBmiRatio(20, kTypeNormal), 100.0);
}

TEST_F(SHealthBMITestFixture, TC_IMP_02_ThreeMemberAverage) {
    const std::string path = writeTempCsv(
        "1,35,50,170\n"
        "2,36,70,170\n"
        "3,37,0,170\n");
    EXPECT_EQ(health.calculateBmi(path), 3);
    const double bmiImputed = expectedBmi(60.0, 170.0);
    EXPECT_NEAR(bmiImputed, 20.761, 0.01);
    expectRatioNear(health.getBmiRatio(30, kTypeUnderweight), 33.33);
    expectRatioNear(health.getBmiRatio(30, kTypeNormal), 33.33);
    expectRatioNear(health.getBmiRatio(30, kTypeOverweight), 33.33);
}

TEST_F(SHealthBMITestFixture, TC_IMP_03_AllZeroWeightsSkipped) {
    const std::string path = writeTempCsv(
        "1,45,0,170\n"
        "2,46,0,170\n");
    EXPECT_EQ(health.calculateBmi(path), 2);
    expectRatioNear(health.getBmiRatio(40, kTypeUnderweight), 100.0);
}

TEST_F(SHealthBMITestFixture, TC_IMP_04_NoCrossBandImputation) {
    const std::string path = writeTempCsv(
        "1,25,0,170\n"
        "2,35,80,170\n");
    EXPECT_EQ(health.calculateBmi(path), 2);
    expectRatioNear(health.getBmiRatio(20, kTypeUnderweight), 100.0);
    expectRatioNear(health.getBmiRatio(30, kTypeObesity), 100.0);
}

TEST_F(SHealthBMITestFixture, TC_IMP_05_BoundaryNormalVsOverweight) {
    const std::string path = writeTempCsv(
        "1,25,55,170\n"
        "2,26,67,170\n");
    EXPECT_EQ(health.calculateBmi(path), 2);
    expectRatioNear(health.getBmiRatio(20, kTypeNormal), 50.0);
    expectRatioNear(health.getBmiRatio(20, kTypeOverweight), 50.0);
}

// --- TC-CLS-08: four categories in one band ---

TEST_F(SHealthBMITestFixture, TC_CLS_08_FourCategoriesQuarterEach) {
    const std::string path = writeTempCsv(
        "1,25,50,170\n"
        "2,26,65,170\n"
        "3,27,67,170\n"
        "4,28,75,170\n");
    EXPECT_EQ(health.calculateBmi(path), 4);
    expectRatioNear(health.getBmiRatio(20, kTypeUnderweight), 25.0);
    expectRatioNear(health.getBmiRatio(20, kTypeNormal), 25.0);
    expectRatioNear(health.getBmiRatio(20, kTypeOverweight), 25.0);
    expectRatioNear(health.getBmiRatio(20, kTypeObesity), 25.0);
}

// --- TC-AGE: age band aggregation ---

TEST_F(SHealthBMITestFixture, TC_AGE_01_Age19Excluded) {
    const std::string path = writeTempCsv("1,19,70,170\n");
    EXPECT_EQ(health.calculateBmi(path), 1);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, kTypeNormal), 0.0);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, kTypeOverweight), 0.0);
}

TEST_F(SHealthBMITestFixture, TC_AGE_02_Age20Included) {
    const std::string path = writeTempCsv("1,20,70,170\n");
    EXPECT_EQ(health.calculateBmi(path), 1);
    expectRatioNear(health.getBmiRatio(20, kTypeOverweight), 100.0);
}

TEST_F(SHealthBMITestFixture, TC_AGE_03_Age29Vs30) {
    const std::string path = writeTempCsv(
        "1,29,70,170\n"
        "2,30,80,180\n");
    EXPECT_EQ(health.calculateBmi(path), 2);
    expectRatioNear(health.getBmiRatio(20, kTypeOverweight), 100.0);
    expectRatioNear(health.getBmiRatio(30, kTypeOverweight), 100.0);
}

TEST_F(SHealthBMITestFixture, TC_AGE_04_Age70Vs79) {
    const std::string path = writeTempCsv(
        "1,70,70,170\n"
        "2,79,80,180\n");
    EXPECT_EQ(health.calculateBmi(path), 2);
    expectRatioNear(health.getBmiRatio(70, kTypeOverweight), 100.0);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(70, kTypeNormal), 0.0);
}

// --- TC-RAT: getBmiRatio contract ---

TEST_F(SHealthBMITestFixture, TC_RAT_01_ValidAgeClassAndTypes) {
    const std::string path = writeTempCsv("1,25,70,170\n");
    ASSERT_EQ(health.calculateBmi(path), 1);
    for (int ageClass = 20; ageClass <= 70; ageClass += 10) {
        const double sum = health.getBmiRatio(ageClass, kTypeUnderweight) +
                           health.getBmiRatio(ageClass, kTypeNormal) +
                           health.getBmiRatio(ageClass, kTypeOverweight) +
                           health.getBmiRatio(ageClass, kTypeObesity);
        if (ageClass == 20) {
            expectRatioNear(sum, 100.0);
        } else {
            EXPECT_DOUBLE_EQ(sum, 0.0);
        }
    }
}

TEST_F(SHealthBMITestFixture, TC_RAT_02_InvalidAgeClass) {
    const std::string path = writeTempCsv("1,25,70,170\n");
    ASSERT_EQ(health.calculateBmi(path), 1);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(19, kTypeNormal), 0.0);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(25, kTypeNormal), 0.0);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(80, kTypeNormal), 0.0);
}

TEST_F(SHealthBMITestFixture, TC_RAT_03_InvalidType) {
    const std::string path = writeTempCsv("1,25,70,170\n");
    ASSERT_EQ(health.calculateBmi(path), 1);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, 0), 0.0);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, 150), 0.0);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, 500), 0.0);
}

TEST_F(SHealthBMITestFixture, TC_RAT_04_NoPriorCalculateBmi) {
    SHealth fresh;
    EXPECT_DOUBLE_EQ(fresh.getBmiRatio(20, kTypeNormal), 0.0);
}

TEST_F(SHealthBMITestFixture, TC_RAT_05_EmptyAgeBand) {
    const std::string path = writeTempCsv("1,35,70,170\n");
    ASSERT_EQ(health.calculateBmi(path), 1);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, kTypeNormal), 0.0);
}

// --- TC-EXC: file and input errors ---

TEST_F(SHealthBMITestFixture, TC_EXC_01_FileNotFound) {
    const std::string path = (tempDir_ / "nonexistent_file.dat").string();
    EXPECT_EQ(health.calculateBmi(path), 0);
}

TEST_F(SHealthBMITestFixture, TC_EXC_02_EmptyFile) {
    const std::string path = (tempDir_ / "empty.dat").string();
    std::ofstream(path).close();
    createdFiles_.push_back(path);
    EXPECT_EQ(health.calculateBmi(path), 0);
}

TEST_F(SHealthBMITestFixture, TC_EXC_03_HeaderOnly) {
    const std::string path = (tempDir_ / "header_only.dat").string();
    std::ofstream out(path);
    out << "id,age,weight,height\n";
    out.close();
    createdFiles_.push_back(path);
    EXPECT_EQ(health.calculateBmi(path), 0);
}

TEST_F(SHealthBMITestFixture, TC_EXC_04_HeaderWithTrailingNewline) {
    const std::string path = (tempDir_ / "header_newline.dat").string();
    std::ofstream out(path);
    out << "id,age,weight,height\n\n";
    out.close();
    createdFiles_.push_back(path);
    EXPECT_EQ(health.calculateBmi(path), 0);
}

TEST_F(SHealthBMITestFixture, TC_EXC_05_MalformedCsvTooFewColumns) {
    const std::string path = (tempDir_ / "bad_columns.dat").string();
    std::ofstream out(path);
    out << "id,age,weight,height\n";
    out << "1,25,70\n";
    out.close();
    createdFiles_.push_back(path);
    EXPECT_EQ(health.calculateBmi(path), 0);
}

TEST_F(SHealthBMITestFixture, TC_EXC_06_MalformedCsvNonNumericAge) {
    const std::string path = (tempDir_ / "bad_age.dat").string();
    std::ofstream out(path);
    out << "id,age,weight,height\n";
    out << "1,abc,70,170\n";
    out.close();
    createdFiles_.push_back(path);
    EXPECT_EQ(health.calculateBmi(path), 0);
}

TEST_F(SHealthBMITestFixture, TC_EXC_11_MalformedCsvSkipsBadRowLoadsValid) {
    const std::string path = writeTempCsv(
        "1,abc,70,170\n"
        "2,25,70,170\n");
    EXPECT_EQ(health.calculateBmi(path), 1);
    expectRatioNear(health.getBmiRatio(20, kTypeOverweight), 100.0);
}

TEST_F(SHealthBMITestFixture, TC_EXC_07_BlankLineStopsLoading) {
    const std::string path = writeTempCsv(
        "1,25,70,170\n"
        "\n"
        "2,35,80,180\n");
    EXPECT_EQ(health.calculateBmi(path), 1);
    expectRatioNear(health.getBmiRatio(20, kTypeOverweight), 100.0);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(30, kTypeNormal), 0.0);
}

TEST_F(SHealthBMITestFixture, TC_EXC_08_InvalidGetBmiRatioArgs) {
    const std::string path = writeTempCsv("1,25,70,170\n");
    ASSERT_EQ(health.calculateBmi(path), 1);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(19, kTypeNormal), 0.0);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, 99), 0.0);
}

TEST_F(SHealthBMITestFixture, TC_EXC_09_EmptyPath) {
    EXPECT_EQ(health.calculateBmi(""), 0);
}

TEST_F(SHealthBMITestFixture, TC_EXC_10_CheckinFixtureRelativePath) {
    const std::filesystem::path fixturePath =
        std::filesystem::path(SHEALTH_TEST_FIXTURE_DIR) / "minimal_valid.dat";
    ASSERT_TRUE(std::filesystem::exists(fixturePath))
        << "Missing fixture: " << fixturePath;
    EXPECT_EQ(health.calculateBmi(fixturePath.string()), 1);
    expectRatioNear(health.getBmiRatio(20, kTypeOverweight), 100.0);
}

// --- TC-HGT: height=0 (FR-S02 HeightImputation) ---

TEST_F(SHealthBMITestFixture, TC_HGT_10_ThirtyBandAverageHeightImputation) {
    const std::string path = writeTempCsv(
        "1,30,50,170\n"
        "2,31,70,175\n"
        "3,32,60,0\n");
    EXPECT_EQ(health.calculateBmi(path), 3);
    const double imputedHeightCm = (170.0 + 175.0) / 2.0;
    EXPECT_NEAR(imputedHeightCm, 172.5, 1e-9);
    const double bmiImputed = expectedBmi(60.0, imputedHeightCm);
    ASSERT_TRUE(std::isfinite(bmiImputed));
    EXPECT_GT(bmiImputed, 18.5);
    EXPECT_LT(bmiImputed, 23.0);
    expectRatioNear(health.getBmiRatio(30, kTypeUnderweight), 33.33);
    expectRatioNear(health.getBmiRatio(30, kTypeNormal), 66.67);
    const double sum = health.getBmiRatio(30, kTypeUnderweight) +
                       health.getBmiRatio(30, kTypeNormal) +
                       health.getBmiRatio(30, kTypeOverweight) +
                       health.getBmiRatio(30, kTypeObesity);
    expectRatioNear(sum, 100.0);
}

TEST_F(SHealthBMITestFixture, TC_HGT_11_FortyBandAllZeroHeightsSkipped) {
    const std::string path = writeTempCsv(
        "1,45,70,0\n"
        "2,46,80,0\n");
    EXPECT_EQ(health.calculateBmi(path), 2);
    const double sum = health.getBmiRatio(40, kTypeUnderweight) +
                       health.getBmiRatio(40, kTypeNormal) +
                       health.getBmiRatio(40, kTypeOverweight) +
                       health.getBmiRatio(40, kTypeObesity);
    EXPECT_DOUBLE_EQ(sum, 0.0);
}

// --- TC-HGT: height=0 edge cases (solo band, no peers to impute) ---

TEST_F(SHealthBMITestFixture, TC_HGT_01_HeightZeroWeightPositive) {
    const std::string path = writeTempCsv("1,25,70,0\n");
    EXPECT_EQ(health.calculateBmi(path), 1);
    const double sum = health.getBmiRatio(20, kTypeUnderweight) +
                       health.getBmiRatio(20, kTypeNormal) +
                       health.getBmiRatio(20, kTypeOverweight) +
                       health.getBmiRatio(20, kTypeObesity);
    EXPECT_DOUBLE_EQ(sum, 0.0);
}

TEST_F(SHealthBMITestFixture, TC_HGT_02_HeightZeroWeightZero) {
    const std::string path = writeTempCsv("1,25,0,0\n");
    EXPECT_EQ(health.calculateBmi(path), 1);
    const double sum = health.getBmiRatio(20, kTypeUnderweight) +
                       health.getBmiRatio(20, kTypeNormal) +
                       health.getBmiRatio(20, kTypeOverweight) +
                       health.getBmiRatio(20, kTypeObesity);
    EXPECT_DOUBLE_EQ(sum, 0.0);
}
