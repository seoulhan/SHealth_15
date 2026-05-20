# Step 09 — Golden Master 자동화 (Transcript Export)

**Task Title:** 09. Golden Master 자동화 (회귀 안전장치)  
**commit string:** `09_Golden_Master_자동화`  
**Persona:** 리팩토링 및 Unit Test expert

---

## User Prompt

```
## Step 09 — Golden Master 자동화 (회귀 안전장치)

# Task Title : 09. Golden Master 자동화 (회귀 안전장치)
## commit string : 09_Golden_Master_자동화

@README.md @CMakeLists.txt @src/main/cpp/SHealthBMI.cpp @shealth.dat @docs/defect_list.md @docs/refactoring_notes.md @src/test/cpp/SHealthBMITest.cpp

[Persona] 리팩토링 및 Unit Test expert

[Context] SHealthBMI 실행 시 연령대별 4분류 printf 출력. 리팩토링·기능 추가 후 회귀 방지.

[Task]
- 현재 SHealthBMI + shealth.dat 기준 stdout을 baseline으로 캡처
- Google Test 또는 스크립트로 Golden Master 비교 테스트 추가 (허용 오차·정규화 규칙 명시)
- baseline 갱신 절차 문서화
- CMake/ctest에 Golden 테스트 등록

[Format]
- 코드/스크립트: src/test/cpp/ 또는 tests/golden/
- Markdown. docs\golden_master.md

[Wrap-up]
- Report\09_Golden_Master_자동화_report.md
- Transcript → Prompting\09_Golden_Master_자동화.md
```

---

## Assistant Actions (요약)

1. **Baseline 캡처** — 프로젝트 루트에서 `SHealthBMI.exe` + `shealth.dat` → 6행 stdout을 `tests/golden/shealth_bmi_stdout.golden.txt`에 저장.
2. **`SHealthGoldenTest.cpp`** — 서브프로세스 stdout 캡처, CRLF/LF·trim 정규화, 정규식 파싱, 비율 ±0.0001 `EXPECT_NEAR`.
3. **`CMakeLists.txt`** — `SHealthGoldenTest` 타깃, `SHEALTH_GOLDEN_*` 경로 정의, `gtest_discover_tests`, `SHealthBMI` 의존.
4. **`docs/golden_master.md`** — 계약·정규화·오차·ctest·baseline 갱신 절차.
5. **`update_baseline.ps1`** — 의도적 변경 시 baseline 재생성.
6. **`defect_list.md`** — DEF-006 Fixed; README 구조·테스트 안내 갱신.
7. **검증** — `ctest`: 39/39 Passed (Golden #39).
8. **Wrap-up** — Report, 본 Transcript.

---

## 산출물

| 파일 | 설명 |
|------|------|
| `tests/golden/shealth_bmi_stdout.golden.txt` | Golden baseline (6행) |
| `src/test/cpp/SHealthGoldenTest.cpp` | GTest Golden 비교 |
| `tests/golden/update_baseline.ps1` | Baseline 갱신 스크립트 |
| `docs/golden_master.md` | 절차·규칙 문서 |
| `CMakeLists.txt` | Golden 타깃·ctest |
| `docs/defect_list.md` | DEF-006 Fixed |
| `README.md` | Golden 실행·갱신 |
| `Report/09_Golden_Master_자동화_report.md` | 작업 보고서 |
| `Prompting/09_Golden_Master_자동화.md` | 본 Transcript |

---

## Baseline 스냅샷 (요약)

| 연령대 | obesity (예) |
|--------|----------------|
| 20 | 60.858257 |
| 70 | 76.366843 |

전체 수치: `docs/golden_master.md` §8.

---

## 다음 Step

- Step 10: `docs/defect_management.md`
- DEF-002(height 보정) 적용 시 `update_baseline.ps1`로 Golden 재캡처 필수
