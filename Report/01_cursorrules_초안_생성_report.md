# 01. .cursorrules 초안 생성 — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 01 — `.cursorrules` 초안 작성·보완 |
| commit string (권장) | `01_cursorrules_초안_생성` |

## 1. 작업 목표

- README.md, CMakeLists.txt, `src/` 구조를 반영한 프로젝트 루트 `.cursorrules` 정리
- BMI/연령대 규칙, docs·Report·Prompting 규칙, 코딩·테스트·AI 작업 원칙, 금지 사항 포함
- 다음 step 전달용 `docs/cursorrules_summary.md` 작성

## 2. 수행 내용

### 2.1 기존 `.cursorrules` 검토 (diff 관점)

- Step 00에서 생성된 초안(44행)은 핵심 항목을 이미 포함
- **보완**: CMake 타깃 3종, 공개 API, 구현·README BMI 경계 차이, 코드 스멜 인벤토리, Activities↔11단계 매핑, 빌드 명령 블록, `SHealth_all_step_prompts.md` 참조 규칙

### 2.2 참조 소스 반영

| 소스 | 반영 내용 |
|------|-----------|
| README.md | BMI 공식·4분류·연령대·weight=0·Activities 순서·STL 허용 |
| CMakeLists.txt | C++17, shealth_lib/SHealthBMI/SHealthBMITest, FetchContent gtest 1.14 |
| SHealth.h/cpp | 배열 10000, 24개 통계 멤버, 보정·분류 루프, getBmiRatio type 코드 |
| SHealthBMI.cpp | shealth.dat, 6연령대 printf 출력 (Golden Master 후보) |
| SHealthBMITest.cpp | FailedTest placeholder — Step 06 교체 예정 |

## 3. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| Cursor Rules | `.cursorrules` | 보완·저장 |
| 요약 | `docs/cursorrules_summary.md` | 생성 |
| 보고서 | `Report/01_cursorrules_초안_생성_report.md` | 본 문서 |
| Transcript | `Prompting/01_cursorrules_초안_생성.md` | 생성 |

## 4. 다음 권장 작업

- **Step 02**: `docs/requirements_analysis.md` — `@.cursorrules`, `@docs/cursorrules_summary.md`, README, 소스, `shealth.dat`
- commit: `01_cursorrules_초안_생성` (사용자 요청 시)

## 5. Wrap-up

- Report: 본 문서
- Prompting: `01_cursorrules_초안_생성.md`
