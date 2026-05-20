# .cursorrules 요약 (Step 01)

> 다음 step(02 요구사항 분석)에서 `@docs/cursorrules_summary.md` 와 함께 `.cursorrules` 참조.

## 한 줄 개요
CMake C++17 **SHealth** — `shealth.dat` CSV로 연령대별 BMI 4분류 비율을 계산·`getBmiRatio(ageClass, type)`로 조회.

## 핵심 비즈니스 규칙
- BMI = kg / (m)², height cm→m
- 분류: ≤18.5 / (18.5,23) / [23,25) / ≥25 (README; 구현 경계는 TC·결함 분석에서 검증)
- 연령대 20~70 (10년), weight=0 → 동연령대 평균 보정
- type: 100·200·300·400 = 저·정·과·비

## 빌드 타깃
`shealth_lib` | `SHealthBMI` | `SHealthBMITest` (FetchContent googletest 1.14)

## 작업 순서 (Activities)
스멜 분석 → **리팩토링(04)** → 테스트(05~06) → 기능·QA(07~11)

## 디렉터리
- `docs/` — 단계 산출물·다음 step 입력
- `Report/` — `NN_…_report.md`
- `Prompting/` — Transcript

## Step 01에서 보강한 점 (기존 초안 대비)
- CMake 타깃·공개 API 명시
- 구현 vs README 경계 이슈(25, 18.5) 명시
- 알려진 스멜 목록·`FailedTest` placeholder
- Activities 번호·11단계 체인과의 정렬

## 다음 step
**Step 02** — `docs/requirements_analysis.md`: 기능/비기능, Must·Should·Could, API·출력 계약, 모호 요구 이슈 목록.
