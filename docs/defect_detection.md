# SHealth BMI — 테스트 실행 & 결함 분석 (Defect Detection)

| 항목 | 내용 |
|------|------|
| 문서 버전 | 1.0 |
| 작성일 | 2026-05-20 |
| Step | 07 — 테스트 실행 & 결함 분석 |
| commit string (권장) | `07_테스트_실행_결함_분석_Defect_Detection` |
| 기준 | `docs/test_cases.md`, `docs/test_plan.md`, `docs/requirements_analysis.md` |
| 구현 | `src/main/cpp/SHealth.cpp`, `src/test/cpp/SHealthBMITest.cpp` |

---

## 1. 실행 요약 (Executive Summary)

| 항목 | 결과 |
|------|------|
| **ctest 결과** | **38 / 38 Passed** (0 Failed) |
| **Total Test time** | 약 0.76 s (real) |
| **빌드 디렉터리** | `build-gcc` (MinGW Makefiles) |
| **테스트 실행 파일** | `build-gcc/SHealthBMITest.exe` |
| **Google Test** | 1.14 (FetchContent) |
| **통합 스모크** | `SHealthBMI.exe` + `shealth.dat` — 6연령대 stdout 정상 (§6) |

**결론:** 현재 P0 단위 테스트 스위트 기준 **실패(Failed) TC 없음**.  
다만 **통과하는 TC가 문서화하는 As-Is 결함**(I-09 CSV 크래시, I-03 height=0 미보정 등)과 **요구사항 대비 잔여 갭**(FR-S02, Golden Master 미구축)은 Step 08 `defect_list.md`로 이관한다.

---

## 2. 실행 환경 & 재현 절차

### 2.1 권장 환경 (본 분석 기준)

| 항목 | 값 |
|------|-----|
| OS | Windows 10/11, PowerShell |
| CMake | 3.10+ |
| Generator | `MinGW Makefiles` |
| 빌드 경로 | `C:\DEV\SHealth_15\build-gcc` |

### 2.2 표준 재현 절차

```powershell
cd C:\DEV\SHealth_15
mkdir build-gcc -ErrorAction SilentlyContinue
cd build-gcc
cmake -G "MinGW Makefiles" ..
cmake --build .
ctest -V
```

요약만 볼 때:

```powershell
cd C:\DEV\SHealth_15\build-gcc
ctest --output-on-failure
```

단일 TC 재현 예:

```powershell
.\SHealthBMITest.exe --gtest_filter=SHealthBMITestFixture.TC_EXC_05_MalformedCsvTooFewColumns
```

### 2.3 `build/` 디렉터리 (README 기본 경로)

| 시도 | 결과 |
|------|------|
| `cd build; cmake ..; cmake --build .` | **실패** — `nmake` 미설치, `CMAKE_CXX_COMPILER not set` |
| `ctest -V` | 테스트 **0건** (타깃 미빌드) |

**권장:** Windows 실습 환경에서는 README의 `build` 대신 **`build-gcc`** 사용. Step 09 Golden Master·CI 문서에 generator 통일 명시 필요 (I-12 연계).

---

## 3. ctest 실행 로그 요약

**실행 일시:** 2026-05-20  
**명령:** `cmake --build .` → `ctest -V` (`build-gcc`)

```text
100% tests passed, 0 tests failed out of 38
Total Test time (real) = 0.76 sec
```

### 3.1 스위트별 결과

| 스위트 / 영역 | TC 수 | 결과 | 비고 |
|---------------|-------|------|------|
| `SHealthBmiClassification` | 7 | 전부 Passed | BMI 18.5 / 23 / 25 경계 직접 검증 |
| `SHealthAgeBand` | 1 | Passed | 연령대 `[band, band+10)` |
| `SHealthBMITestFixture` — BMI | 3 | Passed | 공식·cm→m |
| `SHealthBMITestFixture` — IMP | 5 | Passed | weight=0 보정 |
| `SHealthBMITestFixture` — CLS/AGE/RAT/EXC/HGT | 22 | Passed | 통합·API·예외·height=0 As-Is |
| **합계** | **38** | **0 Failed** | `gtest_discover_tests` 등록과 일치 |

### 3.2 과거 실패 이력 (Stale — 현재 Green)

`build-gcc/Testing/Temporary/LastTestsFailed.log`에 아래 5건이 남아 있으나, **동일 커밋·재빌드 후 전부 Passed** 확인:

| # | GTest 이름 | 과거 실패 추정 원인 |
|---|------------|---------------------|
| 1 | `TC_IMP_01_BasicImputationSameBmi` | Step 04~06 중간 빌드(보정/`classifyBmi` 미반영) |
| 2 | `TC_IMP_02_ThreeMemberAverage` | 동일 |
| 3 | `TC_IMP_04_NoCrossBandImputation` | 동일 |
| 4 | `TC_AGE_03_Age29Vs30` | 연령대 집계 리팩토링 전 |
| 5 | `TC_EXC_05_MalformedCsvTooFewColumns` | `EXPECT_DEATH` 환경·디버그 빌드 차이 가능 |

**조치:** 로그 파일은 CTest 캐시이므로 최신 Green 실행 후 자동 갱신됨. **현 시점 결함으로 분류하지 않음.**

---

## 4. 실패 패턴 분류 (Failure Pattern Taxonomy)

현재 **실패 TC가 없으므로**, Step 07에서는 **잠재·문서화 결함**을 요구사항·구현·테스트 관점으로 분류한다.

| 패턴 ID | 설명 | 대표 이슈 | ctest |
|---------|------|-----------|-------|
| **P-A** | BMI 경계 불일치 | I-01 (BMI=25) | Green — **수정 완료로 판정** |
| **P-B** | 통계 비율 합 ≠ 100% | I-10, I-03 (non-finite 분모 포함) | Green (As-Is 기대값으로 고정) |
| **P-C** | 보정 로직 (weight=0) | I-02 | Green (`TC_IMP_*`) |
| **P-D** | CSV/파싱 견고성 | I-09 | Green (`EXPECT_DEATH` = 결함 존재 인정) |
| **P-E** | 미구현 Should (FR-S02) | I-03 height=0 | Green (`TC_HGT_*` As-Is 문서화) |
| **P-F** | API 계약 모호 | I-08 | Green |
| **P-G** | 회귀·E2E 공백 | Golden Master 없음 | ctest 미포함 |

---

## 5. 결함·갭 상세 분석

### 5.1 분석 매트릭스 (요구사항 vs 구현 vs 테스트)

| ID | 제목 | 요구사항 | 구현 (As-Is) | 테스트 | 판정 | Severity |
|----|------|----------|--------------|--------|------|----------|
| **I-01** | BMI=25 비만 분류 | README ≥25 비만 | `classifyBmi`: `bmi < 25` → 과체중, 그 외 비만 → **25.0 비만** | `TC_CLS_06`, `TC_BMI_02` Pass | **Fixed** (Step 04 반영, TC로 고정) | — (Closed) |
| **I-02** | 연령대 전원 weight=0 | 보정 후 BMI 산출 | `validWeightCount==0` → 보정 스킵, BMI=0 → 저체중 100% | `TC_IMP_03` Pass | **Accepted** (TC가 현행 계약 정의) | Low |
| **I-03** | height=0 보정 | FR-S02 (Should) | 보정 없음; `height=0` → inf/NaN BMI, 집계 시 `isfinite` 스킵 | `TC_HGT_01/02` Pass (합 0%) | **Open — 코드 갭** | Medium |
| **I-09** | CSV 컬럼 부족 | graceful 실패 기대(향후) | `tokens[3]` 접근 → **프로세스 abort** | `TC_EXC_05` **`EXPECT_DEATH` Pass** | **Open — 코드 버그** (TC는 결함 기록) | High |
| **I-09b** | CSV 비숫자 age | 예외 처리 | `stoi` → 예외 전파 | `TC_EXC_06` `EXPECT_THROW` Pass | **Open — 부분** (catch 미구현, throw는 정상) | Medium |
| **I-10** | 4분류 비율 합 | 연령대 내 100% 기대(이상) | 분모=`bandMemberCount`(전원), 분자=finite BMI만 | `TC_HGT_*` 합 0%; `TC_BMI_03` 합 100% | **요구 불명확 / 설계** | Low |
| **I-05** | 빈 연령대 | 0% 또는 N/A | `bandMemberCount==0` → 0% | `TC_RAT_05` Pass | **Accepted** | Low |
| **I-06** | 10,000건 초과 | 미정의 | 배열 고정, 검사 없음 | **TC 없음** | **Open — 미검증** | Medium |
| **I-08** | 잘못된 API 인자 | 0 vs 오류 구분 | 항상 0.0 | `TC_RAT_02/03`, `TC_EXC_08` Pass | **문서화됨** | Low |
| **I-12** | CWD·경로 | fixture/상대 경로 | main `shealth.dat` CWD 의존 | `TC_EXC_10` fixture 절대경로만 | **Open — 운영** | Low |
| **GM-01** | Golden Master | NFR-05 회귀 | 미구축 | 없음 | **Open — Step 09** | Medium |

---

### 5.2 [P-A] BMI 경계 — I-01 (Closed)

**README:** 25 이상 → 비만.  
**구현 (`classifyBmi`):**

```25:35:src/main/cpp/SHealth.cpp
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

`BMI=25.0` → 마지막 분기 **Obesity**. `docs/requirements_analysis.md` §4.3의 “과체중” 기술은 **구 Refactoring 전 스냅샷**이며, Step 07에서 **TC 기준으로 수정 완료** 확정.

| 검증 TC | 기대 | 결과 |
|---------|------|------|
| `TC_CLS_06_ObesityAt25` | Obesity | Pass |
| `TC_BMI_02_CmToMetersObesityAt25` | BMI=25, 비만 100% | Pass |

**분류:** 과거 **코드 버그** → **Fixed** (테스트 기대값 정확).

---

### 5.3 [P-B] 통계 비율 합산 — I-10 / 집계 분모

**구현 (`aggregateAgeBandStatistics`):**

```125:159:src/main/cpp/SHealth.cpp
        for (int i = 0; i < recordCount; i++) {
            if (!isInAgeBand(ages[i], bandStart)) {
                continue;
            }
            bandMemberCount++;
            if (!std::isfinite(bmis[i])) {
                continue;
            }
            switch (classifyBmi(bmis[i])) {
                // ... 카운트 ...
            }
        }
        // ...
        const double scale = PERCENT_SCALE / bandMemberCount;
```

- **분모:** 연령대 **전체 인원** (`bandMemberCount`)
- **분자:** `isfinite(bmi)` 인원만 4분류에 포함

**결과:** height=0·NaN BMI 등이 있으면 **4분류 합 < 100%** (또는 전부 0%). `TC_HGT_01`은 합 **0%**를 기대 — **테스트는 As-Is를 정확히 반영**.

| 관점 | 판정 |
|------|------|
| 요구사항 | README는 “비율”만 명시, 합=100% 미명시 → **불명확** |
| 구현 | 의도적 스킵 + 분모 유지로 해석 가능 |
| 테스트 | As-Is 고정; FR-S02 구현 시 **TC 갱신 필요** |

**권장:** FR-S02 height 보정 후 `TC_HGT_*`를 “합 ≈ 100%”로 변경; 또는 분모를 `finiteCount`로 바꾸는 제품 결정.

**Severity:** Low (문서·Step 08에서 계약 확정)

---

### 5.4 [P-C] weight=0 보정 — I-02

| 시나리오 | 기대 (TC) | 실제 | TC |
|----------|-----------|------|-----|
| 동연령대 평균 보정 | 0 → 평균 | Pass | `TC_IMP_01`, `02`, `05` |
| 타 연령대 교차 보정 없음 | Pass | `TC_IMP_04` |
| 전원 0 → 스킵, BMI=0 저체중 | Pass | `TC_IMP_03` |

**분류:** 요구(FR-03)와 구현·테스트 **일치**. I-02는 “0/0 NaN” 이슈가 `validWeightCount==0` 가드로 **완화**됨.

---

### 5.5 [P-D] CSV 견고성 — I-09 (Open, High)

#### D-1: 컬럼 부족 → abort

**재현:**

1. `bad_columns.dat`: 헤더 + `1,25,70` (height 컬럼 누락)
2. `calculateBmi(path)` 호출
3. **실제:** `tokens[3]` 범위 초과 → 프로세스 종료
4. **테스트:** `EXPECT_DEATH` → **Pass** (= 결함을 “기대된 죽음”으로 문서화)

**분류:**

| 항목 | 판정 |
|------|------|
| 코드 | **버그** (FR-01 견고 파싱 미충족) |
| 테스트 | **오류 아님** — As-Is 기록 + Step 08+ graceful 목표 |
| 요구사항 | Activity 3 “예외 TC” — abort도 허용 가능하나 **프로덕션 부적합** |

**Severity:** **High** (데이터 1행 오류로 전체 배치 중단)

#### D-2: 비숫자 age

**재현:** `TC_EXC_06` — `EXPECT_THROW(std::exception)` Pass.  
**분류:** 예외는 발생하나 `calculateBmi`에서 **catch·0 반환 없음** → Medium.

---

### 5.6 [P-E] height=0 — I-03 / FR-S02 (Open, Medium)

**재현 (`TC_HGT_01`):**

```text
CSV: 1,25,70,0
calculateBmi → 1건 로드
getBmiRatio 20대 4종 합 = 0.0
```

**분류:** README Activity 4 Should — **구현 누락**; 테스트는 **의도적 As-Is**.

**Severity:** Medium (실데이터 `shealth.dat`에는 height=0 미관측이나 스펙상 Should)

---

### 5.7 [P-G] E2E·Golden Master 공백

**스모크:** 프로젝트 루트에서 `build-gcc\SHealthBMI.exe` 실행, `shealth.dat` 6행 출력 확인.

```text
20 - underweight = 3.511053, normal = 23.797139, overweight = 11.833550, obesity = 60.858257
...
70 - underweight = 0.529101, normal = 12.345679, overweight = 10.758377, obesity = 76.366843
```

각 연령대 4분류 합 ≈ **100%** (실데이터는 finite BMI 위주).

**갭:** stdout 문자열 **자동 회귀 TC 없음** → Step 09.

---

## 6. 수정 필요 항목 요약 (Actionable)

| 우선순위 | ID | 유형 | 조치 | 담당 Step |
|----------|-----|------|------|-----------|
| **P0** | I-09 | 코드 버그 | 컬럼 수 검증, 잘못된 행 스킵·로그, `calculateBmi` 0 반환 | 08~10 |
| **P1** | I-03 / FR-S02 | 기능 갭 | height=0 동연령대 평균 보정 + `TC_HGT_*` 갱신 | 08~09 |
| **P1** | I-09b | 코드 | `stoi`/`stod` try-catch, 부분 로드 정책 | 08 |
| **P2** | I-10 | 요구 명확화 | 비율 합 정의(분모·finite) 합의 후 TC | 08 |
| **P2** | I-06 | 코드 | `MAX_RECORDS` 초과 방어 | 08+ |
| **P2** | GM-01 | 테스트 | Golden Master + ctest 등록 | 09 |
| **P3** | I-08, I-12 | 설계/운영 | API 계약·CLI 경로 | 10~11 |
| — | I-01 | — | **수정 완료** — defect_list에서 Closed | — |

---

## 7. 테스트 오류 vs 코드 버그 vs 요구 불명확

| TC ID | ctest | 잘못된 기대값? | 코드 버그? | 요구 불명확? |
|-------|-------|----------------|------------|--------------|
| TC-CLS-01~07 | Pass | 아니오 | 아니오 | 아니오 |
| TC-BMI-02 | Pass | 아니오 | **과거 I-01 수정됨** | 아니오 |
| TC-IMP-* | Pass | 아니오 | 아니오 | I-02 경계는 TC로 확정 |
| TC-HGT-* | Pass | 아니오 | **예 (FR-S02 미구현)** | 향후 Should |
| TC-EXC-05 | Pass (DEATH) | 아니오 | **예 (I-09)** | graceful vs abort |
| TC-EXC-06 | Pass (THROW) | 아니오 | **부분 (미처리 throw)** | 예외 정책 |
| TC-BMI-03 | Pass (합 100%) | 아니오 | 아니오 | — |

**테스트 기대값 오류로 판정된 TC: 없음 (0건).**

---

## 8. Severity 정의 (본 프로젝트)

| Level | 기준 | 예 |
|-------|------|-----|
| **Critical** | 데이터 손실·보안·전원 크래시 | (해당 없음) |
| **High** | 잘못된 입력 1건으로 배치 중단 | I-09 CSV 컬럼 부족 |
| **Medium** | 스펙 Should 미구현·미검증 상한 | I-03, I-06, Golden Master |
| **Low** | 계약 모호·문서·운영 | I-08, I-10, I-12 |

---

## 9. Step 08 입력 (defect_list.md 후보)

| Defect 후보 ID | Source | Status | Severity |
|----------------|--------|--------|----------|
| DEF-001 | I-09 / TC-EXC-05 | Open | High |
| DEF-002 | I-03 / TC-HGT | Open | Medium |
| DEF-003 | I-09b / TC-EXC-06 | Open | Medium |
| DEF-004 | I-10 | Open (spec) | Low |
| DEF-005 | I-06 | Open | Medium |
| DEF-006 | GM-01 | Open | Medium |
| DEF-007 | I-01 | **Closed (Fixed)** | — |

---

## 10. 변경 이력

| 버전 | 일자 | 변경 |
|------|------|------|
| 1.0 | 2026-05-20 | Step 07 — `ctest` 38/38 실행, 잠재 결함·패턴 분류, 재현·Severity |

**다음 Step:** `docs/defect_list.md` (Step 08) — 본 문서 §9 후보를 티켓화.
