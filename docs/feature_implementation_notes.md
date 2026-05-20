# SHealth BMI — 기능 구현 노트 (Feature Implementation Notes)

| 항목 | 내용 |
|------|------|
| 문서 | Step 13+ 구현 시 누적 |
| 설계 기준 | `docs/feature_requirements_design.md` |

---

## §13 — FR-S02: height=0 동연령대 평균 키 보정 (Step 13)

| 항목 | 내용 |
|------|------|
| 요구 ID | FR-S02 |
| 결함 | DEF-002 (I-03) |
| 구현일 | 2026-05-20 |

### 13.1 API·파이프라인

`calculateBmi` 호출 순서 (Normative):

1. `loadRecordsFromFile`
2. `imputeMissingWeightsByAgeBand`
3. `imputeMissingHeightsByAgeBand` ← **신규**
4. `computeBmis`
5. `aggregateAgeBandStatistics`

```cpp
void imputeMissingHeightsByAgeBand();  // private, SHealth.cpp
```

### 13.2 알고리즘

`imputeMissingWeightsByAgeBand()`와 **대칭**:

- 연령대 `bandStart` ∈ {20, 30, …, 70} (`[bandStart, bandStart+10)`)
- 동일 연령대에서 `height != 0.0` (부동소수 **정확 비교**) 레코드만 평균 산출
- `validHeightCount == 0`이면 해당 연령대 **스킵** (I-02)
- `height == 0.0` 레코드에 `averageHeight` in-place 대입

weight·height 보정은 서로의 0값을 평균에 포함하지 않으며, **weight → height** 순서는 결과에 영향 없음(설계 §3.1).

### 13.3 엣지·집계

| 시나리오 | 처리 |
|----------|------|
| 동연령대 유효 키 0건 / 전원 height=0 | 보정 스킵 → BMI non-finite → `aggregate`에서 `!isfinite` 분류 제외 | 
| height=0, weight>0, 동연령대 유효 키 있음 | 평균 키 대입 후 유한 BMI·분류 | 
| height=0, weight=0 | weight 보정 선행; 각각 유효값 없으면 해당 단계 스킵 |
| 타 연령대 키 사용 | **금지** (교차 보정 없음) |

non-finite BMI 집계 정책은 As-Is 유지: `bandMemberCount`에는 포함, 4분류 분자에서는 제외.

### 13.4 테스트

| TC ID | 스위트 | 검증 |
|-------|--------|------|
| TC-HGT-10 | `HeightImputation` | 30대 h=170,175,0 → 172.5; 유한 BMI·Normal 포함·합 100% |
| TC-HGT-11 | `HeightImputation` | 40대 전원 h=0 → 스킵, 4분류 합 0% |
| TC-HGT-01/02 | (유지) | 단독 레코드·동연령대 유효 키 없음 → As-Is와 동일 non-finite |

### 13.5 Golden Master

`shealth.dat`에 height=0 미관측 → FR-S02만으로 **baseline 변경 불필요** (`feature_requirements_design.md` §7.2 E-S02-06).

### 13.6 알려진 제약

- id 컬럼 미파싱 (Step 15 FR-C01에서 해결 예정)
- `height == 0.0`만 보정 대상 (근사 0·음수 키는 미처리)
- 전역·연령대 밖(age 19, 80+) 레코드는 연령대 집계 API에 미반영 (기존과 동일)

---

## §14 — FR-S03: 연령대 BMI 4분류 비율 명시적 API (Step 14)

| 항목 | 내용 |
|------|------|
| 요구 ID | FR-S03 |
| 구현일 | 2026-05-20 |

### 14.1 API 계약

| API | 시그니처 | 설명 |
|-----|----------|------|
| **신규** | `const AgeBandRatios& getAgeBandRatios(int ageClass) const` | 연령대 4분류 비율(% 단위, 0~100)을 **한 번에** 반환 |
| **유지** | `double getBmiRatio(int ageClass, int type)` | legacy `type` 100~400; 내부는 `getAgeBandRatios` + `ratioForCategory` **위임** (SSOT) |

**`ageClass` 유효값:** 20, 30, 40, 50, 60, 70 (`MIN_AGE_BAND`~`MAX_AGE_BAND`, 10 간격).  
**잘못된 `ageClass`:** `getAgeBandRatios` → 모든 필드 0.0인 `AgeBandRatios` (정적 빈 객체 참조); `getBmiRatio` → **0.0** (As-Is).

**`calculateBmi` 선행:** 집계 전 호출 시 초기 0.0과 동일.

### 14.2 `BmiCategory` ↔ legacy `type` 매핑

| `BmiCategory` (enum) | 정수값 | legacy `type` (`getBmiRatio`) | `AgeBandRatios` 필드 |
|----------------------|--------|-------------------------------|----------------------|
| `Underweight` | 100 | 100 | `underweight` |
| `Normal` | 200 | 200 | `normal` |
| `Overweight` | 300 | 300 | `overweight` |
| `Obesity` | 400 | 400 | `obesity` |

`enum class BmiCategory : int` — `static_cast<int>(cat)`가 legacy `type`과 동일.

### 14.3 비율 의미 (변경 없음)

- **분모:** 해당 연령대 전체 레코드 수 (`bandMemberCount`).
- **분자:** `isfinite(bmis[i])`인 레코드만 `classifyBmi`로 카운트.
- **단일 연령대·유한 BMI만:** 4분류 합 **100% ± 0.01** (`TC-API-04`, `TC-CLS-08`).

### 14.4 마이그레이션

| 소비자 | 권장 |
|--------|------|
| 신규 코드 | `getAgeBandRatios(ageClass)` — 4회 `getBmiRatio` 호출 대체 |
| 기존·Golden·main | `getBmiRatio` **그대로 사용 가능** (동일 수치) |
| `SHealthBMI.cpp` | `getAgeBandRatios`로 4필드 출력; **printf 포맷 문자열 변경 없음** (FR-08) |

### 14.5 테스트

| TC ID | 스위트 | 검증 |
|-------|--------|------|
| TC-API-01 | `AgeBandDistributionApi` | 6연령대: `getAgeBandRatios` == 4× `getBmiRatio` |
| TC-API-02 | `AgeBandDistributionApi` | `shealth.dat` 6×4 스냅샷 (Golden baseline 수치) |
| TC-API-03 | `AgeBandDistributionApi` | 잘못된 ageClass → zero ratios / `getBmiRatio` 0.0 |
| TC-API-04 | `AgeBandDistributionApi` | 20대 4명 각 1분류 → 합 ≈ 100% |

### 14.6 Golden Master

FR-S03은 집계 로직 무변경·main `%f` 6자리 포맷 유지 → **baseline 갱신 불필요**. `SHealthBMI`는 신 API로 동일 수치 출력.

---

## §15 — FR-C01/C02: 정상 BMI 목록·전체 범주 비율 (Step 15)

| 항목 | 내용 |
|------|------|
| 요구 ID | FR-C01, FR-C02 |
| 구현일 | 2026-05-20 |

### 15.1 API 계약

| API | 시그니처 | 설명 |
|-----|----------|------|
| **신규** | `std::vector<int> getNormalBmiUserIds() const` | `18.5 < BMI < 23` (`BmiCategory::Normal`)인 사용자 **id** 목록 (오름차순, 중복 없음) |
| **신규** | `const AgeBandRatios& getGlobalBmiRatios() const` | **전체 로드 레코드** 기준 4분류 비율(%) — 연령대 API와 **독립** |

**전제 조건:** `calculateBmi`가 성공(`recordCount > 0`)하여 파이프라인(로드→보정→BMI→연령대·전체 집계)이 완료된 뒤에만 유효한 값을 반환한다.

| 호출 시점 | `getNormalBmiUserIds` | `getGlobalBmiRatios` |
|-----------|----------------------|----------------------|
| `calculateBmi` **미호출** | 빈 `vector` | 모든 필드 0.0 (`kEmptyAgeBandRatios`) |
| `calculateBmi` 실패(0건) | 빈 `vector` | 모든 필드 0.0 (`statisticsReady` false) |
| `calculateBmi` 성공 | 캐시된 정상 id 목록 | `globalBmiRatios` |

내부 플래그 `statisticsReady`는 `aggregateGlobalBmiStatistics()` 완료 시 `true`, `loadRecordsFromFile` 시작 시 `false`.

### 15.2 데이터 모델·파이프라인

| 변경 | 내용 |
|------|------|
| `ids[]` | CSV `tokens[0]` → `std::stoi`; 빈 id·비숫자·파싱 예외 시 **행 스킵** (Step 10 CSV 정책·E-C01-03) |
| `aggregateGlobalBmiStatistics()` | `calculateBmi` 마지막 단계 — `aggregateAgeBandStatistics` **이후** |

`calculateBmi` 순서 (To-Be):

1. `loadRecordsFromFile`
2. `imputeMissingWeightsByAgeBand`
3. `imputeMissingHeightsByAgeBand`
4. `computeBmis`
5. `aggregateAgeBandStatistics`
6. `aggregateGlobalBmiStatistics` ← **신규**

### 15.3 집계 규칙

**FR-C01 (정상 목록)**

- 포함: `isfinite(bmis[i])` 이고 `classifyBmi(bmi) == Normal` (`18.5 < BMI < 23`)
- 제외: BMI=18.5, 23.0 (경계), non-finite BMI, 저체중·과체중·비만

**FR-C02 (전체 비율)**

| 항목 | 규칙 |
|------|------|
| 분모 | `recordCount` (로드 성공 건수) |
| 분자 | `isfinite(bmis[i])` 인 레코드만 `classifyBmi` 카운트 |
| non-finite | 분자 제외·**분모 포함** (연령대 집계와 동일) |
| 연령 19·80+ | 연령대 API에는 미반영, **전체 API에는 포함** (`TC-GLB-02`) |

유한 BMI만 있는 fixture에서 4분류 합 **100% ± 0.01** (`TC-GLB-01`).

### 15.4 main 데모 출력 (Golden 비대상)

FR-08 **6연령대 행** 뒤에 추가 (`SHealthBMI.cpp`):

```
Normal BMI users (count=N): id1, id2, ...
Global - underweight = %f, normal = %f, overweight = %f, obesity = %f
```

- `%f` 6자리 — 기존 6행과 동일
- **`SHealthGoldenTest`는 앞 6행만 비교** → baseline **갱신 불필요** (추가 줄은 Golden 파서 비대상)

### 15.5 테스트

| TC ID | 스위트 | 검증 |
|-------|--------|------|
| TC-LST-01 | `NormalBmiUsers` | 저체중·정상·비만 3명 → 정상 id만 |
| TC-LST-02 | `NormalBmiUsers` | BMI=18.5·23.0 경계 id **제외** |
| TC-LST-03 | `NormalBmiUsers` | `calculateBmi` 미호출 → 빈 목록 |
| TC-GLB-01 | `GlobalBmiRatios` | 4분류 합 ≈ 100% |
| TC-GLB-02 | `GlobalBmiRatios` | age=19: `getBmiRatio(20,*)==0`, global normal>0 |
| TC-GLB-03 | `GlobalBmiRatios` | `calculateBmi` 미호출 → zero ratios |

### 15.6 Golden Master

- FR-08 6행: 포맷·수치 **불변** — `getAgeBandRatios` 경로 유지
- FR-C01/C02 데모 2행 추가: Golden **비포함** (`feature_requirements_design.md` §7.1)
- baseline 갱신: **불필요** (의도적 drift 없음)

---

## §16 — FR-S01: SRP 책임 분리 (Step 16)

| 항목 | 내용 |
|------|------|
| 요구 ID | FR-S01 |
| 구현일 | 2026-05-20 |

### 16.1 구조

| 모듈 | 책임 |
|------|------|
| `SHealthTypes.h` | `BmiCategory`, `AgeBandRatios` |
| `SHealthCsvLoader` | CSV 읽기·토큰화·행 스킵 |
| `SHealthImputer` | weight/height 0 동연령대 보정 |
| `SHealthDomain` | `classifyBmi`, 연령대 헬퍼, `computeBmis` |
| `SHealthStatistics` | 연령대·전체 집계 |
| `SHealthPresenter` | stdout (`printf`) |
| `SHealth` | Facade — 저장소·오케스트레이션·public 조회 |

`SHealthBMI.cpp`: `main`만 — `calculateBmi` + Presenter 3함수 호출.

### 16.2 API·동작

- **public 시그니처 변경 없음** (breaking note 없음).
- 파이프라인 순서·집계 정책·Golden 6행 포맷: Step 15와 **동일**.
- `testClassifyBmi` / `testIsInAgeBand`: `SHealthDomain` 위임.

### 16.3 테스트

| 스위트 | 결과 |
|--------|------|
| Architecture (기존 51 TC) | Green |
| `SHealthGoldenMaster.TC_GM_01` | Green, baseline 불변 |

상세 매트릭스: `docs/feature_srp_refactoring.md`.
