# SHealth BMI — 기능 회귀·Golden Master 보고 (Step 17)

| 항목 | 내용 |
|------|------|
| 문서 버전 | 1.0 |
| 작성일 | 2026-05-20 |
| Step | 17 — 기능 회귀·Golden Master 갱신 (Feature Regression) |
| commit string (권장) | `17_기능_회귀_Golden_Master_Feature` |
| 범위 | Step 13~16 (FR-S02, FR-S03, FR-C01/C02, FR-S01) 반영 후 전체 회귀 |
| NFR | NFR-05 — main 출력·`ctest` Green |

---

## 1. Executive Summary

| 항목 | 결과 |
|------|------|
| **빌드** | `build-gcc`: `cmake --build .` — **성공** |
| **ctest** | **52 / 52 Passed** (100%) |
| **Golden Master** | `TC_GM_01` **Passed** — FR-08 6연령대 baseline **불변** (갱신 없음) |
| **실패 분류** | 해당 없음 (0건) |
| **baseline 갱신** | **수행 안 함** (의도적 stdout drift 없음) |

Step 13~16 기능·SRP 리팩토링 이후 **회귀 이상 없음**. README NFR-05 충족.

---

## 2. 실행 환경·명령

| 항목 | 값 |
|------|-----|
| OS | Windows 10 (22631) |
| 빌드 디렉터리 | `build-gcc` (MinGW Makefiles) |
| 데이터 | 저장소 루트 `shealth.dat` |
| Golden CWD | 프로젝트 루트 (`SHealthGoldenTest` — `shealth.dat` 부모) |

```powershell
cd c:\DEV\SHealth_15\build-gcc
cmake --build .
ctest -V
```

```powershell
cd c:\DEV\SHealth_15
.\build-gcc\SHealthBMI.exe   # 수동 stdout 확인
```

---

## 3. ctest 실행 로그 요약

| 지표 | 값 |
|------|-----|
| 총 테스트 | 52 |
| 통과 | 52 |
| 실패 | 0 |
| 통과율 | **100%** |
| 총 소요 (ctest) | ~5.6 s |

### 3.1 스위트별 분포

| 스위트 | 건수 | 결과 |
|--------|------|------|
| `SHealthBmiClassification` | 7 | 7/7 Pass |
| `SHealthAgeBand` | 1 | 1/1 Pass |
| `SHealthBMITestFixture` | 43 | 43/43 Pass |
| `SHealthGoldenMaster` | 1 | 1/1 Pass |
| **합계** | **52** | **52/52** |

> Step 10 기준 문서(39 unit + 1 Golden) 대비 **+12 unit TC** — Step 13~15에서 `TC-HGT-*`, `TC-API-*`, `TC-LST-*`, `TC-GLB-*` 추가.

### 3.2 Step 13~16 관련 TC (회귀 초점)

| FR | TC ID (대표) | 결과 |
|----|--------------|------|
| FR-S02 height 보정 | `TC_HGT_10`, `TC_HGT_11`, `TC_HGT_01/02` | Pass |
| FR-S03 연령대 API | `TC_API_01`~`04` | Pass |
| FR-C01 정상 목록 | `TC_LST_01`~`03` | Pass |
| FR-C02 전체 비율 | `TC_GLB_01`~`03` | Pass |
| FR-08 / Golden | `TC_GM_01` | Pass |
| SRP (FR-S01) | 기존 51 unit 전부 | Pass (동작·API 불변) |

### 3.3 실패 분류 (이번 실행)

| 분류 | 건수 | 비고 |
|------|------|------|
| 기능 버그 | 0 | — |
| TC 기대값 불일치 | 0 | — |
| Golden drift | 0 | 6행 수치·포맷 baseline과 일치 |
| 빌드/환경 | 0 | — |

---

## 4. Golden Master 상태

### 4.1 FR-08 6연령대 (Golden **대상**)

| 항목 | 상태 |
|------|------|
| baseline 파일 | `tests/golden/shealth_bmi_stdout.golden.txt` |
| 비교 TC | `SHealthGoldenMaster.TC_GM_01_SHealthBMI_stdout_matches_baseline` |
| 비교 범위 | stdout **앞 6행만** (`SHealthGoldenTest.cpp` — `actualLines.resize(6)`) |
| 허용 오차 | 연령대 정수 exact; 4비율 ±0.0001 |
| 이번 Step | **diff 0** — baseline **갱신 불필요** |

**6행 스냅샷 (실행 = baseline, 요약):**

| 연령대 | underweight | normal | overweight | obesity |
|--------|-------------|--------|------------|---------|
| 20 | 3.511053 | 23.797139 | 11.833550 | 60.858257 |
| 30 | 1.863354 | 15.527950 | 10.062112 | 72.546584 |
| 40 | 0.521512 | 10.039113 | 9.126467 | 80.312907 |
| 50 | 2.181401 | 12.629162 | 9.988519 | 75.200918 |
| 60 | 0.862895 | 8.533078 | 10.642378 | 79.961649 |
| 70 | 0.529101 | 12.345679 | 10.758377 | 76.366843 |

**갱신이 필요 없었던 이유 (설계 대비):**

| 변경 | Golden 영향 |
|------|-------------|
| FR-S02 height=0 보정 | `shealth.dat`에 height=0 **미존재** → 수치 불변 |
| FR-S03 `getAgeBandRatios` | 집계 SSOT 동일, `%f` 6행 포맷 유지 |
| FR-S01 SRP 분리 | Presenter가 동일 `printf` 계약 유지 |

### 4.2 신규 main 데모 출력 (Golden **비대상**)

Step 15 이후 `SHealthBMI`는 FR-08 6행 **뒤에** 다음 2종을 추가 출력한다 (`SHealthPresenter.cpp`).

| 출력 | FR | Golden 대상 | 검증 방식 |
|------|-----|-------------|-----------|
| `Normal BMI users (count=N): id, ...` | FR-C01 | **아니오** | `TC_LST_*` (fixture API) |
| `Global - underweight = ..., ...` | FR-C02 | **아니오** | `TC_GLB_*` (fixture API) |

**결정 (Step 17):**

- FR-C01/C02 데모 stdout은 Golden baseline에 **포함하지 않음** (정책 확정).
- 별도 **stdout 스모크 TC는 미추가** — 단위 TC(`TC-LST-*`, `TC-GLB-*`) 및 `TC_API_02`(실데이터 6×4 스냅샷)로 동작·수치를 고정함.
- 향후 데모 포맷을 회귀 잠금하려면 optional: `SHealthDemoSmokeTest` 또는 Golden 2행 확장 — **현재는 YAGNI**.

**수동 스모크 (참고, 2026-05-20):**

```text
Normal BMI users (count=654): 93711, 93715, ...
Global - underweight = 1.596848, normal = 13.562837, overweight = 10.389880, obesity = 74.450436
```

### 4.3 baseline 갱신 절차 (미적용)

의도적 FR-08 변경 시에만 `docs/golden_master.md` §7 및 `tests/golden/update_baseline.ps1` 사용.  
**이번 Step:** 스크립트 실행 **없음** — PR에 갱신 사유 **불필요**.

---

## 5. Open 결함·Known Issues 스냅샷

> `docs/defect_list.md` (v1.1, Step 10)는 DEF-002를 Open으로 표기하나, **Step 13 구현·TC Green** 기준으로는 기능상 **해소**됨. 아래는 Step 17 실행 시점 **실측** 스냅샷.

| ID | 제목 | defect_list 표기 | Step 17 실측 | 우선순위 |
|----|------|------------------|--------------|----------|
| DEF-002 | height=0 동연령대 보정 | Open | **Fixed** (FR-S02, `TC_HGT_10/11` Pass) | — |
| DEF-011 | main `shealth.dat` CWD 의존 | Open | **Open** (운영·문서) | P3 |
| DEF-001,003,005,006,007 | CSV·BMI·Golden 등 | Fixed | **유지 Fixed** | — |
| DEF-004,008~010 | 설계·문서 계약 | Won't fix | **유지** | — |

**권장 후속:** `defect_list.md` v1.2 — DEF-002 → Fixed, ctest **52/52** 반영.

### 5.1 알려진 제약 (기능 회귀 관점)

| 항목 | 영향 |
|------|------|
| `SHealthBMI` 데이터 경로 하드코딩 | CWD ≠ repo root 시 실패 (DEF-011) |
| Golden은 FR-08 6행만 | 데모 2행 변경 시 `TC_GM_01` **Pass 유지** (의도) |
| `shealth.dat`에 height=0 없음 | FR-S02 Golden drift 검증 한계 — fixture `TC_HGT_*`로 보완 |

---

## 6. 추적성

| ID | Step 17 매핑 |
|----|----------------|
| NFR-05 | 52/52 ctest + Golden Pass |
| FR-08 | 6연령대 stdout baseline 불변 |
| FR-S02~S03, FR-C01~C02, FR-S01 | 해당 unit TC 전부 Pass |
| GM-01 / DEF-006 | Golden TC Green (baseline diff 0) |
| `docs/golden_master.md` | 갱신 절차 준수 — 이번 drift 없음 |
| `docs/feature_requirements_design.md` §7 | Golden 분리 전략 **확인 완료** |

---

## 7. 결론·게이트

| 게이트 | 판정 |
|--------|------|
| Activity 4 기능 (Step 13~15) 회귀 | **Pass** |
| SRP (Step 16) 회귀 | **Pass** |
| Golden baseline | **유지** (갱신 없음) |
| NFR-05 | **충족** |

**Step 18(종합 보고)** 진행 가능. DEF-002 문서 동기화·DEF-011(CLI 경로)는 선택적 잔여.

---

## 8. 변경 이력

| 버전 | 일자 | 변경 |
|------|------|------|
| 1.0 | 2026-05-20 | Step 17 — 최초 회귀 실행·Golden·Open 결함 스냅샷 |
