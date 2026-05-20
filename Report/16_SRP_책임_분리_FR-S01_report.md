# 16. SRP 책임 분리 (FR-S01) — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 16 — SRP 책임 분리 (FR-S01) |
| commit string (권장) | `16_SRP_책임_분리_FR-S01` |
| Persona | 시니어 소프트웨어 아키텍트 + 리팩토링 및 Unit Test expert |
| 설계 기준 | `docs/feature_requirements_design.md` §4.5 |

## 1. 작업 목표

README Activity 4「SRP에 따른 책임 분리」— Step 13~15 API·동작을 **유지**한 채 파일 I/O, 보정, BMI, 집계, 조회(Facade), 콘솔 출력(Presenter) 경계를 분리한다.

## 2. 수행 내용

### 2.1 모듈 분리

| 모듈 | 파일 | 책임 |
|------|------|------|
| Types | `SHealthTypes.h` | `BmiCategory`, `AgeBandRatios` |
| Domain | `SHealthDomain.*` | 분류·연령대·BMI 배열 계산 |
| CSV | `SHealthCsvLoader.*` | `loadRecordsFromFile`, `split` |
| Imputer | `SHealthImputer.*` | weight/height 0 보정 |
| Statistics | `SHealthStatistics.*` | 연령대·전체 집계 |
| Presenter | `SHealthPresenter.*` | 6연령대·정상 ID·Global `printf` |
| Facade | `SHealth.*` | 저장소·파이프라인·public API |

### 2.2 `SHealthBMI.cpp`

- **Before:** `printf` 3종 직접 호출
- **After:** `SHealthPresenter::print*` 만 호출 (Presenter 전용)

### 2.3 하위 호환

- public API 시그니처 **변경 없음**
- Golden FR-08 6행: baseline diff **0**

### 2.4 문서

| 산출물 | 경로 |
|--------|------|
| SRP 매트릭스·부채 | `docs/feature_srp_refactoring.md` |
| 구현 노트 §16 | `docs/feature_implementation_notes.md` |
| CMake | `shealth_lib`에 신규 `.cpp` 5개 추가 |

## 3. 테스트 결과

```
build-gcc: cmake --build . && ctest --output-on-failure
52/52 Passed (51 unit + 1 Golden)
```

Architecture 회귀: Step 06~15 fixture TC 전부 Green.

## 4. 남은 부채 (요약)

- Facade 내 고정 배열 `MAX_RECORDS`
- CSV 경로·`istream` DI 미적용
- `SHealthBMI` 데이터 파일 경로 하드코딩

→ `docs/feature_srp_refactoring.md` §7

## 5. 다음 Step

| Step | 내용 |
|------|------|
| 17 | 회귀·Golden 종합 (`feature_regression_report.md`) |
| 18 | Activity 4~5 종합 보고 |
