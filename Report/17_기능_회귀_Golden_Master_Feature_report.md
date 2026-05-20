# 17. 기능 회귀·Golden Master 갱신 (Feature Regression) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 17 — 기능 회귀·Golden Master 갱신 |
| commit string (권장) | `17_기능_회귀_Golden_Master_Feature` |
| Persona | 시니어 QA 엔지니어 + 리팩토링 및 Unit Test expert |
| 상세 보고 | `docs/feature_regression_report.md` |

## 1. 작업 목표

Step 13~16 기능·SRP 반영 후 **전체 회귀**. README NFR-05: `ctest` Green, FR-08 Golden 6연령대 유지, 신규 데모 출력의 Golden 대상 여부 문서화.

## 2. 수행 내용

### 2.1 빌드·테스트

```powershell
cd build-gcc
cmake --build .
ctest -V
```

### 2.2 Golden Master

- `TC_GM_01`: **Passed** — 앞 6행만 baseline 비교 (`SHealthGoldenTest.cpp`)
- `shealth_bmi_stdout.golden.txt`: **변경 없음**
- `update_baseline.ps1`: **미실행**

### 2.3 신규 데모 출력 정책

| 출력 | Golden | 검증 |
|------|--------|------|
| FR-08 6연령대 | **예** | `TC_GM_01` |
| FR-C01 정상 ID 목록 | **아니오** | `TC_LST_*` |
| FR-C02 Global 비율 | **아니오** | `TC_GLB_*` |

별도 stdout 스모크 TC: **미추가** (단위 TC로 충분).

### 2.4 문서

| 산출물 | 경로 |
|--------|------|
| 회귀 보고서 | `docs/feature_regression_report.md` |

## 3. 검증 결과

| 지표 | 결과 |
|------|------|
| ctest | **52/52 Passed** (100%) |
| Golden | **1/1 Passed**, baseline diff **0** |
| 실패 분류 | 0 (기능/TC/Golden/환경) |

**스위트:** Classification 7 + AgeBand 1 + Fixture 43 + Golden 1.

## 4. Open 결함 스냅샷

| ID | Step 17 판정 |
|----|----------------|
| DEF-002 (height=0) | **Fixed** (Step 13, TC Green) — `defect_list.md` 문서 미동기화 |
| DEF-011 (CWD) | **Open** (P3) |

## 5. 다음 Step

| Step | 내용 |
|------|------|
| 18 | Activity 4~5 종합 보고 |
| 권장 | `defect_list.md` DEF-002 Fixed·ctest 52건 반영 |
