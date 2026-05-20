# SHealth BMI — 테스트 케이스 목록 (Step 06)

| 항목 | 내용 |
|------|------|
| 문서 버전 | 1.0 |
| 작성일 | 2026-05-20 |
| Step | 06 — 테스트 케이스 구현 |
| 기준 문서 | `docs/test_plan.md` |
| 구현 파일 | `src/test/cpp/SHealthBMITest.cpp` |
| 실행 | `ctest --output-on-failure` (38 tests, Green) |

---

## 1. 인프라

### 1.1 Fixture 클래스

| 구성요소 | 설명 |
|----------|------|
| `SHealthBMITestFixture` | `TEST_F` 공통 베이스: temp 디렉터리, `SHealth` 인스턴스, CSV 생성·삭제 |
| `writeTempCsv(body)` | 헤더 `id,age,weight,height` + 본문 → `%TEMP%/shealth_bmi_test/shealth_test_N.dat` |
| `expectRatioNear` | 비율 비교 허용오차 ±0.01 |

### 1.2 Test hook (직접 검증)

| API | 용도 |
|-----|------|
| `SHealth::testClassifyBmi(double)` | `classifyBmi` SSOT 경계 (TC-CLS-01~07) |
| `SHealth::testIsInAgeBand(int, int)` | 연령대 `[band, band+10)` (TC-AGE 직접) |

### 1.3 정적 Fixture 파일

| 파일 | 경로 | 내용 | 사용 TC |
|------|------|------|---------|
| `minimal_valid.dat` | `src/test/fixtures/minimal_valid.dat` | 1행: `1,25,70,170` (BMI≈24.22, 과체중) | TC-EXC-10 |

CMake: `SHEALTH_TEST_FIXTURE_DIR` → 소스 트리 `src/test/fixtures` 절대 경로.

---

## 2. 테스트 목록 (38건)

### 2.1 BMI 4분류 경계 — `SHealthBmiClassification` (7)

| TC ID | GTest 이름 | 입력 BMI | 기대 `BmiCategory` |
|-------|------------|----------|-------------------|
| TC-CLS-01 | `TC_CLS_01_UnderweightAt185` | 18.5 | Underweight (100) |
| TC-CLS-02 | `TC_CLS_02_NormalAbove185` | 18.5001 | Normal (200) |
| TC-CLS-03 | `TC_CLS_03_NormalBelow23` | 22.999 | Normal |
| TC-CLS-04 | `TC_CLS_04_OverweightAt23` | 23.0 | Overweight (300) |
| TC-CLS-05 | `TC_CLS_05_OverweightBelow25` | 24.999 | Overweight |
| TC-CLS-06 | `TC_CLS_06_ObesityAt25` | 25.0 | Obesity (400) |
| TC-CLS-07 | `TC_CLS_07_ObesityAbove25` | 25.001 | Obesity |

### 2.2 연령대 필터 — `SHealthAgeBand` (1)

| TC ID | GTest 이름 | 검증 |
|-------|------------|------|
| TC-AGE-* | `TC_AGE_IsInAgeBandBoundaries` | 19∉20대, 20·29∈20대, 30∉20대·∈30대 |

### 2.3 BMI 계산 — `SHealthBMITestFixture` (3)

| TC ID | GTest 이름 | Fixture 요약 | 기대 |
|-------|------------|--------------|------|
| TC-BMI-01 | `TC_BMI_01_StandardFormulaOverweight` | `25,70,170` | 1건, 20대 과체중 100% |
| TC-BMI-02 | `TC_BMI_02_CmToMetersObesityAt25` | `25,100,200` | BMI=25, 비만 100% |
| TC-BMI-03 | `TC_BMI_03_FractionalWeightHeight` | `25,63.5,175.5` | 유한 BMI, 4분류 합 100% |

### 2.4 체중 0 보정 — `SHealthBMITestFixture` (5)

| TC ID | GTest 이름 | Fixture 요약 | 기대 |
|-------|------------|--------------|------|
| TC-IMP-01 | `TC_IMP_01_BasicImputationSameBmi` | 20대 `60` + `0` 동키 | 0→60, 동일 BMI, 정상 100% |
| TC-IMP-02 | `TC_IMP_02_ThreeMemberAverage` | 30대 50·70·0 | 0→60, 3분류 각 33.33% |
| TC-IMP-03 | `TC_IMP_03_AllZeroWeightsSkipped` | 40대 전원 0 | 보정 스킵, 저체중 100% (BMI=0) |
| TC-IMP-04 | `TC_IMP_04_NoCrossBandImputation` | 20대 0만, 30대 80 | 20대 저체중 100%, 30대 비만 100% |
| TC-IMP-05 | `TC_IMP_05_BoundaryNormalVsOverweight` | 20대 55·67 kg | 정상·과체중 각 50% |

### 2.5 집계·4분류 혼합 — `SHealthBMITestFixture` (1)

| TC ID | GTest 이름 | 기대 |
|-------|------------|------|
| TC-CLS-08 | `TC_CLS_08_FourCategoriesQuarterEach` | 20대 4명 → 4 type 각 25% |

### 2.6 연령 집계 — `SHealthBMITestFixture` (4)

| TC ID | GTest 이름 | 기대 |
|-------|------------|------|
| TC-AGE-01 | `TC_AGE_01_Age19Excluded` | age=19 → 20대 비율 0 |
| TC-AGE-02 | `TC_AGE_02_Age20Included` | age=20 → 20대 반영 |
| TC-AGE-03 | `TC_AGE_03_Age29Vs30` | 29→20대, 30→30대 각 100% |
| TC-AGE-04 | `TC_AGE_04_Age70Vs79` | 70→70대, 79 미포함 |

### 2.7 `getBmiRatio` 계약 — `SHealthBMITestFixture` (5)

| TC ID | GTest 이름 | 기대 |
|-------|------------|------|
| TC-RAT-01 | `TC_RAT_01_ValidAgeClassAndTypes` | 20대만 합 100%, 타 연령대 0 |
| TC-RAT-02 | `TC_RAT_02_InvalidAgeClass` | 19·25·80 → 0.0 |
| TC-RAT-03 | `TC_RAT_03_InvalidType` | 0·150·500 → 0.0 |
| TC-RAT-04 | `TC_RAT_04_NoPriorCalculateBmi` | 미호출 시 0.0 |
| TC-RAT-05 | `TC_RAT_05_EmptyAgeBand` | 30대만 데이터 → 20대 0 |

### 2.8 예외·파일 — `SHealthBMITestFixture` (10)

| TC ID | GTest 이름 | 시나리오 | 기대 |
|-------|------------|----------|------|
| TC-EXC-01 | `TC_EXC_01_FileNotFound` | 없는 경로 | `calculateBmi` → 0 |
| TC-EXC-02 | `TC_EXC_02_EmptyFile` | 0 byte | 0 |
| TC-EXC-03 | `TC_EXC_03_HeaderOnly` | 헤더만 | 0 |
| TC-EXC-04 | `TC_EXC_04_HeaderWithTrailingNewline` | 헤더+빈 줄 | 0 |
| TC-EXC-05 | `TC_EXC_05_MalformedCsvTooFewColumns` | 컬럼 부족 | **EXPECT_DEATH** (As-Is assert, I-09) |
| TC-EXC-06 | `TC_EXC_06_MalformedCsvNonNumericAge` | 비숫자 age | `EXPECT_THROW` |
| TC-EXC-07 | `TC_EXC_07_BlankLineStopsLoading` | 중간 빈 줄 | 이후 행 미로드 (1건) |
| TC-EXC-08 | `TC_EXC_08_InvalidGetBmiRatioArgs` | 잘못된 age/type | 0.0 |
| TC-EXC-09 | `TC_EXC_09_EmptyPath` | `""` | 0 |
| TC-EXC-10 | `TC_EXC_10_CheckinFixtureRelativePath` | `minimal_valid.dat` | 1건, 과체중 100% |

### 2.9 height=0 (P1, As-Is) — `SHealthBMITestFixture` (2)

| TC ID | GTest 이름 | 기대 |
|-------|------------|------|
| TC-HGT-01 | `TC_HGT_01_HeightZeroWeightPositive` | non-finite 스킵 → 4분류 합 0% |
| TC-HGT-02 | `TC_HGT_02_HeightZeroWeightZero` | NaN 스킵 → 4분류 합 0% |

---

## 3. README Activity 3 매핑

| Activity 3 | 구현 스위트 | TC 수 |
|------------|-------------|-------|
| BMI 계산 | TC-BMI-01~03 | 3 |
| Age 평균 보정 | TC-IMP-01~05 | 5 |
| 4분류 | TC-CLS-01~08 | 8 |
| 예외 | TC-EXC-01~10, TC-RAT-02~04 | 13+ |

---

## 4. 미구현·향후 (TODO 없음 — P0 Green)

| 항목 | 상태 |
|------|------|
| P0 TC (`test_plan.md` §4~§5) | **38/38 구현·Green** |
| `DISABLED_` placeholder | 없음 |
| Step 08+ FR-S02 height 보정 | `test_plan.md` §6.1 `HeightImputation` 슬롯 예약 |
| Golden Master (Step 09) | 미포함 |

---

## 5. 변경 이력

| 버전 | 일자 | 변경 |
|------|------|------|
| 1.0 | 2026-05-20 | Step 06 — `SHealthBMITest.cpp` P0 TC 구현, `ctest` 38 Green |
