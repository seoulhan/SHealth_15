# SHealth BMI — 테스트 계획서 (Test Plan)

| 항목 | 내용 |
|------|------|
| 문서 버전 | 1.0 |
| 작성일 | 2026-05-20 |
| Step | 05 — 테스트 계획 |
| commit string (권장) | `05_테스트_계획_Test_Plan` |
| 입력 | README Activity 3·4, `docs/requirements_analysis.md`, `docs/refactoring_notes.md`, Step 04 코드 |
| 구현 대상 (Step 06) | `src/test/cpp/SHealthBMITest.cpp` |
| 실행 | `ctest` (`gtest_discover_tests`), 선택: `gen_lcov2.bat` |

---

## 1. 목적·범위

### 1.1 목적

- README **Activity 3**의 4가지 TC 범주(BMI 계산, 연령 평균 보정, 4분류, 예외)를 **Google Test**로 구현할 때의 단일 기준 문서.
- Step 04 리팩토링 이후 **public API 2개**(`calculateBmi`, `getBmiRatio`) 중심의 **블랙박스 단위 테스트**와, 필요 시 **private 헬퍼 직접 검증** 전략을 정의한다.
- Step 08~11(기능 개선) 및 Golden Master(Step 09)를 위한 **확장 슬롯**을 미리 배치한다.

### 1.2 범위

| 포함 | 제외 (별도 Step) |
|------|------------------|
| `SHealth` 도메인 로직 (`shealth_lib`) | `SHealthBMI` main stdout Golden Master (Step 09) |
| `SHealthBMITest` (`TEST` / `TEST_F`) | E2E·성능·10,000건 초과 부하 |
| 임시 CSV fixture 파일 I/O | 실제 `shealth.dat` 전체 스냅샷 회귀 (선택·소수 TC만) |

### 1.3 테스트 인프라

| 도구 | 역할 |
|------|------|
| **Google Test 1.14** | `TEST`, `TEST_F`, `EXPECT_*` / `ASSERT_*` |
| **CMake** | `SHealthBMITest` 타깃, `gtest_discover_tests` → `ctest` 자동 등록 |
| **C++17** | fixture에서 `std::filesystem`로 임시 CSV 생성·삭제 |
| **lcov (선택)** | `gen_lcov2.bat`: `build` 디렉터리 capture, `src/*`·`test/*` extract |

```powershell
mkdir build-gcc && cd build-gcc
cmake -G "MinGW Makefiles" ..
cmake --build .
ctest --output-on-failure
```

---

## 2. 테스트 대상·우선순위

### 2.1 Public API (Must — Step 05~06)

| API | 검증 포인트 | 우선순위 |
|-----|-------------|----------|
| `int calculateBmi(const std::string& filename)` | 로드 건수, 파이프라인(보정→BMI→집계) 트리거, 파일 실패 시 0 | **P0** |
| `double getBmiRatio(int ageClass, int type)` | 6연령대 × 4 type, 합계·경계, 잘못된 인자 → 0.0 | **P0** |

### 2.2 Private 로직 (Step 04 추출 — 선택적 직접 TC)

| 함수/타입 | 역할 | 직접 TC | 우선순위 |
|-----------|------|---------|----------|
| `classifyBmi(double)` | BMI 4분류 SSOT | 권장 (노출 시) | **P0** |
| `isInAgeBand(int, int)` | 연령대 `[band, band+10)` | 권장 | **P0** |
| `imputeMissingWeightsByAgeBand()` | weight=0 보정 | 통합 TC로 커버 가능 | **P0** |
| `computeBmis()` | cm→m, 공식 | 통합 + 산출 비율 역산 | **P1** |
| `loadRecordsFromFile()` | CSV 파싱 | 예외·형식 TC | **P1** |
| `aggregateAgeBandStatistics()` | % 산출 | `getBmiRatio`로 간접 검증 | **P1** |

**노출 전략 (Step 06 구현 시 택1):**

1. `#ifdef SHealth_TESTING` + 헤더에 `classifyBmi` / `isInAgeBand` public test hook
2. `friend class SHealthBMITestFixture`
3. **통합만**: mini CSV + `calculateBmi` + `getBmiRatio` (fixture 파일, Mock 불필요)

권장: **P0 경계(18.5/23/25, 연령 19·20·29·30)** 는 (1) 또는 (2)로 **직접** 검증; 나머지는 (3)으로 커버.

### 2.3 `SHealthBMITest` 구조 (계획)

```cpp
// 공통 fixture (TEST_F)
class SHealthBMITestFixture : public ::testing::Test {
protected:
    void SetUp() override;    // temp dir, SHealth instance
    void TearDown() override; // remove temp CSV
    std::string writeTempCsv(const std::string& body); // header + rows
    SHealth health;
};
```

| 매크로 | 용도 | 예시 스위트 |
|--------|------|-------------|
| `TEST_F(SHealthBMITestFixture, …)` | CSV·상태 의존 TC | 보정, 집계, 파일 예외 |
| `TEST(SHealthBmiClassification, …)` | `classifyBmi` 직접 (노출 시) | 경계값 7점 |
| `TEST(SHealthAgeBand, …)` | `isInAgeBand` 직접 (노출 시) | 19/20/29/30 |

---

## 3. README Activity 3 — 4종 TC 매핑

| README Activity 3 | 테스트 스위트 (계획) | 대표 TC ID | 검증 방법 |
|-------------------|----------------------|------------|-----------|
| **BMI 계산 로직** | `BmiFormula` | TC-BMI-01~03 | 단일 레코드 CSV → 비율·간접 BMI; 또는 `computeBmis` 직접 |
| **Age 평균치 보정** | `WeightImputation` | TC-IMP-01~05 | weight=0 + 동연령대 유효 체중 → 보정 후 BMI/비율 |
| **정상/저체중/과체중/비만 분류** | `BmiClassification` | TC-CLS-01~08 | 경계 BMI + `getBmiRatio` 100/200/300/400 |
| **예외상황** | `FileAndInputErrors` | TC-EXC-01~10 | 파일 없음, 빈 파일, 헤더만, 잘못된 CSV, 잘못된 API 인자 |

---

## 4. 상세 테스트 케이스

### 4.1 BMI 계산 (`BmiFormula`) — P0

| ID | 설명 | 입력 (단일 행 예) | 기대 |
|----|------|-------------------|------|
| TC-BMI-01 | 표준 공식 | `1,25,70,170` → BMI≈24.22 | `calculateBmi` → 1; 해당 연령대 과체중 비율 100% (type 300) |
| TC-BMI-02 | cm→m 변환 | `1,25,100,200` → BMI=25 | 비만 100% (type 400), **25.0은 비만** (Step 04 P0) |
| TC-BMI-03 | 소수 키·체중 | `1,25,63.5,175.5` | 유한 BMI, 집계 정상 |

**산출 검증식 (문서·TC 주석용):**

```
BMI = weight_kg / (height_cm / 100)²
```

### 4.2 체중 0 보정 (`WeightImputation`) — P0

| ID | 설명 | CSV 요약 | 기대 |
|----|------|----------|------|
| TC-IMP-01 | 기본 보정 | 20대: `w=60`, `w=0` 각 1명, 동일 키 | 0 → 60; 두 BMI 동일 → normal/해당 분류 50:50 |
| TC-IMP-02 | 3명 평균 | 30대: 50, 70, 0 | 0 → 60 |
| TC-IMP-03 | **연령대 내 전원 weight=0** | 40대: 모두 `w=0` | Step 04: `validWeightCount==0` → **보정 스킵**, BMI=0 → 집계 시 non-finite 스킵 가능 (I-02) |
| TC-IMP-04 | 타 연령대 평균 미사용 | 20대 w=0, 30대 w=80 | 20대 0은 20대 유효값 없으면 스킵; 30대 영향 없음 |
| TC-IMP-05 | 보정 후 분류 | 보정으로 BMI가 23 경계 근처 | `getBmiRatio`로 과체중/정상 비율 확인 |

### 4.3 BMI 4분류 경계 (`BmiClassification`) — P0

README·`docs/requirements_analysis.md` §4.2 기준 (`classifyBmi` SSOT):

| BMI | 기대 분류 | `type` (legacy) |
|-----|-----------|-----------------|
| 18.5 | 저체중 | 100 |
| 18.5001 | 정상 | 200 |
| 22.999 | 정상 | 200 |
| 23.0 | 과체중 | 300 |
| 24.999 | 과체중 | 300 |
| **25.0** | **비만** | 400 |
| 25.001 | 비만 | 400 |

| ID | 방법 | 기대 |
|----|------|------|
| TC-CLS-01~07 | `classifyBmi` 직접 또는 weight/height로 BMI 고정 CSV 1명 | 위 표와 일치 |
| TC-CLS-08 | 한 연령대 4명, 각 분류 1명씩 | 4× `getBmiRatio` 각 25% (±0.01) |

**구현 분기 (현행 코드 참고):**

```25:36:src/main/cpp/SHealth.cpp
BmiCategory SHealth::classifyBmi(double bmi) {
    if (bmi <= BMI_UNDERWEIGHT_MAX) {
        return BmiCategory::Underweight;
    }
    if (bmi < BMI_NORMAL_MAX) {
        return BmiCategory::Normal;
    }
    if (bmi < BMI_OVERWEIGHT_MAX) {
        return BmiCategory::Overweight;
    }
    return BmiCategory::Obesity;
}
```

### 4.4 연령대 경계 (`AgeBand`) — P0

`isInAgeBand`: `age >= bandStart && age < bandStart + 10`

| age | band 20 | band 30 | 집계 포함 연령대 |
|-----|---------|---------|-------------------|
| **19** | ✗ | ✗ | **없음** (20~70 밖) |
| **20** | ✓ | ✗ | 20대 |
| **29** | ✓ | ✗ | 20대 |
| **30** | ✗ | ✓ | 30대 |

| ID | 설명 | CSV | 기대 |
|----|------|-----|------|
| TC-AGE-01 | age=19 | `1,19,70,170` | `getBmiRatio(20, *)` == 0 (해당 연령대 인원 0) |
| TC-AGE-02 | age=20 | `1,20,…` | 20대 비율에 반영 |
| TC-AGE-03 | age=29 vs 30 | 2행 | 29→20대, 30→30대 각 100% |
| TC-AGE-04 | age=70 vs 79 | 70∈70대; 79는 집계 밖 | 70대 / 미포함 |

### 4.5 height=0 · weight=0 조합 — P1 (As-Is 문서화, FR-S02 전 확장)

| ID | 설명 | As-Is 기대 (Step 05) | Step 08+ (FR-S02) |
|----|------|----------------------|-------------------|
| TC-HGT-01 | height=0, weight>0 | BMI=∞; `aggregate`에서 `!isfinite` → **분류 카운트 제외**, `bandMemberCount`에는 포함 → 합계 % ≠ 100 가능 | 동연령대 평균 키 보정 후 정상 BMI |
| TC-HGT-02 | height=0, weight=0 | 0/0 → NaN, 동일 스킵 | 보정 후 재검증 |

### 4.6 `getBmiRatio` 계약 — P0

| ID | ageClass | type | 기대 |
|----|----------|------|------|
| TC-RAT-01 | 20,30,…,70 | 100~400 | 선행 `calculateBmi` 후 저장 비율과 일치 |
| TC-RAT-02 | 19, 25, 80 | any | **0.0** (잘못된 ageClass) |
| TC-RAT-03 | 20 | 0, 150, 500 | **0.0** (잘못된 type, I-08) |
| TC-RAT-04 | 20 | 100 | `calculateBmi` **미호출** 시 0.0 (초기 상태) |
| TC-RAT-05 | 빈 연령대 | 200 | `bandMemberCount==0` → 0.0 (Step 04 가드) |

### 4.7 예외·특이 입력 (`FileAndInputErrors`) — P0~P1

| ID | 시나리오 | 기대 (`calculateBmi`) | 비고 |
|----|----------|----------------------|------|
| TC-EXC-01 | **파일 없음** | 반환 **0** | stderr 로그 (캡처 optional) |
| TC-EXC-02 | **빈 파일** (0 byte) | 0 | |
| TC-EXC-03 | **헤더만** | 0 | `recordCount==0` |
| TC-EXC-04 | **데이터 0행** (헤더+줄바꿈만) | 0 | |
| TC-EXC-05 | **잘못된 CSV** — 컬럼 부족 | 현행: `stoi`/`stod` **예외 가능** (I-09) | Step 06: `EXPECT_THROW` 또는 향후 graceful 0 |
| TC-EXC-06 | **잘못된 CSV** — 비숫자 age | 동일 | |
| TC-EXC-07 | **중간 빈 줄** | As-Is: `tokens.empty()` → **break**, 이후 행 미로드 | 회귀 문서화 TC |
| TC-EXC-08 | **잘못된 ageClass/type** | `getBmiRatio` → 0.0 | TC-RAT-02~03 |
| TC-EXC-09 | 경로 빈 문자열 | 0 | |
| TC-EXC-10 | 상대 경로 fixture | 정상 건수 | temp dir 기준 |

---

## 5. 우선순위·실행 순서 (Step 06 권장)

| 단계 | TC 묶음 | 목표 |
|------|---------|------|
| 1 | TC-CLS-01~07, TC-AGE-01~04 | 도메인 규칙·경계 고정 |
| 2 | TC-BMI-01~03, TC-IMP-01~02 | 공식·보정 핵심 |
| 3 | TC-RAT-01~05, TC-CLS-08 | API·집계 |
| 4 | TC-EXC-01~04, TC-IMP-03~05 | 예외·엣지 |
| 5 | TC-EXC-05~07, TC-HGT-01~02 | As-Is 문서화·기술 부채 |
| 6 | (선택) `shealth.dat` 1건 스모크 | 실데이터 연동 |

**Exit 기준 (Step 06):** `ctest` 전체 Green; P0 TC 100% 구현; P1은 실패 시 이슈 등록 후 Green 유지 또는 `DISABLED_` 명시.

---

## 6. README Activity 4 — 테스트 확장 슬롯

향후 기능별 **예약 스위트·TC ID** (구현 전 `DISABLED_` 또는 주석 블록으로 placeholder 가능).

### 6.1 Should (FR-S01~S04)

| 요구 ID | 기능 | 예약 스위트 | 예약 TC |
|---------|------|-------------|---------|
| FR-S01 | SRP·책임 분리 | `Architecture` | 리팩토링 후 동일 fixture로 **회귀** (API 시그니처 유지 시 기존 TC 재사용) |
| FR-S02 | height=0 보정 | `HeightImputation` | TC-HGT-10: 30대 h=0,2명 유효 키 → 평균 대입; TC-HGT-11: 전원 h=0 |
| FR-S03 | 연령대 분포 API 정비 | `AgeBandDistributionApi` | `getBmiRatio` 대체/보완 API 추가 시 6×4 매트릭스 스냅샷 |
| FR-S04 | (본 Step) | — | 본 계획서 + Step 06 구현 |

### 6.2 Could (FR-C01~C02)

| 요구 ID | 기능 | 예약 스위트 | 예약 TC |
|---------|------|-------------|---------|
| FR-C01 | 정상 BMI 사용자 ID 목록 | `NormalBmiUsers` | TC-LST-01: 18.5<BMI<23 ID만; TC-LST-02: 경계 18.5·23 제외 |
| FR-C02 | 전체 사용자 범주 비율 | `GlobalBmiRatios` | TC-GLB-01: 4분류 합≈100%; TC-GLB-02: 연령대 합과 독립 |

### 6.3 Golden Master·통합 (Step 09~11)

| 항목 | 유형 | 비고 |
|------|------|------|
| `SHealthBMI` stdout 6행 | 통합 / snapshot | `EXPECT_EQ` golden 파일 또는 regex |
| `shealth.dat` 전체 | 통합 | 수치 baseline commit (Step 04 P0 반영 후) |
| 10,000건 초과 | 부하 | `MAX_RECORDS` — vector 전환 후 TC-EXC-20 |

---

## 7. Test Double 전략

### 7.1 현 단계 권장: **Fixture 파일 (실제 I/O)**

| 장점 | 단점 |
|------|------|
| public API만으로 **동작 동등** 검증 | 디스크 I/O, 경로·CWD 이슈 |
| Step 04 구조 변경 없음 | CSV 파싱 실패 시 프로세스 예외 |
| `TEST_F`에서 `writeTempCsv()` 재사용 용이 | 대용량·에러 주입이 다소 번거로움 |

**Fixture 규칙:**

- 헤더 고정: `id,age,weight,height\n`
- 파일명: `%TEMP%/shealth_test_XXXX.dat` 또는 `std::filesystem::temp_directory_path()`
- `calculateBmi(absolutePath)` 사용 (CWD 독립, I-12 완화)

### 7.2 추후 (Step 08+ DIP): **인터페이스 + Google Mock**

| 조건 | 권장 |
|------|------|
| `IRecordSource` / `std::istream` 주입 도입 | **gmock**으로 `loadRecords` 단위 격리 |
| SRP로 `BmiCalculator`, `AgeBandAggregator` 분리 | 각 클래스 **순수 로직 TEST** (파일 없음) |
| 파일 예외를 **예외 없이** 0 반환하도록 개선 | Mock stream에 잘못된 줄 주입 |

**결정 매트릭스:**

| 접근 | Step 05~06 | Step 08+ |
|------|------------|----------|
| Temp CSV fixture | **주력** | 회귀·통합 유지 |
| `friend` / test hook | `classifyBmi`, `isInAgeBand` P0 | 유지 또는 public 도메인 타입으로 승격 |
| gmock | **불필요** | I/O 분리 후 **선택 도입** |

### 7.3 stderr·로깅

- 파일 없음 TC: 반환값 0만 assert (stderr는 플랫폼별 캡처 optional).
- Step 09 이후 `testing::internal::CaptureStderr()` 검토.

---

## 8. 커버리지·품질 목표

| 지표 | 목표 (실습) |
|------|-------------|
| `SHealth.cpp` line coverage | 핵심 분기(분류·보정·집계) **≥ 80%** (lcov) |
| P0 TC | Step 06 종료 시 **전부 자동화** |
| 4분류 합계 | 단일 연령대·유한 BMI만: **100% ± 0.01** (TC-CLS-08) |
| 회귀 | Step 04 이후 `classifyBmi` 변경 시 TC-CLS-* 즉시 실패 |

**lcov (참고):** Coverage 빌드 플래그(`--coverage`) 적용 후 `gen_lcov2.bat` — `build` 디렉터리와 `gcov` 도구 경로를 로컬 환경에 맞게 조정.

---

## 9. 추적성 매트릭스

| 요구/이슈 | TC ID |
|-----------|-------|
| FR-02 BMI | TC-BMI-* |
| FR-03 weight=0 | TC-IMP-* |
| FR-04 분류 | TC-CLS-* |
| FR-05 연령대 | TC-AGE-*, TC-RAT-* |
| FR-06 getBmiRatio | TC-RAT-* |
| FR-07 calculateBmi | TC-EXC-*, TC-BMI-* |
| I-01 BMI=25 | TC-CLS-06, TC-BMI-02 |
| I-02 전원 weight=0 | TC-IMP-03 |
| I-03 height=0 | TC-HGT-* |
| I-08 잘못된 인자 | TC-RAT-02~03, TC-EXC-08 |
| I-09 CSV 오류 | TC-EXC-05~06 |
| README Activity 3 (4종) | §3 표 |
| README Activity 4 Should/Could | §6 |

---

## 10. 변경 이력

| 버전 | 일자 | 변경 |
|------|------|------|
| 1.0 | 2026-05-20 | Step 05 초안 — 테스트 계획만 (구현은 Step 06) |

**다음 Step:** Step 06 — 본 문서 TC를 `SHealthBMITest.cpp`에 구현, `ctest` Green.
