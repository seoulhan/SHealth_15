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
