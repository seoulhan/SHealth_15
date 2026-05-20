# 00. Step Prompt 생성 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-19 |
| Task | README Activities 기반 11단계 All-Step Prompt 생성 |
| commit string (권장) | `00_generate_steps` |

## 1. 작업 목표

- `README.md`, `CMakeLists.txt`, `src/`를 기준으로 SHealth_15 Activities용 Step-by-Step 프롬프트 체계 정의
- `.cursorrules` 초안을 프로젝트 루트에 기록
- 리팩토링(Step 04)이 Unit Test(Step 05~)보다 앞서도록 11단계 분할
- 각 step별 P/C/T/F + Wrap-up(Report, Prompting) 템플릿 적용

## 2. 수행 내용

### 2.1 `.cursorrules` (Step 01 선행 산출)

- 경로: 프로젝트 루트 `.cursorrules`
- 포함: BMI/연령대 비즈니스 규칙, `docs/`·`Report/`·`Prompting/` 규칙, 리팩토링→테스트 순서, ctest/Golden Master 원칙, AI 작업 시 docs 연계

### 2.2 All-Step Prompt 통합 문서

- 경로: `SHealth_all_step_prompts.md` (11개 step 블록)
- Step 01~11: Task Title, commit string, @ 참조 파일, Persona, Context, Task, Format, Wrap-up
- `docs/` 산출물 체인 및 실행 순서 요약 표 포함

### 2.3 프로젝트 맥락 반영

- 도메인: `SHealth` — BMI 계산, weight=0 연령대 평균 보정, 연령대별 4분류 통계
- 코드 스멜·테스트 포인트: 고정 배열 10000, magic type(100/200/300/400), BMI 경계, `FailedTest` placeholder
- README Activities 4(기능 개선)는 요구사항·QA 단계에서 Must/Should/Could로 분리하도록 프롬프트에 명시

## 3. 산출물 목록

| 산출물 | 상태 |
|--------|------|
| `.cursorrules` | 생성됨 |
| `SHealth_all_step_prompts.md` | 생성됨 (Step 02~ 실행용 마스터 프롬프트) |
| `docs/cursorrules_summary.md` | Step 01 실행 시 생성 예정 |
| `Report/00_generate_steps_report.md` | 본 문서 |
| `Prompting/00_generate_steps_transcript.md` | 대화 Transcript |

## 4. 11단계 실행 순서 (요약)

1. `.cursorrules` → 2. 요구사항 → 3. 코드 품질 → **4. 리팩토링** → 5. 테스트 계획 → 6. TC 작성 → 7. 결함 분석 → 8. defect_list → 9. Golden Master → 10. 결함 관리 → 11. QA 종합

## 5. 다음 권장 작업

- **Step 02**: `docs/requirements_analysis.md` 작성 (프롬프트: `SHealth_all_step_prompts.md` Step 02 블록)
- `docs/`, `Report/`, `Prompting/` 디렉터리를 step마다 누적 사용
- `gen_commit_push.bat` 등으로 단계별 commit string 커밋 가능

## 6. Wrap-up (본 step)

- `Report/00_generate_steps_report.md` — 본 보고서
- `Prompting/00_generate_steps_transcript.md` — 프롬프트 대화 Transcript
