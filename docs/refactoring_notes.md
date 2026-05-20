# SHealth BMI — Step 04 리팩토링 노트

| 항목 | 내용 |
|------|------|
| 문서 버전 | 1.0 |
| 작성일 | 2026-05-20 |
| Step | 04 — 1차 리팩토링 (테스트 Green 유지) |
| 입력 | `docs/code_quality_analysis.md` P0/P1, README Activity 2 |
| commit string (권장) | `04_리팩토링_Refactoring_Support` |

---

## 1. 변경 요약

### 1.1 P0 (정확성·안전)

| ID | 내용 | 구현 |
|----|------|------|
| P0-1 | BMI **25.0 → 비만** (`>= 25`) | `classifyBmi()` 단일 분기: `bmi < BMI_OVERWEIGHT_MAX` 과체중, 그 외 비만 |
| P0-2 | 보정 시 `validWeightCount == 0` 가드 | `imputeMissingWeightsByAgeBand()`에서 평균 계산·대체 스킵 |
| P0-3 | BMI=25 미분류 제거 | P0-1과 동일 (`classifyBmi`가 25.0을 `Obesity`로 처리) |
| P0-4 | BMI 경계 named constants | `BMI_UNDERWEIGHT_MAX`, `BMI_NORMAL_MAX`, `BMI_OVERWEIGHT_MAX` |

### 1.2 P1 (구조·중복 제거)

| ID | 내용 | 구현 |
|----|------|------|
| P1-1 | `calculateBmi` 함수 추출 | `loadRecordsFromFile`, `imputeMissingWeightsByAgeBand`, `computeBmis`, `aggregateAgeBandStatistics` |
| P1-2 | 연령대 필터 통합 | `isInAgeBand(age, bandStart)` |
| P1-3 | 24 멤버 → 구조체 배열 | `AgeBandRatios ageBandRatios[AGE_BAND_COUNT]` (6×4) |
| P1-4 | `enum class BmiCategory` | 100/200/300/400 → `Underweight/Normal/Overweight/Obesity` |
| P1-5 | cm→m, 백분율 상수 | `CM_PER_METER`, `PERCENT_SCALE` |
| P1-6 | `bandMemberCount == 0` 가드 | 집계 시 비율 0으로 초기화 후 `continue` |
| P1-7 | 네이밍 | `count` → `recordCount`; 통계 필드 `underweight/normal/overweight/obesity` |

### 1.3 기타

| 항목 | 내용 |
|------|------|
| `getBmiRatio` | 24-way `if` → `ageBandToIndex` + `ratioForCategory` + legacy type `switch` |
| `SHealthBMI.cpp` | 6회 `printf` → 연령대 배열 루프 (**출력 문자열 형식 동일**) |
| `SHealthBMITest.cpp` | `FailedTest`(`FAIL()`) 제거 — Step 05에서 TC 추가 |
| `CMakeLists.txt` | 변경 없음 |

### 1.4 동작 변경 (의도적)

- **BMI = 25.0** 레코드가 비만으로 분류됨 → 연령대별 obesity 비율이 As-Is 대비 소폭 증가할 수 있음 (README·요구 정합).
- Golden Master(Step 09) 확보 전까지 출력 **포맷**은 유지; **수치**는 P0 수정 반영.

---

## 2. 파일별 변경

| 파일 | 변경 |
|------|------|
| `SHealth.h` | 상수, `BmiCategory`, `AgeBandRatios`, private 헬퍼·파이프라인 메서드 |
| `SHealth.cpp` | 모노리식 `calculateBmi` 분해, `classifyBmi` SSOT |
| `SHealthBMI.cpp` | 연령대 루프화만 (magic 100~400 호출 유지) |
| `SHealthBMITest.cpp` | placeholder 실패 테스트 제거 |

---

## 3. 남은 기술 부채 (Step 05 이후)

| 우선순위 | 항목 | 비고 |
|----------|------|------|
| P2 | `std::vector<HealthRecord>` 전환 | `MAX_RECORDS` 고정 배열, id 미파싱 |
| P2 | `height == 0` 보정 (FR-S02) | Step 08~11 |
| P2 | CSV 컬럼 수·`stoi`/`stod` 예외 처리 | I-09 |
| P2 | 파일 I/O / `istream` 주입 (DIP) | 테스트 격리 |
| P2 | `SHealthBMI` Presenter 분리 | main에서 `printf` 제거 |
| P3 | `getBmiRatio` 잘못된 인자 vs 0% 구분 | optional/예외 (I-08) |
| P3 | CLI 데이터 경로 인자 | I-12 |
| — | 빈 줄 시 `break`로 이후 레코드 미읽음 | As-Is 동작 유지 |
| — | `classifyBmi` / 파이프라인 **단위 테스트 없음** | Step 05~06 |

---

## 4. Step 05~06에 노출·검증할 public API

현재 **public** 표면은 Step 04 이전과 동일 2개. 테스트는 아래를 우선 대상으로 한다.

| API | 시그니처 | Step 05 TC 제안 |
|-----|----------|-----------------|
| 파이프라인 진입 | `int calculateBmi(const std::string& filename)` | 파일 없음 → 0; 정상 로드 → `recordCount` |
| 비율 조회 | `double getBmiRatio(int ageClass, int type)` | 6연령대 × 4 type(100~400); 잘못된 age/type → 0.0 |

### 4.1 Step 05에서 테스트하기 쉬운 추출 후보 (현재 private)

리팩토링으로 **단일 진실 공급원**이 생겼으나, GTest에서 직접 호출하려면 Step 05에서 **선택적 노출**을 검토한다.

| 함수/타입 | 역할 | TC 예 |
|-----------|------|-------|
| `classifyBmi(double)` | BMI 4분류 | 18.5, 23, **25** 경계 |
| `isInAgeBand(int, int)` | 연령대 포함 | 19/20/29/30 |
| `BmiCategory` | 분류 enum | legacy type 매핑 |
| `imputeMissingWeightsByAgeBand()` | 체중 0 보정 | TC-IMP-01, ageCount=0 |
| `computeBmis()` | BMI 공식 | TC-BMI-01 (cm→m) |

**권장 (Step 05):** `#ifdef SHealth_TESTING` 또는 `friend class SHealthBMITestFixture`로 위 private를 노출하거나, 동일 로직을 테스트 전용 free function으로 한 번 더 두지 말고 **한 곳만** 유지한다.

### 4.2 상수 (헤더 `constexpr`, 테스트에서 재사용 가능)

- `BMI_UNDERWEIGHT_MAX` (18.5), `BMI_NORMAL_MAX` (23), `BMI_OVERWEIGHT_MAX` (25)
- `MIN_AGE_BAND` (20), `MAX_AGE_BAND` (70), `AGE_BAND_WIDTH` (10)
- `BmiCategory::{Underweight=100, Normal=200, Overweight=300, Obesity=400}`

---

## 5. Before / After (품질)

| 지표 | Before | After (Step 04) |
|------|--------|-----------------|
| `calculateBmi` LOC | ~103 | ~8 (오케스트레이션) |
| `getBmiRatio` 분기 | 24 `else if` | index + category switch |
| 통계 저장 | 24 `double` 멤버 | `AgeBandRatios[6]` |
| BMI 분류 정의처 | 집계 루프 내 if | `classifyBmi()` 1곳 |
| 단위 테스트 | 1× `FAIL()` | 0 (Green, Step 05에서 추가) |

---

## 6. 빌드·테스트

```powershell
mkdir build-gcc
cd build-gcc
cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/mingw64/bin/g++.exe ..
mingw32-make
ctest
```

- `SHealthBMI.exe`는 프로젝트 루트(`shealth.dat` CWD)에서 실행.
- `ctest`: 등록된 TEST 매크로 없음 → **0 tests, exit 0** (FailedTest 제거).

---

## 7. 참조

- `docs/code_quality_analysis.md` — P0/P1 출처
- `docs/requirements_analysis.md` — BMI 경계·이슈 I-01~I-12
- README Activity 2 — 네이밍 → 상수 → 추출 → 중복 제거

## 8. 변경 이력

| 버전 | 일자 | 변경 |
|------|------|------|
| 1.0 | 2026-05-20 | Step 04 1차 리팩토링 반영 |
