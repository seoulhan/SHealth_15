# SHealth BMI — 결함 관리 (Defect Management)

| 항목 | 내용 |
|------|------|
| 문서 버전 | 1.0 |
| 작성일 | 2026-05-20 |
| Step | 10 — 결함 관리 & 보고 |
| commit string (권장) | `10_결함_관리_보고_Defect_Management` |
| 입력 | `docs/defect_list.md`, `docs/defect_detection.md`, `docs/golden_master.md` |
| 구현 | `src/main/cpp/SHealth.cpp`, `src/test/cpp/SHealthBMITest.cpp` |

---

## 1. 결함 상태 스냅샷 (2026-05-20)

### 1.1 요약

| 상태 | 건수 | Defect ID |
|------|------|-----------|
| **Fixed** (Step 10 신규) | 3 | DEF-001, DEF-003, DEF-005 |
| **Fixed** (이전) | 2 | DEF-006, DEF-007 |
| **Open** | 2 | DEF-002, DEF-011 |
| **Won't fix** | 4 | DEF-004, DEF-008, DEF-009, DEF-010 |

### 1.2 테스트 재실행 결과

| 항목 | 결과 |
|------|------|
| **빌드** | `build-gcc`, MinGW Makefiles |
| **ctest** | **40 / 40 Passed** (39 unit + 1 Golden) |
| **Golden Master** | `SHealthGoldenMaster.TC_GM_01_*` — **Passed** (baseline 변경 없음) |
| **실행 명령** | `cd build-gcc; cmake --build .; ctest --output-on-failure` |
| **소요** | Total Test time ≈ 1.02 s |

> CSV 파싱 수정(DEF-001/003/005)은 `shealth.dat` 정상 행만 로드하므로 **stdout Golden baseline 불변**.

### 1.3 Open · 잔여 리스크

| ID | 제목 | Priority | 잔여 리스크 | 계획 |
|----|------|----------|-------------|------|
| **DEF-002** | height=0 동연령대 키 보정 미구현 | P1 | FR-S02 미충족; height=0 시 4분류 합 0% | **Activity 4** 구현 + `TC_HGT_*` 갱신 + Golden 재캡처 |
| **DEF-011** | main `shealth.dat` CWD 의존 | P3 | 잘못된 CWD에서 빈/0 출력 | README·CLI 경로 옵션 (Step 11) |

### 1.4 Known Issues (운영·계약)

| ID | 현상 | 완화 |
|----|------|------|
| DEF-004 | non-finite BMI 시 연령대 4분류 합 < 100% | **의도적**(분모=전원). FR-S02 후 TC 재검토 |
| DEF-008 | 연령대 전원 weight=0 → 저체중 100% | `TC_IMP_03` 계약 |
| DEF-009 | 빈 연령대 비율 0% | `TC_RAT_05` 계약 |
| DEF-010 | 잘못된 API 인자 → 0.0 (미호출과 동일) | `TC_RAT_02/03/04`, `TC_EXC_08` |
| DEF-011 | `build/`에서 exe 실행 시 `shealth.dat` 미발견 | **프로젝트 루트**에서 실행 또는 `build-gcc` + README |

---

## 2. 조치 이력 (Step 10)

### 2.1 DEF-001 — CSV 컬럼 부족 abort → **Fixed**

| 일자 | 조치 | 검증 |
|------|------|------|
| 2026-05-20 | `tokens.size() < 4` → `cerr` 로그 후 **continue** | `TC_EXC_05`: `calculateBmi` → **0** |
| 2026-05-20 | 혼합 CSV 부분 로드 | `TC_EXC_11` 신규 — 유효 1건 로드 Pass |

**소스:** `loadRecordsFromFile` — 컬럼 수 검증 분기.

### 2.2 DEF-003 — 비숫자 필드 예외 전파 → **Fixed**

| 일자 | 조치 | 검증 |
|------|------|------|
| 2026-05-20 | `stoi`/`stod` **try/catch** → 행 스킵 | `TC_EXC_06`: 반환 **0** (기존 `EXPECT_THROW` 제거) |
| 2026-05-20 | bad + good 행 혼합 | `TC_EXC_11` |

### 2.3 DEF-005 — 10,000건 초과 → **Fixed** (방어)

| 일자 | 조치 | 검증 |
|------|------|------|
| 2026-05-20 | `recordCount >= MAX_RECORDS` 시 로그 후 **break** | ctest Green; 10k+ TC는 미추가 |

**잔여:** `vector` 전환·명시적 오류 코드는 NFR 범위 외(Step 11+).

### 2.4 DEF-004 — 비율 합 ≠ 100% → **Won't fix**

| 일자 | 결정 | 근거 |
|------|------|------|
| 2026-05-20 | **옵션 A** 채택 | README에 “합=100%” 미명시; `aggregateAgeBandStatistics` 분모=연령대 전원 유지. `TC_HGT_*`는 As-Is 문서화 유지 |

### 2.5 DEF-002 — height=0 보정 → **Open** (연기)

| 일자 | 결정 | 근거 |
|------|------|------|
| 2026-05-20 | Activity 4로 **연기** | README Activity 4 / FR-S02(Should); `shealth.dat`에 height=0 미관측. 단계 10 범위는 P0 CSV 견고성 우선 |

### 2.6 DEF-011 — CWD 의존 → **Open** (문서)

| 일자 | 조치 | 검증 |
|------|------|------|
| 2026-05-20 | Known Issue 등록; Golden은 repo 루트 CWD로 검증 | GM-01 Pass 유지 |

---

## 3. Activity 4 기능 개선 — 회귀 체크리스트 (제안)

Activity 4(FR-S02 height 보정, SRP 분리, 신규 API 등) 착수 전·후에 아래를 실행한다.

### 3.1 자동 (필수)

| # | 항목 | 명령 / 기준 |
|---|------|-------------|
| R-01 | 전체 단위·Golden | `cd build-gcc; ctest --output-on-failure` → **40/40** |
| R-02 | Golden만 | `ctest -R SHealthGoldenMaster -V` |
| R-03 | BMI 경계 | `TC_CLS_06`, `TC_BMI_02` (BMI=25 비만) |
| R-04 | weight=0 보정 | `TC_IMP_01`~`05`, `TC_IMP_03` (전원 0 계약) |
| R-05 | CSV 견고성 | `TC_EXC_05`, `06`, `11` (스킵·부분 로드) |

### 3.2 FR-S02 height=0 (구현 시 추가)

| # | 항목 | 기대 |
|---|------|------|
| R-06 | `TC_HGT_01` | 동연령대 평균 키 보정 후 **4분류 합 ≈ 100%** |
| R-07 | `TC_HGT_02` | weight=0 + height=0 조합 재정의 |
| R-08 | Golden baseline | `tests/golden/update_baseline.ps1` — stdout 수치 변경 시만 갱신 |
| R-09 | `shealth.dat` 스모크 | 루트에서 `SHealthBMI.exe` — 6행 합 ≈ 100% |

### 3.3 수동·문서

| # | 항목 |
|---|------|
| R-10 | `docs/defect_list.md` DEF-002 → Fixed, DEF-004 TC 영향 검토 |
| R-11 | README Activity 4 체크리스트와 요구 ID(FR-S02) 동기화 |
| R-12 | 의도적 baseline 변경 시 PR/커밋 메시지에 Golden 갱신 사유 명시 |

---

## 4. 추적성 (Step 10 변경)

| Defect | Step 10 결과 | 테스트 |
|--------|--------------|--------|
| DEF-001 | Fixed | `TC_EXC_05`, `TC_EXC_11` |
| DEF-003 | Fixed | `TC_EXC_06`, `TC_EXC_11` |
| DEF-005 | Fixed | (코드 방어) |
| DEF-004 | Won't fix | `TC_HGT_*`, `TC_BMI_03` 유지 |
| DEF-002 | Open | `TC_HGT_*` As-Is |
| DEF-006 | Fixed (Step 09) | `TC_GM_01` |
| DEF-007 | Fixed | `TC_CLS_06`, `TC_BMI_02` |

---

## 5. 변경 이력

| 버전 | 일자 | 변경 |
|------|------|------|
| 1.0 | 2026-05-20 | Step 10 — 스냅샷, 조치 이력, ctest/Golden 결과, Activity 4 회귀 체크리스트 |

**관련 문서:** `docs/defect_list.md` v1.1, `Report/10_결함_관리_보고_Defect_Management_report.md`
