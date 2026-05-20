# 15. 정상 BMI 목록·전체 범주 비율 (FR-C01, FR-C02) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 15 — 정상 BMI 목록·전체 범주 비율 (FR-C01, FR-C02) |
| commit string (권장) | `15_정상목록_전체비율_FR-C01_FR-C02` |
| Persona | 리팩토링 및 Unit Test expert |
| 설계 기준 | `docs/feature_requirements_design.md` §4.3, §4.4 |

## 1. 작업 목표

- **FR-C01**: CSV `id` 파싱·저장 후 정상 BMI(`18.5 < BMI < 23`) 사용자 ID 목록 조회
- **FR-C02**: 연령대 무관 **전체 레코드** 기준 4분류 비율(%) 조회
- `calculateBmi` 완료 전제 조건 명시·테스트
- main 데모 출력 추가 (Golden 6행과 분리)

## 2. 수행 내용

### 2.1 구현

| 항목 | 내용 |
|------|------|
| id 저장 | `ids[]`, `loadRecordsFromFile`에서 `tokens[0]` → `stoi` (빈 id·비숫자 행 스킵) |
| 파이프라인 | `aggregateGlobalBmiStatistics()` — `aggregateAgeBandStatistics` **이후** 호출 |
| FR-C01 API | `std::vector<int> getNormalBmiUserIds() const` — 오름차순, `statisticsReady` 전 빈 vector |
| FR-C02 API | `const AgeBandRatios& getGlobalBmiRatios() const` — 미준비 시 zero ratios |
| 전제 플래그 | `statisticsReady` — 집계 완료 시 true, 로드 시작 시 false |

### 2.2 집계 규칙

| API | 분모 | 분자 | 경계 |
|-----|------|------|------|
| `getNormalBmiUserIds` | — | `Normal` + finite BMI | 18.5, 23.0 **제외** |
| `getGlobalBmiRatios` | `recordCount` | finite BMI만 분류 카운트 | 연령 19·80+ **포함** |

### 2.3 main 데모 (`SHealthBMI.cpp`, Golden 비대상)

6연령대 FR-08 행 **이후** 추가:

```
Normal BMI users (count=N): id1, id2, ...
Global - underweight = %f, normal = %f, overweight = %f, obesity = %f
```

### 2.4 테스트

| TC ID | 스위트 | 검증 |
|-------|--------|------|
| **TC-LST-01** | `NormalBmiUsers` | 저체중·정상·비만 3명 → id 2만 |
| **TC-LST-02** | `NormalBmiUsers` | BMI 경계(18.5/23) id 제외 |
| **TC-LST-03** | `NormalBmiUsers` | `calculateBmi` 미호출 → 빈 목록 |
| **TC-GLB-01** | `GlobalBmiRatios` | 4분류 합 ≈ 100% |
| **TC-GLB-02** | `GlobalBmiRatios` | age=19: 연령대 API 0%, global normal 100% |
| **TC-GLB-03** | `GlobalBmiRatios` | 미호출 → zero ratios |

### 2.5 Golden Master

| 항목 | 처리 |
|------|------|
| FR-08 6행 | 수치·포맷 **불변** — baseline 갱신 **불필요** |
| FR-C01/C02 2행 | `SHealthGoldenTest` — actual stdout **앞 6행만** 비교 (`resize`) |
| `shealth_bmi_stdout.golden.txt` | diff 0 유지 |

## 3. 테스트 결과

```
build-gcc: cmake --build . && ctest --output-on-failure
52/52 Passed (51 unit + 1 Golden)
```

## 4. 산출물

| 산출물 | 경로 | 상태 |
|--------|------|------|
| 헤더/구현 | `src/main/cpp/SHealth.h`, `SHealth.cpp` | 수정 |
| main | `src/main/cpp/SHealthBMI.cpp` | 데모 2행 추가 |
| 단위 테스트 | `src/test/cpp/SHealthBMITest.cpp` | TC-LST-*, TC-GLB-* |
| Golden 테스트 | `src/test/cpp/SHealthGoldenTest.cpp` | 6행만 비교 |
| 구현 노트 | `docs/feature_implementation_notes.md` | §15 |
| 작업 보고서 | `Report/15_정상목록_전체비율_FR-C01_FR-C02_report.md` | 본 문서 |
| Transcript | `Prompting/15_정상목록_전체비율_FR-C01_FR-C02.md` | 생성 |

## 5. Wrap-up

| 항목 | 내용 |
|------|------|
| FR-C01 | id 파싱·정상 목록 API 완료 |
| FR-C02 | 전체 4분류 비율 API 완료 (연령대 API와 독립) |
| I-11 | id 미사용 이슈 해소 |
| 다음 Step | 16 FR-S01 SRP 책임 분리 |
