@README.md @CMakeLists.txt @src/ 
[Persona] 리팩토링 및 Unit Test expert
[Context] CMakeLists 를 기준으로 한 C++ 컴파일러 환경의 SHealth_15 project
[Task] README.md 의 내용을 바탕으로, 프로젝트에 대한 Activities 처리를 위한 Step by step Prompt 생성
 - 가장 먼저 .cursorrules 초안을 생성하고 프로젝트에 파일을 기록한다.
 - Activities 의 세부 단계를 분할할 경우 다음의 예시 목록을 참고한다.
```
# 01. .cursorrules 초안 생성
# 02. 요구사항 분석 (Requirements Analysis)
# 03. 코드 품질 분석 (SOLID & Code Smell)
# 04. 리팩토링 지원 (테스트 Green 유지)
# 05. 테스트 계획 (Test Plan)
# 06. 테스트 케이스 작성 (Test Cases)
# 07. 테스트 실행 & 결함 분석 (Defect Detection)
# 08. 결함 목록 문서화 (defect_list.md)
# 09. Golden Master 자동화 (회귀 안전장치)
# 10. 결함 관리 & 보고 (Defect Management)
# 11. QA 종합 검토 (최종 보고서)
```
 - 리팩토링 단계는 Unit Test 단계보다 반드시 앞에 오도록 한다.
 - 각 step 실행이 종료될 때 마다, 분석 결과 및 다음 단계로 전달할 필요가 있는 정보를 docs\ 디렉토리 아래에 markdown 서식의 파일로 생성
 - 각 step이 실행되면 프롬프트에서 요구하는 작업을 진행할 때 docs\ 아래에 있는 정보들을 활용
 - 아래의 1 - step prompt 템플릿을 참조하여, P/C/T/F 작업 및 step output (Wrap-up) 작업을 하는 All - step prompt 를 생성하여 제출하라.

```
# Task Title : 04. 테스트 계획 (Test Plan)
## commit string : 04_테스트_계획_Test_Plan

@TVController.h @README.md @requirements_analysis.md

[Persona] 시니어 QA 리드입니다.
[Context] C++17, Google Test, GMock, CMake, lcov
[Task] 테스트 계획서를 작성해줘.
    - TEST_F 기반 단위 테스트 범위/우선순위 (TVControllerTest)
    - 경계값 케이스 목록 (채널 0/99, 빈 선호채널, 검색 목록 없을 때의 Up/Down)
    - 예외/특이 케이스 목록 (연속 숫자 입력 중 OTHER 키 무효화, '0'->'7' 입력 처리)
    - Test Double 활용 계획: FakeTuner(상태) vs MockTuner(호출 횟수 검증)
[Format] Markdown 문서. docs\ 디렉토리 아래 test_plan.md로 저장
[Wrap-up] 
 - Report\ 디렉토리 아래에 작업 내용 보고서를 번호를 붙여 생성하고 저장
 - 현재까지의 프롬프트 대화 전체를 Prompting\ 디렉토리 아래에 Export Transcript 해줘. Transcript 파일명은 보고서의 이름에서 _report 접미사를 뺀 것으로 해줘.
```

 - Task Title 제목에는 보고서 명칭으로 사용할 작업의 제목을 할당하라.
 - commit string 제목에는 작업 제목의 whitespace-safe 한 명칭 (git commit -m "%1" 전달 안전) 을 변환하여 할당하라.
 - @ 참조 파일 목록란에는 현재 SHealth_15 프로젝트 파일에서 각 단뎨를 진행하는 데 필요한 파일명을 적절히 할당하라.
 - [Persona]

[Format] Markdown format 으로 된 모든 step prompt 의 통합된 출력