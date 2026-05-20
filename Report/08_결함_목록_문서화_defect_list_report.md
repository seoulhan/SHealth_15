# 08. 결함 목록 문서화 (defect_list) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 08 — 결함 목록 문서화 |
| commit string (권장) | `08_결함_목록_문서화_defect_list` |
| 기준 문서 | `docs/defect_detection.md`, `docs/requirements_analysis.md`, `docs/test_cases.md` |

## 1. 작업 목표

- Step 07 결함 분석 결과를 **추적 가능한 결함 티켓** 형태로 정리
- 결함 ID, 재현 단계, 기대/실제, Severity, Priority, 상태(Open/Fixed/Won't fix) 명시
- 요구사항 ID · 관련 테스트명 · 소스 위치 매핑
- **README BMI 경계**와 구현 불일치 항목 별도 표기

## 2. 수행 내용

### 2.1 티켓화 범위

| 상태 | Defect ID | 건수 |
|------|-----------|------|
| **Open** | DEF-001, 002, 003, 004, 005, 006, 011 | 7 |
| **Fixed** | DEF-007 (I-01, BMI=25) | 1 |
| **Won't fix** | DEF-008, 009, 010 (I-02, I-05, I-08) | 3 |

### 2.2 README BMI 경계 (§3 별도)

- **과거 불일치:** BMI=25.0 → README 비만 vs 구현 과체중 (I-01)
- **현행:** `classifyBmi` + `TC_CLS_06` / `TC_BMI_02` 기준 **Fixed** — README와 일치
- `requirements_analysis.md` §4.3 “×” 행은 구현 수정 전 스냅샷으로 문서에 명시

### 2.3 Open 결함 핵심 (P0~P1)

| ID | 요약 | Severity |
|----|------|----------|
| DEF-001 | CSV 컬럼 부족 → abort (`TC_EXC_05` DEATH) | High |
| DEF-002 | height=0 보정 미구현 (FR-S02) | Medium |
| DEF-003 | 비숫자 age → 예외 전파 | Medium |

### 2.4 ctest 현황 (변경 없음)

- **38 / 38 Passed** — Open 결함은 As-Is 문서화·미구현·회귀 공백으로 Green과 공존

## 3. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| 결함 목록 | `docs/defect_list.md` | 생성 |
| 보고서 | `Report/08_결함_목록_문서화_defect_list_report.md` | 본 문서 |
| Transcript | `Prompting/08_결함_목록_문서화_defect_list.md` | 생성 |

## 4. Wrap-up

- Report: 본 문서
- Prompting: `08_결함_목록_문서화_defect_list.md`
- commit (사용자 요청 시): `08_결함_목록_문서화_defect_list`

## 5. 다음 권장 작업

1. **DEF-001** — `loadRecordsFromFile` 컬럼 검증; `TC_EXC_05` graceful 전환
2. **DEF-002** — FR-S02 height 보정 구현; `TC_HGT_*` 기대값 갱신
3. **DEF-006** — Step 09 Golden Master + ctest
4. **DEF-004** — 비율 합 100% 계약 제품·요구서 합의
