# SHealth BMI — Golden Master (회귀 안전장치)

| 항목 | 내용 |
|------|------|
| 문서 버전 | 1.0 |
| 작성일 | 2026-05-20 |
| Step | 09 — Golden Master 자동화 |
| commit string (권장) | `09_Golden_Master_자동화` |
| 관련 결함 | DEF-006 (GM-01), NFR-05 |
| 입력 | `SHealthBMI` + `shealth.dat` |

---

## 1. 목적

리팩토링·기능 추가 후 **main 프로그램 stdout**이 의도치 않게 바뀌지 않았는지 자동으로 검증한다.  
단위 테스트(`SHealthBMITest`)가 도메인 API를 검증한다면, Golden Master는 **E2E 출력 계약**(6연령대 × 4분류 `printf`)을 고정한다.

---

## 2. 구성 요소

| 경로 | 역할 |
|------|------|
| `tests/golden/shealth_bmi_stdout.golden.txt` | 기준(baseline) stdout (6행) |
| `src/test/cpp/SHealthGoldenTest.cpp` | `SHealthBMI` 실행 → baseline 비교 (Google Test) |
| `tests/golden/update_baseline.ps1` | baseline 수동 갱신 스크립트 |
| `CMakeLists.txt` | `SHealthGoldenTest` 타깃 + `gtest_discover_tests` → **ctest** 등록 |

---

## 3. 출력 계약 (캡처 대상)

`SHealthBMI.cpp`가 `shealth.dat` 로드 후 연령대 20·30·40·50·60·70에 대해 다음 형식으로 6행을 출력한다.

```text
{ageBand} - underweight = {float}, normal = {float}, overweight = {float}, obesity = {float}
```

- `ageBand`: 10년 단위 연령대 시작값 (20, 30, …, 70)
- 4개 비율: `getBmiRatio(ageBand, 100|200|300|400)` — README·FR-08과 동일
- `printf` `%f` 기본 정밀도(**소수 6자리**)

**실행 조건:** 작업 디렉터리(CWD)에 `shealth.dat`가 있어야 한다. Golden 테스트는 **저장소 루트**(`shealth.dat` 부모)를 CWD로 두고 `SHealthBMI`를 실행한다.

---

## 4. 정규화 규칙

비교 전 양쪽 stdout/baseline에 동일 규칙을 적용한다 (`SHealthGoldenTest.cpp`).

| 규칙 | 설명 |
|------|------|
| 줄바꿈 | `\r\n` → `\n` (CRLF/LF 통일) |
| 파일 끝 | 마지막 연속 `\n` 제거 |
| 빈 줄 | 무시 |
| 행 끝 공백 | trim |
| 행 수 | 비어 있지 않은 줄 **정확히 6행** |

---

## 5. 허용 오차 (비교)

| 항목 | 값 | 비고 |
|------|-----|------|
| 연령대 정수 (`20` 등) | **정확 일치** | `EXPECT_EQ` |
| 4개 비율 float | **절대 오차 ≤ 0.0001** | `EXPECT_NEAR`, `kGoldenRatioTolerance` |
| 파싱 | 정규식으로 5개 필드 추출 | 형식 변경 시 baseline·파서 동시 갱신 |

> 의도적 동작 변경(예: BMI=25 비만 수정, height 보정) 시 수치가 바뀌면 **baseline 갱신**이 필요하다. 포맷 문자열만 바뀌면 파서·golden 파일을 함께 수정한다.

---

## 6. 빌드 및 ctest 실행

```powershell
mkdir build-gcc
cd build-gcc
cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/mingw64/bin/g++.exe ..
cmake --build .
ctest --output-on-failure
```

Golden 테스트만 실행:

```powershell
ctest -R SHealthGoldenMaster -V
# 또는
.\SHealthGoldenTest.exe --gtest_filter=SHealthGoldenMaster.*
```

**선행 조건:** `SHealthBMI`(또는 `SHealthBMI.exe`)가 빌드되어 있어야 한다. `add_dependencies(SHealthGoldenTest SHealthBMI)`로 빌드 순서를 보장한다.

---

## 7. Baseline 갱신 절차

의도된 출력 변경(리팩토링·버그 수정·데이터 변경) 후에만 수행한다.

### 7.1 사전 확인

1. 변경 사유를 `docs/refactoring_notes.md` 또는 PR에 기록한다.
2. `SHealthBMITest` 등 단위 테스트가 Green인지 확인한다.
3. 수동으로 한 번 실행해 stdout이 기대와 일치하는지 확인한다.

```powershell
cd c:\DEV\SHealth_15
.\build-gcc\SHealthBMI.exe
```

### 7.2 스크립트로 갱신 (권장)

```powershell
cd c:\DEV\SHealth_15
.\tests\golden\update_baseline.ps1 -BuildDir build-gcc
```

- 기본 출력: `tests/golden/shealth_bmi_stdout.golden.txt`
- UTF-8 **BOM 없음**, 줄바꿈 **LF**

### 7.3 수동 갱신

1. 프로젝트 루트에서 `SHealthBMI` 실행
2. stdout 6행을 복사
3. `tests/golden/shealth_bmi_stdout.golden.txt`에 덮어쓰기 (끝에 단일 `\n` 권장)

### 7.4 검증

```powershell
cd build-gcc
cmake --build .
ctest -R SHealthGoldenMaster -V
```

커밋에는 **코드 변경 + 갱신된 golden 파일**을 함께 포함하고, 커밋 메시지에 baseline 갱신 이유를 적는다.

---

## 8. 현재 baseline 스냅샷 (요약)

`shealth.dat` 전체 로드 기준 (2026-05-20, Step 04 리팩토링·BMI=25 수정 반영):

| 연령대 | underweight | normal | overweight | obesity |
|--------|-------------|--------|------------|---------|
| 20 | 3.511053 | 23.797139 | 11.833550 | 60.858257 |
| 30 | 1.863354 | 15.527950 | 10.062112 | 72.546584 |
| 40 | 0.521512 | 10.039113 | 9.126467 | 80.312907 |
| 50 | 2.181401 | 12.629162 | 9.988519 | 75.200918 |
| 60 | 0.862895 | 8.533078 | 10.642378 | 79.961649 |
| 70 | 0.529101 | 12.345679 | 10.758377 | 76.366843 |

---

## 9. 추적성

| ID | 매핑 |
|----|------|
| NFR-05 | 회귀 — Golden + ctest |
| FR-08 | main 6연령대 stdout 계약 |
| GM-01 / DEF-006 | Step 09에서 **Fixed** (`defect_list.md` 갱신) |
| TC | `SHealthGoldenMaster.TC_GM_01_SHealthBMI_stdout_matches_baseline` |

---

## 10. 변경 이력

| 버전 | 일자 | 변경 |
|------|------|------|
| 1.0 | 2026-05-20 | Step 09 — baseline·GTest·ctest·갱신 절차 최초 작성 |
