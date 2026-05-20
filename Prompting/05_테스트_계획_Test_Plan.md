# Step 05 — 테스트 계획 (Transcript Export)

**Task Title:** 05. 테스트 계획 (Test Plan)  
**commit string:** `05_테스트_계획_Test_Plan`  
**Persona:** 시니어 QA 리드

---

## User Prompt

```
## Step 05 — 테스트 계획 (Test Plan)

# Task Title : 05. 테스트 계획 (Test Plan)
## commit string : 05_테스트_계획_Test_Plan

@README.md @CMakeLists.txt @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @docs/requirements_analysis.md @docs/refactoring_notes.md @src/test/cpp/SHealthBMITest.cpp

[Persona] 시니어 QA 리드

[Context] C++17, Google Test, CMake, ctest. (lcov는 `gen_lcov2.bat` 참고 가능) SHealth 도메인 테스트.

[Task] 테스트 계획서 작성.
- `TEST` / `TEST_F` 기반 단위 테스트 범위·우선순위 (`SHealthBMITest`)
- 경계값: BMI 18.5/23/25, 연령 19·20·29·30, weight=0·height=0, 연령대 내 전원 weight=0
- 예외/특이: 파일 없음, 빈 데이터, 헤더만, 잘못된 CSV, ageClass/type 잘못된 getBmiRatio
- README Activities 3 TC 4종(BMI 계산, 평균 보정, 분류, 예외) 매핑
- Activities 4 향후 기능에 대한 테스트 확장 슬롯(Should/Could)
- Test Double 필요성: 파일 I/O 분리 시 임시 fixture 파일 vs 추후 인터페이스 Mock 여부

[Format] Markdown. `docs\test_plan.md`

[Wrap-up]
- `Report\05_테스트_계획_Test_Plan_report.md`
- Transcript → `Prompting\05_테스트_계획_Test_Plan.md`
```

---

## Assistant Actions (요약)

1. **참조 문서·코드 분석**: README Activity 3·4, `requirements_analysis.md` 경계·이슈, Step 04 `classifyBmi`·보정·집계 가드, public API 2개, `SHealthBMITest.cpp` 현황( TC 미구현 ).
2. **`docs/test_plan.md` 작성**:
   - `SHealthBMITestFixture` + `TEST_F` / 선택 `TEST` 구조
   - P0~P1 TC ID: TC-BMI, TC-IMP, TC-CLS, TC-AGE, TC-RAT, TC-EXC, TC-HGT
   - 경계표: BMI 18.5/23/25, 연령 19·20·29·30, weight=0·전원 0, height=0 As-Is
   - README Activity 3 → 4 스위트 매핑표
   - Activity 4 Should/Could 확장 슬롯 (FR-S02, FR-C01~C02, Golden Master)
   - Test Double: Step 05~06 fixture CSV 주력; Step 08+ gmock
   - lcov·추적성·Step 06 Exit 기준
3. **Wrap-up**: `Report/05_테스트_계획_Test_Plan_report.md`, 본 Transcript

---

## 산출물

| 파일 | 설명 |
|------|------|
| `docs/test_plan.md` | 테스트 계획서 본문 |
| `Report/05_테스트_계획_Test_Plan_report.md` | 작업 보고서 |
| `Prompting/05_테스트_계획_Test_Plan.md` | 본 Transcript |

---

## Step 06 구현 시 첫 작업 체크리스트

- [ ] `SHealthBMITestFixture::writeTempCsv()`
- [ ] TC-CLS-01~07 (`classifyBmi` hook 또는 CSV)
- [ ] TC-AGE-01~03, TC-IMP-01~02, TC-EXC-01~04
- [ ] `ctest` Green
