# 18. 기능 개선 종합·Activity 5 회고 — 작업 보고서

| 항목 | 내용 |
|------|------|
| 작업일 | 2026-05-20 |
| Task | Step 18 — 기능 개선 종합·Activity 5 회고 갱신 |
| commit string (권장) | `18_기능_개선_종합_Activity5_회고` |
| Persona | QA·기술 리드 (발표·회고 담당) |
| 기준 문서 | Steps 12~17, `docs/qa_final_report.md` v1.0 |

## 1. 작업 목표

- Step 11 `qa_final_report.md` §7「미완 Activity 4 로드맵」을 **Step 18 실제 달성**으로 갱신(부록 형태, 원본 최소 변경).
- FR-S01~S03, FR-C01~C02 **달성도 표** 및 Before/After(코드·테스트·Golden) 정리.
- README Activity 5 회고 항목(목표·AI·TC·클린코드) **Step 18 기준** 반영.
- 잔여 리스크·다음 스프린트 권고.

## 2. 수행 내용

### 2.1 현황 검증

| 항목 | 결과 |
|------|------|
| 빌드 | `build-gcc`, MinGW Makefiles |
| **ctest** | **52 / 52 Passed** (51 unit + 1 Golden) |
| 소요 | Total Test time ≈ 0.99 s |

### 2.2 종합 분석 (요약)

| 영역 | Step 11 | Step 18 |
|------|---------|---------|
| Activity 4 | ~25% | **~95%** |
| FR-S02 height=0 | Open (DEF-002) | **구현·TC Green** |
| FR-S01 SRP | 부분 | **6 모듈 분리 완료** |
| FR-S03, FR-C01/C02 | 미완/미착수 | **완료** |
| ctest | 40/40 | **52/52** |
| Golden 6행 | Green | **Green, baseline 불변** |

상세 표·회고·로드맵은 `docs/feature_final_report.md` 참조. `docs/qa_final_report.md` **부록 B**에 Activity 4~5 통합 요약 추가.

### 2.3 Step 11 로드맵 대비

| 백로그 항목 | 달성 |
|-------------|------|
| height=0 보정 | ✅ Step 13 |
| SRP 분리 | ✅ Step 16 |
| 정상 BMI ID·전체 비율 | ✅ Step 15 |
| FR-S03 API | ✅ Step 14 |
| CLI (DEF-011) | ❌ 잔여 |
| vector 전환 | ❌ 잔여 |

## 3. 산출물

| 산출물 | 경로 | 상태 |
|--------|------|------|
| 기능 종합·회고 | `docs/feature_final_report.md` | 생성 |
| QA 보고서 갱신 | `docs/qa_final_report.md` (부록 B, v1.1) | 갱신 |
| 작업 보고서 | `Report/18_기능_개선_종합_Activity5_회고_report.md` | 본 문서 |
| Transcript | `Prompting/18_기능_개선_종합_Activity5_회고.md` | 생성 |

## 4. Wrap-up

| 항목 | 내용 |
|------|------|
| 코드 변경 | 없음 (문서·회고 전용 Step) |
| 테스트 | 52/52 Pass (검증만 수행) |
| 실습 완결도 | Activities 1~4 **실질 완료**; Activity 5 회고 **Step 18 갱신** |
| 권장 후속 | `defect_list.md` DEF-002 Fixed, DEF-011 CLI |

**commit (권장):** `18_기능_개선_종합_Activity5_회고`

## 5. 발표 시 권장 슬라이드 구성

1. **Step 11 → 18 한 장** — Activity 4 25%→95%, 40→52 tests  
2. **달성도 표** — FR-S01~S03, FR-C01~C02 (완료/근거)  
3. **Before/After** — God Class → 6 모듈 + API 4개  
4. **Golden** — 6행 유지·데모 2행은 unit TC  
5. **AI·TC 회고** — 설계 선행·Golden 분리  
6. **Next** — DEF-011, vector, 10/80대
