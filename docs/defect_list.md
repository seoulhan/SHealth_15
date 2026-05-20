# SHealth BMI — 결함 목록 (Defect List)

| 항목 | 내용 |
|------|------|
| 문서 버전 | 1.0 |
| 작성일 | 2026-05-20 |
| Step | 08 — 결함 목록 문서화 |
| commit string (권장) | `08_결함_목록_문서화_defect_list` |
| 입력 | `docs/defect_detection.md`, `docs/requirements_analysis.md`, `docs/test_cases.md` |
| 구현 | `src/main/cpp/SHealth.cpp`, `src/test/cpp/SHealthBMITest.cpp` |

---

## 1. 요약 (Executive Summary)

| 구분 | 건수 | 비고 |
|------|------|------|
| **Open** | 6 | 코드·스펙·회귀 갭 |
| **Fixed** | 1 | README BMI=25 경계 (DEF-007) |
| **Won't fix** | 3 | 설계 수용·문서화 계약 (DEF-008~010) |
| **ctest (P0)** | 38 / 38 Passed | 실패 TC 없음; Open 결함은 As-Is·미구현·회귀 공백 |

**우선 조치:** DEF-001 (CSV 컬럼 부족 → 프로세스 abort, **High**), DEF-002 (FR-S02 height=0, **Medium**).

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
| **상태** | **Open** |
| **Severity** | **High** |
| **Priority** | **P0** |

**재현 단계**

1. CSV 작성: 헤더 `id,age,weight,height` + 본문 `1,25,70` (height 컬럼 누락).
2. `SHealth::calculateBmi(path)` 호출.
3. `loadRecordsFromFile` 내 `std::stod(tokens[3])` 실행.

**기대 결과**

- 잘못된 행 스킵 또는 오류 로그 후 `calculateBmi` → **0** 반환 (graceful 실패).
- FR-01 견고 파싱; 배치 전체는 중단되지 않음.

**실제 결과**

- `tokens` 크기 < 4 → **미정의 동작 / 프로세스 abort** (플랫폼·빌드에 따라 crash).

**추적성**

| 유형 | ID / 이름 |
|------|-----------|
| 요구사항 | FR-01, I-09 (`requirements_analysis.md` §7) |
| 테스트 | `TC-EXC-05` — `SHealthBMITestFixture.TC_EXC_05_MalformedCsvTooFewColumns` (`EXPECT_DEATH`, Pass) |
| 소스 | `SHealth.cpp` L71–77 (`loadRecordsFromFile`) |

**비고:** 테스트는 As-Is 결함을 **문서화**하며 Green. 수정 후 `EXPECT_DEATH` → 정상 반환 TC로 전환 필요.

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
| **상태** | **Open** |
| **Severity** | **Medium** |
| **Priority** | **P1** |

**재현 단계**

1. CSV: `1,abc,70,170` (age 비숫자).
2. `calculateBmi(path)` 호출.

**기대 결과**

- 행 단위 스킵·로그 또는 `calculateBmi` → 0; 호출자에게 예외 전파 없음.

**실제 결과**

- `std::stoi` → **`std::exception` 전파** (`calculateBmi` 미 catch).

**추적성**

| 유형 | ID / 이름 |
|------|-----------|
| 요구사항 | FR-01, I-09 |
| 테스트 | `TC-EXC-06` `TC_EXC_06_MalformedCsvNonNumericAge` (`EXPECT_THROW`, Pass) |
| 소스 | `SHealth.cpp` L75–77 |

---

### DEF-004 — 연령대 4분류 비율 합 ≠ 100% (요구 불명확)

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-004 |
| **이슈 ID** | I-10 |
| **제목** | non-finite BMI 레코드 존재 시 4분류 비율 합이 100% 미만 |
| **상태** | **Open** (스펙·설계 합의 필요) |
| **Severity** | **Low** |
| **Priority** | **P2** |

**재현 단계**

1. `TC_HGT_01` fixture: `1,25,70,0`.
2. 20대 `getBmiRatio` 4종 합산.

**기대 결과 (이상적·미정의)**

- README는 “비율”만 명시; **합=100%** 미명시.
- 제품 결정: (A) 분모=`bandMemberCount` 유지·합<100% 허용, 또는 (B) 분모=`finiteCount`, 또는 (C) FR-S02 보정 후 합≈100%.

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
| **상태** | **Open** |
| **Severity** | **Medium** |
| **Priority** | **P2** |

**재현 단계**

1. 10,001행 이상 유효 데이터 CSV 준비.
2. `calculateBmi` 호출.

**기대 결과**

- 명시적 오류 반환 또는 `vector` 확장 (NFR-04).

**실제 결과**

- 상한 검사 없음 → **미정의(오버플로우 가능)**.

**추적성**

| 유형 | ID / 이름 |
|------|-----------|
| 요구사항 | §5.1 최대 건수, I-06, NFR-07 |
| 테스트 | **없음** |
| 소스 | `SHealth.h` 배열 크기; `loadRecordsFromFile` L78 `recordCount++` |

---

### DEF-006 — Golden Master / E2E 회귀 테스트 미구축

| 필드 | 내용 |
|------|------|
| **결함 ID** | DEF-006 |
| **이슈 ID** | GM-01 |
| **제목** | `SHealthBMI` stdout Golden Master 자동 회귀 TC 없음 |
| **상태** | **Open** |
| **Severity** | **Medium** |
| **Priority** | **P2** |

**재현 단계**

1. `build-gcc/SHealthBMI.exe` 실행 (CWD에 `shealth.dat`).
2. 6연령대 stdout 수동 확인.

**기대 결과**

- NFR-05: `shealth.dat` 기준 stdout 문자열 **자동 비교** + `ctest` 등록.

**실제 결과**

- 수동 스모크만 수행 (Step 07); **자동 TC 없음**.

**추적성**

| 유형 | ID / 이름 |
|------|-----------|
| 요구사항 | NFR-05, FR-08, §6.3 main 출력 계약 |
| 테스트 | **없음** (Step 09 예정) |
| 소스 | `SHealthBMI.cpp` (main `printf`) |

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
| DEF-001 | I-09 | FR-01 | `TC_EXC_05_MalformedCsvTooFewColumns` | `loadRecordsFromFile` L71–77 | Open | High | P0 |
| DEF-002 | I-03 | FR-S02 | `TC_HGT_01`, `TC_HGT_02` | `computeBmis`, (보정 없음) | Open | Med | P1 |
| DEF-003 | I-09b | FR-01 | `TC_EXC_06_MalformedCsvNonNumericAge` | `loadRecordsFromFile` L75–77 | Open | Med | P1 |
| DEF-004 | I-10 | FR-05 | `TC_HGT_*`, `TC_BMI_03` | `aggregateAgeBandStatistics` L125–159 | Open | Low | P2 |
| DEF-005 | I-06 | §5.1 | — | `loadRecordsFromFile` L78 | Open | Med | P2 |
| DEF-006 | GM-01 | NFR-05 | — | `SHealthBMI.cpp` | Open | Med | P2 |
| DEF-007 | I-01 | FR-04 | `TC_CLS_06`, `TC_BMI_02` | `classifyBmi` L25–35 | **Fixed** | — | — |
| DEF-008 | I-02 | FR-03 | `TC_IMP_03` | `imputeMissingWeightsByAgeBand` | Won't fix | Low | P3 |
| DEF-009 | I-05 | FR-05 | `TC_RAT_05` | `aggregateAgeBandStatistics` | Won't fix | Low | P3 |
| DEF-010 | I-08 | FR-06 | `TC_RAT_02/03/04`, `TC_EXC_08` | `getBmiRatio` L173–197 | Won't fix | Low | P3 |
| DEF-011 | I-12 | FR-08 | `TC_EXC_10` (부분) | `SHealthBMI.cpp` | Open | Low | P3 |

---

## 6. 우선순위별 조치 로드맵

| Priority | Defect | 조치 | Step |
|----------|--------|------|------|
| P0 | DEF-001 | 컬럼 수 검증, 행 스킵, `TC_EXC_05` 갱신 | 08~10 |
| P1 | DEF-002 | height=0 보정 + `TC_HGT_*` | 08~09 |
| P1 | DEF-003 | parse try-catch, 부분 로드 정책 | 08 |
| P2 | DEF-004 | 비율 합 계약 문서·TC 합의 | 08 |
| P2 | DEF-005 | `MAX_RECORDS` 방어 또는 `vector` | 08+ |
| P2 | DEF-006 | Golden Master + ctest | 09 |
| P3 | DEF-010, DEF-011 | API·CLI 경로 | 10~11 |

---

## 7. 변경 이력

| 버전 | 일자 | 변경 |
|------|------|------|
| 1.0 | 2026-05-20 | Step 08 — `defect_detection.md` §9 후보 티켓화, README 경계 별도 §3 |

**다음 Step:** DEF-001/002 코드 수정 또는 Step 09 Golden Master.
