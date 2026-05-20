# 09. Golden Master 자동화 (회귀 안전장치) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 09 — Golden Master 자동화 |
| commit string (권장) | `09_Golden_Master_자동화` |
| 기준 문서 | `docs/refactoring_notes.md`, `docs/defect_list.md` (DEF-006) |

## 1. 작업 목표

- `SHealthBMI` + `shealth.dat` 실행 stdout을 **baseline**으로 고정
- Google Test 기반 **자동 비교** (정규화·허용 오차 명시)
- baseline **갱신 절차** 문서화
- **CMake / ctest**에 Golden 테스트 등록

## 2. 수행 내용

### 2.1 Baseline 캡처

| 항목 | 내용 |
|------|------|
| 입력 | 저장소 루트 `shealth.dat` |
| 실행 | `build-gcc/SHealthBMI.exe` (CWD = 프로젝트 루트) |
| 산출 | `tests/golden/shealth_bmi_stdout.golden.txt` (6연령대 × 4비율) |

### 2.2 Golden Master 테스트

| 항목 | 내용 |
|------|------|
| 소스 | `src/test/cpp/SHealthGoldenTest.cpp` |
| TC | `SHealthGoldenMaster.TC_GM_01_SHealthBMI_stdout_matches_baseline` |
| 동작 | `SHealthBMI` 서브프로세스 실행 → stdout 파싱 → baseline과 비교 |
| 정규화 | CRLF→LF, 빈 줄 제거, 행 끝 trim |
| 허용 오차 | 연령대 정수 exact; 비율 4종 **±0.0001** (`EXPECT_NEAR`) |

### 2.3 CMake / ctest

- `SHealthGoldenTest` 타깃 추가, `add_dependencies(SHealthGoldenTest SHealthBMI)`
- `gtest_discover_tests(SHealthGoldenTest)` → ctest **#39** 등록
- `ctest -R SHealthGoldenMaster` 단독 실행 가능

### 2.4 문서·스크립트

| 산출물 | 경로 |
|--------|------|
| Golden Master 가이드 | `docs/golden_master.md` |
| Baseline 갱신 | `tests/golden/update_baseline.ps1` |
| README | 프로젝트 구조·Golden 실행·갱신 안내 추가 |

### 2.5 결함 목록 갱신

- **DEF-006 (GM-01): Open → Fixed**
- Open 건수: 6 → 5

## 3. 검증 결과

```text
build-gcc> ctest -R SHealthGoldenMaster --output-on-failure
Test #39: SHealthGoldenMaster.TC_GM_01_* ... Passed

build-gcc> ctest
39/39 tests passed (38 unit + 1 golden)
```

## 4. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| Baseline | `tests/golden/shealth_bmi_stdout.golden.txt` | 생성 |
| Golden GTest | `src/test/cpp/SHealthGoldenTest.cpp` | 생성 |
| 갱신 스크립트 | `tests/golden/update_baseline.ps1` | 생성 |
| 문서 | `docs/golden_master.md` | 생성 |
| CMake | `CMakeLists.txt` | 수정 |
| 결함 목록 | `docs/defect_list.md` | DEF-006 Fixed |
| README | `README.md` | Golden 섹션 추가 |
| 보고서 | `Report/09_Golden_Master_자동화_report.md` | 본 문서 |
| Transcript | `Prompting/09_Golden_Master_자동화.md` | 생성 |

## 5. Wrap-up

- Report: 본 문서
- Prompting: `09_Golden_Master_자동화.md`
- commit (사용자 요청 시): `09_Golden_Master_자동화`

## 6. 다음 권장 작업

1. **DEF-001** — CSV 컬럼 부족 graceful 처리
2. **DEF-002** — height=0 보정 후 Golden baseline **재캡처**
3. **Step 10** — `docs/defect_management.md` (DEF 상태·우선순위 운영)
