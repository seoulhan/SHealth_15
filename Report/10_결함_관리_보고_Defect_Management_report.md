# 10. 결함 관리 & 보고 (Defect Management) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 10 — 결함 관리 & 보고 |
| commit string (권장) | `10_결함_관리_보고_Defect_Management` |
| 기준 문서 | `docs/defect_list.md`, `docs/defect_detection.md`, `docs/golden_master.md` |

## 1. 작업 목표

- Open 결함 중 **Priority 높은 항목** 수정 또는 Won't fix 근거 기록
- 수정 후 **ctest·Golden Master** 재실행 및 결과 반영
- 결함 상태 갱신, 잔여 리스크·Known Issues 정리
- Activity 4 착수 시 **회귀 체크리스트** 제안

## 2. 수행 내용

### 2.1 코드 수정 (P0~P2)

| Defect | 조치 | 상태 |
|--------|------|------|
| **DEF-001** | `loadRecordsFromFile`: 컬럼 `< 4` 행 스킵 | **Fixed** |
| **DEF-003** | `stoi`/`stod` try-catch, 무효 행 스킵 | **Fixed** |
| **DEF-005** | `MAX_RECORDS` 초과 시 로드 중단 | **Fixed** |
| **DEF-004** | 옵션 A(분모=전원, 합<100% 허용) | **Won't fix** |
| **DEF-002** | FR-S02 height 보정 | **Open** → Activity 4 |
| **DEF-011** | CWD 의존 | **Open** → Known Issue |

**테스트 갱신:** `TC_EXC_05`/`06` — `EXPECT_DEATH`/`EXPECT_THROW` → `EXPECT_EQ(..., 0)`; **`TC_EXC_11`** 부분 로드 추가.

### 2.2 테스트 재실행

| 스위트 | 결과 |
|--------|------|
| 단위 (SHealthBMITest) | **39 / 39 Passed** |
| Golden (`TC_GM_01`) | **1 / 1 Passed** |
| **합계** | **40 / 40 Passed** |

Golden baseline(`tests/golden/shealth_bmi_stdout.golden.txt`) **변경 없음** — 파싱 수정이 `shealth.dat` 출력에 영향 없음.

### 2.3 문서

| 산출물 | 경로 |
|--------|------|
| 결함 관리 | `docs/defect_management.md` |
| 결함 목록 갱신 | `docs/defect_list.md` v1.1 |

## 3. 결함 상태 요약 (After)

| 상태 | 건수 |
|------|------|
| Fixed | 5 (DEF-001, 003, 005, 006, 007) |
| Open | 2 (DEF-002, 011) |
| Won't fix | 4 (DEF-004, 008, 009, 010) |

## 4. 잔여 리스크

1. **DEF-002** — height=0 시 BMI 비정상·4분류 0%; Activity 4에서 보정 + Golden 재캡처 필요.
2. **DEF-011** — 실행 디렉터리에 따라 main 실패; 문서화로 완화.
3. **DEF-005** — 10k 초과 시 silent truncate(로그만); 장기적으로 `vector` 검토.

## 5. Activity 4 회귀 체크리스트

`docs/defect_management.md` §3 — R-01~R-12 (ctest 40건, Golden, BMI=25, IMP/EXC, FR-S02 TC·baseline).

## 6. Wrap-up

| 산출물 | 경로 | 상태 |
|--------|------|------|
| 결함 관리 | `docs/defect_management.md` | 생성 |
| 결함 목록 | `docs/defect_list.md` | v1.1 갱신 |
| 보고서 | `Report/10_결함_관리_보고_Defect_Management_report.md` | 본 문서 |
| Transcript | `Prompting/10_결함_관리_보고_Defect_Management.md` | 생성 |
| 코드 | `SHealth.cpp`, `SHealthBMITest.cpp` | 수정 |

**commit (권장):** `10_결함_관리_보고_Defect_Management`

## 7. 다음 권장 작업

1. Activity 4 — DEF-002 height 보정 + `TC_HGT_*` + Golden baseline
2. Step 11 — DEF-011 CLI 경로, SRP 분리
3. 선택 — 10,001행 TC 또는 `vector` 전환
