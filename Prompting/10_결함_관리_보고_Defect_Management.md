# Step 10 — 결함 관리 & 보고 (Transcript Export)

**Task Title:** 10. 결함 관리 & 보고 (Defect Management)  
**commit string:** `10_결함_관리_보고_Defect_Management`  
**Persona:** QA 리드 + 개발 리드

---

## User Prompt

```
## Step 10 — 결함 관리 & 보고 (Defect Management)

# Task Title : 10. 결함 관리 & 보고 (Defect Management)
## commit string : 10_결함_관리_보고_Defect_Management

@docs/defect_list.md @docs/defect_detection.md @src/main/cpp/SHealth.cpp @src/test/cpp/SHealthBMITest.cpp @docs/golden_master.md

[Persona] QA 리드 + 개발 리드

[Context] Open 결함 처리, 테스트 재실행, Golden Master 확인.

[Task]
- docs\defect_list.md Open 항목 중 Priority 높은 것부터 수정 또는 Won't fix 근거 기록
- 수정 후 ctest·Golden Master 재실행 결과 반영
- 결함 상태 갱신, 잔여 리스크·Known Issues 정리
- Activities 4 기능 개선 착수 시 회귀 체크리스트 제안

[Format] Markdown. docs\defect_management.md

[Wrap-up]
- Report\10_결함_관리_보고_Defect_Management_report.md
- Transcript → Prompting\10_결함_관리_보고_Defect_Management.md
```

---

## Assistant Actions (요약)

1. **Open 결함 우선순위 처리**
   - **DEF-001 (P0):** `loadRecordsFromFile` — `tokens.size() < 4` 스킵, `cerr` 로그.
   - **DEF-003 (P1):** `stoi`/`stod` try-catch, 무효 행 스킵.
   - **DEF-005 (P2):** `recordCount >= MAX_RECORDS` 방어.
   - **DEF-004 (P2):** 옵션 A Won't fix — 분모=연령대 전원, 합<100% 허용.
   - **DEF-002 (P1):** Activity 4 연기 (FR-S02).
   - **DEF-011 (P3):** Known Issue 유지.

2. **테스트 갱신** — `TC_EXC_05`/`06` graceful 기대; `TC_EXC_11` 신규.

3. **ctest·Golden** — `build-gcc`: **40/40 Passed**; Golden baseline 불변.

4. **문서** — `docs/defect_management.md`, `docs/defect_list.md` v1.1, Report, 본 Transcript.

---

## 산출물

| 파일 | 설명 |
|------|------|
| `src/main/cpp/SHealth.cpp` | CSV 파싱 견고성 |
| `src/test/cpp/SHealthBMITest.cpp` | TC_EXC_05/06/11 |
| `docs/defect_management.md` | 스냅샷·조치 이력·회귀 체크리스트 |
| `docs/defect_list.md` | v1.1 상태 갱신 |
| `Report/10_결함_관리_보고_Defect_Management_report.md` | 작업 보고서 |
| `Prompting/10_결함_관리_보고_Defect_Management.md` | 본 Transcript |

---

## 테스트 결과 (Step 10)

```
100% tests passed, 0 tests failed out of 40
Total Test time (real) = 1.02 sec
```

---

## Open 잔여 (Activity 4+)

| ID | 제목 |
|----|------|
| DEF-002 | height=0 동연령대 키 보정 |
| DEF-011 | main CWD / shealth.dat 경로 |
