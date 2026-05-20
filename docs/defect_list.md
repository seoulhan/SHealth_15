# SHealth BMI — 결함 목록 (Defect List)

| 항목 | 내용 |
|------|------|
| 문서 버전 | 1.1 |
| 작성일 | 2026-05-20 |
| Step | 08 — 결함 목록 문서화 · **1.1 갱신 Step 10** |
| commit string (권장) | `10_결함_관리_보고_Defect_Management` |
| 입력 | `docs/defect_detection.md`, `docs/requirements_analysis.md`, `docs/test_cases.md` |
| 구현 | `src/main/cpp/SHealth.cpp`, `src/test/cpp/SHealthBMITest.cpp` |

---

## 1. 요약 (Executive Summary)

| 구분 | 건수 | 비고 |
|------|------|------|
| **Open** | 2 | Activity 4·운영 (DEF-002, DEF-011) |
| **Fixed** | 5 | DEF-001, 003, 005, 006, 007 (Step 10) |
| **Won't fix** | 4 | DEF-004, 008~010 (설계·문서 계약) |
| **ctest** | **39** unit + **1** Golden | Step 10: `TC_EXC_11` 추가; **40/40 Passed** |

**우선 조치:** DEF-002 (FR-S02 height=0, **Activity 4**), DEF-011 (CWD·CLI, **P3 문서**).

---

## 2. Severity · Priority 정의

| Severity | 기준 | Priority 매핑 |
|----------|------|----------------|
| **Critical** | 데이터 손실·전체 서비스 중단 | P0 |
| **High** | 잘못된 입력 1건으로 배치 중단 | P0 |
| **Medium** | Should 미구현·미검증 상한·회귀 공백 | P1~P2 |
| **Low** | 계약 모호·운영·문서 | P2~P3 |

| Priority | 의미 |
|----------|------|
| **P0** | 다음 스프린트 즉시 수정 |
| **P1** | 기능 스펙(Should) 반영 |
| **P2** | 요구 명확화·방어 코드·회귀 |
| **P3** | 문서·운영 개선 |

---

## 3. README BMI 경계 정의 vs 구현 불일치 (별도 표기)

README: *「18.5초과 23미만 정상체중, 23이상 25미만 과체중, **25이상 비만**」* (`README.md` L7).

| BMI | README 기대 | 구현 (현행 `classifyBmi`) | 결함 ID | 상태 |
|-----|-------------|---------------------------|---------|------|
| 18.5 | 저체중 | `<= 18.5` → 저체중 | — | 일치 |
| 18.5 < x < 23 | 정상 | `< 23` (and `> 18.5`) | — | 일치 |
| 23 ≤ x < 25 | 과체중 | `< 25` (and `>= 23`) | — | 일치 |
| **25.0** | **비만** | **비만** (`Obesity`, `bmi >= 25` 분기) | **DEF-007** | **Fixed** |
| 25 < x | 비만 | 비만 | — | 일치 |

> **이력:** Step 04 이전에는 `bmi > 25`만 비만으로 처리되어 BMI=25.0이 **과체중**으로 분류됨 (`requirements_analysis.md` §4.3 스냅샷). Step 04~07에서 수정·`TC_CLS_06` / `TC_BMI_02`로 고정. **현재 README와 구현 일치.**

**소스:** `SHealth.cpp` L25–35 (`classifyBmi`); 상수 `BMI_OVERWEIGHT_MAX` 등은 `SHealth.h`.

---

## 4. 결함 티켓 목록

### DEF-001 — CSV 컬럼 부족 시 프로세스 abort

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-001 |
| **이슈 ID** | I-09 |
| **제목** | 데이터 행 컬럼 수 부족 시 `tokens[3]` 범위 초과로 프로세스 종료 |
| **상태** | **Fixed** (Step 10) |
| **Severity** | — (Closed) |
| **Priority** | — |

**재현 단계 (과거)**

1. CSV: 헤더 + `1,25,70` (height 누락).
2. `calculateBmi(path)` → `tokens[3]` 범위 초과 → **abort**.

**기대 / 실제 (현행)**

| | 내용 |
|---|------|
| **기대** | 잘못된 행 스킵·`cerr` 로그; 유효 행 없으면 `calculateBmi` → **0** |
| **실제 (Step 10)** | `tokens.size() < 4` → 스킵; `TC_EXC_05` → `EXPECT_EQ(..., 0)` Pass |

**추적성**

| 유형 | ID / 이름 |
|------|-----------|
| 요구사항 | FR-01, I-09 |
| 테스트 | `TC_EXC_05`, `TC_EXC_11` (혼합 행 부분 로드) |
| 소스 | `SHealth.cpp` `loadRecordsFromFile` |

---

### DEF-002 — height=0 동연령대 평균 보정 미구현 (FR-S02)

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-002 |
| **이슈 ID** | I-03 |
| **제목** | `height == 0` 레코드에 대한 동연령대 평균 키 보정 없음 |
| **상태** | **Open** |
| **Severity** | **Medium** |
| **Priority** | **P1** |

**재현 단계**

1. 임시 CSV: `1,25,70,0` (체중 양수, 키 0).
2. `calculateBmi(path)` 호출.
3. `getBmiRatio(20, 100~400)` 4종 합산.

**기대 결과 (FR-S02 / README Activity 4)**

- 동일 연령대 `height != 0` 레코드의 **산술 평균 키**로 보정 후 BMI·4분류 비율 산출.
- 연령대 내 4분류 합 ≈ **100%** (유효 BMI 기준).

**실제 결과 (As-Is)**

- 보정 없음 → `height_m = 0` → BMI **inf/NaN**.
- `aggregateAgeBandStatistics`에서 `!std::isfinite(bmis[i])` → 분류 스킵 → **4분류 합 0%**.

**추적성**

| 유형 | ID / 이름 |
|------|-----------|
| 요구사항 | **FR-S02** (Should), I-03 |
| 테스트 | `TC-HGT-01` `TC_HGT_01_HeightZeroWeightPositive`, `TC-HGT-02` `TC_HGT_02_HeightZeroWeightZero` (합 0% 기대, Pass) |
| 소스 | `computeBmis()` L110–114; 보정 루프 없음 (`imputeMissingWeightsByAgeBand`만 존재 L84–107) |

**비고:** `shealth.dat` 샘플에는 height=0 미관측; 스펙·Activity 4 기준 갭.

---

### DEF-003 — CSV 비숫자 필드 예외 미처리

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-003 |
| **이슈 ID** | I-09b |
| **제목** | `stoi`/`stod` 예외가 `calculateBmi`에서 catch되지 않음 |
| **상태** | **Fixed** (Step 10) |
| **Severity** | — (Closed) |
| **Priority** | — |

**재현 / 현행**

| | 내용 |
|---|------|
| **과거** | `stoi`/`stod` 예외 → `calculateBmi` 밖으로 전파 |
| **현행** | `try`/`catch` → 행 스킵·로그; 전부 무효 시 **0** 반환 (`TC_EXC_06` Pass) |

**추적성:** FR-01, I-09b; `TC_EXC_06`, `TC_EXC_11`; `loadRecordsFromFile`.

---

### DEF-004 — 연령대 4분류 비율 합 ≠ 100% (요구 불명확)

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-004 |
| **이슈 ID** | I-10 |
| **제목** | non-finite BMI 레코드 존재 시 4분류 비율 합이 100% 미만 |
| **상태** | **Won't fix** (Step 10 — 옵션 A 채택) |
| **Severity** | Low |
| **Priority** | P2 |

**재현 단계**

1. `TC_HGT_01` fixture: `1,25,70,0`.
2. 20대 `getBmiRatio` 4종 합산.

**기대 결과 (이상적·미정의)**

- README는 “비율”만 명시; **합=100%** 미명시.
- **Step 10 결정 (A):** 분모=`bandMemberCount` 유지, non-finite BMI는 분류 제외·합<100% **허용**. FR-S02 구현 시 (C)로 TC 갱신 검토.

**실제 결과**

- 분모: 연령대 **전원** (`bandMemberCount`).
- 분자: `isfinite(bmi)` 인원만 카운트 → **합 0%** (`TC_HGT_01`) 또는 **합 < 100%**.

**추적성**

| 유형 | ID / 이름 |
|------|-----------|
| 요구사항 | FR-05, I-10 |
| 테스트 | `TC-HGT-01/02` (합 0%); `TC-BMI-03` (합 100%, finite만) |
| 소스 | `SHealth.cpp` L117–159 (`aggregateAgeBandStatistics`) |

---

### DEF-005 — 레코드 10,000건 초과 미방어

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-005 |
| **이슈 ID** | I-06 |
| **제목** | 고정 배열 `ages[10000]` 초과 시 버퍼 오버플로우 위험 |
| **상태** | **Fixed** (Step 10 — 방어적 상한) |
| **Severity** | — (Closed) |
| **Priority** | — |

**재현 / 현행**

| | 내용 |
|---|------|
| **과거** | `recordCount >= MAX_RECORDS` 검사 없음 → 오버플로우 위험 |
| **현행** | `recordCount >= MAX_RECORDS` 시 `cerr` 로그 후 **추가 로드 중단** (`SHealth.h` `MAX_RECORDS=10000`) |

**추적성:** I-06, NFR-07; `loadRecordsFromFile`; 10k+ 전용 TC는 미추가(부하·범위 외).

---

### DEF-006 — Golden Master / E2E 회귀 테스트 — **Fixed**

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-006 |
| **이슈 ID** | GM-01 |
| **제목** | `SHealthBMI` stdout Golden Master 자동 회귀 TC 없음 |
| **상태** | **Fixed** |
| **Severity** | — (Closed) |
| **Priority** | — |

**재현 단계 (과거)**

1. `build-gcc/SHealthBMI.exe` 실행 (CWD에 `shealth.dat`).
2. 6연령대 stdout 수동 확인만 가능.

**기대 / 실제 (현행)**

| | 내용 |
|---|------|
| **기대 (NFR-05)** | `shealth.dat` 기준 stdout **자동 비교** + `ctest` 등록 |
| **실제 (Step 09)** | `tests/golden/shealth_bmi_stdout.golden.txt`, `SHealthGoldenTest`, `ctest -R SHealthGoldenMaster` |

**추적성**

| 유형 | ID / 이름 |
|------|-----------|
| 요구사항 | NFR-05, FR-08, §6.3 main 출력 계약 |
| 테스트 | `SHealthGoldenMaster.TC_GM_01_SHealthBMI_stdout_matches_baseline` |
| 문서 | `docs/golden_master.md` |
| 소스 | `SHealthBMI.cpp`, `src/test/cpp/SHealthGoldenTest.cpp` |

---

### DEF-007 — BMI=25.0 비만 분류 불일치 (README) — **Fixed**

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-007 |
| **이슈 ID** | I-01 |
| **제목** | BMI 25.0이 README상 비만이나 과체중으로 분류됨 |
| **상태** | **Fixed** |
| **Severity** | — (Closed) |
| **Priority** | — |

**재현 단계 (과거)**

1. `classifyBmi(25.0)` 또는 `weight=100, height=200` → BMI=25 통합 경로.

**기대 / 실제 (현행)**

| | 내용 |
|---|------|
| **기대 (README)** | BMI ≥ 25 → **비만** |
| **실제 (현행)** | `BmiCategory::Obesity` — **일치** |

**추적성**

| 유형 | ID / 이름 |
|------|-----------|
| 요구사항 | FR-04, README §BMI 기준 |
| 테스트 | `TC-CLS-06`, `TC-BMI-02` — Pass |
| 소스 | `SHealth.cpp` L25–35 |

**비고:** §3 README 경계 표 참조. `requirements_analysis.md` §4.3 “×” 행은 **구 Refactoring 전** 스냅샷.

---

### DEF-008 — 동연령대 전원 weight=0 시 저체중 100%

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-008 |
| **이슈 ID** | I-02 |
| **제목** | 보정 대상 없을 때 BMI=0 → 저체중 100% 집계 |
| **상태** | **Won't fix** (현행 계약·TC로 확정) |
| **Severity** | Low |
| **Priority** | P3 |

**재현 단계**

1. 40대 전원 `weight=0` CSV (`TC_IMP_03`).

**기대 / 실제**

| | 내용 |
|---|------|
| **기대 (TC)** | 보정 스킵, 20대 저체중 **100%** |
| **실제** | `validWeightCount==0` → 가드 후 BMI=0 → `classifyBmi(0)` 저체중 |

**추적성:** FR-03, `TC-IMP-03`, `imputeMissingWeightsByAgeBand` L98–99.

---

### DEF-009 — 빈 연령대 비율 0%

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-009 |
| **이슈 ID** | I-05 |
| **제목** | 데이터 없는 연령대 `getBmiRatio` → 0.0 |
| **상태** | **Won't fix** |
| **Severity** | Low |
| **Priority** | P3 |

**재현:** 30대만 데이터 → 20대 비율 0 (`TC_RAT_05`).  
**추적성:** FR-05/FR-06, `TC-RAT-05`, `aggregateAgeBandStatistics` L150–152.

---

### DEF-010 — `getBmiRatio` 잘못된 인자 시 조용히 0.0

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-010 |
| **이슈 ID** | I-08 |
| **제목** | 잘못된 `ageClass`/`type`과 미호출 상태 구분 불가 (모두 0.0) |
| **상태** | **Won't fix** (문서·TC 계약) |
| **Severity** | Low |
| **Priority** | P3 |

**재현:** `TC_RAT_02/03/04`, `TC_EXC_08`.  
**소스:** `SHealth.cpp` L173–197.

---

### DEF-011 — main `shealth.dat` CWD 의존 (운영)

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-011 |
| **이슈 ID** | I-12 |
| **제목** | 실행 디렉터리에 `shealth.dat` 없으면 로드 실패 |
| **상태** | **Open** (운영·문서) |
| **Severity** | Low |
| **Priority** | **P3** |

**재현:** `build/`에서 `SHealthBMI.exe` 실행 시 상대 경로 실패.  
**추적성:** I-12; `TC-EXC-10`은 fixture **절대경로**만 검증.  
**소스:** `SHealthBMI.cpp` (하드코딩 경로).

---

## 5. 추적성 매트릭스 (요약)

| Defect ID | Issue | Req ID | 테스트 (GTest) | 소스 위치 | 상태 | Sev | Pri |
|-----------|-------|--------|----------------|-----------|------|-----|-----|
| DEF-001 | I-09 | FR-01 | `TC_EXC_05`, `TC_EXC_11` | `loadRecordsFromFile` | **Fixed** | — | — |
| DEF-002 | I-03 | FR-S02 | `TC_HGT_01`, `TC_HGT_02` | `computeBmis`, (보정 없음) | Open | Med | P1 |
| DEF-003 | I-09b | FR-01 | `TC_EXC_06`, `TC_EXC_11` | `loadRecordsFromFile` | **Fixed** | — | — |
| DEF-004 | I-10 | FR-05 | `TC_HGT_*`, `TC_BMI_03` | `aggregateAgeBandStatistics` | Won't fix | Low | P2 |
| DEF-005 | I-06 | §5.1 | (방어 코드) | `loadRecordsFromFile` | **Fixed** | — | — |
| DEF-006 | GM-01 | NFR-05 | `TC_GM_01` (`SHealthGoldenTest`) | `SHealthBMI.cpp` | **Fixed** | — | — |
| DEF-007 | I-01 | FR-04 | `TC_CLS_06`, `TC_BMI_02` | `classifyBmi` L25–35 | **Fixed** | — | — |
| DEF-008 | I-02 | FR-03 | `TC_IMP_03` | `imputeMissingWeightsByAgeBand` | Won't fix | Low | P3 |
| DEF-009 | I-05 | FR-05 | `TC_RAT_05` | `aggregateAgeBandStatistics` | Won't fix | Low | P3 |
| DEF-010 | I-08 | FR-06 | `TC_RAT_02/03/04`, `TC_EXC_08` | `getBmiRatio` L173–197 | Won't fix | Low | P3 |
| DEF-011 | I-12 | FR-08 | `TC_EXC_10` (부분) | `SHealthBMI.cpp` | Open | Low | P3 |

---

## 6. 우선순위별 조치 로드맵

| Priority | Defect | 조치 | Step |
|----------|--------|------|------|
| P0 | DEF-001 | 컬럼 수 검증, 행 스킵, `TC_EXC_05` 갱신 | **10 (완료)** |
| P1 | DEF-002 | height=0 보정 + `TC_HGT_*` | **Activity 4** |
| P1 | DEF-003 | parse try-catch, 부분 로드 | **10 (완료)** |
| P2 | DEF-004 | 옵션 A Won't fix | **10 (완료)** |
| P2 | DEF-005 | `MAX_RECORDS` 방어 | **10 (완료)** |
| P2 | DEF-006 | Golden Master + ctest | **09 (완료)** |
| P3 | DEF-011 | API·CLI 경로 | 11 |

---

## 7. 변경 이력

| 버전 | 일자 | 변경 |
|------|------|------|
| 1.0 | 2026-05-20 | Step 08 — `defect_detection.md` §9 후보 티켓화, README 경계 별도 §3 |
| 1.1 | 2026-05-20 | Step 10 — DEF-001/003/005 Fixed, DEF-004 Won't fix(A), ctest 40/40 |

**다음 Step:** Activity 4 — DEF-002 FR-S02, Golden baseline 재검토, DEF-011 CLI.
