# 14. 연령대 BMI 분포 API 정비 (FR-S03) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 14 — 연령대 BMI 분포 API 정비 (FR-S03) |
| commit string (권장) | `14_연령대_BMI_분포_API_FR-S03` |
| Persona | 리팩토링 및 Unit Test expert |
| 설계 기준 | `docs/feature_requirements_design.md` §4.2 |

## 1. 작업 목표

README「특정 연령대의 BMI 분포 비율」을 **명시적·확장 가능한 API**로 정식화한다. 기존 `getBmiRatio(ageClass, type)` **하위 호환**을 유지하고, `AgeBandRatios` 기반 `getAgeBandRatios`를 SSOT로 두어 legacy 조회는 위임한다.

## 2. 수행 내용

### 2.1 구현

| 항목 | 내용 |
|------|------|
| 신규 API | `const AgeBandRatios& getAgeBandRatios(int ageClass) const` |
| SSOT | `getBmiRatio` → `getAgeBandRatios` + `ratioForCategory` 위임 |
| 유효성 | `isValidAgeClass` private static; 잘못된 ageClass → 빈 `AgeBandRatios` |
| main | `SHealthBMI.cpp` — `getAgeBandRatios`로 4필드 출력; **printf 포맷 불변** |

### 2.2 `BmiCategory` / legacy `type` 매핑

| 분류 | enum | type | `AgeBandRatios` 필드 |
|------|------|------|----------------------|
| 저체중 | `Underweight` (100) | 100 | `underweight` |
| 정상 | `Normal` (200) | 200 | `normal` |
| 과체중 | `Overweight` (300) | 300 | `overweight` |
| 비만 | `Obesity` (400) | 400 | `obesity` |

### 2.3 테스트 (`AgeBandDistributionApi`)

| TC ID | 검증 |
|-------|------|
| **TC-API-01** | 6연령대: `getAgeBandRatios` == 4× `getBmiRatio` |
| **TC-API-02** | `shealth.dat` 6×4 스냅샷 (Golden baseline 수치) |
| **TC-API-03** | 잘못된 ageClass → zero ratios / `getBmiRatio` 0.0 |
| **TC-API-04** | 20대 4명 각 1분류 → 합 ≈ 100% ± 0.01 |

### 2.4 Golden Master

`SHealthGoldenMaster.TC_GM_01_*` — **Pass**, baseline diff 0.  
집계 로직 무변경·`%f` 6자리 포맷 유지 → **baseline 갱신 불필요**.

### 2.5 문서

| 산출물 | 경로 |
|--------|------|
| 구현 노트 §14 | `docs/feature_implementation_notes.md` |
| CMake | `SHEALTH_TEST_DATA_FILE` — TC-API-02용 `shealth.dat` 절대 경로 |

## 3. 테스트 결과

```
build-gcc: cmake --build . && ctest --output-on-failure
46/46 Passed (43 unit + 4 TC-API 신규 + 1 Golden)
```

## 4. 산출물

| 산출물 | 경로 | 상태 |
|--------|------|------|
| 헤더/구현 | `src/main/cpp/SHealth.h`, `SHealth.cpp` | 수정 |
| main | `src/main/cpp/SHealthBMI.cpp` | 신 API 사용 (포맷 동일) |
| 단위 테스트 | `src/test/cpp/SHealthBMITest.cpp` | TC-API-01~04 |
| CMake | `CMakeLists.txt` | `SHEALTH_TEST_DATA_FILE` |
| 구현 노트 | `docs/feature_implementation_notes.md` | §14 |
| 작업 보고서 | `Report/14_연령대_BMI_분포_API_FR-S03_report.md` | 본 문서 |
| Transcript | `Prompting/14_연령대_BMI_분포_API_FR-S03.md` | 생성 |

## 5. Wrap-up

| 항목 | 내용 |
|------|------|
| FR-S03 | API 정식화 완료 — 비즈니스 로직 As-Is |
| 하위 호환 | `getBmiRatio` 시그니처·동작 유지 |
| Golden | baseline **갱신 불필요** |
| 다음 Step | Step 15 — FR-C01/C02 (id 파싱, global 비율) |

**commit (권장):** `14_연령대_BMI_분포_API_FR-S03`
