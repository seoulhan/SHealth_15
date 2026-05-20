# 02. 요구사항 분석 (Requirements Analysis) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 02 — 요구사항 분석서 작성 |
| commit string (권장) | `02_요구사항_분석_Requirements_Analysis` |

## 1. 작업 목표

- README·소스·`shealth.dat`·`.cursorrules`를 기반으로 **기능/비기능 요구**, **MoSCoW**, **API·출력 계약**, **BMI 경계**, **모호 이슈**를 문서화
- 후속 Step(03 코드 품질, 04~11 구현·테스트)의 단일 요구 기준선 확보

## 2. 수행 내용

### 2.1 참조 분석

| 소스 | 반영 |
|------|------|
| README.md | BMI 공식, 4분류, weight=0, Activities 4 향후 기능 |
| SHealth.h/cpp | FR-01~08 As-Is, `getBmiRatio` type·연령 매핑 |
| SHealthBMI.cpp | stdout 6연령대 출력 계약 (Golden Master 후보) |
| shealth.dat | weight=0 1건, height=0 미발견, age 다양 |
| .cursorrules / cursorrules_summary | NFR, 디렉터리 규칙, 구현·README 경계 주의 |

### 2.2 핵심 산출 요약

| 영역 | 결론 |
|------|------|
| Must | CSV 로드, BMI, weight=0 보정, 20~70대 4분류 %, `calculateBmi`/`getBmiRatio`, main 6행 출력 |
| Should | SRP, height=0 보정, API 정비, 단위 테스트 |
| Could | 정상 BMI ID 목록, 전체 사용자 범주 비율 |
| 경계 | README: ≤18.5 / (18.5,23) / [23,25) / ≥25 — **코드는 BMI=25를 비만으로 처리하지 않음 (I-01)** |
| 이슈 | 12건 등록 (I-01~I-12), P0: BMI=25, weight 보정 0건 |

## 3. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| 요구사항 분석서 | `docs/requirements_analysis.md` | 생성 |
| 보고서 | `Report/02_요구사항_분석_Requirements_Analysis_report.md` | 본 문서 |
| Transcript | `Prompting/02_요구사항_분석_Requirements_Analysis.md` | 생성 |

## 4. 다음 권장 작업

- **Step 03**: `docs/code_quality_analysis.md` — `@docs/requirements_analysis.md`, SOLID·코드 스멸·P0/P1/P2
- commit: `02_요구사항_분석_Requirements_Analysis` (사용자 요청 시)

## 5. Wrap-up

- Report: 본 문서
- Prompting: `02_요구사항_분석_Requirements_Analysis.md`
