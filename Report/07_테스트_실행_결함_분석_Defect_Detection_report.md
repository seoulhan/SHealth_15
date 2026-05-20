# 07. 테스트 실행 & 결함 분석 (Defect Detection) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 07 — 테스트 실행 & 결함 분석 |
| commit string (권장) | `07_테스트_실행_결함_분석_Defect_Detection` |
| 기준 문서 | `docs/test_cases.md`, `docs/test_plan.md`, `docs/requirements_analysis.md` |

## 1. 작업 목표

- `cmake --build` + `ctest -V`로 **전체 단위 테스트** 실행
- 실패 TC 원인 분석(요구사항 vs 구현 vs 테스트)
- BMI 경계·통계 비율 합산 등 **패턴 분류**
- 수정 필요 항목(코드 / 테스트 / 요구 불명확) 및 Severity 정리
- 산출물: `docs/defect_detection.md`

## 2. 수행 내용

### 2.1 테스트 실행

| 항목 | 값 |
|------|-----|
| 빌드 디렉터리 | `build-gcc` (MinGW Makefiles) |
| 명령 | `cmake --build .` → `ctest -V` |
| 결과 | **38 passed, 0 failed** (~0.76 s) |
| README `build/` | nmake 미설치로 configure 실패 — **본 환경에서는 build-gcc 사용** |

### 2.2 결함 분석 요약

| 구분 | 건수 | 비고 |
|------|------|------|
| **ctest 실패** | 0 | P0 Green 유지 |
| **Closed (Fixed)** | I-01 BMI=25 | `TC_CLS_06`, `TC_BMI_02`로 검증 |
| **Open — 코드** | I-09, I-09b, I-03, I-06 | CSV abort, height=0, 10k 상한 |
| **Open — spec/설계** | I-10 | 비율 합 분모 정의 |
| **Open — 회귀** | GM-01 | Golden Master 미구축 |
| **테스트 기대값 오류** | 0 | — |

### 2.3 실패 패턴 분류

- **P-A** BMI 경계 — I-01 수정 완료
- **P-B** 비율 합 ≠ 100% (non-finite) — I-10, `TC_HGT_*`
- **P-C** weight=0 보정 — Green
- **P-D** CSV 견고성 — `TC_EXC_05` DEATH = I-09 Open High
- **P-E** FR-S02 height=0 — Open Medium
- **P-G** E2E Golden — Step 09

### 2.4 통합 스모크

- `SHealthBMI.exe` + `shealth.dat`: 6연령대 stdout 정상, 연령대별 4분류 합 ≈ 100%

## 3. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| 결함 분석서 | `docs/defect_detection.md` | 생성 |
| 보고서 | `Report/07_테스트_실행_결함_분석_Defect_Detection_report.md` | 본 문서 |
| Transcript | `Prompting/07_테스트_실행_결함_분석_Defect_Detection.md` | 생성 |

## 4. Wrap-up

- Report: 본 문서
- Prompting: `07_테스트_실행_결함_분석_Defect_Detection.md`
- commit (사용자 요청 시): `07_테스트_실행_결함_분석_Defect_Detection`

## 5. 다음 권장 작업

1. **Step 08** — `docs/defect_list.md` (§9 후보 DEF-001~007 티켓화)
2. **I-09** — CSV graceful parse, `TC_EXC_05`를 `EXPECT_DEATH` → 정상 반환 TC로 전환
3. **FR-S02** — height=0 보정 + `TC_HGT_*` 갱신
4. **Step 09** — `shealth.dat` Golden Master + ctest 등록
5. Windows CI용 **CMake generator** (`MinGW` vs `Ninja`) README/README 빌드 섹션 정리
