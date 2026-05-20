# Step 12 — 기능 개선 요구사항·설계 (Feature Branch Kickoff) (Transcript Export)

**Task Title:** 12. 기능 개선 요구사항·설계 (Feature Branch Kickoff)  
**commit string:** `12_기능_개선_요구사항_설계_Feature_Kickoff`  
**Persona:** 시니어 비즈니스·시스템 분석가 + 소프트웨어 아키텍트

---

## User Prompt

```
## Step 12 — 기능 개선 요구사항·설계 (Feature Branch Kickoff)

# Task Title : 12. 기능 개선 요구사항·설계 (Feature Branch Kickoff)
## commit string : 12_기능_개선_요구사항_설계_Feature_Kickoff

@README.md @docs/requirements_analysis.md @docs/qa_final_report.md @docs/test_plan.md @docs/refactoring_notes.md @docs/golden_master.md @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @.cursorrules

[Persona] 시니어 비즈니스·시스템 분석가 + 소프트웨어 아키텍트

[Context] Step 11까지 As-Is( FR-01~08 )·테스트·Golden Master가 확보됨. README Activity 4 5항목 중 **연령대 BMI 분포**는 FR-05/FR-06으로 이미 존재 → FR-S03은 API 정비로 해석. `feature` 브랜치에서 신규 API·보정·출력 확장을 설계한다.

[Task] 기능 개선 요구·설계서 작성 (코드 변경 없음 또는 최소).
- README Activity 4 5항목 구현 단위·P0/P1
- FR-S02, FR-S03, FR-C01, FR-C02, FR-S01 상세 (입출력·전제·예외)
- test_plan §6 TC 매핑
- Golden Master 호환 전략
- Step 13~17 의존 다이어그램

[Format] Markdown. `docs\feature_requirements_design.md`

[Wrap-up]
- Report\12_기능_개선_요구사항_설계_Feature_Kickoff_report.md
- Transcript → Prompting\12_기능_개선_요구사항_설계_Feature_Kickoff.md
```

---

## Assistant Actions (요약)

1. **입력 문서·코드 분석** — `requirements_analysis.md`, `qa_final_report.md`, `test_plan.md` §6, `refactoring_notes.md`, `golden_master.md`, `SHealth.h/cpp`, `SHealthBMI.cpp`.
2. **As-Is 확인** — 파이프라인 4단계; id 미파싱; height=0 미보정; `shealth.dat`에 height=0 없음(weight=0 1건).
3. **설계서 작성** — `docs/feature_requirements_design.md`
   - P0: FR-S02, FR-S03 / P1: FR-C01, FR-C02, FR-S01
   - To-Be 파이프라인: weight → **height** → BMI → 집계
   - API: `getAgeBandRatios` + `getBmiRatio` 위임; `getNormalBmiUserIds`, `getGlobalBmiRatios`
   - Golden: FR-08 6행만 유지; 신규 데모 출력 비대상
   - TC: TC-HGT-10/11, TC-API-01~04, TC-LST-*, TC-GLB-*
   - Mermaid: Step 13~18 의존 관계
4. **Wrap-up** — Report, 본 Transcript.

---

## 주요 설계 결정

| 항목 | 결정 |
|------|------|
| FR-S03 | 기존 `getBmiRatio` 유지 + `getAgeBandRatios` 명시 API |
| 보정 순서 | `imputeMissingWeights` → `imputeMissingHeights` → `computeBmis` |
| Golden baseline | shealth.dat 기준 FR-S02만으로는 갱신 불필요 기대 |
| SRP | Step 16, 13~15 동작 동결 후 최소 클래스 분리 |

---

## 산출물

| 파일 |
|------|
| `docs/feature_requirements_design.md` |
| `Report/12_기능_개선_요구사항_설계_Feature_Kickoff_report.md` |
| `Prompting/12_기능_개선_요구사항_설계_Feature_Kickoff.md` |

---

## 다음 Step

- 브랜치: `git checkout -b feature` (from `tc`)
- Step 13: `imputeMissingHeightsByAgeBand`, TC-HGT-10/11, `docs/feature_implementation_notes.md` §13
