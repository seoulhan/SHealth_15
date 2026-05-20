# 06. 테스트 케이스 작성 (Test Cases) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 06 — Google Test TC 구현 |
| commit string (권장) | `06_테스트_케이스_작성_Test_Cases` |
| 기준 문서 | `docs/test_plan.md` |

## 1. 작업 목표

- `docs/test_plan.md`의 P0 TC를 `SHealthBMITest.cpp`에 구현
- BMI 계산·weight=0 보정·4분류 경계·연령대·`getBmiRatio`·파일 예외 커버
- `FailedTest` 없이 `ctest` 전체 Green

## 2. 수행 내용

### 2.1 코드 변경

| 파일 | 변경 |
|------|------|
| `src/test/cpp/SHealthBMITest.cpp` | `SHealthBMITestFixture` + 38 `TEST`/`TEST_F` 구현 |
| `src/main/cpp/SHealth.h` | `testClassifyBmi`, `testIsInAgeBand` test hook |
| `src/main/cpp/SHealth.cpp` | test hook 구현 |
| `src/test/fixtures/minimal_valid.dat` | 체크인 fixture 1건 |
| `CMakeLists.txt` | `SHEALTH_TEST_FIXTURE_DIR`, `SHEALTH_TESTING` (fixture 경로) |

### 2.2 테스트 전략

| 영역 | 방법 |
|------|------|
| BMI 18.5/23/25, 연령 19·20·29·30 | `testClassifyBmi` / `testIsInAgeBand` 직접 + 통합 CSV |
| 보정·공식·집계 | temp CSV + `calculateBmi` + `getBmiRatio` |
| CSV 예외 | TC-EXC-05: `EXPECT_DEATH` (As-Is vector assert); TC-EXC-06: `EXPECT_THROW` |
| Fixture | 런타임 temp CSV + `src/test/fixtures/minimal_valid.dat` |

### 2.3 실행 결과

```text
100% tests passed, 0 tests failed out of 38
Total Test time (real) ≈ 0.78 sec
```

빌드: `build-gcc`, MinGW, CMake + Google Test 1.14.

## 3. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| 테스트 구현 | `src/test/cpp/SHealthBMITest.cpp` | 완료 |
| TC 목록·fixture 설명 | `docs/test_cases.md` | 생성 |
| 정적 fixture | `src/test/fixtures/minimal_valid.dat` | 생성 |
| 보고서 | `Report/06_테스트_케이스_작성_Test_Cases_report.md` | 본 문서 |
| Transcript | `Prompting/06_테스트_케이스_작성_Test_Cases.md` | 생성 |

## 4. Wrap-up

- Report: 본 문서
- Prompting: `06_테스트_케이스_작성_Test_Cases.md`
- commit (사용자 요청 시): `06_테스트_케이스_작성_Test_Cases`

## 5. 다음 권장 작업

1. (선택) coverage 빌드 + `gen_lcov2.bat`
2. Step 07~08: height=0 보정(FR-S02) 시 `HeightImputation` 스위트 추가
3. Step 09: `SHealthBMI` stdout Golden Master
