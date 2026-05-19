# Step 01 — .cursorrules 초안 생성 (Transcript Export)

**Task Title:** 01. .cursorrules 초안 생성  
**commit string:** `01_cursorrules_초안_생성`  
**Persona:** 리팩토링 및 Unit Test expert

---

## User Prompt

```
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
```

---

## Assistant Actions (요약)

1. **입력 분석**: README, CMakeLists.txt, SHealth.h/cpp, SHealthBMI.cpp, SHealthBMITest.cpp, 기존 `.cursorrules`(Step 00 초안) 읽기
2. **`.cursorrules` 보완**: CMake 타깃·API·구현/요구 경계 주의·코드 스멜 목록·Activities 순서·빌드 명령·11단계 docs 체인 반영
3. **`docs/cursorrules_summary.md`**: Step 02용 한 페이지 요약
4. **`Report/01_cursorrules_초안_생성_report.md`**: 작업 보고
5. **본 Transcript** 저장

---

## Key Decisions

- 기존 44행 초안은 유지·확장(삭제보다 구조화·표 형식 보강)
- README “25 이상” vs 코드 `>25` 불일치는 규칙 표 + “검증 대상”으로 명시(Step 07 연계)
- `FailedTest`는 Step 06 TC 작성 전까지 placeholder로 문서화

---

## Deliverables

| 파일 | 설명 |
|------|------|
| `.cursorrules` | 프로젝트 AI·개발 규칙 (루트) |
| `docs/cursorrules_summary.md` | Step 02 입력 요약 |
| `Report/01_cursorrules_초안_생성_report.md` | 단계 보고서 |
| `Prompting/01_cursorrules_초안_생성.md` | 본 Transcript |

---

*Export: Step 01 wrap-up — 2026-05-20*
