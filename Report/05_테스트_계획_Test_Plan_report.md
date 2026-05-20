# 05. 테스트 계획 (Test Plan) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 05 — 테스트 계획서 작성 |
| commit string (권장) | `05_테스트_계획_Test_Plan` |
| 코드 변경 | 없음 (문서만) |

## 1. 작업 목표

- README **Activity 3** Unit Test 4종(BMI 계산, 평균 보정, 분류, 예외)을 `SHealthBMITest` **TEST / TEST_F** 설계로 구체화
- BMI·연령·보정 **경계값** 및 파일/CSV/API **예외** TC 목록·우선순위 정의
- README **Activity 4** Should/Could 기능용 **테스트 확장 슬롯** 예약
- Test Double: **임시 fixture CSV** vs 추후 **Mock/istream 주입** 결정 기준 문서화

## 2. 수행 내용

### 2.1 참조 분석

| 소스 | 반영 |
|------|------|
| README.md Activity 3·4 | 4종 TC 매핑, 향후 FR-S/C 슬롯 |
| `docs/requirements_analysis.md` | FR·경계·이슈 I-01~I-12 → TC ID |
| `docs/refactoring_notes.md` | public API 2개, private test hook 후보, Step 04 P0 반영(BMI=25, 보정 가드) |
| `SHealth.h/cpp` | `classifyBmi`, `isInAgeBand`, 파이프라인 4단계, `getBmiRatio` 계약 |
| `SHealthBMITest.cpp` | 현재 TC 없음(placeholder 주석만) — Step 06 구현 대상 |
| `CMakeLists.txt` | gtest 1.14, `gtest_discover_tests`, `ctest` |

### 2.2 계획서 핵심 요약

| 영역 | 결론 |
|------|------|
| 구조 | `SHealthBMITestFixture` (`TEST_F`) + 경계 직접 검증용 `TEST` (선택) |
| P0 | BMI 18.5/23/25, 연령 19·20·29·30, weight=0 보정, `getBmiRatio` 6×4·잘못된 인자, 파일 없음/헤더만 |
| P1 | height=0 As-Is, CSV 파싱 예외, 빈 줄 break, lcov |
| Activity 3 | 4 스위트 ↔ TC-BMI / TC-IMP / TC-CLS / TC-EXC |
| Activity 4 | `HeightImputation`, `NormalBmiUsers`, `GlobalBmiRatios`, Golden Master 슬롯 |
| Test Double | Step 05~06: **temp CSV fixture**; Step 08+: **gmock** (I/O 분리 후) |

### 2.3 Step 04 대비 테스트 관점

| 항목 | Step 04 | Step 05 계획 |
|------|---------|--------------|
| BMI 25 | `classifyBmi` 비만 처리 | TC-CLS-06, TC-BMI-02로 **회귀 고정** |
| 보정 0/0 | `validWeightCount==0` 스킵 | TC-IMP-03 |
| 빈 연령대 | `bandMemberCount==0` → 0% | TC-RAT-05 |
| ctest | 0 tests Green | Step 06에서 P0 TC 등록 목표 |

## 3. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| 테스트 계획서 | `docs/test_plan.md` | 생성 |
| 보고서 | `Report/05_테스트_계획_Test_Plan_report.md` | 본 문서 |
| Transcript | `Prompting/05_테스트_계획_Test_Plan.md` | 생성 |

## 4. 다음 권장 작업 (Step 06)

1. `SHealthBMITestFixture` + `writeTempCsv()` 구현
2. P0: TC-CLS, TC-AGE, TC-BMI, TC-IMP, TC-RAT, TC-EXC-01~04
3. `classifyBmi` / `isInAgeBand` test hook (`friend` 또는 `SHealth_TESTING`) 적용 여부 결정
4. `ctest --output-on-failure` Green
5. (선택) coverage 빌드 + `gen_lcov2.bat`

## 5. Wrap-up

- Report: 본 문서
- Prompting: `05_테스트_계획_Test_Plan.md`
- commit (사용자 요청 시): `05_테스트_계획_Test_Plan`
