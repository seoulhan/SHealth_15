# Step 08 — 결함 목록 문서화 (Transcript Export)

**Task Title:** 08. 결함 목록 문서화 (defect_list)  
**commit string:** `08_결함_목록_문서화_defect_list`  
**Persona:** QA 리드 (결함 관리)

---

## User Prompt

```
## Step 08 — 결함 목록 문서화 (defect_list)

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
```

---

## Assistant Actions (요약)

1. **입력 문서 검토** — `defect_detection.md` §9 DEF-001~007 후보, `requirements_analysis.md` 이슈·FR 매핑, `test_cases.md` 38 TC, `SHealth.cpp` / `SHealthBMITest.cpp` 소스 라인 확인.
2. **`docs/defect_list.md` 작성** — 11건 티켓(DEF-001~011): Open 7, Fixed 1, Won't fix 3.
3. **§3 README BMI 경계** — I-01/DEF-007 Fixed; 18.5/23/25 경계 표; `requirements_analysis.md` §4.3 구 스냅샷 주석.
4. **티켓 필드** — 각 건마다 재현·기대/실제·Severity·Priority·상태·요구사항·GTest·소스 라인.
5. **추적성 매트릭스·로드맵** — §5~§6 P0~P3 조치 순서.
6. **Wrap-up** — Report, 본 Transcript.

---

## 산출물

| 파일 | 설명 |
|------|------|
| `docs/defect_list.md` | 결함 티켓·README 경계·추적성 매트릭스 |
| `Report/08_결함_목록_문서화_defect_list_report.md` | 작업 보고서 |
| `Prompting/08_결함_목록_문서화_defect_list.md` | 본 Transcript |

---

## Open 결함 요약 (Step 09+ 입력)

| ID | 제목 | Pri |
|----|------|-----|
| DEF-001 | CSV 컬럼 부족 abort | P0 |
| DEF-002 | height=0 미보정 | P1 |
| DEF-003 | parse 예외 미처리 | P1 |
| DEF-004 | 비율 합 스펙 불명확 | P2 |
| DEF-005 | 10k 상한 미방어 | P2 |
| DEF-006 | Golden Master 없음 | P2 |
| DEF-011 | CWD 의존 | P3 |

**Fixed:** DEF-007 (BMI=25 비만). **Won't fix:** DEF-008~010.
