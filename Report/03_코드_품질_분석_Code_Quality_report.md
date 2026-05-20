# 03. 코드 품질 분석 (SOLID & Code Smell) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 03 — 코드 품질 분석 보고서 작성 |
| commit string (권장) | `03_코드_품질_분석_Code_Quality` |
| 코드 변경 | 없음 |

## 1. 작업 목표

- `SHealth` 단일 클래스의 **SOLID 위반**·**코드 스멜**·**경계 조건 결함 가능성**을 문서화
- README Activities 1 「코드 스멜 찾기」 항목과 매핑
- **P0/P1/P2** 리팩토링 우선순위 및 **Step 04** 권장 작업 목록 제시

## 2. 수행 내용

### 2.1 참조 분석

| 소스 | 반영 |
|------|------|
| `SHealth.h/cpp` | God Class, 24 멤버, `calculateBmi` Long Method, `getBmiRatio` 24분기 |
| `SHealthBMI.cpp` | main 6연령대 printf 중복, magic type 100~400 |
| `docs/requirements_analysis.md` | BMI=25 결함(I-01), 보정·빈 연령대 이슈 |
| README.md | Activity 1 스멜 찾기, Activity 2 리팩토링 순서 |
| `.cursorrules` | 알려진 스멜·Step 순서 |

### 2.2 핵심 진단 요약

| 영역 | 결론 |
|------|------|
| **SRP** | 파일 I/O·보정·BMI·통계·저장·조회가 `SHealth`/`calculateBmi`에 혼재 |
| **OCP** | 연령대·type·경계 확장 시 분기·멤버·main 동시 수정 |
| **스멜** | Long Method, Duplicate Code, Magic Number, Data Clumps, Primitive Obsession, God Class |
| **P0 결함** | BMI=25: README 비만 vs 코드 `>25` + **미분류** 가능 |
| **P0 리스크** | `ageCount==0` 보정 시 0/0 → NaN |
| **Step 04** | 상수·`classifyBmi`·함수 추출·24멤버 구조화·main 루프화 (9단계) |

## 3. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| 코드 품질 분석서 | `docs/code_quality_analysis.md` | 생성 |
| 보고서 | `Report/03_코드_품질_분석_Code_Quality_report.md` | 본 문서 |
| Transcript | `Prompting/03_코드_품질_분석_Code_Quality.md` | 생성 |

## 4. README Activity 1 매핑 (요약)

| Activity 1 | 문서 섹션 |
|------------|-----------|
| 기본 코드구조 | §1.2, §3 God Class |
| BMI 로직 | §4 경계, §3.2.3 Magic Number |
| 코드 스멜 찾기 | §3 전체, §2 SOLID |

## 5. 다음 권장 작업

- **Step 04**: 1차 리팩토링 — P0-1 BMI 25 수정, 함수 추출, 24 멤버 구조화, `enum class BmiCategory`
- **Step 05~06**: 경계·보정·`getBmiRatio` TC (`SHealthBMITest` 교체)
- commit: `03_코드_품질_분석_Code_Quality` (사용자 요청 시)

## 6. Wrap-up

- Report: 본 문서
- Prompting: `03_코드_품질_분석_Code_Quality.md`
