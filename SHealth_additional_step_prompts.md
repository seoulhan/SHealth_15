# SHealth_15 — Additional Step Prompts (Activity 4 · feature 브랜치)

> Step 01~11(`SHealth_all_step_prompts.md`) 완료 후, **Git `feature` 브랜치**에서 수행.  
> 선행: `tc` 브랜치에서 Golden Master·결함 관리(Step 09~10)까지 Green.  
> README Activities **4. 기능 개선** + MoSCoW(FR-S01~S03, FR-C01~C02) 반영.  
> 각 step 종료 시 `docs\` 산출물 생성·다음 step에서 `docs\` 참조. Wrap-up 시 `Report\`, `Prompting\` 정리.

---

## Step 12 — 기능 개선 요구사항·설계 (Feature Branch Kickoff)

# Task Title : 12. 기능 개선 요구사항·설계 (Feature Branch Kickoff)
## commit string : 12_기능_개선_요구사항_설계_Feature_Kickoff

@README.md @docs/requirements_analysis.md @docs/qa_final_report.md @docs/test_plan.md @docs/refactoring_notes.md @docs/golden_master.md @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @.cursorrules

[Persona] 시니어 비즈니스·시스템 분석가 + 소프트웨어 아키텍트

[Context] Step 11까지 As-Is( FR-01~08 )·테스트·Golden Master가 확보됨. README Activity 4 5항목 중 **연령대 BMI 분포**는 FR-05/FR-06으로 이미 존재 → FR-S03은 API 정비로 해석(`requirements_analysis.md` Note). `feature` 브랜치에서 신규 API·보정·출력 확장을 설계한다.

[Task] 기능 개선 요구·설계서 작성 (코드 변경 없음 또는 최소).
- README Activity 4 5항목을 **구현 단위**로 분해·우선순위(P0/P1) 부여
  - FR-S02 height=0 동연령대 평균 키 보정 (weight=0 보정과 대칭, 파이프라인 순서 명시)
  - FR-S03 연령대 BMI 4분류 비율 **명시적 API** (`getBmiRatio` 호환·대체·보완 방안)
  - FR-C01 정상 BMI(18.5 < BMI < 23) 사용자 **ID 목록**
  - FR-C02 **전체** 사용자 기준 4분류 비율(%)
  - FR-S01 SRP: 파일 I/O·보정·BMI·통계·조회·출력 경계 (Step 04 이후 구조 기준)
- 각 기능별 **입력·출력·전제·예외**(동연령대 유효값 0건, non-finite BMI, id 파싱) 정의
- `docs\test_plan.md` §6 예약 TC ID 매핑(TC-HGT-10/11, AgeBandDistributionApi, TC-LST-*, TC-GLB-*)
- 기존 `SHealthBMI` 6연령대 출력(FR-08)·Golden Master와의 **호환 전략**(유지 vs 확장 출력, baseline 갱신 조건)
- Step 13~17 구현 순서·의존 관계 다이어그램

[Format] Markdown. `docs\feature_requirements_design.md`

[Wrap-up]
- `Report\12_기능_개선_요구사항_설계_Feature_Kickoff_report.md`
- Transcript → `Prompting\12_기능_개선_요구사항_설계_Feature_Kickoff.md`

---

## Step 13 — Height=0 평균 보정 (FR-S02)

# Task Title : 13. Height=0 평균 보정 (FR-S02)
## commit string : 13_Height0_평균_보정_FR-S02

@README.md @CMakeLists.txt @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @src/test/cpp/SHealthBMITest.cpp @docs/feature_requirements_design.md @docs/requirements_analysis.md @docs/test_plan.md @.cursorrules

[Persona] 리팩토링 및 Unit Test expert

[Context] As-Is는 `height==0` 시 BMI inf/NaN·집계 스킵 가능(I-03). FR-S02: **동일 연령대** 내 `height != 0` 레코드의 산술 평균(cm)으로 보정 후 BMI 계산. `imputeMissingWeightsByAgeBand()`와 **대칭** 설계·호출 순서(보정 → BMI → 집계) 문서화.

[Task]
- `imputeMissingHeightsByAgeBand()`(또는 동등 책임 함수) 구현·`calculateBmi` 파이프라인에 연동
- 동연령대 유효 키 0건·전원 height=0·weight=0 조합 등 엣지는 `feature_requirements_design.md` 및 I-02/I-03에 맞게 처리
- `docs\test_plan.md` §6.1 `HeightImputation`: **TC-HGT-10**, **TC-HGT-11** 구현·Green
- 기존 P0 TC·Golden Master: 의도적 변경 없으면 Green 유지; baseline 변경 시 `docs\golden_master.md` 절차 준수
- 구현·API·알려진 제약을 `docs\feature_implementation_notes.md`에 누적(§13 절 추가)

[Format]
- 코드: `src/main/cpp/SHealth.h`, `SHealth.cpp`
- 코드: `src/test/cpp/SHealthBMITest.cpp`, 필요 시 fixture
- Markdown. `docs\feature_implementation_notes.md` (신규 또는 갱신)

[Wrap-up]
- `Report\13_Height0_평균_보정_FR-S02_report.md`
- Transcript → `Prompting\13_Height0_평균_보정_FR-S02.md`

---

## Step 14 — 연령대 BMI 분포 API 정비 (FR-S03)

# Task Title : 14. 연령대 BMI 분포 API 정비 (FR-S03)
## commit string : 14_연령대_BMI_분포_API_FR-S03

@README.md @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @src/main/cpp/SHealthBMI.cpp @src/test/cpp/SHealthBMITest.cpp @docs/feature_requirements_design.md @docs/feature_implementation_notes.md @docs/test_plan.md @.cursorrules

[Persona] 리팩토링 및 Unit Test expert

[Context] README「특정 연령대의 BMI 분포 비율」은 비즈니스상 `getBmiRatio(ageClass, type)`·`AgeBandRatios`로 제공 중. FR-S03: **명시적·확장 가능한 API**로 정식화(예: `getAgeBandDistribution(int ageClass)` → 4분류 %, 잘못된 ageClass 처리, `BmiCategory`/`type` 100~400 매핑 문서화). 기존 `getBmiRatio` **하위 호환** 유지 권장.

[Task]
- FR-S03 API 설계안대로 public 메서드·구조체(또는 `AgeBandRatios` 활용) 구현
- 6연령대 × 4분류 **스냅샷 검증** (`AgeBandDistributionApi` 스위트, `docs\test_plan.md` §6.1)
- 단일 연령대 4분류 합 **100% ± 0.01**(유한 BMI만) 회귀 확인
- `SHealthBMI.cpp`: 필요 시 동일 수치를 신 API로 출력·기존 6행 `printf` **형식 호환** 유지(변경 시 Golden Master 갱신 사유 기록)
- `docs\feature_implementation_notes.md`에 FR-S03 API 계약·마이그레이션 노트 추가

[Format]
- 코드: `src/main/cpp/*`, `src/test/cpp/SHealthBMITest.cpp`
- Markdown. `docs\feature_implementation_notes.md` (갱신)

[Wrap-up]
- `Report\14_연령대_BMI_분포_API_FR-S03_report.md`
- Transcript → `Prompting\14_연령대_BMI_분포_API_FR-S03.md`

---

## Step 15 — 정상 BMI 목록·전체 범주 비율 (FR-C01, FR-C02)

# Task Title : 15. 정상 BMI 목록·전체 범주 비율 (FR-C01, FR-C02)
## commit string : 15_정상목록_전체비율_FR-C01_FR-C02

@README.md @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @src/main/cpp/SHealthBMI.cpp @src/test/cpp/SHealthBMITest.cpp @docs/feature_requirements_design.md @docs/feature_implementation_notes.md @docs/requirements_analysis.md @docs/test_plan.md @.cursorrules

[Persona] 리팩토링 및 Unit Test expert

[Context] CSV **id** 필드 파싱·저장 필요(I-11). 정상 BMI: README 기준 **18.5 초과, 23 미만**. 전체 범주 비율: 연령대 무관 **전체 레코드** 기준 4분류 %(FR-C02), 연령대별 합과 독립.

[Task]
- **FR-C01**: `getNormalBmiUserIds()`(또는 합의된 이름) — 정상 범위 ID `std::vector<int>`(또는 `std::vector<std::string>`) 반환; 경계 18.5·23 **제외** TC-LST-01/02
- **FR-C02**: `getGlobalBmiRatios()`(또는 합의된 이름) — 전체 4분류 %; TC-GLB-01(합≈100%), TC-GLB-02(연령대 통계와 독립)
- `calculateBmi` 실행 후에만 조회 가능 등 **전제 조건** 명시·테스트
- `SHealthBMI.cpp`: 데모 출력 추가(형식은 `feature_requirements_design.md` 합의); 기존 6행 Golden과 분리·또는 baseline 갱신 절차 문서화
- `docs\feature_implementation_notes.md`에 FR-C01/C02 절 추가

[Format]
- 코드: `src/main/cpp/*`, `src/test/cpp/SHealthBMITest.cpp`, id 파싱 시 `SHealth.cpp` 갱신
- Markdown. `docs\feature_implementation_notes.md` (갱신)

[Wrap-up]
- `Report\15_정상목록_전체비율_FR-C01_FR-C02_report.md`
- Transcript → `Prompting\15_정상목록_전체비율_FR-C01_FR-C02.md`

---

## Step 16 — SRP 책임 분리 (FR-S01)

# Task Title : 16. SRP 책임 분리 (FR-S01)
## commit string : 16_SRP_책임_분리_FR-S01

@README.md @CMakeLists.txt @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @src/main/cpp/SHealthBMI.cpp @src/test/cpp/SHealthBMITest.cpp @src/test/cpp/SHealthGoldenTest.cpp @docs/feature_requirements_design.md @docs/feature_implementation_notes.md @docs/code_quality_analysis.md @docs/refactoring_notes.md @.cursorrules

[Persona] 시니어 소프트웨어 아키텍트 + 리팩토링 및 Unit Test expert

[Context] README Activity 4「SRP에 따른 책임 분리등 리팩토링」. Step 04는 함수 추출·상수화 수준; FR-S01은 **역할별 분리**(파일 I/O, weight/height 보정, BMI 계산, 연령대·전체 집계, 조회 API, 콘솔 출력). Step 13~15에서 추가한 API·동작 **유지**하며 구조만 개선.

[Task]
- `feature_requirements_design.md`의 SRP 경계에 따라 클래스/파일 분리 또는 명확한 네임스페이스·컴포지션 적용 (과도한 추상화 금지)
- public API 시그니처·동작 **하위 호환**; 변경 시 `docs\feature_implementation_notes.md`에 breaking note
- 기존·신규 TC 전부 Green; `Architecture` 회귀 스위트(`docs\test_plan.md` §6.1 FR-S01)로 fixture 동등성 확인
- `SHealthBMI`는 **Presenter** 역할만 남기도록 정리(가능 범위)
- 분리 전·후 책임 매트릭스·남은 부채를 `docs\feature_srp_refactoring.md`에 기록

[Format]
- 코드: `src/main/cpp/*`, `CMakeLists.txt` (신규 .cpp/.h 시)
- 코드: `src/test/cpp/*` (경로·include 조정)
- Markdown. `docs\feature_srp_refactoring.md`

[Wrap-up]
- `Report\16_SRP_책임_분리_FR-S01_report.md`
- Transcript → `Prompting\16_SRP_책임_분리_FR-S01.md`

---

## Step 17 — 기능 회귀·Golden Master 갱신 (Feature Regression)

# Task Title : 17. 기능 회귀·Golden Master 갱신 (Feature Regression)
## commit string : 17_기능_회귀_Golden_Master_Feature

@README.md @CMakeLists.txt @src/test/cpp/SHealthBMITest.cpp @src/test/cpp/SHealthGoldenTest.cpp @tests/golden/shealth_bmi_stdout.golden.txt @tests/golden/update_baseline.ps1 @docs/golden_master.md @docs/feature_implementation_notes.md @docs/feature_requirements_design.md @docs/test_cases.md @.cursorrules

[Persona] 시니어 QA 엔지니어 + 리팩토링 및 Unit Test expert

[Context] Step 13~16 기능·SRP 반영 후 **전체 회귀**. README NFR-05: main 출력·`ctest` Green. `SHealthGoldenTest`·`tests/golden/`·`update_baseline.ps1` 사용.

[Task]
- `cmake --build .` 후 `ctest -V` 전체 실행; 실패 분류(기능 버그 / TC 기대값 / Golden drift)
- Golden Master: FR-08 6연령대 출력 **유지** 시 baseline 불변 확인; **의도적** stdout 변경 시 `docs\golden_master.md` 절차로 baseline 갱신·PR에 사유 명시
- 신규 기능 데모 출력이 Golden 대상인지 여부 결정·문서화(대상이 아니면 별도 스모크 TC)
- Open 결함·Known Issues 스냅샷: `docs\feature_regression_report.md` (실행 로그 요약, TC 통과율, Golden 상태)

[Format] Markdown. `docs\feature_regression_report.md`

[Wrap-up]
- `Report\17_기능_회귀_Golden_Master_Feature_report.md`
- Transcript → `Prompting\17_기능_회귀_Golden_Master_Feature.md`

---

## Step 18 — 기능 개선 종합·Activity 5 회고 갱신

# Task Title : 18. 기능 개선 종합·Activity 5 회고 갱신
## commit string : 18_기능_개선_종합_Activity5_회고

@README.md @docs/requirements_analysis.md @docs/qa_final_report.md @docs/feature_requirements_design.md @docs/feature_implementation_notes.md @docs/feature_srp_refactoring.md @docs/feature_regression_report.md @docs/test_plan.md @docs/golden_master.md @Report/

[Persona] QA·기술 리드 (발표·회고 담당)

[Context] README Activities **4** 완료·**5** 회고. Step 11 `qa_final_report.md`의「미완 Activities 4 로드맵」을 **실제 달성 결과**로 갱신.

[Task]
- FR-S01~S03, FR-C01~C02 요구 대비 **달성도 표**(완료/부분/미구현·근거)
- Before(Step 11 시점) / After(Step 18) 코드 품질·테스트·Golden 요약
- README Activity 5 항목 반영: 목표 달성도, AI 활용, TC가 기능 개선에 미친 영향, 클린코드·리팩토링 체감
- 잔여 리스크·권장 다음 스프린트(10대·80대, vector 전환, DIP 등)
- `docs\qa_final_report.md` **부록 또는 v2 섹션**으로 Activity 4~5 통합 요약 추가(원본 훼손 최소화)

[Format] Markdown. `docs\feature_final_report.md` (+ `docs\qa_final_report.md` 갱신)

[Wrap-up]
- `Report\18_기능_개선_종합_Activity5_회고_report.md`
- Transcript → `Prompting\18_기능_개선_종합_Activity5_회고.md`

---

## 실행 순서 요약

| Step | 제목 | docs 산출물 | 선행 docs |
|------|------|-------------|-----------|
| 12 | 기능 개선 요구·설계 | feature_requirements_design.md | requirements_analysis, qa_final_report, test_plan |
| 13 | height=0 보정 | feature_implementation_notes.md (§13) | 12 |
| 14 | 연령대 분포 API | feature_implementation_notes.md (§14) | 12, 13 |
| 15 | 정상 목록·전체 비율 | feature_implementation_notes.md (§15) | 12, 13, 14 |
| 16 | SRP 분리 | feature_srp_refactoring.md | 12~15, refactoring_notes |
| 17 | 기능 회귀·Golden | feature_regression_report.md | 13~16, golden_master |
| 18 | 기능 종합·회고 | feature_final_report.md, qa_final_report 갱신 | 12~17, qa_final_report |

## README Activity 4 ↔ Step 매핑

| README 기능 개선 항목 | 요구 ID | 담당 Step |
|----------------------|---------|-----------|
| SRP에 따른 책임 분리등 리팩토링 | FR-S01 | **16** (13~15 구현 후 구조 정리) |
| 특정 연령대의 BMI 분포 비율 계산 기능 추가 | FR-S03 | **14** (API 정비; 비즈니스 로직은 As-Is 확장) |
| Height가 0인 경우 평균치 보정 | FR-S02 | **13** |
| BMI 정상 범위 사용자 목록 조회 | FR-C01 | **15** |
| 전체 사용자 대비 각 BMI 범주 비율 | FR-C02 | **15** |

## Git · 브랜치

- **브랜치**: `git checkout tc` → `git checkout -b feature` (Step 12 시작 전)
- **커밋**: step별 `commit string` 사용 (`gen_commit_push.bat`)
- **PR**: 교재 정책에 따라 `feature` → `tc` PR + Review (merge 생략 가능)
- **원칙**: Step 12 설계 후 13→15 구현; **16 SRP**는 신규 코드 안정 후 수행; **17**에서 `ctest`·Golden 전체 Green; **18**에서 Activity 5 회고 마무리.
