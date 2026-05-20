# Step 14 — 연령대 BMI 분포 API 정비 (FR-S03) — Transcript

## User prompt (요약)

Step 14 FR-S03: `getAgeBandRatios(ageClass)` 명시 API 추가, `getBmiRatio` 하위 호환·위임, `BmiCategory`/type 100~400 문서화. `AgeBandDistributionApi` 스위트 TC-API-01~04, 6×4 스냅샷, 단일 연령대 합 100% 회귀. main printf 포맷 유지. `docs/feature_implementation_notes.md` §14, Report·Prompting wrap-up.

## Agent actions

1. `SHealth.h` — `getAgeBandRatios(int ageClass) const`, `isValidAgeClass` private
2. `SHealth.cpp` — `getAgeBandRatios` 구현; `getBmiRatio`를 SSOT 위임으로 리팩터
3. `SHealthBMI.cpp` — `getAgeBandRatios`로 4필드 출력 (포맷 문자열 동일)
4. `SHealthBMITest.cpp` — `TC_API_01`~`04` (`AgeBandDistributionApi`)
5. `CMakeLists.txt` — `SHEALTH_TEST_DATA_FILE` for TC-API-02
6. `docs/feature_implementation_notes.md` — §14 API 계약·마이그레이션
7. `build-gcc`: `ctest` 46/46 Pass (Golden 포함)

## Key design decisions

- 채택 방안 D: `getAgeBandRatios` public + `getBmiRatio` 위임 (`feature_requirements_design.md` §4.2.2)
- 잘못된 ageClass → 정적 빈 `AgeBandRatios` 참조 (zero ratios)
- TC-API-02: Golden baseline 6행 수치를 `shealth.dat` 스냅샷으로 고정

## Outcome

FR-S03 API 정식화·테스트 Green. Golden baseline 불변.

**commit string:** `14_연령대_BMI_분포_API_FR-S03`
