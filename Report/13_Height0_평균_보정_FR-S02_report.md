# 13. Height=0 평균 보정 (FR-S02) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 13 — Height=0 평균 보정 (FR-S02) |
| commit string (권장) | `13_Height0_평균_보정_FR-S02` |
| Persona | 리팩토링 및 Unit Test expert |
| 설계 기준 | `docs/feature_requirements_design.md` §4.1, §3 |

## 1. 작업 목표

`height == 0` 레코드를 동일 연령대 `height != 0` 레코드의 산술 평균(cm)으로 보정한 뒤 BMI·4분류 집계를 수행한다. `imputeMissingWeightsByAgeBand()`와 대칭 설계·호출 순서를 고정하고 DEF-002(I-03)를 해소한다.

## 2. 수행 내용

### 2.1 구현

| 항목 | 내용 |
|------|------|
| 신규 함수 | `void SHealth::imputeMissingHeightsByAgeBand()` (`SHealth.h` / `SHealth.cpp`) |
| 파이프라인 | `load` → weight 보정 → **height 보정** → `computeBmis` → `aggregateAgeBandStatistics` |
| 비교 | `height == 0.0` 정확 비교; `validHeightCount == 0` 시 연령대 스킵 (I-02) |

### 2.2 테스트 (`HeightImputation`)

| TC ID | 검증 |
|-------|------|
| **TC-HGT-10** | 30대 h=170,175,0 → 평균 172.5; 유한 BMI(Normal); 30대 4분류 합 ≈ 100% |
| **TC-HGT-11** | 40대 전원 h=0 → 보정 스킵; 4분류 합 0% |
| TC-HGT-01/02 | 단독 레코드·동연령대 유효 키 없음 → Green 유지 |

### 2.3 Golden Master

`SHealthGoldenMaster.TC_GM_01_*` — **Pass**, baseline diff 0 (`shealth.dat`에 height=0 없음).

### 2.4 문서

| 산출물 | 경로 |
|--------|------|
| 구현 노트 §13 | `docs/feature_implementation_notes.md` |

## 3. 테스트 결과

```
build-gcc: cmake --build . && ctest --output-on-failure
42/42 Passed (39 unit + 2 TC-HGT 신규 + 1 Golden)
```

## 4. 산출물

| 산출물 | 경로 | 상태 |
|--------|------|------|
| 소스 | `src/main/cpp/SHealth.h`, `SHealth.cpp` | 수정 |
| 단위 테스트 | `src/test/cpp/SHealthBMITest.cpp` | TC-HGT-10/11 추가 |
| 구현 노트 | `docs/feature_implementation_notes.md` | 신규 §13 |
| 작업 보고서 | `Report/13_Height0_평균_보정_FR-S02_report.md` | 본 문서 |
| Transcript | `Prompting/13_Height0_평균_보정_FR-S02.md` | 생성 |

## 5. Wrap-up

| 항목 | 내용 |
|------|------|
| DEF-002 | 구현 완료 — 동연령대 키 보정 |
| Golden | baseline **갱신 불필요** |
| 다음 Step | Step 14 — FR-S03 `getAgeBandRatios` API 정비 |

**commit (권장):** `13_Height0_평균_보정_FR-S02`
