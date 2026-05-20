# Step 04 — 리팩토링 지원 (Transcript Export)

**Task Title:** 04. 리팩토링 지원 (테스트 Green 유지)  
**commit string:** `04_리팩토링_Refactoring_Support`  
**Persona:** 리팩토링 및 Unit Test expert

---

## User Prompt

```
## Step 04 — 리팩토링 지원 (테스트 Green 유지)

# Task Title : 04. 리팩토링 지원 (테스트 Green 유지)
## commit string : 04_리팩토링_Refactoring_Support

@README.md @CMakeLists.txt @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @src/main/cpp/SHealthBMI.cpp @src/test/cpp/SHealthBMITest.cpp @docs/requirements_analysis.md @docs/code_quality_analysis.md @.cursorrules

[Persona] 리팩토링 및 Unit Test expert

[Context] C++17, CMake, Google Test. README Activities 2 순서: 네이밍 → 하드코드/전역 제거 → 함수 추출 → 반복/중복 제거. **Unit Test 본격 작성(05~) 이전**에 수행.

[Task] 1차 리팩토링 수행 (동작 동등성 유지).
- docs\code_quality_analysis.md의 P0/P1 항목부터 적용
- BMI 상수·분류 enum/struct, 연령대 통계 구조 단순화, split/파싱·보정·계산·집계 함수 분리
- Magic number(100/200/300/400) 의미 있는 이름으로 치환
- 기존 테스트가 있으면 Green 유지; FailedTest만 있으면 제거 또는 skip 후 리팩토링
- SHealthBMI main 출력 형식은 Golden Master 전까지 변경 최소화
- 변경 요약·남은 기술 부채·05 테스트에 노출할 public API 목록 기록

[Format]
- 코드: src/main/cpp/* (필요 시 CMakeLists 최소 수정)
- Markdown. docs\refactoring_notes.md

[Wrap-up]
- Report\04_리팩토링_Refactoring_Support_report.md
- Transcript → Prompting\04_리팩토링_Refactoring_Support.md
```

---

## Assistant Actions (요약)

1. **P0**: `classifyBmi()`에 BMI 경계 상수·`>= 25` 비만; 보정 `validWeightCount==0` 가드; 집계 `bandMemberCount==0` 가드
2. **P1**: `loadRecordsFromFile` / `imputeMissingWeightsByAgeBand` / `computeBmis` / `aggregateAgeBandStatistics` 추출; `AgeBandRatios[6]`; `enum class BmiCategory`; `isInAgeBand`, `ageBandToIndex`
3. **getBmiRatio**: 24분기 제거 → 인덱스 + legacy type switch
4. **main**: 연령대 루프, printf 형식 동일
5. **테스트**: `FailedTest` 제거 → ctest Green (0 tests)
6. **문서**: `docs/refactoring_notes.md`, Report, 본 Transcript
7. **빌드**: `build-gcc` MinGW — compile·run OK

---

## 주요 코드 구조 (After)

```
calculateBmi()
  → loadRecordsFromFile()
  → imputeMissingWeightsByAgeBand()  // isInAgeBand
  → computeBmis()                    // CM_PER_METER
  → aggregateAgeBandStatistics()     // classifyBmi → AgeBandRatios[]

getBmiRatio(ageClass, type)
  → ageBandToIndex + BmiCategory from legacy type
```

---

## 산출물

| 파일 |
|------|
| `src/main/cpp/SHealth.h` |
| `src/main/cpp/SHealth.cpp` |
| `src/main/cpp/SHealthBMI.cpp` |
| `src/test/cpp/SHealthBMITest.cpp` |
| `docs/refactoring_notes.md` |
| `Report/04_리팩토링_Refactoring_Support_report.md` |
| `Prompting/04_리팩토링_Refactoring_Support.md` |

---

## 참고

- P0-1로 BMI=25 비만 분류 → 출력 **수치**는 As-Is와 다를 수 있음 (형식은 동일)
- Step 05: private `classifyBmi` 등 테스트 노출 방안 — `refactoring_notes.md` §4
