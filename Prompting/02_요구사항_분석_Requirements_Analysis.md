# Step 02 — 요구사항 분석 (Transcript Export)

**Task Title:** 02. 요구사항 분석 (Requirements Analysis)  
**commit string:** `02_요구사항_분석_Requirements_Analysis`  
**Persona:** 시니어 비즈니스·시스템 분석가

---

## User Prompt

```
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
```

---

## Assistant Actions (요약)

1. **입력 분석**: README, CMakeLists.txt, SHealth.h/cpp, SHealthBMI.cpp, shealth.dat 샘플, .cursorrules, cursorrules_summary.md
2. **As-Is 기능 요구**: FR-01~08 (로드, BMI, weight=0 보정, 4분류, 연령대 %, API, main 출력)
3. **MoSCoW**: Activity 4 항목 — Must(현행), Should(SRP, height=0, API 정비, UT), Could(정상 목록, 전체 비율)
4. **BMI 경계**: README normative 표 + 구현 `>25` vs `≥25` 불일치 → I-01
5. **API·main 계약**: `calculateBmi`/`getBmiRatio` 전후조건, type 100~400, printf 6행 패턴
6. **이슈 12건**: height=0, age 범위 외, 빈 연령대, 10k 상한 등
7. **산출물**: `docs/requirements_analysis.md`, Report, 본 Transcript

---

## Key Decisions

- Activity 4 “연령대 BMI 분포 추가”는 **이미 FR-05/06에 존재** → Should는 API·구조 정비(FR-S03)로 해석
- README BMI=25는 **비만**, 코드는 **과체중** — P0 결함 후보로 문서화(Step 07·TC 연계)
- 10대·80대+ 집계는 **Won't**(현 릴리스), 데이터에는 포함 가능(I-04)

---

## Deliverables

| 파일 | 설명 |
|------|------|
| `docs/requirements_analysis.md` | 요구사항 분석서 (Step 03+ 입력) |
| `Report/02_요구사항_분석_Requirements_Analysis_report.md` | 단계 보고서 |
| `Prompting/02_요구사항_분석_Requirements_Analysis.md` | 본 Transcript |

---

*Export: Step 02 wrap-up — 2026-05-20*
