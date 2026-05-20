# SHealth_15 — All Step Prompts (Activities)

> README Activities(6h)를 11단계로 분할. **리팩토링(04) → 테스트(05~)** 순서 준수.  
> 각 step 종료 시 `docs\` 산출물 생성·다음 step에서 `docs\` 참조. Wrap-up 시 `Report\`, `Prompting\` 정리.

---

## Step 01 — .cursorrules 초안 생성

# Task Title : 01. .cursorrules 초안 생성
## commit string : 01_cursorrules_초안_생성

@README.md @CMakeLists.txt @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @src/main/cpp/SHealthBMI.cpp @src/test/cpp/SHealthBMITest.cpp

[Persona] 리팩토링 및 Unit Test expert

[Context] CMake 기반 C++17 SHealth BMI 프로젝트. Google Test는 FetchContent로 연동. 도메인: 연령대별 BMI 통계, 체중 0 보정, shealth.dat 입력.

[Task]
- README.md·CMakeLists.txt·src 구조를 반영한 `.cursorrules` 초안을 작성·프로젝트 루트에 저장한다.
- 포함 항목: 프로젝트 개요, BMI/연령대 비즈니스 규칙, 디렉터리 규칙(docs/Report/Prompting), 코딩·리팩토링·테스트 원칙, AI 작업 시 docs 연계, 금지 사항.
- 기존 `.cursorrules`가 있으면 diff 관점으로 보완·정리한다.

[Format] `.cursorrules` (프로젝트 루트). 요약·다음 step 전달용 `docs\cursorrules_summary.md` (Markdown)

[Wrap-up]
- `Report\` 아래 `01_cursorrules_초안_생성_report.md` 생성
- 현재 프롬프트 대화 전체를 `Prompting\01_cursorrules_초안_생성.md`로 Export Transcript

---

## Step 02 — 요구사항 분석 (Requirements Analysis)

# Task Title : 02. 요구사항 분석 (Requirements Analysis)
## commit string : 02_요구사항_분석_Requirements_Analysis

@README.md @CMakeLists.txt @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @shealth.dat @.cursorrules @docs/cursorrules_summary.md

[Persona] 시니어 비즈니스·시스템 분석가

[Context] C++17, CMake, shealth.dat CSV(id,age,weight,height). README Activities 1~5 및 기능 개선 항목(SRP 분리, 연령대 BMI 분포, height=0 보정, 정상 BMI 사용자 목록, 전체 대비 범주 비율) 반영.

[Task] 요구사항 분석서 작성.
- 기능 요구: BMI 계산, 체중 0 연령대 평균 보정, 4분류 통계, 연령대(20~70)별 비율 출력
- 비기능: 빌드/ctest, 코드 품질 개선, STL 허용
- README Activities 4의 **향후** 기능을 Must/Should/Could로 구분
- 입력·출력·데이터 제약, BMI 경계값 정의(18.5, 23, 25) 명시
- 현재 API(`calculateBmi`, `getBmiRatio`)와 main 출력 계약 정리
- 모호·누락 요구( height=0, 연령 10대 미만 등) 이슈 목록

[Format] Markdown. `docs\requirements_analysis.md`

[Wrap-up]
- `Report\02_요구사항_분석_Requirements_Analysis_report.md`
- Transcript → `Prompting\02_요구사항_분석_Requirements_Analysis.md`

---

## Step 03 — 코드 품질 분석 (SOLID & Code Smell)

# Task Title : 03. 코드 품질 분석 (SOLID & Code Smell)
## commit string : 03_코드_품질_분석_Code_Quality

@README.md @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @src/main/cpp/SHealthBMI.cpp @docs/requirements_analysis.md @.cursorrules

[Persona] 시니어 소프트웨어 아키텍트 (클린코드·SOLID)

[Context] SHealth 단일 클래스에 파일 I/O·보정·BMI·통계·getter가 혼재. 고정 배열[10000], 연령대×4분류 멤버 24개, getBmiRatio 거대 분기, magic number(100/200/300/400) 존재.

[Task] 코드 품질 분석 보고서 작성 (코드 변경 없음).
- SOLID 위반: SRP(책임 과다), OCP(연령대/타입 확장 시 분기 증가) 등
- 코드 스멜: Long Method, Duplicate Code, Magic Number, Data Clumps, Primitive Obsession, 경계 조건 버그 가능성(BMI 25 정확히, bmis[i]>25 vs >=25)
- README Activities 1「코드 스멜 찾기」항목 매핑
- 리팩토링 우선순위(P0/P1/P2) 및 04 step 권장 작업 목록

[Format] Markdown. `docs\code_quality_analysis.md`

[Wrap-up]
- `Report\03_코드_품질_분석_Code_Quality_report.md`
- Transcript → `Prompting\03_코드_품질_분석_Code_Quality.md`

---

## Step 04 — 리팩토링 지원 (테스트 Green 유지)

# Task Title : 04. 리팩토링 지원 (테스트 Green 유지)
## commit string : 04_리팩토링_Refactoring_Support

@README.md @CMakeLists.txt @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @src/main/cpp/SHealthBMI.cpp @src/test/cpp/SHealthBMITest.cpp @docs/requirements_analysis.md @docs/code_quality_analysis.md @.cursorrules

[Persona] 리팩토링 및 Unit Test expert

[Context] C++17, CMake, Google Test. README Activities 2 순서: 네이밍 → 하드코드/전역 제거 → 함수 추출 → 반복/중복 제거. **Unit Test 본격 작성(05~) 이전**에 수행.

[Task] 1차 리팩토링 수행 (동작 동등성 유지).
- `docs\code_quality_analysis.md`의 P0/P1 항목부터 적용
- BMI 상수·분류 enum/struct, 연령대 통계 구조 단순화, split/파싱·보정·계산·집계 함수 분리
- Magic number(100/200/300/400) 의미 있는 이름으로 치환
- 기존 테스트가 있으면 Green 유지; `FailedTest`만 있으면 제거 또는 skip 후 리팩토링
- `SHealthBMI` main 출력 형식은 Golden Master 전까지 변경 최소화
- 변경 요약·남은 기술 부채·05 테스트에 노출할 public API 목록 기록

[Format]
- 코드: `src/main/cpp/*` (필요 시 CMakeLists 최소 수정)
- Markdown. `docs\refactoring_notes.md`

[Wrap-up]
- `Report\04_리팩토링_Refactoring_Support_report.md`
- Transcript → `Prompting\04_리팩토링_Refactoring_Support.md`

---

## Step 05 — 테스트 계획 (Test Plan)

# Task Title : 05. 테스트 계획 (Test Plan)
## commit string : 05_테스트_계획_Test_Plan

@README.md @CMakeLists.txt @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @docs/requirements_analysis.md @docs/refactoring_notes.md @src/test/cpp/SHealthBMITest.cpp

[Persona] 시니어 QA 리드

[Context] C++17, Google Test, CMake, ctest. (lcov는 `gen_lcov2.bat` 참고 가능) SHealth 도메인 테스트.

[Task] 테스트 계획서 작성.
- `TEST` / `TEST_F` 기반 단위 테스트 범위·우선순위 (`SHealthBMITest`)
- 경계값: BMI 18.5/23/25, 연령 19·20·29·30, weight=0·height=0, 연령대 내 전원 weight=0
- 예외/특이: 파일 없음, 빈 데이터, 헤더만, 잘못된 CSV, ageClass/type 잘못된 getBmiRatio
- README Activities 3 TC 4종(BMI 계산, 평균 보정, 분류, 예외) 매핑
- Activities 4 향후 기능에 대한 테스트 확장 슬롯(Should/Could)
- Test Double 필요성: 파일 I/O 분리 시 임시 fixture 파일 vs 추후 인터페이스 Mock 여부

[Format] Markdown. `docs\test_plan.md`

[Wrap-up]
- `Report\05_테스트_계획_Test_Plan_report.md`
- Transcript → `Prompting\05_테스트_계획_Test_Plan.md`

---

## Step 06 — 테스트 케이스 작성 (Test Cases)

# Task Title : 06. 테스트 케이스 작성 (Test Cases)
## commit string : 06_테스트_케이스_작성_Test_Cases

@README.md @CMakeLists.txt @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @src/test/cpp/SHealthBMITest.cpp @docs/test_plan.md @docs/refactoring_notes.md @docs/requirements_analysis.md

[Persona] 리팩토링 및 Unit Test expert

[Context] Google Test, `SHealthBMITest` executable, shealth_lib 링크.

[Task]
- `docs\test_plan.md`에 따라 `src/test/cpp/SHealthBMITest.cpp`에 테스트 구현
- BMI 계산, 연령대 평균 보정(weight=0), 4분류, 예외/경계 케이스
- 소형 fixture CSV를 테스트 디렉터리 또는 빌드 산출물에 생성·사용
- `FailedTest` 제거, 모든 신규 테스트 Green 목표 (미구현 시 EXPECT 실패와 TODO 구분 명시)
- 테스트 목록·fixture 설명을 문서화

[Format]
- 코드: `src/test/cpp/SHealthBMITest.cpp`, 필요 시 fixture 파일
- Markdown. `docs\test_cases.md`

[Wrap-up]
- `Report\06_테스트_케이스_작성_Test_Cases_report.md`
- Transcript → `Prompting\06_테스트_케이스_작성_Test_Cases.md`

---

## Step 07 — 테스트 실행 & 결함 분석 (Defect Detection)

# Task Title : 07. 테스트 실행 & 결함 분석 (Defect Detection)
## commit string : 07_테스트_실행_결함_분석_Defect_Detection

@README.md @CMakeLists.txt @src/test/cpp/SHealthBMITest.cpp @src/main/cpp/SHealth.cpp @docs/test_cases.md @docs/test_plan.md @docs/requirements_analysis.md

[Persona] 시니어 QA 엔지니어

[Context] `build`에서 `cmake --build .` 후 `ctest -V`. Windows/PowerShell 환경.

[Task]
- 테스트 전체 실행, 실패 케이스별 원인 분석 (요구사항 vs 구현 vs 테스트 오류)
- BMI 경계·통계 비율 합산 등 실패 패턴 분류
- 수정이 필요한 항목: 코드 버그 / 테스트 기대값 오류 / 요구사항 불명확
- 실행 로그 요약, 재현 절차, 우선순위(Severity)

[Format] Markdown. `docs\defect_detection.md`

[Wrap-up]
- `Report\07_테스트_실행_결함_분석_Defect_Detection_report.md`
- Transcript → `Prompting\07_테스트_실행_결함_분석_Defect_Detection.md`

---

## Step 08 — 결함 목록 문서화 (defect_list.md)

# Task Title : 08. 결함 목록 문서화 (defect_list)
## commit string : 08_결함_목록_문서화_defect_list

@docs/defect_detection.md @docs/requirements_analysis.md @docs/test_cases.md @src/main/cpp/SHealth.cpp @src/test/cpp/SHealthBMITest.cpp

[Persona] QA 리드 (결함 관리)

[Context] Step 07 분석 결과를 추적 가능한 결함 티켓 형태로 정리.

[Task]
- 결함 ID, 제목, 재현 단계, 기대/실제, Severity, Priority, 상태(Open/Fixed/Won't fix)
- 요구사항 ID·관련 테스트명·소스 위치 매핑
- README BMI 경계 정의와 구현 불일치 항목 별도 표기

[Format] Markdown. `docs\defect_list.md`

[Wrap-up]
- `Report\08_결함_목록_문서화_defect_list_report.md`
- Transcript → `Prompting\08_결함_목록_문서화_defect_list.md`

---

## Step 09 — Golden Master 자동화 (회귀 안전장치)

# Task Title : 09. Golden Master 자동화 (회귀 안전장치)
## commit string : 09_Golden_Master_자동화

@README.md @CMakeLists.txt @src/main/cpp/SHealthBMI.cpp @shealth.dat @docs/defect_list.md @docs/refactoring_notes.md @src/test/cpp/SHealthBMITest.cpp

[Persona] 리팩토링 및 Unit Test expert

[Context] `SHealthBMI` 실행 시 연령대별 4분류 printf 출력. 리팩토링·기능 추가 후 회귀 방지.

[Task]
- 현재 `SHealthBMI` + `shealth.dat` 기준 stdout을 baseline으로 캡처
- Google Test 또는 스크립트로 Golden Master 비교 테스트 추가 (허용 오차·정규화 규칙 명시)
- baseline 갱신 절차 문서화
- CMake/ctest에 Golden 테스트 등록

[Format]
- 코드/스크립트: `src/test/cpp/` 또는 `tests/golden/` (프로젝트 관례에 맞게)
- Markdown. `docs\golden_master.md`

[Wrap-up]
- `Report\09_Golden_Master_자동화_report.md`
- Transcript → `Prompting\09_Golden_Master_자동화.md`

---

## Step 10 — 결함 관리 & 보고 (Defect Management)

# Task Title : 10. 결함 관리 & 보고 (Defect Management)
## commit string : 10_결함_관리_보고_Defect_Management

@docs/defect_list.md @docs/defect_detection.md @src/main/cpp/SHealth.cpp @src/test/cpp/SHealthBMITest.cpp @docs/golden_master.md

[Persona] QA 리드 + 개발 리드

[Context] Open 결함 처리, 테스트 재실행, Golden Master 확인.

[Task]
- `docs\defect_list.md` Open 항목 중 Priority 높은 것부터 수정 또는 Won't fix 근거 기록
- 수정 후 ctest·Golden Master 재실행 결과 반영
- 결함 상태 갱신, 잔여 리스크·Known Issues 정리
- Activities 4 기능 개선 착수 시 회귀 체크리스트 제안

[Format] Markdown. `docs\defect_management.md` (결함 상태 스냅샷 + 조치 이력)

[Wrap-up]
- `Report\10_결함_관리_보고_Defect_Management_report.md`
- Transcript → `Prompting\10_결함_관리_보고_Defect_Management.md`

---

## Step 11 — QA 종합 검토 (최종 보고서)

# Task Title : 11. QA 종합 검토 (최종 보고서)
## commit string : 11_QA_종합_검토_Final_Report

@README.md @docs/requirements_analysis.md @docs/code_quality_analysis.md @docs/refactoring_notes.md @docs/test_plan.md @docs/test_cases.md @docs/defect_list.md @docs/defect_management.md @docs/golden_master.md @Report/

[Persona] QA·기술 리드 (발표·회고 담당)

[Context] README Activities 5「회고 및 발표」: 목표 달성도, Before/After, AI 활용, TC 영향, 클린코드 체감.

[Task] QA 종합 최종 보고서 작성.
- 요구사항 대비 달성도, 테스트·결함·Golden Master 요약
- 코드 품질 Before/After (03·04 문서 인용)
- AI 활용 효과·한계, TC 작성 팁
- 미완 Activities 4 기능 로드맵, 권장 다음 스프린트

[Format] Markdown. `docs\qa_final_report.md`

[Wrap-up]
- `Report\11_QA_종합_검토_Final_Report_report.md`
- Transcript → `Prompting\11_QA_종합_검토_Final_Report.md`

---

## 실행 순서 요약

| Step | 제목 | docs 산출물 | 선행 docs |
|------|------|-------------|-----------|
| 01 | .cursorrules | cursorrules_summary.md | — |
| 02 | 요구사항 분석 | requirements_analysis.md | 01 |
| 03 | 코드 품질 | code_quality_analysis.md | 02 |
| 04 | 리팩토링 | refactoring_notes.md | 02, 03 |
| 05 | 테스트 계획 | test_plan.md | 02, 04 |
| 06 | 테스트 케이스 | test_cases.md | 05 |
| 07 | 결함 분석 | defect_detection.md | 06 |
| 08 | 결함 목록 | defect_list.md | 07 |
| 09 | Golden Master | golden_master.md | 04, 08 |
| 10 | 결함 관리 | defect_management.md | 08, 09 |
| 11 | QA 종합 | qa_final_report.md | 전체 |

**원칙**: 04(리팩토링) 완료 전 05~06 본격 테스트 구현 지양. 매 step Wrap-up으로 Report·Prompting 정리.
