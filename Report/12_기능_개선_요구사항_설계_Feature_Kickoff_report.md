# 12. 기능 개선 요구사항·설계 (Feature Branch Kickoff) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 12 — 기능 개선 요구사항·설계 (Feature Branch Kickoff) |
| commit string (권장) | `12_기능_개선_요구사항_설계_Feature_Kickoff` |
| Persona | 시니어 비즈니스·시스템 분석가 + 소프트웨어 아키텍트 |
| 기준 문서 | `docs/requirements_analysis.md`, `docs/qa_final_report.md`, `docs/test_plan.md`, Step 04~11 산출물 |

## 1. 작업 목표

`feature` 브랜치 Activity 4 구현 전, README 5항목을 **구현 단위·P0/P1·입출력·예외·TC·Golden 전략·Step 13~17 의존 다이어그램**으로 고정하는 설계서를 작성한다. **코드 변경 없음.**

## 2. 수행 내용

### 2.1 Activity 4 분해·우선순위

| 요구 ID | 구현 단위 | 우선순위 | Step |
|---------|-----------|----------|------|
| FR-S02 | `imputeMissingHeightsByAgeBand` (weight와 대칭) | **P0** | 13 |
| FR-S03 | `getAgeBandRatios` + `getBmiRatio` 위임 | **P0** | 14 |
| FR-C01 | id 파싱 + `getNormalBmiUserIds` | **P1** | 15 |
| FR-C02 | `getGlobalBmiRatios` / 전체 집계 | **P1** | 15 |
| FR-S01 | I/O·보정·BMI·집계·조회·Presenter 경계 | **P1** | 16 |

FR-S03은 비즈니스 신규가 아닌 **API 정식화**로 재정의 (`requirements_analysis.md` Note).

### 2.2 파이프라인 (To-Be)

`load` → **weight 보정** → **height 보정** → `computeBmis` → 연령대 집계 → (Step 15) 전체 집계.

### 2.3 Golden Master 전략

| 항목 | 결정 |
|------|------|
| FR-08 6연령대 stdout | **유지** — Golden 대상 |
| FR-C01/C02 데모 출력 | Golden **비대상** |
| `shealth.dat` | height=0 없음 → FR-S02만으로 baseline **불변 기대** |
| 갱신 | 의도적 수치/포맷 변경 시 `update_baseline.ps1` + 문서 기록 |

### 2.4 테스트 매핑

- FR-S02: `TC-HGT-10`, `TC-HGT-11` (`HeightImputation`)
- FR-S03: `AgeBandDistributionApi` (`TC-API-01`~`04` 설계)
- FR-C01/C02: `TC-LST-*`, `TC-GLB-*` (`docs/test_plan.md` §6.2)

### 2.5 구현 로드맵

13 → 14 → 15 → 16 → 17(회귀·Golden) → 18(종합·회고). 상세는 `docs/feature_requirements_design.md` §8.

## 3. 산출물

| 산출물 | 경로 | 상태 |
|--------|------|------|
| 기능 개선 요구·설계서 | `docs/feature_requirements_design.md` | 생성 |
| 작업 보고서 | `Report/12_기능_개선_요구사항_설계_Feature_Kickoff_report.md` | 본 문서 |
| Transcript | `Prompting/12_기능_개선_요구사항_설계_Feature_Kickoff.md` | 생성 |

## 4. Wrap-up

| 항목 | 내용 |
|------|------|
| 코드 변경 | **없음** (설계·문서 전용) |
| 테스트 | 미실행 (Step 11 기준 40/40 유지 가정) |
| 다음 Step | Step 13 — FR-S02 height=0 보정 구현 |

**브랜치:** `tc` Green 후 `git checkout -b feature` 권장 (`SHealth_additional_step_prompts.md`).

**commit (권장):** `12_기능_개선_요구사항_설계_Feature_Kickoff`
