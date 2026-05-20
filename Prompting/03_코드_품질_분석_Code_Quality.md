# Step 03 — 코드 품질 분석 (Transcript Export)

**Task Title:** 03. 코드 품질 분석 (SOLID & Code Smell)  
**commit string:** `03_코드_품질_분석_Code_Quality`  
**Persona:** 시니어 소프트웨어 아키텍트 (클린코드·SOLID)

---

## User Prompt

```
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
```

---

## Assistant Actions (요약)

1. **입력 분석**: SHealth.h/cpp, SHealthBMI.cpp, requirements_analysis.md, README, .cursorrules
2. **SOLID**: SRP·OCP 높음; DIP·ISP 중간; LSP 해당 약함(API 0.0 모호)
3. **코드 스멜**: God Class, Long Method(~103줄), 3× 연령대 루프, 24-way getBmiRatio, Magic Number, Data Clumps, Primitive Obsession
4. **경계**: BMI=25 README 불일치 + 미분류; ageCount=0 NaN; height=0; sum==0
5. **Activity 1 매핑**: §3.1 표
6. **우선순위**: P0 정확성(25, 보정 가드) → P1 Step04 클린코드 → P2 구조/STL
7. **Step 04 목록**: 9단계 (상수·classifyBmi·함수 추출·통계 구조화·main 루프)
8. **산출물**: docs/code_quality_analysis.md, Report, 본 Transcript

---

## Key Decisions

- BMI=25는 **P0 결함**으로 유지 (requirements I-01과 동일); Step 04에서 수정 시 Golden Master 갱신 필요(Step 09)
- Step 04 범위는 README Activity 2에 한정; **클래스 파일 분리·height=0**은 Step 08+
- `getBmiRatio` 0.0 반환은 스멜/계약 이슈(P3)로 분리

---

## Deliverables

| 파일 | 설명 |
|------|------|
| `docs/code_quality_analysis.md` | 본 분석 본문 |
| `Report/03_코드_품질_분석_Code_Quality_report.md` | 완료 보고 |
| `Prompting/03_코드_품질_분석_Code_Quality.md` | 본 Transcript |

---

## Next Step

- Step 04: `docs/refactoring_plan.md` 또는 동등 + P0/P1 리팩토링 실행
- commit: `03_코드_품질_분석_Code_Quality` (사용자 요청 시)
